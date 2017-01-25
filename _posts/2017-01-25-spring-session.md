---
title: spring-session
layout: detail
---

# Spring Session源码解读

## ServletContainerInitializer

> ServletContainerInitializer是 Servlet 3.0 新增的一个接口，主要用于在容器启动阶段通过编程风格注册Filter, Servlet以及Listener，以取代通过web.xml配置注册。这样就利于开发内聚的web应用框架。

在Spring中，我们使用它的web功能时，需要在web.xml中依据Spring的规范新建一堆配置。这样就相当于将框架和容器紧耦合了。而在3.x后注册的功能内聚到Spring里，Spring-web就变成一个纯粹的即插即用的组件，不用依据应用环境定义一套新的配置。
Web容器启动阶段依据Java SPI获取到所有ServletContainerInitializer的实现类，然后执行ononStartup方法：

```java
public interface ServletContainerInitializer {

	public void onStartup(Set<Class<?>> c, ServletContext ctx)
		throws ServletException;
    
}
```

1. 实现类需要通过Java SPI声明自己是ServletContainerInitializer的Provider。
2. 可以通过@HandlesTypes注解定义希望处理类型，容器会将当前应用中所有这一类型（继承或者实现）的类放在集合参数1中传递进来。如果不定义处理类型，或者应用中不存在相应的实现类，则集合参数1为空。

## 在Spring Session中应用

spring-web通过Java SPI声明了SpringServletContainerInitializer。参考spring-web.jar下的meta-inf/services。核心代码如下：

```java
@HandlesTypes(WebApplicationInitializer.class)
public class SpringServletContainerInitializer implements ServletContainerInitializer {
	@Override
	public void onStartup(Set<Class<?>> webAppInitializerClasses, ServletContext servletContext)
			throws ServletException {
        List<WebApplicationInitializer> initializers = new LinkedList<WebApplicationInitializer>();
		if (webAppInitializerClasses != null) {
			for (Class<?> waiClass : webAppInitializerClasses) {
				initializers.add((WebApplicationInitializer) waiClass.newInstance());
			}
		}
		for (WebApplicationInitializer initializer : initializers) {
			initializer.onStartup(servletContext);
		}
	}
}
```

spring-session只处理WebApplicationInitializer类型，全部实例化后再依次调用WebApplicationInitializer的onStartup接口。

```java
public interface WebApplicationInitializer {
	void onStartup(ServletContext servletContext) throws ServletException;
}
```

该接口定义了一个onStartup的方法，所以实现类都需要自己实现。spring-session提供了一个抽象的公用基类–org.springframework.session.web.context.AbstractHttpSessionApplicationInitializer，由它实现了接口的onStartup方法

```java
public void onStartup(ServletContext servletContext)
		throws ServletException {
	beforeSessionRepositoryFilter(servletContext);
	if(configurationClasses != null) {
		AnnotationConfigWebApplicationContext rootAppContext = new AnnotationConfigWebApplicationContext();
		rootAppContext.register(configurationClasses);
		servletContext.addListener(new ContextLoaderListener(rootAppContext));
	}
	insertSessionRepositoryFilter(servletContext);
	afterSessionRepositoryFilter(servletContext);
}
```

这个方法的主要过程： 

1. 添加ContextLoaderListener，使用过spring-mvc的肯定对这个Listener不陌生，这个Listener是整个spring-web的核心，由它来自动装配spring的核心容器ApplicationContext。第一步中的ApplicationContext作为构造参数传进了该listener。web容器启动时会通知该listener，该listener就会启动ApplicationContext的生命周期。并将bean加载完毕。因为这是spring-web边界内的，所以就不再展开，只要知道大概做了什么事情就行了。
2. 通过insertSessionRepositoryFilter注册了一个Filter，从名字知道这个Filter叫SessionRepositoryFilter，顾名思义，它肯定是通过一定方式做session持久化的。
```java
private void insertSessionRepositoryFilter(ServletContext servletContext) {
	String filterName = DEFAULT_FILTER_NAME;
	DelegatingFilterProxy springSessionRepositoryFilter = new DelegatingFilterProxy(filterName);
	String contextAttribute = getWebApplicationContextAttribute();
	if(contextAttribute != null) {
		springSessionRepositoryFilter.setContextAttribute(contextAttribute);
	}
	registerFilter(servletContext, true, filterName, springSessionRepositoryFilter);
}
```

这个方法主要是动态装载了一个DelegatingFilterProxy，构造函数的入参是个静态变量，对应的值是“springSessionRepositoryFilter”。从“DelegatingFilterProxy”这个名字就大概能猜到这个filter最终会把请求delegate给具体的filter，看下入参大概就可以猜到应该是委派给“springSessionRepositoryFilter”这个filter，并且这个filter应该是由spring容器管理的，在filter生命周期的某个阶段会通过spring的依赖注入进来，并执行相关的拦截请求。
这里有一点会比较容易疑惑，spring为什么要通过一个代理类来做委派，直接注册SpringSessionRepositoryFilter不是更好。
这个类继承自GenericFilterBean，GenericFilterBean是spring抽象出来的一个filter公用接口。先看下GenericFilterBean的init方法：

```java
public final void init(FilterConfig filterConfig) throws ServletException {
	// 伪代码，利用filterConfig里的参数为filter做属性自动注入
	// Let subclasses do whatever initialization they like.
	initFilterBean();
}
```

这里只抽出了其中的核心逻辑，最后调用子类的initFilterBean
DelegatingFilterProxy覆盖了父类的initFilterBean方法

```java
protected void initFilterBean() throws ServletException {
	synchronized (this.delegateMonitor) {
		if (this.delegate == null) {
			// If no target bean name specified, use filter name.
			if (this.targetBeanName == null) {
				this.targetBeanName = getFilterName();
			}
			WebApplicationContext wac = findWebApplicationContext();
			if (wac != null) {
				this.delegate = initDelegate(wac);
			}
		}
	}
}
protected Filter initDelegate(WebApplicationContext wac) throws ServletException {
	Filter delegate = wac.getBean(getTargetBeanName(), Filter.class);
	if (isTargetFilterLifecycle()) {
		delegate.init(getFilterConfig());
	}
	return delegate;
}
```

initFilterBean方法通过spring容器来取得具体的filter bean，而WebApplicationContext对象就是上文提到的AnnotationConfigWebApplicationContext对象。ContextLoaderListener在spring容器refresh完后会把对应的容器保存进ServletContext里，默认的key值为WebApplicationContext.class.getName() + “.ROOT”。这里获取spring context的findWebApplicationContext方法就是从ServletContext里通过对应的key取得的。
至此DelegatingFilterProxy初始化完成，里面做实际处理的filter最终通过spring的依赖注入初始化并且注入进来。

## 启用Redis Session

