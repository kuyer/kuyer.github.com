---
layout: detail
permalink: /java/logback
title: Logback使用指南
category: java
cover: /images/java/logback.jpg
tags: Java日志,Logback
description: 本文介绍了Logback的配置和使用方式
published: true
---

# Logback

本文介绍了Logback的配置和使用方式

## configuration
* scan 当此属性设置为true时，配置文件如果发生改变，将会被重新加载，默认值为true
* scanPeriod 设置监测配置文件是否有修改的时间间隔，如果没有给出时间单位，默认单位是毫秒。当scan为true时，此属性生效。默认的时间间隔为1分钟
* debug 当此属性设置为true时，将打印出logback内部日志信息，实时查看logback运行状态。默认值为false

```xml
<configuration scan="true" scanPeriod="60 seconds" debug="false">
	<!-- 其它配置 -->
</configuration>
```

## contextName
> contextName，即上下文名称。每个logger都关联到logger上下文，默认上下文名称为“default”。但可以使用<contextName>设置成其他名字，用于区分不同应用程序的记录。一旦设置，不能修改。

```xml
<configuration>
	<contextName>logapp</contextName>
</configuration>
```

## property
> 用来定义变量值的标签，<property> 有两个属性，name和value；其中name的值是变量的名称，value的值时变量定义的值。通过<property>定义的值会被插入到logger上下文中。定义变量后，可以使“${}”来使用变量。

```xml
<configuration>
	<property name="appname" value="logapp" />
	<contextName>${appname}</contextName>
</configuration>
```

## timestamp
> timestamp有两个属性，key标示，datePattern设置将当前时间（解析配置文件的时间）转换为字符串的模式，遵循Java.text.SimpleDateFormat的格式。
