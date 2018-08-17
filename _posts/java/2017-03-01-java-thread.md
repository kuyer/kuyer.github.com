---
layout: detail
permalink: /java/thread
title: Java多线程
category: data
cover: /images/java/thread/thread.jpg
tags: [java,thread,多线程]
description: 
published: true
---

# Java多线程（Thread）

## 创建线程的两种方式

* 继承Thread类，代码如下：

```java
public ThreadDemo1 extends Thread {

	@Override
	public void run() {
		System.out.println("run thread by extends Thread.");
	}
	
	public static void main() {
		ThreadDemo1 td = new ThreadDemo1();
		td.start();
	}

}
```

运行结果：

```
run thread by extends Thread.
```

* 实现Runnable接口，代码如下：

```java
public ThreadDemo2 implements Runnable {

	@Override
	public void run() {
		System.out.println("run thread by implements Runnable.");
	}
	
	public static void main() {
		ThreadDemo2 td = new ThreadDemo2();
		td.start();
	}

}
```

运行结果：

```
run thread by implements Runnable.
```

## volatile修饰符

* volatile是java的一种同步机制，用来确保volatile修饰的变量发生变更操作时，能够通知到其它线程。
* 把变量用volatile修饰时，编译器会注意到这个变量是共享的，因此不会把这个变量上的操作与其它内存操作一起进行重排序。
* volatile变量不会被缓存在寄存器或其它处理器不可见的地方，因此获取volatile变量时，总会返回其最新写入的值。
* 写入volatile变量相当于退出同步代码块，而读取volatile变量等于进入了同步代码块。
* volatile变量在用作验证的场景时，才使用它，如接收多线程消息的开关，把开关变量的修饰为volatile时，打开开关变量能及时收到消息，关闭开关变量，能随时屏蔽消息。
* volatile变量不能保证复杂操作的原子性，如count++操作。

## synchronized

synchronized关键字主要用于线程临界冲突的资源分配

## ThreadLocal

ThreadLocal字面可理解为线程本地存储，这种线程本地存储结构使得每个线程只能获取或变更该线程自己的变量，从而实现了多线程之间的资源相互隔离，进而达到并发安全的问题。
ThreadLocal主要用于每个线程自己独享的变量，从而避免了资源的争夺，实现了以空间换时间的思想。
