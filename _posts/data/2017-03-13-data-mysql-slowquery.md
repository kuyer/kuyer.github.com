---
layout: detail
permalink: /data/mysql/slowquery
title: MySQL慢查询
category: data
cover: /images/data/mysql/slowquery.jpg
tags: [Linux,CentOS,MySQL]
description: 开启MySQL慢查询日志，可以让MySQL记录下查询超过指定时间的语句，通过定位分析性能的瓶颈，才能更好的优化数据库系统的性能。装MySQL5.7.x
published: true
---
# MySQL慢查询

开启MySQL慢查询日志，可以让MySQL记录下查询超过指定时间的语句，通过定位分析性能的瓶颈，才能更好的优化数据库系统的性能。

## 查看MySQL慢查询状态

```sql
show variables like 'slow_query%';
```

```
Variable_name        Value                                         
-------------------  ----------------------------------------------
slow_query_log       OFF                                           
slow_query_log_file  /var/mysql/data/localhost-slow.log 
```

* slow_query_log 慢查询开启状态 OFF未开启 ON已开启

* slow_query_log_file 蛮查询日志存放位置

```sql
show variables like 'long_query%';
```

```
Variable_name    Value      
---------------  -----------
long_query_time  10.000000  
```

* long_query_time 查询超过多少秒才记录，默认10秒

## 临时修改慢查询配置

临时修改的缺点是，MySQL服务重启后失效

```sql
-- 1表示开启，0表示关闭
set global slow_query_log=1
-- 查询
show variables like 'slow_query%';
```

```
Variable_name        Value                                         
-------------------  ----------------------------------------------
slow_query_log       ON                                            
slow_query_log_file  /var/mysql/data/localhost-slow.log 
```

## 永久修改慢查询配置

修改配置文件：/etc/my.cnf，在[mysqld]下增加：

```
slow_query_log=1
slow_query_log_file=/var/mysql/data/slowquery.log
long_query_time=1
```

* slow_query_log 慢查询开启状态：1表示开启，0表示关闭

* slow_query_log_file 慢日志存放的路径

* long_query_time 查询超过多少秒才记录到慢查询日志中，默认10秒

修改完成后，重启MySQL服务

## 测试慢查询

* 执行慢查询操作

```sql
select sleep(12);
```

* 查看慢查询日志

cat /var/mysql/data/slowquery.log

```
Time                 Id Command    Argument
# Time: 170605  9:25:09
# User@Host: root[root] @ localhost [::1]  Id:    12
# Query_time: 12.066000  Lock_time: 0.000000 Rows_sent: 1  Rows_examined: 0
use test;
SET timestamp=1496625909;
select sleep(12);
```

* 查询慢查询记录数

```sql
show global status like '%slow_queries%';
```

```
Variable_name  Value   
-------------  --------
Slow_queries   1       
```

## 慢查询分析工具

mysqldumpslow
