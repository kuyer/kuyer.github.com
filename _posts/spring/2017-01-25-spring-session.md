---
layout: detail
permalink: /spring/session
title: Spring Session源码解读
category: spring
cover: /images/spring/session.jpg
tags: [SpringSession,Session共享,Session水平扩展,非Web访问Session]
description: Spring在Servlet3.x的基础上，实现了多服务的Session共享问题，本文在源码的基础上解读
published: true
---

# Spring Session源码解读

Spring在Servlet3.x的基础上，实现了多服务的Session共享问题，本文在源码的基础上解读

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
Spring通过EnableRedisHttpSession注解来启用Redis Session

```java
@Import(RedisHttpSessionConfiguration.class)
@Configuration
public @interface EnableRedisHttpSession {
	int maxInactiveIntervalInSeconds() default 1800;
}
```

该注解有两个元注解，一个是Configuration, 一个是Import, 上一篇提过Configuration，它就相当于是beans配置，而Import则相当于是beans里嵌套了另一个beans配置项。另外再介绍一下Bean这个注解，这个注解相当于是beans配置里的bean标签，它是注解在方法上的，被注解方法的返回值就是一个spring bean，而相应的方法名就是作为bean name，如果显示的设置了name属性，那就以name属性值作为bean name。
所以可以理解为EnableRedisHttpSession就是一个编程式的配置定义，而RedisHttpSessionConfiguration显然也应该是一个编程式配置定义。

## 容器对springSessionRepositoryFilter的依赖管理

在RedisHttpSessionConfiguration里很惊喜的发现了第一篇里被注入的“springSessionRepositoryFilter”。

```java
@Bean
public <S extends ExpiringSession> SessionRepositoryFilter<? extends ExpiringSession> springSessionRepositoryFilter(SessionRepository<S> sessionRepository, ServletContext servletContext) {
	SessionRepositoryFilter<S> sessionRepositoryFilter = new SessionRepositoryFilter<S>(sessionRepository);
	sessionRepositoryFilter.setServletContext(servletContext);
	if(httpSessionStrategy != null) {
		sessionRepositoryFilter.setHttpSessionStrategy(httpSessionStrategy);
	}
	return sessionRepositoryFilter;
}
```

等价于

```xml
<bean name="springSessionRepositoryFilter" class="org.springframework.session.web.http.SessionRepositoryFilter" />
```

两个参数SessionRepository和ServletContext都是由Spring容器管理的依赖，SessionRepository和SpringSessionRepositoryFilter定义在同一个Configuration里。ServletContext的注入可能会比较令人疑惑，在这个Configuration没有定义它无法定义它，因为它是由Servlet容器生成的。

## ContextLoaderListener

在介绍ServletContext的依赖之前，再聊下前文提过的ContextLoaderListener。它由initializer动态注册，其构造函数参数为一个WebApplicationContext，继承于ContextLoader（实际启动WebApplicationContext初始化工作的对象），构造函数参数是一个WebApplicationContext。
Servlet容器启动时会通知ContextLoaderListener：

```java
@Override
public void contextInitialized(ServletContextEvent event) {
	initWebApplicationContext(event.getServletContext());
}
```

ContextLoaderListener会调用父类ContextLoader的initWebApplicationContext方法来启动ApplicationContext的初始化

```java
public WebApplicationContext initWebApplicationContext(ServletContext servletContext) {
	configureAndRefreshWebApplicationContext(cwac, servletContext);
	servletContext.setAttribute(WebApplicationContext.ROOT_WEB_APPLICATION_CONTEXT_ATTRIBUTE, this.context);
	return this.context;
}
```

上面只抽取了ContextLoader的关键逻辑

1. ApplicationContext被默认放入了ServletContext中，key是个默认值。上一篇提过在获取对应name的filter时会从ServletContext里先得ApplicationContext，再获取对应依赖。
2. configureAndRefreshWebApplicationContext这个方法最终启动了ApplictionContext的最核心refresh，这个方法被放在公共抽象类AbstractApplicationContext里，几乎所有的ApplicationContext都会执行。这个阶段结束后，ApplicationContext也就启动完毕，整个容器的依赖也就完成。因为篇幅太长ApplicationContext启动的细节就不再展开。

