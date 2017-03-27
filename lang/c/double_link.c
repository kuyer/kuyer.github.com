#include <stdio.h>
#include <malloc.h>

// 双向链表的节点
typedef struct double_node {
	struct double_node* prev;
	struct double_node* next;
	void* value;
} node;

// 表头。注意，表头不存放元素值
static node* head = NULL;
// 节点大小
static int size = 0;

// 创建节点。成功返回节点指针，否则返回NULL
static node* create_node(void* value) {
	node* curr = NULL;
	curr = (node*)malloc(sizeof(node));
	if(!curr) {
		printf("create double node error.\n");
		return NULL;
	}
	curr->prev = curr->next = curr;//默认当前节点的上级节点和下级节点为其自身
	curr->value = value;//节点值
}

// 新建双向链表。成功，返回表头；否则，返回NULL
int create() {
	head = create_node(NULL);
	if(!head) {
		return -1;
	}
	size = 0;
	return 0;
}
/** 撤销双向链表。成功，返回0；否则，返回-1 **/
int destory() {
	if(!head) {
		printf("%s failed. double_link is null.\n", __func__);
		return -1;
	}
	node* next = head->next;
	node* temp = NULL;
	while(next != head) {
		temp = next;
		next = next.next;
		free(temp);
	}
	free(head);
	head = NULL;
	size = 0;
	return 0;
}

/** 判断双向链表是否为空，为空，返回1；否则，返回0 **/
int is_empty() {
	return size == 0;
}
/** 返回双向链表的大小 **/
int size() {
	return size;
}

/** 获取双向链表第index个位置。成功，返回节点指针；否则，返回-1 **/
static node* get_node(int index) {
	if(index<0 || index>=size) {
		printf("%s failed. index out of bound.\n", __func__);
		return NULL;
	}
	//正向查找
	if(index<=(size/2)) {
		int i = 0;
		node* next = head->next;
		while((i++)<index) {
			next = next->next;
		}
		return next;
	}
	//反向查找
	int j=0;
	int temp = size-index-1;
	int prev = head->prev;
	while((j++)<temp) {
		prev = prev->prev;
	}
	return prev;
}
void* get(int index) {
	node* curr = get_node(index);
	if(!curr) {
		printf("%s failed!\n", __func__);
		return NULL;
	}
	return curr->value;
}
/** 获取双向链表第0个位置。成功，返回节点指针；否则，返回-1 **/
static node* get_first_node() {
	return get_node(0);
}
void* get_first() {
	return get(0);
}
/** 获取双向链表最后一个位置。成功，返回节点指针；否则，返回-1 **/
static node* get_last_node() {
	return get_node(size-1);
}
void* get_last() {
	return get(size-1);
}

/** 将value插入到双向链表的第index位置。成功，返回0，否则，返回-1 **/
int insert(int index, void* value) {
	if(index == 0) {
		return insert_first(value);
	}
	node* temp = get_node(index);
	if(!temp) {
		return -1;
	}
	node* curr = create_node(value);
	if(!curr) {
		return -1;
	}
	curr->next = temp;
	curr->prev = temp->prev;
	temp->prev->next = curr;
	temp->prev = curr;
	size++;
	return 0;
}
/** 将value插入到双向链表的第一个位置。成功，返回0，否则，返回-1 **/
int insert_first(void* value) {
	node* curr = create_node(value);
	if(!curr) {
		return -1;
	}
	curr->prev = head;
	curr->next = head->next;
	head->next->prev = curr;
	head->next = curr;
	size++;
	return 0;
}
/** 将value插入到双向链表的最后一个位置。成功，返回0，否则，返回-1 **/
int insert_last(void* value) {
	node* curr = create_node(value);
	if(!curr) {
		return -1;
	}
	curr->next = head;
	curr->prev = head->prev;
	head->prev->next = curr;
	head->prev = curr;
	size++;
	return 0;
}

/** 删除双向链表中的第index位置。成功，返回0，否则，返回-1 **/
int delete(int index) {
	node* curr = get_node(index);
	if(!curr) {
		printf("%s failed. the index out of bound.\n", __func__);
		return -1;
	}
	curr->prev->next = curr->next;
	curr->next->prev = curr->prev;
	free(curr);
	size--;
	return 0;
}
/** 删除双向链表中的第一个位置。成功，返回0，否则，返回-1 **/
int delete_first() {
	return delete(0);
}
/** 删除双向链表中的最后一个位置。成功，返回0，否则，返回-1 **/
int delete_last() {
	return delete(size-1);
}
