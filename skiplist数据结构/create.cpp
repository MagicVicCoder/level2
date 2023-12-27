#include"��ͷ.h"
#include<stdio.h>
#include<string.h>
/*��̬��������ڵ�*/
static skip_list_node* skip_list_node_creat(int level, int key, int value) {
	struct skip_list_node* node = NULL;
	/*�ڵ�ռ��СΪ�ڵ����ݴ�С+level��������ռ�õĴ�С*/
	node = (skip_list_node*)SKIP_LIST_MALLOC(sizeof(*node) + level * sizeof(node));
	if (node == NULL) {
		printf("fail to malloc node");
		return NULL;
	}

	/*������ռ��ʼ��Ϊ0*/
	memset(node, 0, sizeof(*node) + level * sizeof(node));
	node->key = key;
	node->value = value;
	node->max_level = level;

	return node;
}
/*��������ͷ�ڵ�*/
struct skip_list* skip_list_creat(int max_level) {
	struct skip_list* list = NULL;
	list = (skip_list*)SKIP_LIST_MALLOC(sizeof(*list));
	if (list == NULL) {
		printf("fail to malloc list");
		return NULL;
	}
	
	list->level = 1;
	list->num = 0;
	list->head = skip_list_node_creat(max_level, 0, 0);
	if (list->head = NULL) {
		SKIP_LIST_FREE(list);
		printf("fail to malloc list");
		return NULL;
	}
	return list;
}

	 