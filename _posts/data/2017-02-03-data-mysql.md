---
layout: detail
permalink: /data/mysql
title: MySQL介绍和使用
category: data
cover: /images/data/mysql.jpg
tags: [数据库,MySQL,MySQL找回密码,MySQL权限]
description: 本文介绍了MySQL的用户权限，用户密码的设置和找回方式
published: true
---
# MySQL介绍和使用

本文介绍了MySQL的*用户权限*，*用户密码的设置和找回*方式

## 基本配置

```
[mysql]
default-character-set=utf8 # 设置客户端默认字符集

[mysqld]
port=3306
basedir=/usr/local/mysql
datadir=/var/data/mysql
character-set-server=utf8 # 设置服务端默认字符集
```

## 查看连接数

```sql
# 查看连接，如果是root用户，可以看到所有的连接；普通用户只能看到自己的连接
show processlist;

# 查看全部连接
show full processlist;

# 查看最大连接数
show variables like 'max_connections';

# 修改最大连接数
set global max_connections=500;
```

配置文件（my.cnf）修改方式：

```
[mysqld]
max_connections=500
```

## MySQL找回root用户密码

* 修改my.cnf文件，在[mysqld]进程配置文件中，增添：**skip-grant-tables**，保存并重启MySQL服务。

* 运行：${MYSQL_HOME}/bin/mysql -p，提示输入密码是点回车键直接进入，修改root密码：

```sql
update mysql.user set password=password('rootpassword') where user='root'
```

* 修改my.cnf文件，在[mysqld]进程配置文件中，删除：**skip-grant-tables**，保存并重启MySQL服务。

* 运行：${MYSQL_HOME}/bin/mysql -u root -p rootpassword; 重新登录MySQL即可

## MySQL用户及权限

* 创建用户

```sql
create user 'username'@'host' idendified by 'password';
create user 'test1'@'%' idendified by '111111';
```

* 设置用户密码

```sql
set password for 'test1'@'%'=password('222222');
```

* 授权

```sql
grant privileges on databasename.tablename to 'username'@'host';
-- 给test1用户赋dbtest1数据库中tuser表的新增、修改及查询权限
grant insert,update,select on dbtest1.tuser to 'test1'@'%';
-- 给test1用户赋dbtest1数据库的所有权限
grant all on dbtest1.* to 'test1'@'%';
-- 设置该用户可以给其它用户授权
grant privileges on databasename.tablename to 'username'@'host' with grant option;
```

* 查看用户权限

```sql
show grants for 'test1'@'%';
```

* 撤销用户权限

```sql
revoke privilege on databasename.tablename from 'username'@'host';
revoke insert,update on dbtest1.* from 'test1'@'%';
```

* 删除用户

```sql
drop user 'username'@'host';
```

## 字符串函数

* 从左截取字符串

left(str, length)

```sql
select left(c_name, 2) from t_user;
```

* 从右截取字符串

right(str, length)

```sql
select right(c_name, 2) from t_user;
```

* 截取字符串

substring(str, postion, length)

```sql
select substring(c_name, 2, 2) from t_user;
```

* 按关键字截取字符串

substring_index(str, delim, count)

```sql
select substring_index(c_name, '.', 2) from t_user;
```

## Windows7下安装MySQL服务

```bash
mysqld --install MySQL --defaults-file="D:\Develop\MySQL\mysql-5.6.32-winx64\my.ini"
```
