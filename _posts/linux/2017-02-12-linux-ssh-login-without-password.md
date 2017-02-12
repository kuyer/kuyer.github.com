---
layout: detail
permalink: /linux/ssh/login-without-password
title: SSH免密码登录
category: linux
cover: /images/linux/ssh/login-without-password.jpg
tags: [Linux,SSH]
description: SSH是一个专为远程登录会话和其他网络服务提供安全性的协议。默认状态下SSH链接是需要密码认证的，可以通过添加系统认证（即公钥-私钥）的修改，修改后系统间切换可以避免密码输入和SSH认证。
published: true
---

# SSH免密码登录

## 什么是SSH

SSH是一个专为远程登录会话和其他网络服务提供安全性的协议。默认状态下SSH链接是需要密码认证的，可以通过添加系统认证（即公钥-私钥）的修改，修改后系统间切换可以避免密码输入和SSH认证。

## 什么是公钥认证

对信息的加密和解密采用不同的key，这对key分别称作private key和public key，其中，public key存放在欲登录的服务器上，而private key为特定的客户机所持有。
当客户机向服务器发出建立安全连接的请求时，首先发送自己的public key，如果这个public key是被服务器所允许的，服务器就发送一个经过public key加密的随机数据给客户机，这个数据只能通过private key解密，客户机将解密后的信息发还给服务器，服务器验证正确后即确认客户机是可信任的，从而建立起一条安全的信息通道。
通过这种方式，客户机不需要向外发送自己的身份标志“private key”即可达到校验的目的，并且private key是不能通过public key反向推断出来的。这避免了网络窃听可能造成的密码泄露。客户机需要小心的保存自己的private key，以免被其他人窃取，一旦这样的事情发生，就需要各服务器更换受信的public key列表。

## 机器分布

| 机器名         |       h1       |       h2       |       h3       |       h4       |       h5       |
|:---------------|:--------------:|:--------------:|:--------------:|:--------------:|:--------------:|
| IP地址         | 192.168.56.111 | 192.168.56.112 | 192.168.56.113 | 192.168.56.114 | 192.168.56.115 |
| 用户           | rory           | rory           | rory           | rory           | rory           |

* 设置机器名，如进入到：192.168.56.111机器，vim /etc/hostname，则内容为：

```sh
h1
```

* 修改hosts文件，如进入到：192.168.56.111机器，vim /etc/hosts，则内容为：

```sh
192.168.56.111    h1
192.168.56.112    h2
192.168.56.113    h3
192.168.56.114    h4
192.168.56.115    h5
```

## 免密登录实现

### 创建公钥

在h1、h2、h3、h4、h5机器上分别执行如下命令：

```sh
ssh-keygen -t rsa
```

> ssh-keygen:生成秘钥，其中：-t指定算法； -f 指定生成秘钥路径；-N 指定密码

执行完成后，会在生成id_rsa（私钥）和id_rsa.pub（公钥）两个文件。

### 将id_rsa.pub公钥文件传输到h1~h5机器上

h1机器：

```sh
scp /home/rory/.ssh/id_rsa.pub rory@h1 /home/rory/.ssh/authorized_keys.h1
scp /home/rory/.ssh/id_rsa.pub rory@h2 /home/rory/.ssh/authorized_keys.h1
scp /home/rory/.ssh/id_rsa.pub rory@h3 /home/rory/.ssh/authorized_keys.h1
scp /home/rory/.ssh/id_rsa.pub rory@h4 /home/rory/.ssh/authorized_keys.h1
scp /home/rory/.ssh/id_rsa.pub rory@h5 /home/rory/.ssh/authorized_keys.h1
```

h2机器：

```sh
scp /home/rory/.ssh/id_rsa.pub rory@h1 /home/rory/.ssh/authorized_keys.h2
scp /home/rory/.ssh/id_rsa.pub rory@h2 /home/rory/.ssh/authorized_keys.h2
scp /home/rory/.ssh/id_rsa.pub rory@h3 /home/rory/.ssh/authorized_keys.h2
scp /home/rory/.ssh/id_rsa.pub rory@h4 /home/rory/.ssh/authorized_keys.h2
scp /home/rory/.ssh/id_rsa.pub rory@h5 /home/rory/.ssh/authorized_keys.h2
```

h3机器：

```sh
scp /home/rory/.ssh/id_rsa.pub rory@h1 /home/rory/.ssh/authorized_keys.h3
scp /home/rory/.ssh/id_rsa.pub rory@h2 /home/rory/.ssh/authorized_keys.h3
scp /home/rory/.ssh/id_rsa.pub rory@h3 /home/rory/.ssh/authorized_keys.h3
scp /home/rory/.ssh/id_rsa.pub rory@h4 /home/rory/.ssh/authorized_keys.h3
scp /home/rory/.ssh/id_rsa.pub rory@h5 /home/rory/.ssh/authorized_keys.h3
```

h4机器：

```sh
scp /home/rory/.ssh/id_rsa.pub rory@h1 /home/rory/.ssh/authorized_keys.h4
scp /home/rory/.ssh/id_rsa.pub rory@h2 /home/rory/.ssh/authorized_keys.h4
scp /home/rory/.ssh/id_rsa.pub rory@h3 /home/rory/.ssh/authorized_keys.h4
scp /home/rory/.ssh/id_rsa.pub rory@h4 /home/rory/.ssh/authorized_keys.h4
scp /home/rory/.ssh/id_rsa.pub rory@h5 /home/rory/.ssh/authorized_keys.h4
```

h5机器：

```sh
scp /home/rory/.ssh/id_rsa.pub rory@h1 /home/rory/.ssh/authorized_keys.h5
scp /home/rory/.ssh/id_rsa.pub rory@h2 /home/rory/.ssh/authorized_keys.h5
scp /home/rory/.ssh/id_rsa.pub rory@h3 /home/rory/.ssh/authorized_keys.h5
scp /home/rory/.ssh/id_rsa.pub rory@h4 /home/rory/.ssh/authorized_keys.h5
scp /home/rory/.ssh/id_rsa.pub rory@h5 /home/rory/.ssh/authorized_keys.h5
```

### 将h1~h5机器的公钥信息添加到每台机器的authorized_keys文件中

> 在h1~h5机器上执行以下命令：

```sh
# 进入到.ssh文件夹中
cd /home/rory/.ssh
# 创建authorized_keys文件
touch authorized_keys
# 将authorized_keys.h1 ~ authorized_keys.h5文件的内容复制到authorized_keys文件中
cat authorized_keys.h1 >> authorized_keys
cat authorized_keys.h2 >> authorized_keys
cat authorized_keys.h3 >> authorized_keys
cat authorized_keys.h4 >> authorized_keys
cat authorized_keys.h5 >> authorized_keys
# 删除authorized_keys.h1 ~ authorized_keys.h5文件
rm -rf authorized_keys.h*
# 设置只有rory用户才有authorized_keys文件的读写权限
chmod 600 authorized_keys
# 设置只有rory用户才有.ssh目录的读写执行权限
chmod 700 /home/rory/.ssh
```

### 测试

进入到h1机器，执行：ssh h2，则进入h2机器，同理进入到其它几台机器，在ssh登录的过程中，不需要输入密码，表明免密登录设置成功。
