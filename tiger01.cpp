#define _CRT_SECURE_NO_WARNINGS
#include "resource.h"
#include "acllib.h"
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>
void timerEvent(int id);
void init();
void adjustGameSpeed();
void gameLoop();
void keyEvent(int key, int e);
void paint();
void initGameWindow();

const int winWidth = 600, winHeight = 600;
const int imgWidth = 200, imgHeight = 200;
const int NUM = 4, ROW = 3, COL = 3;
float gameSpeed = 1000; 
const int minGameSpeed = 1200; 
const int maxGameSpeed = 800; 
const int maxTriesPerCoin = 100; 
int triesThisCoin = 0;
int winsThisRound = 0; 
bool gameRunning = true; 
ACL_Image img[NUM];
int pos[ROW][COL];
const int moveID = 0;
int playerCoins = 10; 
const int maxCoins = 100; 
HINSTANCE hInstance = GetModuleHandle(NULL); 

// ������Ϸ��Ӯ���������Ϸ�ٶ�
void adjustGameSpeed()
{
    if (winsThisRound > 0)
    {
        // ÿӮһ�Σ��ʵ��ӿ��ٶ�
        gameSpeed = max(minGameSpeed, gameSpeed - 100);
    }
    else
    {
        // ������ˣ��ʵ������ٶ�
        gameSpeed = min(maxGameSpeed, gameSpeed + 100);
    }
    startTimer(moveID, (int)gameSpeed); 
}

// ��Ϸ��ѭ��
void gameLoop() {
    while (gameRunning && (playerCoins > 0 && playerCoins < maxCoins)) {
        init();
        paint();

        triesThisCoin = 0;
        winsThisRound = 0; // ÿ�ֿ�ʼʱ����ʤ������

        bool innerLoopRunning = true;
        while (innerLoopRunning && triesThisCoin < maxTriesPerCoin && playerCoins > 0 && playerCoins < maxCoins) {
            // �ȴ���Ұ�����ʹ�ø��Ͻ�����Ϣѭ��������ȷ������ȷ��Ӧ�ʹ�����Ϣ
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    gameRunning = false;
                    innerLoopRunning = false;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            // ����Ƿ�ﵽ����Դ���
            if (triesThisCoin >= maxTriesPerCoin) {
                innerLoopRunning = false;
                break;
            }
        }

        //����Ӳ������
        if (winsThisRound > 0) {
            playerCoins += 10;
        }
        else {
            playerCoins--; // ���û��ʤ����������һ��Ӳ��
        }

        if (playerCoins <= 0 || playerCoins >= maxCoins) {
            gameRunning = false;
            cancelTimer(moveID); 
            break;
        }

        adjustGameSpeed(); // ������Ϸ�ٶ�
    }
    cancelTimer(moveID);
    gameRunning = false;

    if (playerCoins == 0) {
        beginPaint();
        char str[50];
        strcpy(str, "No more coins!");
        paintText(winWidth / 2 - strlen(str) * 5, winHeight / 2, str);
        endPaint();
        Sleep(2000); // �ȴ�2��
    }
    else if (playerCoins >= maxCoins) {
        beginPaint();
        char str[50];
        strcpy(str, "You've reached the max coins!");
        paintText(winWidth / 2 - strlen(str) * 5, winHeight / 2, str);
        endPaint();
        Sleep(2000); // �ȴ�2��
    }
}


// ������̰����¼�
void keyEvent(int key, int e)
{
    if (!gameRunning) {
        return;
    }
    if (e != KEY_DOWN || triesThisCoin >= maxTriesPerCoin)
        return;

    beginPaint();
    cancelTimer(moveID);
    setTextSize(40);
    char str[50];
    switch (key) {
    case VK_RETURN:
        if (pos[1][0] == pos[1][1] && pos[1][1] == pos[1][2]) {
            strcpy(str, "you win!");
            paintText(winWidth / 2 - strlen(str) * 10, winHeight / 2, str);
            winsThisRound++;
        }
        else {
            strcpy(str, "you lost!");
            paintText(winWidth / 2 - strlen(str) * 10, winHeight / 2, str);
        }
        triesThisCoin++;
        break;
    }
    endPaint();
    if (triesThisCoin >= maxTriesPerCoin) {
        // ׼����һ����Ϸ
        init();
        paint();
    }
    if (gameRunning) {
        startTimer(moveID, (int)gameSpeed);
    }
}

// ������ں���
int Setup()
{
    initGameWindow();
    gameLoop();
    return 0;
}
// ��Ϸ��ʼ������
void init() {
    srand((unsigned)time(NULL));
    registerTimerEvent(timerEvent);
    startTimer(moveID, (int)gameSpeed);
    registerKeyboardEvent(keyEvent);
    loadImage(hInstance, 102, &img[0]);
    loadImage(hInstance, 104, &img[1]);
    loadImage(hInstance, 105, &img[2]);
    loadImage(hInstance, 106, &img[3]);
    for (int i = 0, j = 0; i < ROW; ++i)
        for (int j = 0; j < COL; ++j)
            pos[i][j] = rand() % NUM;
}

// ������Ϸ����
void paint() {
    if (gameRunning) {
        beginPaint();
        for (int i = 0; i < ROW; ++i)
            for (int j = 0; j < COL; ++j)
                putImageScale(&img[pos[i][j]], j * imgWidth, i * imgHeight, imgWidth, imgHeight);
        endPaint();
    }
}

// ��ʱ���¼�������
void timerEvent(int id) {
    if (id == 0 && gameRunning) {
        for (int i = ROW - 2; i >= 0; i--) {
            for (int j = 0; j < COL; j++) {
                pos[i + 1][j] = pos[i][j];
            }
        }
        for (int i = 0; i < COL; ++i)pos[0][i] = rand() % 4;
        paint();//�ػ�����
    }
}

void initGameWindow()
{
    initWindow("tiger game", 0, 0, winWidth, winHeight);
}