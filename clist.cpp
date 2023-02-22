#include <stdlib.h>
#include <string.h>
#include "clist.h"
#include<process.h>
#include <Windows.h>
CList_t* CList_New(void)
{
    CList_t* l = (CList_t*)malloc(sizeof(CList_t));
    l->end = 0x0;
    l->head = 0x0;
    l->iterators = 0x0;
    l->compare = 0x0;
    return l;
}
void CList_Delete(CList_t* list)
{
    CList_Node* temp;
    while (list->head)
    {
        temp = list->head->next;
        free(list->head);
        list->head = temp;
    }
    free(list);
}
void CList_AddItem_end(CList_t* list, void* data, unsigned int datasize)
{
    if (list->end == NULL)
    {
        // 列表为空
        list->head = (CList_Node*)malloc(sizeof(CList_Node) + datasize);
        list->end = list->head;
        memset(list->head, 0, sizeof(CList_Node));
        memcpy(list->head->data, data, datasize);
    }
    else
    {
        // 不为空
        CList_Node* temp = (CList_Node*)malloc(sizeof(CList_Node) + datasize);
        temp->next = NULL;
        temp->front = list->end;
        memcpy(temp->data, data, datasize);
        list->end->next = temp;
        list->end = temp;
    }
}
void CList_AddItem_head(CList_t* list, void* data, unsigned int datasize)
{
    if (list->head == NULL)
    {
        // 列表为空
        list->head = (CList_Node*)malloc(sizeof(CList_Node) + datasize);
        list->end = list->head;
        memset(list->head, 0, sizeof(CList_Node));
        memcpy(list->head->data, data, datasize);
    }
    else
    {
        // 不为空
        CList_Node* temp = (CList_Node*)malloc(sizeof(CList_Node) + datasize);
        temp->next = list->head;
        temp->front = NULL;
        memcpy(temp->data, data, datasize);
        list->head->front = temp;
        list->head = temp;
    }
}

int CList_RemoveItemIf(CList_t* list, void* tamplate_data, int (*cmp_func)(void* a, void* b))
{
    CList_Node* temp = list->head;
    int removed_num = 0;
    while (temp)
    {
        if (1 == cmp_func(temp->data, tamplate_data))
        {
            // 该项要删除
            // 连接链表前后
            if (temp->front != NULL)
                temp->front->next = temp->next;
            if (temp->next != NULL)
                temp->next->front = temp->front;
            // 检查链表首尾
            if (list->head == temp)
                list->head = temp->next;
            if (list->end == temp)
                list->end = temp->front;
            // 检查迭代器
            if (list->iterators == temp)
                list->iterators = temp->next;
            CList_Node* backup = temp;
            temp = temp->next;
            free(backup);
            removed_num++;
        }
        else
        {
            temp = temp->next;
        }
    }
    return removed_num;
}
int CList_RemoveItem(CList_t* list, void* tamplate_data)
{
    return CList_RemoveItemIf(list, tamplate_data, list->compare);
}

