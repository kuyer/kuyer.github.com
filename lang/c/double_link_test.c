#include <stdio.h>
#include "double_link.h"

void test_int() {
	printf("%s start.\n", __func__);
	int arr[4] = {1, 2, 3, 4};
	create();
	insert(0, &arr[0]);
	insert_first(&arr[1]);
	insert_last(&arr[2]);
	insert(1, &arr[3]);
	
	printf("int is_empty=%d\n", is_empty());
	printf("int size=%d\n", size());
	
	// 2, 4, 1, 3
	int i;
	int *p;
	int len = size();
	for(i=0; i<len; i++) {
		p = (int*)get(i);
		printf("int get(%d)=%d\n", i, *p);
	}
	destory();
	printf("%s finish.\n", __func__);
}

void test_string() {
	printf("%s start.\n", __func__);
	char* arr = {"one", "two", "three", "four"};
	create();
	insert(0, &arr[0]);
	insert_first(&arr[1]);
	insert_last(&arr[2]);
	insert(1, &arr[3]);
	
	printf("string is_empty=%d\n", is_empty());
	printf("string size=%d\n", size());
	
	// "two", "four", "one", "three"
	int i;
	char *p;
	int len = size();
	for(i=0; i<len; i++) {
		p = (char*)get(i);
		printf("string get(%d)=%s\n", i, p);
	}
	destory();
	printf("%s finish.\n", __func__);
}

typedef struct user_srt {
	int id;
	char name[20];
} user;

static user arr_user = {
	{1, "rory"},
	{2, "kuyer"},
	{3, "dongfang"},
	{4, "beifeng"},
};

#define arr_user_size ( (sizeof(arr_user)) / (sizeof(arr_user[0])) )

void test_object() {
	printf("%s start.\n", __func__);
	create();
	
	insert(0, &arr_user[0]);
	insert_first(&arr_user[1]);
	insert_last(&arr_user[2]);
	insert(1, &arr_user[3]);
	
	printf("object is_empty=%d\n", is_empty());
	printf("object size=%d\n", size());
	
	int i;
	user *p;
	int len = size();
	for(i=0; i<len; i++) {
		p = (user*)get(i);
		printf("string get(%d)=[%d, %s]\n", i, p->id, p->name);
	}
	
	destory();
	printf("%s finish.\n", __func__);
}

int main() {
	test_int();
	test_string();
	return 0;
}