## ServletContext的依赖

在容器refresh阶段会通过postProcessBeanFactory这个策略方法对容器对应的BeanFactory做后处理，一般是设置一些beanPostProcessor之类，对于WebApplicationContext则会处理ServletContext的依赖。

```java
protected void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) {
	beanFactory.addBeanPostProcessor(new ServletContextAwareProcessor(this.servletContext, this.servletConfig));
	beanFactory.ignoreDependencyInterface(ServletContextAware.class);
	beanFactory.ignoreDependencyInterface(ServletConfigAware.class);
	//在这里将ServletContext作为一个bean管理起来WebApplicationContextUtils.registerWebApplicationScopes(beanFactory, this.servletContext);
	WebApplicationContextUtils.registerEnvironmentBeans(beanFactory, this.servletContext, this.servletConfig);
}
```

至此，springSessionRepositoryFilter的依赖也注入完毕。

## ServletRequestWrapper

Servlet规范从2.3起引入了ServletRequestWrapper包装类，它把调用交给被包装的ServletRequest来执行。这样就可以对ServletRequest进行扩展。例如Tomcat就是将自己的Request类作为包装类的实体。

```java
public class ServletRequestWrapper implements ServletRequest {

	private ServletRequest request;
	public ServletRequestWrapper(ServletRequest request) {
	    if (request == null) {
	        throw new IllegalArgumentException("Request cannot be null");   
	    }
	    this.request = request;
	}

	public ServletRequest getRequest() {
	    return this.request;
	}

	public Object getAttribute(String name) {
	    return this.request.getAttribute(name);
	}
}
```

为了更好的支持HttpServletRequest，Servlet2.3还支持了提供了HttpServletRequestWrapper，其实现了接口HttpServletRequest，并且继承了ServletRequestWrapper。

```java
public class HttpServletRequestWrapper extends ServletRequestWrapper implements HttpServletRequest {

    public HttpServletRequestWrapper(HttpServletRequest request) {
        super(request);
    }

    private HttpServletRequest _getHttpServletRequest() {
        return (HttpServletRequest) super.getRequest();
    }

	......

}
```

SessionRepositoryRequestWrapper继承了HttpServletWrapper，并覆盖了getSession方法，通过spring自己的策略生成session。

1. Spring定义了新的response wrapper–OnCommittedResponseWrapper，其关联了自实现的字符和字节输出流，并定义了一个模板方法onResponseCommitted，由继承子类来实现。
2. OnCommittedResponseWrapper关联了自己实现的一个字节流和字符流。他们和普通的字节字符流一样也是个包装类。

## 通过SessionRepositoryFilter对Servlet侵入

spring-session通过Filter将自定义的Request wrapper和Response Wrapper侵入到Servlet容器中。
在Tomcat中Servlet的service方法会由ApplicationFilterChain调用，而FilterChain的参数通过Filter的filterChain.doFilter传入。Spring-session通过自定义的全局拦截器SessionRepositoryFilter将request和response侵入进容器。

```java
protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain) throws ServletException, IOException {

	request.setAttribute(SESSION_REPOSITORY_ATTR, sessionRepository);

	SessionRepositoryRequestWrapper wrappedRequest = new SessionRepositoryRequestWrapper(request, response, servletContext);
	SessionRepositoryResponseWrapper wrappedResponse = new SessionRepositoryResponseWrapper(wrappedRequest,response);

	HttpServletRequest strategyRequest = httpSessionStrategy.wrapRequest(wrappedRequest, wrappedResponse);
	HttpServletResponse strategyResponse = httpSessionStrategy.wrapResponse(wrappedRequest, wrappedResponse);

	try {
	    filterChain.doFilter(strategyRequest, strategyResponse);
	} finally {
	    wrappedRequest.commitSession();
	}

}
```

SessionRepositoryFilter继承了OncePerRequestFilter，父类的doFilter最终会将具体的处理逻辑交给子类处理。

