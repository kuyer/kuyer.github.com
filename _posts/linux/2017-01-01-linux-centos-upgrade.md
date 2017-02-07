---
layout: detail
permalink: /linux/centos/upgrade
title: CentOS版本从6.x升级到7.x
category: linux
cover: /images/centos/upgrade.jpg
tags: Linux,CentOS,CentOS升级
description: 本文介绍了CentOS版本从6.x升级到7.x的操作过程。
published: true
---

# CentOS版本从6.x升级到7.x

本文介绍了CentOS版本从6.x升级到7.x的操作过程。

## 安装升级工具依赖的软件包

```bash
yum -y install openscap pcre-devel libxml2-devel libxslt-devel m2crypto python-simplejson mod_wsgi
```

## 安装以下软件的最新版

下载地址：http://dev.centos.org/centos/6/upg/x86_64/Packages

```bash
rpm -ihv http://dev.centos.org/centos/6/upg/x86_64/Packages/preupgrade-assistant-1.0.2-36.0.1.el6.centos.x86_64.rpm
rpm -ihv http://dev.centos.org/centos/6/upg/x86_64/Packages/preupgrade-assistant-contents-0.5.14-1.el6.centos.noarch.rpm
rpm -ihv http://dev.centos.org/centos/6/upg/x86_64/Packages/preupgrade-assistant-ui-1.0.2-36.0.1.el6.centos.x86_64.rpm
rpm -ihv http://dev.centos.org/centos/6/upg/x86_64/Packages/redhat-upgrade-tool-0.7.22-3.el6.centos.noarch.rpm
```

## 执行升级

```bash
redhat-upgrade-tool --network 7.0 --instrepo http://mirrors.aliyun.com/centos/7.2.1511/os/x86_64/ --force
```

> 升级成功后会看到消息“Finished. Reboot to start upgrade.”

## 重启服务器

重启主机后，OS将启动一个称为System Upgrade的新的grub对象，支持将所有已经通过upgrade tool下载的软件包更新。

## 检查系统版本

```bash
cat /etc/redhat-release
```

> 看到版本 CentOS Linux release 7.0.1406 (Core) 证明升级成功
