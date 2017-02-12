---
layout: detail
permalink: /linux/introduce
title: Linux命令介绍和使用
category: linux
cover: /images/linux/introduce.jpg
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

## 网络配置

### Ubuntu环境：

> 修改配置文件：

```sh
vim /etc/network/interfaces
```

```sh
auto eth0
iface ech0 inet static
address 192.168.56.2
netmask 255.255.255.0
geteway 192.168.56.1
```

> 重启网络配置

```sh
/etc/init.d/networking restart
```

> 修改DNS配置：

```sh
vim /etc/resolv.conf
```

```sh
nameserver 202.103.224.68
```


### CentOS 环境

* VirtualBox虚拟机全局网络设置

```sh
Adapter：
192.168.56.1
255.255.255.0
DHCP Server:
192.168.56.100
255.255.255.0
192.168.56.101
192.168.56.254
```

> 启动两块网卡：

Adapter1: Host-only 用于主宿机通过192.168.56.x互访，主机是否能上网不影响双方通信
Adapter2: NAT 宿机用这一网卡通过主机上网


* 进入到/etc/sysconfig/network-scripts目录，修改网卡信息

> 通过vi修改ifcfg-eth0文件，即修改第一块网卡信息

```sh
DEVICE=eth0 #第一块网卡
ONBOOT=yes #开机启动
BOOTPROTO=none #启动协议，none/dhcp
DNS=192.168.56.1 # 配置DNS
PEERDNS=yes #设置了DNS指令后是否自动修改/etc/resolv.conf文件，启动协议为dhcp时此项默认yes
IPADDR=192.168.56.110 #设置IP地址
NETMASK=255.255.255.0 #掩码
GATEWAY=192.168.56.1 #网关
HWADDR=08:00:27:75:AD:13 #网卡设备的物理地址，默认，与VirtualBox网络设置界面一致
USERCTL=no #非root用户不能控制此设备
```


> 通过vi修改ifcfg-eth1文件，即修改第二块网卡信息

```sh
DEVICE=eth1
ONBOOT=yes
BOOTPROTO=dhcp
PEERDNS=yes
HWADDR=08:00:27:90:9B:08
USERCTL=no
```

> 重启网络

```sh
service network restart
```

* 在VirtualBox中复制虚拟机并重新初始化MAC地址，配置网络的时候会出现Device eth0 does not seem to be present, delaying initialization的错误，
其解决方式为：

> 删除/etc/udev/rules.d/70-persistent-net.rules这个文件
> 70-persistent-net.rules这个文件确定了网卡与MAC地址的绑定，删除后重启电脑，会生成新的70-persistent-net.rule文件
> 修改/etc/sysconfig/network-scripts/ifcfg-eth0文件，并修改MAC地址后重启网络：service network restart即可

* 当ping 网络出现 unknown host 时

```sh
service network restart
Shutting down interface eth0:  Device state: 3 (disconnected)
Shutting down interface eth1:  Error: Device 'eth1' (/org/freedesktop/NetworkManager/Devices/0)
disconnecting failed: This device is not active
```

原因可能是network与NetworkManager有冲突，NetworkManger是用户在登录后，设置IP地址的一个工具，解决方式：

```sh
service NetworkManger stop
chkconfig NetworkManager off
```

修改主机名

```sh
vim /etc/sysconfig/network
```

修改网络名

```sh
vim /etc/hosts
```

## xshell连接太慢解决方式

> vim /etc/ssh/sshd_config

```sh
UsePAM yes UseDNS yes
```

将UseDNS yes修改为：UseDNS no

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
