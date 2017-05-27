---
layout: detail
permalink: /spring/rest/template
title: Spring RestTemplate
category: spring
cover: /images/spring/rest/template.jpg
tags: [Spring,RestTemplate]
description: Spring在Servlet3.x的基础上，实现了多服务的Session共享问题，本文在源码的基础上解读
published: true
---

# Spring RestTemplate

```java
SimpleClientHttpRequestFactory requestFactory = new SimpleClientHttpRequestFactory();
requestFactory.setConnectTimeout(1000);
requestFactory.setReadTimeout(1000);
RestTemplate restTemplate = new RestTemplate(requestFactory);
```

## Post

* 接口

```java
public <T> T postForObject(String url, Object request, Class<T> responseType, Object... uriVariables) throws RestClientException
public <T> T postForObject(String url, Object request, Class<T> responseType, Map<String, ?> uriVariables) throws RestClientException
public <T> T postForObject(URI url, Object request, Class<T> responseType) throws RestClientException
```

* 常用

```java
MultiValueMap<String, String> bodyMap = new LinkedMultiValueMap<String, String>();
bodyMap.setAll(urlVariables);
ResponseClass responseClass = restTemplate.postForObject(CAR_CES_URL, bodyMap, ResponseClass.class);
```

```java
HttpHeaders headers = new HttpHeaders();
headers.add("X-Auth-Token", "e348bc22-5efa-4299-9142-529f07a18ac9");
MultiValueMap<String, String> postParameters = new LinkedMultiValueMap<String, String>();
postParameters.add("owner", "11");
postParameters.add("subdomain", "aoa");
postParameters.add("comment", "");
HttpEntity<MultiValueMap<String, String>> requestEntity  = new HttpEntity<MultiValueMap<String, String>>(postParameters, headers);
ParseResultVo exchange = null;
try {
	exchange = restTemplate.postForObject("http://l-dnsutil1.ops.beta.cn6.qunar.com:10085/v1/cnames/tts.piao",  requestEntity, ParseResultVo.class);
	logger.info(exchange.toString());
} catch (RestClientException e) {
	logger.info("。。。。");
}
```

```java
DomainParam domainParam = new DomainParam();
domainParam.setCustomerId(1);
//...
logger.info("....");
restTemplate.getMessageConverters().add(new MappingJacksonHttpMessageConverter());
restTemplate.getMessageConverters().add(new StringHttpMessageConverter());
String responseResult = restTemplate.postForObject(url, domainParam, String.class);   
```
