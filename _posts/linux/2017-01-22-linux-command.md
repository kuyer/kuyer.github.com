---
layout: detail
permalink: /linux/command
title: Linux命令介绍和使用
category: linux
cover: /images/linux/command.jpg
tags: [Linux,Linux命令,Linux压缩解压命令]
description: 介绍Linux操作系统基础命令的使用方式，包括tar等命令
published: true
---

# Linux命令介绍和使用

介绍Linux操作系统基础命令的使用方式，包括tar等命令

## tar命令

### tar压缩
```bash
tar -zcvf mytest.tar.gz mytest # 压缩
```

### tar解压
```bash
tar -zxvf mytest.tar.gz # 解压
```

## Linux访问控制

/etc/hosts.allow和/etc/hosts.deny这两个是控制远程访问配置文件，通过它们可以允许或者拒绝某个IP或者IP段的客户端访问Linux的某项服务。比如SSH服务

* 修改/etc/hosts.allow文件

```bash
#
# hosts.allow
#		This file contains access rules which are used to
#		allow or deny connections to network services that
#		either use the tcp_wrappers library or that have been
#		started through a tcp_wrappers-enabled xinetd.
#
#		See 'man 5 hosts_options' and 'man 5 hosts_access'
#		for information on rule syntax.
#		See 'man tcpd' for information on tcp_wrappers
#
sshd:192.168.56.*:allow
sshd:121.27.18.*:allow
all:121.27.19.21
```

> 以上写法表示允许192.168.56和121.27.18两个ip段的客户端连接sshd服务，允许121.27.19.21的客户端连接所有的服务


