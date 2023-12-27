#include"��ͷ.h"
#include<stdio.h>
int skip_list_delete(struct skip_list* list, int key)
{
	struct skip_list_node** update = NULL; /*��������ÿ������ָ�룬��Ų���λ�õ�ǰ������ڵ�����*/
	struct skip_list_node* cur = NULL;
	struct skip_list_node* prev = NULL;
	int i = 0;

	if (list == NULL && list->num == 0)
		return -1;

	/*����update�ռ����ڱ���ÿ��Ľڵ�����ָ��*/
	update = (struct skip_list_node**)SKIP_LIST_MALLOC(sizeof(list->level * sizeof(struct skip_list_node*)));
	if (update == NULL)
		return -2;

	/*����ѯ,����ɾ��λ�õ�ǰ������ڵ�����
	 *update[0] ��ŵ�һ���ɾ��λ��ǰ���ڵ㣬update[0]->next[0]��ʾɾ��λ�õ�ǰ���ڵ����һ�ڵ�(update[0]->next[0])�ĵ�һ������ֵ
	 *update[1] ��ŵڶ����ɾ��λ��ǰ���ڵ㣬update[1]->next[1]��ʾɾ��λ�õ�ǰ���ڵ����һ�ڵ�(update[1]->next[0])�ĵڶ�������ֵ
	 *update[n] ��ŵ�һ���ɾ��λ��ǰ���ڵ㣬update[n]->next[n]��ʾɾ��λ�õ�ǰ���ڵ����һ�ڵ�(update[n]->next[0])�ĵ�n������ֵ
	 */
	prev = list->head; /*�ӵ�һ���ڵ㿪ʼ�����ϲ㿪ʼ��*/
	i = list->level - 1;
	for (; i >= 0; i--)
	{
		/* ����ÿ���ڵ����һ���ڵ㲻Ϊ�� && �¸��ڵ��keyС��Ҫ�����key */
		while (((cur = prev->next[i]) != NULL) && (cur->key < key))
		{
			prev = cur; /* ����ƶ� */
		}
		update[i] = prev; /* ����Ҫɾ���ڵ��ǰ���ڵ� */
	}

	/* ��ǰkey���� */
	if ((cur != NULL) && (cur->key == key))
	{
		/*���ɾ��*/
		for (i = 0; i < list->level; i++)
		{
			if (update[i]->next[i] == cur)
			{
				update[i]->next[i] = cur->next[i];
			}
		}

		SKIP_LIST_FREE(cur);
		cur = NULL;

		/*���������Ĳ���*/
		for (i = list->level - 1; i >= 0; i--)
		{
			/*���ɾ���ڵ��,ĳ���ͷ�������ڵ�Ϊ��,��˵���ò�������ָ��,����������Ҫ��1*/
			if (list->head->next[i] == NULL)
			{
				list->level--;
			}
			else
			{
				break;
			}
		}

		list->num--; /*�ڵ�����1*/
	}
	else
	{
		return -3;
	}

	return 0;
}

	