void* CList_GetItemData(CList_t* list)
{
    if (list->iterators == NULL)
    {
        // 新迭代
        if (list->head != NULL)
        {
            list->iterators = list->head;
            return list->iterators->data;
        }
        else
        {
            return NULL; // 空列表
        }
    }
    else
    {
        // 非空迭代
        if (list->iterators->next == NULL)
        {
            return NULL; // 迭代器已处于列表尾端
        }
        else
        {
            list->iterators = list->iterators->next;
            return list->iterators->data;
        }
    }
}
void CList_ResetIterators(CList_t* list)
{
    list->iterators = NULL;
}
void CList_SetCompareFunction(CList_t* list, int (*compare_function)(void* a, void* b))
{
    list->compare = compare_function;
}
struct MULTIPLY_QUICK_SORT_ARGS
{
    struct CLIST_NODE** list_ptr;
    int start;
    int end;
    int (*compare_function)(void* a, void* b);
}; 
inline void QuickSort(void* lpParamter)
{
    struct MULTIPLY_QUICK_SORT_ARGS* arg = (struct MULTIPLY_QUICK_SORT_ARGS*)lpParamter;
    struct CLIST_NODE** list_ptr = arg->list_ptr;
    int start = arg->start;
    int end = arg->end;
    int (*compare_function)(void* a, void* b) = arg->compare_function;
    if (start >= end)
        return;
    int a = start, b = end;
    struct CLIST_NODE* center = list_ptr[start];
    while (start < end)
    {
        while (start != end && compare_function(center->data, list_ptr[end]->data) == 1)
        {
            end--;
        }
        list_ptr[start] = list_ptr[end];
        while (start != end && compare_function(list_ptr[start]->data, center->data) == 1)
        {
            start++;
        }
        list_ptr[end] = list_ptr[start];
    }
    list_ptr[start] = center;
    struct MULTIPLY_QUICK_SORT_ARGS n1, n2;
    n1.compare_function = arg->compare_function;
    n1.list_ptr = arg->list_ptr;
    n1.start = a;
    n1.end = start - 1;
    n2.compare_function = arg->compare_function;
    n2.list_ptr = arg->list_ptr;
    n2.start = start+1;
    n2.end = b;
    QuickSort(&n1);
    QuickSort(&n2);
}
void QuickSort_safety(struct CLIST_NODE** list_ptrs, int start_i,int end_i, int (*compare_function)(void* a, void* b))
{
    //将任务加入列表
    if (start_i >= end_i) return;
    CList_t* clist = CList_New();
    struct STATE {
        int start;
        int end;
    } state,*state_ptr,newstate;
    state.end = end_i;
    state.start = start_i;
    CList_AddItem_head(clist, &state, sizeof(state));
    while (1)
    {
        clist->iterators=NULL;
        state_ptr =(struct STATE*)CList_GetItemData(clist);
        if (!state_ptr) break;
        state = *state_ptr;
        struct CLIST_NODE* center = list_ptrs[state_ptr->start];
        while (state_ptr->start < state_ptr->end)
        {
            while (state_ptr->start != state_ptr->end && compare_function(center->data, list_ptrs[state_ptr->end]->data) == 1)
            {
               state_ptr->end--;
            }
            list_ptrs[state_ptr->start] = list_ptrs[state_ptr->end];
            while (state_ptr->start != state_ptr->end && compare_function(list_ptrs[state_ptr->start]->data, center->data) == 1)
            {
                state_ptr->start++;
            }
            list_ptrs[state_ptr->end] = list_ptrs[state_ptr->start];
        }
        list_ptrs[state_ptr->start] = center;
        int now = state_ptr->start;
        //删除当前项
        void* ptr = clist->head;
        if (clist->head->next != NULL)
        {
            clist->head->next->front = NULL;
        }
        clist->head = clist->head->next;
        free(ptr);
        //检查新的两个任务节点并加入任务列表
        if (state.start < now)
        {
            newstate.start = state.start;
            newstate.end = now - 1;
            CList_AddItem_head(clist, &newstate, sizeof(newstate));
        }
        if (state.end > now)
        {
            newstate.start=now+1;
            newstate.end = state.end;
            CList_AddItem_head(clist, &newstate, sizeof(newstate));
        }
    }
    CList_Delete(clist);

}
void BubbleSort(struct CLIST_NODE** list, int length, int (*compare_function)(void* a, void* b))
{
    struct CLIST_NODE* temp;
    int sign = 0;
    for (int i = 1; i < length; i++)
    {
        for (int j = 0; j < length - i; j++)
        {
            if (!compare_function(list[j]->data ,list[j+1]->data))
            {
                //交换
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}
void CList_Sort_QuickSort(CList_t* list, int (*compare_function)(void* a, void* b))
{
    
    // 快速排序
    if (list->head == NULL)
        return;
    int list_size = 0;
    // 统计列表长度
    struct CLIST_NODE* temp = list->head;
    while (temp)
    {
        list_size++;
        temp = temp->next;
    }
    // 创建相应大小的指针数组
    struct CLIST_NODE** list_ptrs = (struct CLIST_NODE**)malloc(sizeof(struct CLIST_NODE*) * list_size);
    temp = list->head;
    for (int i = 0; i < list_size; i++)
    {
        list_ptrs[i] = temp;
        temp = temp->next;
    }
    struct MULTIPLY_QUICK_SORT_ARGS n1, n2;
    n1.compare_function = compare_function;
    n1.list_ptr =list_ptrs;
    n1.start = 0;
    n1.end =list_size-1;
    QuickSort(&n1);
    //BubbleSort(list_ptrs, list_size, compare_function);
    //重新构建列表
    for (int i = 0; i < list_size; i++)
    {
        if (i == 0)
        {
            list_ptrs[0]->front = NULL;
        }
        else
        {
            list_ptrs[i]->front = list_ptrs[i - 1];
        }
        if (i == list_size - 1)
        {
            list_ptrs[list_size - 1]->next = NULL;
        }
        else
        {
            list_ptrs[i]->next = list_ptrs[i + 1];
        }
    }
    list->head = list_ptrs[0];
    list->end = list_ptrs[list_size - 1];
    free(list_ptrs);
}
void CList_Sort_QuickSort_safe(CList_t* list, int (*compare_function)(void* a, void* b))
{

    // 快速排序
    if (list->head == NULL)
        return;
    int list_size = 0;
    // 统计列表长度
    struct CLIST_NODE* temp = list->head;
    while (temp)
    {
        list_size++;
        temp = temp->next;
    }
    // 创建相应大小的指针数组
    struct CLIST_NODE** list_ptrs = (struct CLIST_NODE**)malloc(sizeof(struct CLIST_NODE*) * list_size);
    temp = list->head;
    for (int i = 0; i < list_size; i++)
    {
        list_ptrs[i] = temp;
        temp = temp->next;
    }
    QuickSort_safety(list_ptrs,0,list_size-1,compare_function);
    //BubbleSort(list_ptrs, list_size, compare_function);
    //重新构建列表
    for (int i = 0; i < list_size; i++)
    {
        if (i == 0)
        {
            list_ptrs[0]->front = NULL;
        }
        else
        {
            list_ptrs[i]->front = list_ptrs[i - 1];
        }
        if (i == list_size - 1)
        {
            list_ptrs[list_size - 1]->next = NULL;
        }
        else
        {
            list_ptrs[i]->next = list_ptrs[i + 1];
        }
    }
    list->head = list_ptrs[0];
    list->end = list_ptrs[list_size - 1];
    free(list_ptrs);
}
void CList_Sort_BubbleSort(CList_t* list, int (*compare_function)(void* a, void* b))
{
    // 快速排序
    if (list->head == NULL)
        return;
    int list_size = 0;
    // 统计列表长度
    struct CLIST_NODE* temp = list->head;
    while (temp)
    {
        list_size++;
        temp = temp->next;
    }
    // 创建相应大小的指针数组
    struct CLIST_NODE** list_ptrs = (struct CLIST_NODE**)malloc(sizeof(struct CLIST_NODE*) * list_size);
    temp = list->head;
    for (int i = 0; i < list_size; i++)
    {
        list_ptrs[i] = temp;
        temp = temp->next;
    }
    BubbleSort(list_ptrs, list_size, compare_function);
    //重新构建列表
    for (int i = 0; i < list_size; i++)
    {
        if (i == 0)
        {
            list_ptrs[0]->front = NULL;
        }
        else
        {
            list_ptrs[i]->front = list_ptrs[i - 1];
        }
        if (i == list_size - 1)
        {
            list_ptrs[list_size - 1]->next = NULL;
        }
        else
        {
            list_ptrs[i]->next = list_ptrs[i + 1];
        }
    }
    list->head = list_ptrs[0];
    list->end = list_ptrs[list_size - 1];
    free(list_ptrs);
}