```java
abstract class OncePerRequestFilter implements Filter {

	public final void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain) throws ServletException, IOException {
		//alreadyFilteredAttributeName是个静态变量，由类名+.filtered构成
		//这样就能保证同一个类只被调用一次。
		boolean hasAlreadyFilteredAttribute = request.getAttribute(alreadyFilteredAttributeName) != null;

		if (hasAlreadyFilteredAttribute) {
		    filterChain.doFilter(request, response);
		} else {
			request.setAttribute(alreadyFilteredAttributeName, Boolean.TRUE);
			try {
				//通过模板方法，将处理交给子类
				doFilterInternal(httpRequest, httpResponse, filterChain);
			}
		}
	}

}
```

OncePerRequestFilter是用来保证一次完整的拦截链中，同一个类只会被调用一次。
因为spring无法保证同一个Filter类只有一个实例。有可能一个Filter既有可能在web.xml里配置由容器初始化了，还有可能被作为spring的依赖引入进了DelegatingFilterProxy。这样在一次filter chain中就会存在同一个Filter的多个实例。

## Session获取

spring-session实现了HttpServletRequest的子类–SessionRepositoryRequestWrapper，由它覆盖getSession方法，将由web容器处理的逻辑接管过来。

```java
public HttpSession getSession(boolean create) {
	HttpSessionWrapper currentSession = getCurrentSession();
	if(currentSession != null) {
		return currentSession;
	}
	String requestedSessionId = getRequestedSessionId();
	if(requestedSessionId != null) {
		S session = sessionRepository.getSession(requestedSessionId);
		if(session != null) {
			this.requestedSessionIdValid = true;
			currentSession = new HttpSessionWrapper(session, getServletContext());
			//从数据仓库提取出来的session状态不为new
			currentSession.setNew(false);
			setCurrentSession(currentSession);
			return currentSession;
		}
	}
	if(!create) {
		return null;
	}
	S session = sessionRepository.createSession();
	currentSession = new HttpSessionWrapper(session, getServletContext());
	setCurrentSession(currentSession);
	return currentSession;
}
```

session id存在的情况下，如果通过session id能找到持久化的session就直接返回，否则根据create是否为true决定是否新建一个Session。Session在后续的使用中会陆续更改，添加、删除或者修改属性值。从数据仓库中提取出来的session会默认修改lastAccessTime属性以避免session失效。
数据仓库中的持久化session也存在失效时间，消费端可以通过RedisHttpSessionConfiguration.setMaxInactiveIntervalInSeconds(long timemills)来设置，默认是1800秒。Redis会定时清除过期数据。

## spring-session的Session实现

Spring为了将Session与具体的协议分开，单独提炼出Session实体，再通过HttpSessionWrapper将session包装起来，从而扩展HttpSession.如果以后还需要支持另外一种应用协议，就只要增加一种应用类型的wrapper就行了。

```java
private final class HttpSessionWrapper implements HttpSession {

	public HttpSessionWrapper(S session, ServletContext servletContext) {
		this.session = session;
		this.servletContext = servletContext;
	}
	//省略了大部分方法，都是委托给被包装的Session处理的

	//对session坐invalidate时去数据仓库删掉对应的数据
	public void invalidate() {
		checkState();
		this.invalidated = true;
		requestedSessionInvalidated = true;
		setCurrentSession(null);
		sessionRepository.delete(getId());
	}

	public void setNew(boolean isNew) {
		this.old = !isNew;
	}

	public boolean isNew() {
		checkState();
		return !old;
	}

}
```

Session实体主要定义通用的getAttribute和setAttribute等方法。此外扩展了一个ExpiringSession，这是spring的默认session，它主要用来判断session是否失效。

```java
public interface ExpiringSession extends Session {

    //session的创建时间
    long getCreationTime();

    //session的上次访问时间
    long getLastAccessedTime();

    //设置最大访问间隔，超过这个间隔session会被invalidate
    void setMaxInactiveIntervalInSeconds(int interval);

    int getMaxInactiveIntervalInSeconds();

    //session是否失效
    boolean isExpired();

}
```

session提交

```java
private void commitSession() {
    HttpSessionWrapper wrappedSession = getCurrentSession();
    if(wrappedSession == null) {
        if(isInvalidateClientSession()) {
            httpSessionStrategy.onInvalidateSession(this, response);
        }
    } else {
        S session = wrappedSession.session;
        sessionRepository.save(session);
        if(!isRequestedSessionIdValid() || !session.getId().equals(getRequestedSessionId())) {
            httpSessionStrategy.onNewSession(session, this, response);
        }
    }
}
```

