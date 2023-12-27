#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

typedef const int cint;
typedef const char cchar;

/*һ����д���ֵĽṹ��*/
typedef struct
{
    int pixel[1024];//���ص�
    int label;//
}Digit;

/*һ����label�ľ���ṹ��*/
typedef struct
{
    float distance;
    int label;
}Distance;

/*�ļ�·��+����*/
cchar* trainingFile = "C:\\Users\86147\Desktop\mnist_dataset\train-labels.txt";
cchar* testingFile = "C:\\Users\86147\Desktop\mnist_dataset\t10k-labels.txt";
cchar* predictFile = "C:\\Users\86147\Desktop\mnist_dataset\t10k-images.idx3-ubyte";

/*ÿ�����ݼ������ָ���*/
cint   ntrain = 3;//943
cint    ntest = 6;//196
cint npredict = 1;//9

float calDistance(Digit digit1, Digit digit2)
/*�����*/
{
    int i, squareSum = 0.0;
    for (i = 0; i < 1024; i++)
    {
        squareSum += pow(digit1.pixel[i] - digit2.pixel[i], 2.0);
    }
    return sqrtf(squareSum);
}

int loadDigit(Digit* digit, FILE* fp, int* labels)
/*��ȡdigit*/
{
    int index = 0;
    for (index = 0; index < 1024; index++)
    {
        if (!fscanf_s(fp, "%d", &(digit->pixel[index])))
        {
            printf("FILE already read finish.\n");
            return -1;
        }
    }
    fscanf_s(fp, "%d", &(digit->label));
    *labels = digit->label;

    return 1;
}

void showDigit(Digit digit)
/*��ʾһ��Digit �ṹ��*/
{
    int i, j, id;
    for (i = 0; i < 32; i++)
    {
        for (j = 0; j < 32; j++)
        {
            printf("%d", digit.pixel[i * 32 + j]);
        }
        printf("\n");
    }
    printf(" %d \n", digit.label);
}

void exchange(Distance* in, int index1, int index2)
/*�����ַ�������*/
{
    Distance tmp = (Distance)in[index1];
    in[index1] = in[index2];
    in[index2] = tmp;
}

void selectSort(Distance* in, int length)
/*ѡ������*/
{
    int i, j, min;
    int N = length;
    for (i = 0; i < N - 1; i++)
    {
        min = i;
        for (j = i + 1; j < N; j++)
        {
            if (in[j].distance < in[min].distance) min = j;
        }
        exchange(in, i, min);
    }
}
int prediction(int K, Digit in, Digit* train, int nt)
/*����ѵ������Ԥ��һ������digit*/
{
    int i, it;
    Distance* distance=(Distance*)malloc(nt*sizeof(Distance));//����һ������
    /*��ȡ����digit��ѵ�����ݵľ���*/
    for (it = 0; it < nt; it++)
    {
        distance[it].distance = calDistance(in, train[it]);
        distance[it].label = train[it].label;
    }
    /*������ľ�������(ѡ������)*/
    int predict = 0;
    selectSort(distance, nt);
    for (i = 0; i < K; i++)/*��������õ�������ȡǰK�����ݣ���������in��K�����ݣ�*/
    {
        predict += distance[i].label;
    }
    return (int)(predict / K);//�����Ľ�
}
void knn_classifiy(int K)
/*�ò������ݼ����в���*/
{
    printf(".knn_classifiy.\n");

    int i;
    FILE* fp;

    /*����ѵ������*/
    int trainLabels[ntrain];
    int trainCount[10] = { 0 };
    Digit* Dtrain = (Digit*)malloc(ntrain * sizeof(Digit));
    fp = fopen(trainingFile, "r");
    printf("..load training digits.\n");
    for (i = 0; i < ntrain; i++)
    {
        loadDigit(&Dtrain[i], fp, &trainLabels[i]);
        trainCount[Dtrain[i].label]++;
    }
    fclose(fp);
    printf("..Done.\n");

    /*�����������*/
    int testLabels[ntest];
    int testCount[10] = { 0 };
    Digit* Dtest = (Digit*)malloc(ntest * sizeof(Digit));
    fp = fopen(testingFile, "r");
    printf("..load testing digits.\n");
    for (i = 0; i < ntest; i++)
    {
        loadDigit(&Dtest[i], fp, &testLabels[i]);
        testCount[Dtest[i].label]++;
    }
    fclose(fp);
    printf("..Done.\n");

    /*�����������ѵ������֮��ľ���*/
    printf("..Cal Distance begin.\n");
    Distance Distance2Train[ntrain];
    int CorrectCount[10] = { 0 };
    int itrain, itest, predict;
    for (itest = 0; itest < ntest; itest++)
    {
        predict = prediction(K, Dtest[itest], Dtrain, ntrain);
        //printf("%d-%d\n",predict, Dtest[itest].label);

        /*��Ԥ��׼ȷ�Ľ��м���*/
        if (predict == Dtest[itest].label)
        {
            CorrectCount[predict]++;
        }
    }

    /*����������ݵ�׼ȷ��*/
    printf("    Correct radio:   \n\n");
    for (i = 0; i < 10; i++)
    {
        printf("%d:  (  %2d / %2d ) =  %.2f%%\n",
            i,
            CorrectCount[i],
            testCount[i],
            (float)(CorrectCount[i] * 1.0 / testCount[i] * 100));
    }
}
void knn_predict(int K)
/*Ԥ������*/
{
    int i;
    FILE* fp;

    /*����ѵ������*/
    int trainLabels[ntrain];
    int trainCount[10] = { 0 };
    Digit* Dtrain = (Digit*)malloc(ntrain * sizeof(Digit));
    fp = fopen(trainingFile, "r");
    printf("..load training digits.\n");
    for (i = 0; i < ntrain; i++)
    {
        loadDigit(&Dtrain[i], fp, &trainLabels[i]);
        trainCount[Dtrain[i].label]++;
    }
    fclose(fp);
    printf("..Done.\n");

    /*������ҪԤ�������*/
    int predictLabels[npredict];
    int predictCount[10] = { 0 };
    Digit* Dpredict = (Digit*)malloc(npredict * sizeof(Digit));
    fp = fopen(predictFile, "r");
    printf("..load predict digits.\n");
    for (i = 0; i < npredict; i++)
    {
        loadDigit(&Dpredict[i], fp, &predictLabels[i]);
        predictCount[Dpredict[i].label]++;
    }
    fclose(fp);
    printf("..Done.\n");

    /*������������ѵ������֮��ľ���*/
    printf("..Cal Distance begin.\n");
    Distance Distance2Train[ntrain];
    int itrain, ipredict, predict;
    for (ipredict = 0; ipredict < npredict; ipredict++)
    {
        predict = prediction(K, Dpredict[ipredict], Dtrain, ntrain);
        printf("%d\n", predict);
    }
}
int main(int argc, char** argv)
{
    int K = 1;
    /*����֪���ݽ��в��ԣ�ͳ��Ԥ�����ȷ��*/
    knn_classifiy(K);
    /*��λ�����ݽ���Ԥ��*/
    knn_predict(K);

    return 1;
}