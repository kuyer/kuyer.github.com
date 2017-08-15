
# NPM Node项目管理工具

## 什么是NPM

NPM，是Node Package Manager的简称，它的主要功能是安装、卸载、更新、查看、搜索和发布等Node包。NPM基于CouchDB数据库，记录了每个包的信息，包括作者、版本、依赖等。

## 常用命令

* npm -v 查看版本
* npm help 查看帮助文档
* npm list 查看当前项目下已经安装的模块列表
* npm init 初始化项目，生成package.json
* npm install 根据package.json里面的依赖关系进行安装
* npm install moduleName 安装模块
* npm install moduleName@version 安装模块的指定版本
* npm install moduleName --save 本地安装模块
* npm install moduleName --save-dev 本地安装开发用的模块
* npm install moduleName@version --save 本地安装指定版本的模块
* npm install moduleName@version --save-dev 本地安装开发用的指定版本的模块
* npm install moduleName -g 全局安装模块
* npm uninstall moduleName 卸载模块
* npm update moduleName 更新模块
* npm root 查看模块安装路径
* npm root -g 查看全局安装路径
* npm config get registry 查看NPM镜像代理
* npm config set registry https://registry.npm.taobao.org 设置NPM镜像代理
* npm config get prefix 查看全局模块路径
* npm config set prefix "D:\Develop\NodeJS\Globals" 设置全局模块路径
* npm config get cache 查看缓存路径
* npm config set cache "D:\Develop\NodeJS\Caches" 设置缓存路径
