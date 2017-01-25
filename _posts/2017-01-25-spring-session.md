---
title: spring-session
layout: detail
---

# Spring Session

Spring Session源码解读

# ServletContainerInitializer

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
