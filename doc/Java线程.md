# Java线程

## volatile

参考：http://www.cnblogs.com/chengxiao/p/6528109.html

volatile是Java提供的一种轻量级的同步机制，它同synchronzied（重量级锁）相比，没有线程上下文切换所带来的性能开销。它的主要特性：

1. 保证共享变量对所有线程的可见性
2. 禁止指令重排序

> 注意：volatile对单个共享变量的读写具有原子性；但是对于类似i++的复合操作，volatile无法保证其原子性。

### 可见性

```java
public class VolatileTest {
	volatile boolean isStart = false;
	public void updateStatus() {
		isStart = true;
	}
	public void start() {
		if(isStart) {
			System.out.println("start ...");
		}
	}
}
```

### 禁止指令重排序

Java的指令重排序优化是指编译器和处理器为了优化程序性能而对指令序列进行排序的一种手段。但重排序也需遵守以下规则：

* 重排序操作不会对存在数据依赖关系的操作进行重排序，如：

```java
int a = 100;//1
int b = a;//2
```

由于第二个操作要依赖于第一个操作，所以在编译时和处理器运行时这两个操作不会被重排序。

* 重排序是为了优化性能，但无论如何排序，在单线程下，运行的结果都不会被改变

```java
int a = 100;//1
int b = 200;//2
int c = a + b;//3
```

由于第一步和第二步不存在依赖关系，则可能会发生重排序；但是第三步这个操作不会被重排序的，因为运行的结果肯定是300

```java
public class VolatileTest {
	int a = 100;
	boolean isStart = false;
	public void updateStatus() {
		a = 200;//1
		isStart = true;//2
	}
	public void start() {
		if(isStart) {
			int b = a + 100;//3
			System.out.println(b);
		}
	}
}
```

假如线程1执行updateStatus后，线程2的结果不一定是300，有可能是200；原因是第一步和第二步有可能在编译或运行的时候进行指令重排序，使第二步先于第一步运行，解决的方式在int a = 100;或boolean isStart = false;前加上volatile关键词

使用volatile修饰共享变量，在编译时，会在指令序列中插入内存屏障来禁止特定类型的处理器重排序，规则如下：

1. 当第二个操作是volatile写时，无论第一个操作是什么，不会重排序
2. 当第一个操作是volatile读时，无聊第二个操作是什么，不会重排序
3. 当第一个操作是volatile写时，第二个操作是volatile是读时，不会重排序
