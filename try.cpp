#include "clist.h"
#include <stdio.h>

#include <windows.h>

int cmp(int* a, int* b)
{
    if (*a <= *b)
        return 1;
    else
        return 0;
}
int main(void)
{

    //format 格式
    //scan format
    //print format
start:
    printf("1.从文件获取数据 2.随机生成数据(用于测试性能) >>");
    int input_method;
    int w;//这个变量用来保存排序的数据量
    scanf("%d", &input_method);
    if (input_method == 2)
    {
        printf("冒泡排序或快速排序算法时间比较\n请输入要比较的数据量:");
        scanf("%d", &w);

        if (w < 1)
        {
            printf("请输入大于0的数\n");
            return 1;
        }
    }
    printf("请输入要使用的算法 1:冒泡排序 2:快速排序\n请输入:");
    int type;
    scanf("%d", &type);
    if (type != 1 && type != 2)
    {
        printf("请输入正确的选项\n");
        return 1;
    }
    int i = 0;
    double run_time;
    _LARGE_INTEGER time_start;	//开始时间
    _LARGE_INTEGER time_over;	//结束时间
    double dqFreq;		//计时器频率
    LARGE_INTEGER f;	//计时器频率
    QueryPerformanceFrequency(&f);
    dqFreq = (double)f.QuadPart;
    //CPU内部临时存储数据的东西
    // 存储器一般指的是内存
    //乘以1000000把单位由秒化为微秒，精度为1000 000/（cpu主频）微秒
    //生成随机数表
    // 添加至列表
    int* k;
    CList_t* list = CList_New();
    if (input_method == 2)
    {
        printf("开始生成随机数\n");
        for (int i = w; i > 0; i--)
        {
            //QueryPerformanceCounter(&time_start);
            //srand(time_start.QuadPart);
            int k = i *99999999999 * 1024 + 3;
            CList_AddItem_end(list, &k, sizeof(int));
        }
        printf("正在输出生成的随机数到文件a.txt\n");
        FILE* fp = fopen("a.txt", "w");
        if (fp == NULL)
        {
            printf("无法打开文件\n");
            return 1;
        }
        
        do
        {
            k = (int*)CList_GetItemData(list);
            if (k != NULL)
            {
                //fprintf(fp,"%d ", *k);
            }
        } while (k);
        fclose(fp);
    }
    else
    {
        printf("请输入输入文件路径:");
        char* input_path = (char*)malloc(1024);
        scanf("%s", input_path);
        FILE* fp = fopen(input_path,"r");
        if (fp == NULL)
        {
            printf("打开文件失败\n");
            return 1;
        }
        int d;
        printf("开始读取文件数据\n");
        int count = 0;
        while (1)
        {
            count++;
            if (1 != fscanf(fp, "%d", &d))
                break;
            CList_AddItem_end(list, &d, sizeof(int));
        }
        fclose(fp);
        printf("读取了%d个数据\n", count-1);
    }
    // 排序//11306293.300000//
    printf("开始排序\n");
    if (type == 2)
    {
        QueryPerformanceCounter(&time_start);	//计时开始
        CList_Sort_QuickSort(list, (int (*)(void*, void*))cmp);
        QueryPerformanceCounter(&time_over);	//计时结束
    }
    else
    {
        QueryPerformanceCounter(&time_start);	//计时开始
        CList_Sort_BubbleSort(list, (int (*)(void*, void*))cmp);
        QueryPerformanceCounter(&time_over);	//计时结束
    }
    run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
    printf("排序完成，排序用时用时%d微秒\n",(int)run_time);
    CList_ResetIterators(list);
    printf("正在将排序结果输出到文件b.txt\n");
    FILE* fp = fopen("b.txt", "w");
    if (fp == NULL)
    {
        printf("打开b.txt失败\n");
        return 1;
    }
    do
    {
        k = (int*)CList_GetItemData(list);
        if (k != NULL)
        {
            fprintf(fp,"%d ", *k);
        }
    } while (k);
    fclose(fp);
    printf("输出完成\n开始清理内存\n");
    CList_Delete(list);
    printf("清理完成\n");
    system("pause");
    goto start;
    return 0;
}