# Bower 前端包管理工具

bower是twitter推出的一款包管理工具，基于nodejs的模块化思想，把功能分散到各个模块中，让模块和模块之间存在联系，通过bower来管理模块间的这种联系。

* 官方：http://bower.io

目前bower已标记过时，推荐使用yarn

## 安装

npm install bower -g

## 命令

* bower -v	查看bower安装版本
* bower init	初始化bower，会生成一个bower.json
* bower help	显示Bower帮助文档
* bower home moduleName#version	通过浏览器打开一个包的github发布页
* bower install moduleName --save	安装最新模块
* bower install moduleName#version --save	安装指定版本模块
* bower uninstall moduleName	卸载模块
* bower info moduleName	查看指定模块信息
* bower update moduleName	更新模块
* bower search moduleName	搜索模块
* bower cache list	列出缓存模块情况
* bower cache list moduleName	列出指定模块名称的缓存情况
* bower cache clean	删除所有模块的缓存
* bower cache clean moduleName	删除指定模块的所有缓存
* bower cache clean moduleName#version	删除指定模块的指定版本缓存