1. getSession时会通过setCurrentSession把新建或者提取出来的session放到request的HttpServletRequestWrapper.class.getName()属性上；相应的在session invalidate时也会将session从request里移除掉，此时通过getCurrentSession将取不到数据。
2. 在做session持久化时，会首先判断session是否被invalidate，如果是则会删除

## session通用策略

Session在浏览器通常是通过cookie保存的，cookie里保存了jessionid，代表用户的session id。一个访问路径只有一个session cookie（事实上在客户端就只有一个cookie，jsessionid是作为cookie值的一部分，这里把cookie抽象成类似服务器端的实现），也就是一个访问路径在一个浏览器上只有一个session，这是绝大多数容器对session的实现。而spring却可以支持单浏览器多用户session。下面就看看spring是怎样去支持多用户session的。
spring session通过增加session alias概念来实现多用户session，每一个用户都映射成一个session alias。当有多个session时，spring会生成“alias1 sessionid1 alias2 sessid2…….”这样的cookie值结构。
spring session提交时如果有新session生成，会触发onNewSession动作生成新的session cookie

```java
public void onNewSession(Session session, HttpServletRequest request, HttpServletResponse response) {
	Set<String> sessionIdsWritten = getSessionIdsWritten(request);
	if(sessionIdsWritten.contains(session.getId())) {
	    return;
	}
	sessionIdsWritten.add(session.getId());
	Map<String,String> sessionIds = getSessionIds(request);
	String sessionAlias = getCurrentSessionAlias(request);
	sessionIds.put(sessionAlias, session.getId());
	Cookie sessionCookie = createSessionCookie(request, sessionIds);
	response.addCookie(sessionCookie);
}
```

* 确保已经存在cookie里的session不会再被处理。
* 生成一个包含所有alias的session id的map，并通过这个map构造新的session cookie值。

createSessionCookie会根据一个alias-sessionid的map去构造session cookie。

```java
private Cookie createSessionCookie(HttpServletRequest request, Map<String, String> sessionIds) {

	//cookieName是"SESSION"，spring的session cookie都是
	//以"SESSION"命名的
	Cookie sessionCookie = new Cookie(cookieName,"");
	//省略部分非关键逻辑

	if(sessionIds.isEmpty()) {
	    sessionCookie.setMaxAge(0);
	    return sessionCookie;
	}

	if(sessionIds.size() == 1) {
	    String cookieValue = sessionIds.values().iterator().next();
	    sessionCookie.setValue(cookieValue);
	    return sessionCookie;
	}
	StringBuffer buffer = new StringBuffer();
	for(Map.Entry<String,String> entry : sessionIds.entrySet()) {
	    String alias = entry.getKey();
	    String id = entry.getValue();

	buffer.append(alias);
	buffer.append(" ");
	buffer.append(id);
	buffer.append(" ");
	}
	buffer.deleteCharAt(buffer.length()-1);

	sessionCookie.setValue(buffer.toString());
	return sessionCookie;

}
```

* 当session被invalidate，可能会存在seesionids为空的情况，这种情况下将session cookie的最大失效时间设成立即。
* 如果只有一个session id，则和普通session cookie一样处理，cookie值就是session id。
* 如果存在多个session id，则生成前文提到的session cookie值结构。

## session cookie的获取

getSessionIds方法会取出request里的session cookie值，并且对每种可能的值结构进行相应的格式化生成一个key-value的map。

```java
public Map<String,String> getSessionIds(HttpServletRequest request) {
    Cookie session = getCookie(request, cookieName);
    String sessionCookieValue = session == null ? "" : session.getValue();
    Map<String,String> result = new LinkedHashMap<String,String>();
    StringTokenizer tokens = new StringTokenizer(sessionCookieValue, " ");
    //单用户cookie的情况
    if(tokens.countTokens() == 1) {
        result.put(DEFAULT_ALIAS, tokens.nextToken());
        return result;
    }
    while(tokens.hasMoreTokens()) {
        String alias = tokens.nextToken();
        if(!tokens.hasMoreTokens()) {
            break;
        }
        String id = tokens.nextToken();
        result.put(alias, id);
    }
    return result;
}
```

