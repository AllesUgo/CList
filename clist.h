#pragma once
typedef struct CLIST
{
    int (*compare)(void* a, void* b);
    struct CLIST_NODE* head;
    struct CLIST_NODE* end;
    struct CLIST_NODE* iterators;
} CList_t;

typedef struct CLIST_NODE
{
    struct CLIST_NODE* front;
    struct CLIST_NODE* next;
    char data[0];
} CList_Node;

CList_t* CList_New(void);//�����б�
void CList_SetCompareFunction(CList_t* list, int (*compare_function)(void* a, void* b));//�����б�Ԫ�صıȽϷ���
void CList_Delete(CList_t* list);//ɾ���б�����ʹ���б���ȫʧЧ���ٴ�ʹ����Ҫ���´���
void CList_AddItem_end(CList_t* list, void* data, unsigned int datasize);//���б��������Ŀ
void CList_AddItem_head(CList_t* list, void* data, unsigned int datasize);
int CList_RemoveItem(CList_t* list, void* tamplate_data);//���б����Ƴ����з���Ҫ�����Ŀ��ʹ��Ԥ�õıȽϷ���
int CList_RemoveItemIf(CList_t* list, void* tamplate_data, int (*cmp_func)(void* a, void* b));//���б����Ƴ����з���Ҫ�����Ŀ����ʹ��Ԥ�õıȽϷ���
void* CList_GetItemData(CList_t* list);//���б��л�ȡһ����Ŀ����ʹ����������
void CList_ResetIterators(CList_t* list);//�����б������Ϊ��ʼ״̬
void CList_Sort_BubbleSort(CList_t* list, int (*compare_function)(void* a, void* b));//ð������
void CList_Sort_QuickSort_safe(CList_t* list, int (*compare_function)(void* a, void* b));//��ȫ�Ŀ�������
void CList_Sort_QuickSort(CList_t* list, int (*compare_function)(void* a, void* b));//��������