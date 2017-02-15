---
layout: detail
permalink: /java/sort/string
title: 字符串匹配算法
category: data
cover: /images/java/sort/string.jpg
tags: [java,算法]
description: 本文介绍了BF(BruteForce)，KMP字符串匹配算法
published: true
---

# 字符串匹配算法

## BF算法

### 算法思想

BF算法的思想就是从主串S的第一个字符开始和模式T的第一个字符进行比较，若相等，则继续比较两者的后续字符；若不相等，则从主串S的第二个字符开始和模式T的第一个字符进行比较，重复上述过程，若T中的字符全部比较完毕，则说明本趟匹配成功；若S中的字符全部比较完毕，则匹配失败。这个算法称为朴素的模式匹配算法，简称BF算法

### 时间复杂度

该算法最坏情况下要进行M*(N-M+1)次比较，时间复杂度为O(M*N)。

### Java实现

```java
String s = "yorhellomynameisrrhowareyouwhatisyornameyou";
String t = "your";
boolean flag = false;
int index = 0;
int len = s.length();
for(int i=0; i<len; i++) {
	char c = s.charAt(i);
	if(c == t.charAt(index)) {
		index++;
		if(index == t.length()) {
			flag = true;
			break;
		}
	} else {
		index = 0;
	}
}
System.out.println("result: "+flag);
```
