---
layout: detail
permalink: /gcc/static
title: GCC静态库的使用
category: linux
cover: /images/gcc/static.jpg
tags: [Linux,GCC]
description: GCC静态库的使用，ar命令介绍
published: true
---

# GCC静态库的使用

## 编写源代码

* add.c

```c
int add(int x, int y) {
    return x + y;
}
```

* sub.c

```c
int sub(int x, int y) {
    return x - y;
}
```

## 生成.o文件

```sh
gcc -c -static add.c -o add.o
gcc -c -static sub.c -o sub.o
```

## 打包到libtools.a

```sh
ar -r libtools.a add.o
ar -r libtools.a sub.o
```

也可使用更简单的方式

```sh
ar -r libtools.a add.o sub.o
```

> 静态库的名称，最好以lib开头

## 使用libtools.a静态库

* main.c

```c
#include <stdio.h>

int add(int x, int y);
int sub(int x, int y);

int main() {
    printf("5+2=%d\n", add(5, 2));
    printf("5-2=%d\n", sub(5, 2));
    return 0;
}
```

* 生成main.o文件

```sh
gcc -c main.c -o main.o
```

* 链接成为可执行文件

```sh
gcc main.o libtools.a -o main
或者
gcc main.o -o main -L. -ltools
```

* 运行main文件

```sh
./main
结果
5+2=7
5-2=3
```

## ar命令

| 选项 | 描述 |
| -d | 从.a文件中删除.o文件，如：ar -d libtools.a add.o |
| -r | 添加.o文件到.a文件中，如：ar -r libtools.a add.o |
| -t | 显示.a文件中的内容，如：ar -t libtools.a |
| -x | 解压.a文件，如：ar -x libtools.a |
