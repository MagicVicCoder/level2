#include"��ͷ.h"
#include"create.cpp"
#include<stdio.h>
#include<stdlib.h>
/*��50%�ĸ��ʲ�������Ԫ�ص���������*/
static int skip_list_level(struct skip_list* list) {
	int i = 0, level = 1;
	for (i = 1; i < list->head->max_level; i++) 
	{
		if ((rand() % 2) == 1) 
		{
			level++;
		}
	}
	return level;
}

/*��������ڵ�*/
int skip_list_insert(struct skip_list* list, int key, int value) {
	struct skip_list_node** update = NULL; /*��������ÿ������ָ�룬��Ų���λ�õ�ǰ������ڵ�����*/
	struct skip_list_node* cur = NULL;
	struct skip_list_node* prev = NULL;
	struct skip_list_node* insert = NULL;
	int i = 0, level = 0;
	if (list == NULL)return -1;

	/*����update�ռ����ڱ���ÿ�������ָ��*/
	update = (struct skip_list_node**)SKIP_LIST_MALLOC(sizeof(list->head->max_level));
	if (update == NULL)return -2;

	/*����ѯ�����Ҳ���λ�õ�ǰ������ڵ�����
	 *update[0] ��ŵ�һ��Ĳ���λ��ǰ���ڵ㣬update[0]->next[0]��ʾ����λ�õ�ǰ���ڵ����һ�ڵ�(update[0]->next[0])�ĵ�һ������ֵ
	 *update[1] ��ŵڶ���Ĳ���λ��ǰ���ڵ㣬update[1]->next[1]��ʾ����λ�õ�ǰ���ڵ����һ�ڵ�(update[1]->next[0])�ĵڶ�������ֵ
	 *update[n] ��ŵ�һ��Ĳ���λ��ǰ���ڵ㣬update[n]->next[n]��ʾ����λ�õ�ǰ���ڵ����һ�ڵ�(update[n]->next[0])�ĵ�n������ֵ
     */
	prev = list->head;/*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	for (i = list->level - 1; i >= 0; i--) {
		while (((cur = prev->next[i]) != NULL) && (cur->key < key)) {
			prev = cur;/*����ƶ�*/
		}
		update[i] = prev;/*����Ҫ�����ǰ���ڵ�*/
	}
	/*��ǰkey�Ѵ��ڣ����ش���*/
	if ((cur != NULL) && (cur->key == key))return -3;

	/*��ȡ����Ԫ�ص���������������������������*/
	level = skip_list_level(list);
	/*������ǰ�ڵ�*/
	insert = skip_list_node_creat(level, key, value);
	if (level > list->level) {
		for (i = list->level; i < level; i++) {
			update[i] = list->head;
		}
		list->level = level;
	}

	for (i = 0; i < level; i++) {
		insert->next[i] = update[i]->next[i];
		update[i]->next[i] = insert;
	}
	list->num++;
}