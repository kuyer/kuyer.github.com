# redis

## redis-cli连接
无密码连接
> redis-cli -h 127.0.0.1 -p 6379

有密码连接
> redis-cli -h 127.0.0.1 -p 6379 -a password

## redis-cli中文乱码
> redis-cli -h 127.0.0.1 -p 6379 --raw
