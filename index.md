---
title: 枯叶
layout: layout
---

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
