#include<stdlib.h>
#define SKIP_LIST_MALLOC(size) malloc(size);
#define SKIP_LIST_CALLOC(n,size) calloc(n,size);
#define SKIP_LIST_FREE(p) free(p);

struct skip_list_node
{
	int key;//
	int value;//�洢������
	int max_level;//��ǰ�ڵ�������
	struct skip_list_node* next[];//��������
	//���ݸýڵ�����Ĳ�ָͬ���С��ͬ������
};
struct skip_list {
	int level;//�������������
	int num;//�ڵ���Ŀ
	struct skip_list_node* head;
};
extern struct  skip_list* skip_list_creat(int max_level);
extern int skip_list_insert(struct skip_list* list, int key, int value);
extern int skip_list_delete(struct skip_list* list, int key);
extern int skip_list_modify(struct skip_list* list, int key, int value);
extern int skip_list_search(struct skip_list* list, int key, int* value);
extern int skip_list_destroy(struct skip_list* list);
