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

CList_t* CList_New(void);//创建列表
void CList_SetCompareFunction(CList_t* list, int (*compare_function)(void* a, void* b));//设置列表元素的比较方法
void CList_Delete(CList_t* list);//删除列表，将会使该列表完全失效，再次使用需要重新创建
void CList_AddItem_end(CList_t* list, void* data, unsigned int datasize);//向列表中添加项目
void CList_AddItem_head(CList_t* list, void* data, unsigned int datasize);
int CList_RemoveItem(CList_t* list, void* tamplate_data);//从列表中移除所有符合要求的项目，使用预置的比较方法
int CList_RemoveItemIf(CList_t* list, void* tamplate_data, int (*cmp_func)(void* a, void* b));//从列表中移除所有符合要求的项目，不使用预置的比较方法
void* CList_GetItemData(CList_t* list);//从列表中获取一个项目，并使迭代器递增
void CList_ResetIterators(CList_t* list);//重置列表迭代器为初始状态
void CList_Sort_BubbleSort(CList_t* list, int (*compare_function)(void* a, void* b));//冒泡排序
void CList_Sort_QuickSort_safe(CList_t* list, int (*compare_function)(void* a, void* b));//安全的快速排序
void CList_Sort_QuickSort(CList_t* list, int (*compare_function)(void* a, void* b));//快速排序