1. 对单用户session cookie的处理，只取出值，默认为是默认别名（默认为0）用户的session。
2. 对多用户，则依据值结构的格式生成alias-sessionid的map。
3. 以上两种格式化都是对创建session的逆操作。

getCurrentSessionAlias用来获取当前操作用户。可以通过在request里附加alias信息，从而让spring可以判断是哪个用户在操作。别名是通过”alias name=alias”这样的格式传入的，alias name默认是_s，可以通过setSessionAliasParamName(String)方法修改。我们可以在url上或者表单里添加”_s=your user alias”这样的形式来指明操作用户的别名。如果不指明用户别名，则会认为是默认用户，可以通过setSessionAliasParamName(null)取消别名功能。

```java
public String getCurrentSessionAlias(HttpServletRequest request) {
    if(sessionParam == null) {
        return DEFAULT_ALIAS;
    }
    String u = request.getParameter(sessionParam);
    if(u == null) {
        return DEFAULT_ALIAS;
    }
    if(!ALIAS_PATTERN.matcher(u).matches()) {
        return DEFAULT_ALIAS;
    }
    return u;
}
```

## 触发session提交

spring会通过两个方面确保session提交：

* response提交，主要包括response的sendRedirect和sendError以及其关联的字节字符流的flush和close方法。

```java
abstract class OnCommittedResponseWrapper extends HttpServletResponseWrapper {
    public OnCommittedResponseWrapper(HttpServletResponse response) {
        super(response);
    }
    /**
     * Implement the logic for handling the {@link javax.servlet.http.HttpServletResponse} being committed
     */
    protected abstract void onResponseCommitted();

    @Override
    public final void sendError(int sc) throws IOException {
        doOnResponseCommitted();
        super.sendError(sc);
    }
    //sendRedirect处理类似sendError
    @Override
    public ServletOutputStream getOutputStream() throws IOException {
        return new SaveContextServletOutputStream(super.getOutputStream());
    }

    @Override
    public PrintWriter getWriter() throws IOException {
        return new SaveContextPrintWriter(super.getWriter());
    }

    private void doOnResponseCommitted() {
        if(!disableOnCommitted) {
            onResponseCommitted();
            disableOnResponseCommitted();
        } else if(logger.isDebugEnabled()){
            logger.debug("Skip invoking on");
        }
    }

    private class SaveContextPrintWriter extends PrintWriter {
        private final PrintWriter delegate;

        public SaveContextPrintWriter(PrintWriter delegate) {
            super(delegate);
            this.delegate = delegate;
        }

        public void flush() {
            doOnResponseCommitted();
            delegate.flush();
        }
	//close方法与flush方法类似
    }
	//SaveContextServletOutputStream处理同字符流
}
```

onResponseCommitted的实现由子类SessionRepositoryResponseWrapper提供

```java
private final class SessionRepositoryResponseWrapper extends OnCommittedResponseWrapper {

    private final SessionRepositoryRequestWrapper request;
    /**
     * @param response the response to be wrapped
     */
    public SessionRepositoryResponseWrapper(SessionRepositoryRequestWrapper request, HttpServletResponse response) {
        super(response);
        if(request == null) {
            throw new IllegalArgumentException("request cannot be null");
        }
        this.request = request;
    }

    @Override
    protected void onResponseCommitted() {
        request.commitSession();
    }

}
```

response提交后触发了session提交。

* SessionRespositoryFilter

仅仅通过response提交时触发session提交并不能完全保证session的提交，有些情况下不会触发response提交，比如对相应资源的访问没有servlet处理，这种情况就需要通过全局filter做保证。

```java
protected void doFilterInternal(HttpServletRequest request, HttpServletResponse response, FilterChain filterChain) throws ServletException, IOException {
	try {
		//省略
		//filterChain会在所有filter都执行完毕后调用对应的servlet
	    filterChain.doFilter(strategyRequest, strategyResponse);
	} finally {
		//所有的处理都完成后提交session
		wrappedRequest.commitSession();
	}
}
```
