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

    //format ��ʽ
    //scan format
    //print format
start:
    printf("1.���ļ���ȡ���� 2.�����������(���ڲ�������) >>");
    int input_method;
    int w;//��������������������������
    scanf("%d", &input_method);
    if (input_method == 2)
    {
        printf("ð���������������㷨ʱ��Ƚ�\n������Ҫ�Ƚϵ�������:");
        scanf("%d", &w);

        if (w < 1)
        {
            printf("���������0����\n");
            return 1;
        }
    }
    printf("������Ҫʹ�õ��㷨 1:ð������ 2:��������\n������:");
    int type;
    scanf("%d", &type);
    if (type != 1 && type != 2)
    {
        printf("��������ȷ��ѡ��\n");
        return 1;
    }
    int i = 0;
    double run_time;
    _LARGE_INTEGER time_start;	//��ʼʱ��
    _LARGE_INTEGER time_over;	//����ʱ��
    double dqFreq;		//��ʱ��Ƶ��
    LARGE_INTEGER f;	//��ʱ��Ƶ��
    QueryPerformanceFrequency(&f);
    dqFreq = (double)f.QuadPart;
    //CPU�ڲ���ʱ�洢���ݵĶ���
    // �洢��һ��ָ�����ڴ�
    //����1000000�ѵ�λ���뻯Ϊ΢�룬����Ϊ1000 000/��cpu��Ƶ��΢��
    //�����������
    // ������б�
    int* k;
    CList_t* list = CList_New();
    if (input_method == 2)
    {
        printf("��ʼ���������\n");
        for (int i = w; i > 0; i--)
        {
            //QueryPerformanceCounter(&time_start);
            //srand(time_start.QuadPart);
            int k = i *99999999999 * 1024 + 3;
            CList_AddItem_end(list, &k, sizeof(int));
        }
        printf("����������ɵ���������ļ�a.txt\n");
        FILE* fp = fopen("a.txt", "w");
        if (fp == NULL)
        {
            printf("�޷����ļ�\n");
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
        printf("�����������ļ�·��:");
        char* input_path = (char*)malloc(1024);
        scanf("%s", input_path);
        FILE* fp = fopen(input_path,"r");
        if (fp == NULL)
        {
            printf("���ļ�ʧ��\n");
            return 1;
        }
        int d;
        printf("��ʼ��ȡ�ļ�����\n");
        int count = 0;
        while (1)
        {
            count++;
            if (1 != fscanf(fp, "%d", &d))
                break;
            CList_AddItem_end(list, &d, sizeof(int));
        }
        fclose(fp);
        printf("��ȡ��%d������\n", count-1);
    }
    // ����//11306293.300000//
    printf("��ʼ����\n");
    if (type == 2)
    {
        QueryPerformanceCounter(&time_start);	//��ʱ��ʼ
        CList_Sort_QuickSort(list, (int (*)(void*, void*))cmp);
        QueryPerformanceCounter(&time_over);	//��ʱ����
    }
    else
    {
        QueryPerformanceCounter(&time_start);	//��ʱ��ʼ
        CList_Sort_BubbleSort(list, (int (*)(void*, void*))cmp);
        QueryPerformanceCounter(&time_over);	//��ʱ����
    }
    run_time = 1000000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
    printf("������ɣ�������ʱ��ʱ%d΢��\n",(int)run_time);
    CList_ResetIterators(list);
    printf("���ڽ�������������ļ�b.txt\n");
    FILE* fp = fopen("b.txt", "w");
    if (fp == NULL)
    {
        printf("��b.txtʧ��\n");
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
    printf("������\n��ʼ�����ڴ�\n");
    CList_Delete(list);
    printf("�������\n");
    system("pause");
    goto start;
    return 0;
}