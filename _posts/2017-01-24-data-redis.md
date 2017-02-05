---
layout: detail
permalink: /data/redis
title: Redis的介绍和使用
category: data
cover: /images/data/redis.jpg
tags: 缓存,Redis
description: 本文介绍了Redis缓存数据库的使用方式和问题的解决
published: true
---

# Redis的介绍和使用

本文介绍了Redis缓存数据库的使用方式和问题的解决

## redis-cli连接
无密码连接
> redis-cli -h 127.0.0.1 -p 6379

有密码连接
> redis-cli -h 127.0.0.1 -p 6379 -a password

## redis-cli中文乱码
> redis-cli -h 127.0.0.1 -p 6379 --raw

## redis批量删除脚本
```bash
#!/bin/bash
redis-cli -h localhost -p 6379 -a password keys "test*" | while read key
do
	echo "del key: $key."
	redis-cli -h localhost -p 6379 -a password del $key
done
```
