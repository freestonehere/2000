#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int tiger(int initMoney, int winPer, int lostPer, int num, int finishMoney);

int main9()
{
	int itime,winPer, lostPer,num;
 	srand((unsigned int)time(NULL));
	scanf_s("%d%d%d",&winPer, &lostPer,&num);
	itime = tiger(10, winPer, lostPer, num, 100);
	/*��ʼ������ÿ��Ӯ�ı�����ÿ����ı�����ÿ����Ϸ�����Ĵ��������������ı�������*/
	printf("sum=%d\n", itime);
	system("pause");
	return 0;
}

int tiger(int initMoney, int winPer, int lostPer, int num, int finishMoney)
{
	int sum = initMoney, n = 0;
	int data[3] = { 0 }, i, j;
	int bsame = 0, itime = 0;
	for (; sum < finishMoney && sum > 0;)
	{
		bsame = 0;	//�ǲ���������3���ظ����� 
		j = 0;		//ÿ�α������num�� 
		while (!bsame && j < num) {
			for (i = 0; i < 3; ++i) { data[i] = rand() % 4; }
			if (data[0] == data[1] && data[1] == data[2])
				bsame = 1;
			++j;
		}
		if (bsame == 1)sum += winPer;
		else sum -= lostPer;
		++itime;//���� 
	} // end of for 
	printf("money=%d\n", sum);
	return itime;
}
