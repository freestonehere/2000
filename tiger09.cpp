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
	/*初始币数，每次赢的币数，每次输的币数，每轮游戏最多玩的次数，结束条件的币数量；*/
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
		bsame = 0;	//是不是生成了3个重复数据 
		j = 0;		//每次比赛最多num组 
		while (!bsame && j < num) {
			for (i = 0; i < 3; ++i) { data[i] = rand() % 4; }
			if (data[0] == data[1] && data[1] == data[2])
				bsame = 1;
			++j;
		}
		if (bsame == 1)sum += winPer;
		else sum -= lostPer;
		++itime;//次数 
	} // end of for 
	printf("money=%d\n", sum);
	return itime;
}
