---
layout: detail
permalink: /linux/memory
title: Linux内存
category: linux
cover: /images/linux/memory.jpg
tags: [Linux,Memory]
description: Linux内存
published: true
url1: https://www.mawenbao.com/research/linux-ate-my-memory.html
url2: http://blog.yufeng.info/archives/2456
---

# Linux内存

## 内存“无端丢失”问题

```sh
$ free -g
# 结果
             total       used       free     shared    buffers     cached
Mem:            31         31          0          0          0          1
-/+ buffers/cache:         27          4
Swap:            0          0          0
```

| /             | total         | used          | free          | shared        | buffers       | cached        |
|:--------------|:--------------|:--------------|:--------------|:--------------|:--------------|:--------------|
| Mem           | 总物理内存 | 当前使用的内存(包括slab+buffers+cached) | 完全没有使用的内存 | 进程间共享的内存 | 缓存文件的元数据 | 缓存文件的具体内容 |
| -/+ buffers/cache | | 当前使用的内存(不包括buffers+cached，但包括slab) | 未使用和缓存的内存(free+buffers+cached) | | | |
| Swap          | 总的交换空间 | 已使用的交换空间 | 	未使用的交换空间 | | | |

```sh
# 查看所有进程占用的总内存
ps aux | awk '{mem += $6} END {print mem/1024/1024}'
# 结果
0.839323
```

> free, ps的统计方式的差别和Copy-on-write和Shared libraries等内存优化机制的存在，这两者的统计结果通常是不一样的。free统计了Slab缓存。

## Slab是什么

Slab Allocation是Linux 2.2之后引入的一个内存管理机制，专门用于缓存内核的数据对象，可以理解为一个内核专用的对象池，可以提高系统性能并减少内存碎片。(Linux 2.6.23之后，SLUB成为了默认的allocator。)

```sh
# 查看内存
cat /proc/meminfo
# Slab缓存
Slab:             33514212 kB
SReclaimable:      5879801 kB
SUnreclaim:       27634411 kB
```

> SReclaimable(Linux 2.6.19+)都是clean的缓存，随时可以释放。

free把Slab缓存统计到了used memory中。查看/proc/slabinfo(或使用slabtop命令)来查看Slab缓存的具体使用情况。结果发现，ext3_inode_cache和dentry_cache占用了绝大部分内存。可能服务器频繁地用rsync同步大量的文件等情况。
通过/proc/sys/vm/drop_caches这个配置项，我们可以手动清除指定的可回收缓存(SReclaimable)

```sh
# 清除Slab可回收缓存
echo 2 > /proc/sys/vm/drop_caches
# 执行后会主动释放Slab中clean的缓存(包括inode和dentry的缓存)，执行free -g，未使用内存会增加
```

> 手动清除缓存可能会在一段时间内降低系统性能。原则上不推荐这么做，因为如果有需要，系统会自动释放出内存供其他程序使用。另外，手动清除Slab缓存是一个治标不治本的办法。因为问题不在Slab，而在于我们那个会引起Slab缓存飙涨的进程（如rsync）。实际操作的时候发现，清除缓存一段时间后，Slab缓存很快又会“反弹”回去。如果需要治本，要么搞定问题进程，要么修改系统配置。

## 调整系统vm配置

> 配置文件：/etc/sysctl.conf

### vm.vfs_cache_pressure

系统在进行内存回收时，会先回收page cache, inode cache, dentry cache和swap cache。vfs_cache_pressure越大，每次回收时，inode cache和dentry cache所占比例越大。
vfs_cache_pressure默认是100，值越大inode cache和dentry cache的回收速度会越快，越小则回收越慢，为0的时候完全不回收(OOM!)。

### vm.min_free_kbytes

系统的"保留内存"的大小，"保留内存"用于低内存状态下的"atomic memory allocation requests"(eg. kmalloc + GFP_ATOMIC)，该参数也被用于计算开始内存回收的阀值，默认在开机的时候根据当前的内存计算所得，越大则表示系统会越早开始内存回收。
min_free_kbytes过大可能会导致OOM，太小可能会导致系统出现死锁等问题。

### vm.swappiness

该配置用于控制系统将内存swap out到交换空间的积极性，取值范围是[0, 100]。swappiness越大，系统的交换积极性越高，默认是60，如果为0则不会进行交换。
