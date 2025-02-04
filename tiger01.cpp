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

// 根据游戏输赢情况调整游戏速度
void adjustGameSpeed()
{
    if (winsThisRound > 0)
    {
        // 每赢一次，适当加快速度
        gameSpeed = max(minGameSpeed, gameSpeed - 100);
    }
    else
    {
        // 如果输了，适当减慢速度
        gameSpeed = min(maxGameSpeed, gameSpeed + 100);
    }
    startTimer(moveID, (int)gameSpeed); 
}

// 游戏主循环
void gameLoop() {
    while (gameRunning && (playerCoins > 0 && playerCoins < maxCoins)) {
        init();
        paint();

        triesThisCoin = 0;
        winsThisRound = 0; // 每轮开始时重置胜利次数

        bool innerLoopRunning = true;
        while (innerLoopRunning && triesThisCoin < maxTriesPerCoin && playerCoins > 0 && playerCoins < maxCoins) {
            // 等待玩家按键，使用更严谨的消息循环机制来确保能正确响应和处理消息
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

            // 检查是否达到最大尝试次数
            if (triesThisCoin >= maxTriesPerCoin) {
                innerLoopRunning = false;
                break;
            }
        }

        //调整硬币数量
        if (winsThisRound > 0) {
            playerCoins += 10;
        }
        else {
            playerCoins--; // 如果没有胜利，则消耗一个硬币
        }

        if (playerCoins <= 0 || playerCoins >= maxCoins) {
            gameRunning = false;
            cancelTimer(moveID); 
            break;
        }

        adjustGameSpeed(); // 调整游戏速度
    }
    cancelTimer(moveID);
    gameRunning = false;

    if (playerCoins == 0) {
        beginPaint();
        char str[50];
        strcpy(str, "No more coins!");
        paintText(winWidth / 2 - strlen(str) * 5, winHeight / 2, str);
        endPaint();
        Sleep(2000); // 等待2秒
    }
    else if (playerCoins >= maxCoins) {
        beginPaint();
        char str[50];
        strcpy(str, "You've reached the max coins!");
        paintText(winWidth / 2 - strlen(str) * 5, winHeight / 2, str);
        endPaint();
        Sleep(2000); // 等待2秒
    }
}


// 处理键盘按键事件
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
        // 准备下一轮游戏
        init();
        paint();
    }
    if (gameRunning) {
        startTimer(moveID, (int)gameSpeed);
    }
}

// 程序入口函数
int Setup()
{
    initGameWindow();
    gameLoop();
    return 0;
}
// 游戏初始化函数
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

// 绘制游戏界面
void paint() {
    if (gameRunning) {
        beginPaint();
        for (int i = 0; i < ROW; ++i)
            for (int j = 0; j < COL; ++j)
                putImageScale(&img[pos[i][j]], j * imgWidth, i * imgHeight, imgWidth, imgHeight);
        endPaint();
    }
}

// 定时器事件处理函数
void timerEvent(int id) {
    if (id == 0 && gameRunning) {
        for (int i = ROW - 2; i >= 0; i--) {
            for (int j = 0; j < COL; j++) {
                pos[i + 1][j] = pos[i][j];
            }
        }
        for (int i = 0; i < COL; ++i)pos[0][i] = rand() % 4;
        paint();//重画窗口
    }
}

void initGameWindow()
{
    initWindow("tiger game", 0, 0, winWidth, winHeight);
}