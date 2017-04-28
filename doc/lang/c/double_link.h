#ifndef _DOUBLE_LINK_H
#define _DOUBLE_LINK_H

// 新建双向链表。成功，返回表头；否则，返回NULL
extern int create();
// 撤销双向链表。成功，返回0；否则，返回-1
extern int destory();

// 判断双向链表是否为空，为空，返回1；否则，返回0
extern int is_empty();
// 返回双向链表的大小
extern int size();

// 获取双向链表第index个位置。成功，返回节点指针；否则，返回-1
extern void* get(int index);
// 获取双向链表第0个位置。成功，返回节点指针；否则，返回-1
extern void* get_first();
// 获取双向链表最后一个位置。成功，返回节点指针；否则，返回-1
extern void* get_last();

// 将value插入到双向链表的第index位置。成功，返回0，否则，返回-1
extern int insert(int index, void* value);
// 将value插入到双向链表的第一个位置。成功，返回0，否则，返回-1
extern int insert_first(void* value);
// 将value插入到双向链表的最后一个位置。成功，返回0，否则，返回-1
extern int insert_last(void* value);

// 删除双向链表中的第index位置。成功，返回0，否则，返回-1
extern int delete(int index);
// 删除双向链表中的第一个位置。成功，返回0，否则，返回-1
extern int delete_first();
// 删除双向链表中的最后一个位置。成功，返回0，否则，返回-1
extern int delete_last();

#endif