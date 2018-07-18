---
layout: detail
permalink: /linux/gnome/eclipse
title: Docker
category: linux
cover: /images/linux/gnome/eclipse.jpg
tags: [linux,gnome,eclipse]
description: gnome3
published: true
---

# gnome3桌面环境

最新的ubuntu18、fedora及centos7等操作系统的桌面环境都使用gnome3。当使用eclipse开发工具时，如何在gnome3上创建eclipse的快捷方式呢？
因为eclipse是无需安装的，下载eclipse后，解压后，配置好相应的环境变量后就可以使用了。

## 解压eclipse

```
tar -zxvf /root/eclipse-jee-photon-R-linux-gtk-x86_64.tar.gz -C /usr/local/
```

eclipse的位置为：/usr/local/eclipse

## 创建eclipse快捷方式

添加/usr/share/applications/eclipse.desktop文件，内容为：

```
[Desktop Entry]
Type=Application
Name=Eclipse
Exec=/usr/local/eclipse/eclipse
Icon=/usr/local/eclipse/icon.xpm
Encoding=UTF-8
StartupNotify=true
```

保存后，即可在应用里看到Eclipse的图标
