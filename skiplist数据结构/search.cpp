#include"��ͷ.h"
#include<stdio.h>
int skip_list_search(struct skip_list* list, int key, int* value)
{
	struct skip_list_node* cur = NULL;
	struct skip_list_node* prev = NULL;
	int i = 0;

	if (list == NULL && value == NULL && list->num == 0)
		return -1;

	/*������,���Ҳ�ѯλ��ԭʼ����Ľڵ�*/
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL) && (cur->key < key))
		{
			prev = cur; /* ����ƶ� */
		}
	}

	/* ��ǰkey���� */
	if ((cur != NULL) && (cur->key == key))
	{
		*value = cur->value;
	}
	else
	{
		return -3;
	}

	return 0;
}