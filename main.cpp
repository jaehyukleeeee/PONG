#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include "Screen.h"

typedef enum _DIRECT { TOP, RIGHT_TOP, RIGHT_DOWN, DOWN, LEFT_DOWN, LEFT_TOP, LEFT, RIGHT }DIRECT;

typedef struct _BALL
{
	int nIsReady;         //준비상태, 슛상태
	int nMoveX, nMoveY;      //이동좌표
	DIRECT nDirect;         //이동방향
	clock_t MoveTime;      //이동시간 간격
	clock_t OldTime;      //이전 이동시간
}BALL;

typedef struct _BAR1
{
	int nMoveX;
	int nMoveY[3];
	clock_t OldTime;
	clock_t MoveTime;
}BAR1;

typedef struct _BAR2
{
	int nMoveX;
	int nMoveY[3];
	clock_t OldTime;
	clock_t MoveTime;
}BAR2;

BAR1 g_sBar1;
BAR2 g_sBar2;
BALL g_sBall;

int p1scr;
int p2scr;

// 벽과 충돌하게 되면 방향 상태
int g_StateTable[4][6] = {
	{ 3, 2,-1,-1,-1, 4 },
	{ -1, 5, 4,-1,-1,-1 },
	{ -1,-1, 1, 0, 5,-1 },
	{ -1,-1,-1,-1, 2, 1 }
};

// 초기화
void Init()
{
	//막대기1 초기화
	g_sBar1.nMoveX = 2;
	g_sBar1.nMoveY[0] = 11;
	g_sBar1.nMoveY[1] = 12;
	g_sBar1.nMoveY[2] = 13;
	g_sBar1.MoveTime = 1;
	g_sBar1.OldTime = clock();

	//막대기2 초기화
	g_sBar2.nMoveX = 76;
	g_sBar2.nMoveY[0] = 11;
	g_sBar2.nMoveY[1] = 12;
	g_sBar2.nMoveY[2] = 13;
	g_sBar2.MoveTime = 1;
	g_sBar2.OldTime = clock();

	//공 초기화
	g_sBall.nIsReady = 1;
	g_sBall.nMoveX = 38;
	g_sBall.nMoveY = 12;
	g_sBall.nDirect = RIGHT;
	g_sBall.OldTime = clock();
	g_sBall.MoveTime = 50;
}

int g_BlockState[6] = { 3, 2, 1, 0, 5, 4 };

//Note : 충돌 체크 함수
int Collision(int nX, int nY)
{
	// 막대기와 충돌체크
	if (nX == g_sBar1.nMoveX)
	{
		if (g_sBar1.nMoveY[0] == g_sBall.nMoveY)
		{
			g_sBall.nDirect = RIGHT_TOP;
			printf("\a");
		}
		if (g_sBar1.nMoveY[1] == g_sBall.nMoveY)
		{
			g_sBall.nDirect = RIGHT;
			printf("\a");
		}
		if (g_sBar1.nMoveY[2] == g_sBall.nMoveY)
		{
			g_sBall.nDirect = RIGHT_DOWN;
			printf("\a");
		}
	}
	if (nX == g_sBar2.nMoveX)
	{
		if (g_sBar2.nMoveY[0] == g_sBall.nMoveY)
		{
			g_sBall.nDirect = LEFT_TOP;
			printf("\a");
		}
		if (g_sBar2.nMoveY[1] == g_sBall.nMoveY)
		{
			g_sBall.nDirect = LEFT;
			printf("\a");
		}
		if (g_sBar2.nMoveY[2] == g_sBall.nMoveY)
		{
			g_sBall.nDirect = LEFT_DOWN;
			printf("\a");
		}
	}

	//Note : 위쪽 0
	if (nY < 0)
	{
		g_sBall.nDirect = (DIRECT)g_StateTable[0][g_sBall.nDirect];
		return 1;
	}
	//Note : 오른쪽 1
	if (nX > 78)
	{
		g_sBall.nIsReady = 1;
		g_sBall.nMoveX = 37;
		g_sBall.nMoveY = 12;
		p1scr++;
		return 0;
	}
	//Note : 아래쪽 2
	if (nY > 24)
	{
		g_sBall.nDirect = (DIRECT)g_StateTable[2][g_sBall.nDirect];
		return 1;
	}
	//Note : 왼쪽 3
	if (nX < 0)
	{
		g_sBall.nIsReady = 1;
		g_sBall.nMoveX = 37;
		g_sBall.nMoveY = 12;
		p2scr++;
		return 0;
	}
	return 0;
}

// 데이터 갱신
void Update()
{
	clock_t CurTime = clock();
	if (g_sBall.nIsReady == 0)
	{
		if (CurTime - g_sBall.OldTime > g_sBall.MoveTime)
		{
			g_sBall.OldTime = CurTime;
			switch (g_sBall.nDirect)
			{
			case TOP:
				if (Collision(g_sBall.nMoveX, g_sBall.nMoveY - 1) == 0)
					g_sBall.nMoveY--;
				break;
			case RIGHT_TOP:
				if (Collision(g_sBall.nMoveX + 1, g_sBall.nMoveY - 1) == 0)
				{
					g_sBall.nMoveX++;
					g_sBall.nMoveY--;
				}
				break;
			case RIGHT_DOWN:
				if (Collision(g_sBall.nMoveX + 1, g_sBall.nMoveY + 1) == 0)
				{
					g_sBall.nMoveX++;
					g_sBall.nMoveY++;
				}
				break;
			case DOWN:
				if (Collision(g_sBall.nMoveX, g_sBall.nMoveY + 1) == 0)
					g_sBall.nMoveY++;
				break;
			case LEFT_DOWN:
				if (Collision(g_sBall.nMoveX - 1, g_sBall.nMoveY + 1) == 0)
				{
					g_sBall.nMoveX--;
					g_sBall.nMoveY++;
				}
				break;
			case LEFT_TOP:
				if (Collision(g_sBall.nMoveX - 1, g_sBall.nMoveY - 1) == 0)
				{
					g_sBall.nMoveX--;
					g_sBall.nMoveY--;
				}
				break;
			case LEFT:
				if (Collision(g_sBall.nMoveX - 1, g_sBall.nMoveY - 1) == 0)
				{
					g_sBall.nMoveX -= 2;
				}
				break;
			case RIGHT:
				if (Collision(g_sBall.nMoveX + 1, g_sBall.nMoveY + 1) == 0)
				{
					g_sBall.nMoveX += 2;
				}
				break;
				/*case L:
				g_sBall.nMoveX-=2;
				break;
				case R:
				g_sBall.nMoveX+=2;
				break;
				case STOP:
				break;*/
			}

		}
		/*Note : 경계영역과 충돌
		if(g_sBall.nMoveX < 0 || g_sBall.nMoveX > 78 || g_sBall.nMoveY < 0 || g_sBall.nMoveY > 24)
		{
		g_sBall.nIsReady = 1;
		g_sBall.nMoveX = 38;
		g_sBall.nMoveY = 12;
		g_sBall.nDirect = TOP;
		g_sBall.OldTime = clock();
		}*/
	}
}
// 렌더링
void Render()
{
	ScreenClear();
	for (int i = 0; i < 25; i++)
	{
		if (i == 2)
		{
			ScreenPrint(38,i," ");
		}
		else if (i == 3)
		{
			ScreenPrint(35, i, "shot : f");
		}
		else
		{
			ScreenPrint(38, i, "|");
		}
	}

	char string[100] = { 32,4 };
	sprintf_s(string, "%d", p1scr);
	ScreenPrint(32, 4, string);
	sprintf_s(string, "%d", p2scr);
	ScreenPrint(44, 4, string);
	//ScreenPrint(32, 4, "%d", p1scr);

	ScreenPrint(35, 2, "p o");
	ScreenPrint(39, 2, "n g");

	ScreenPrint(g_sBall.nMoveX, g_sBall.nMoveY, "●");

	ScreenPrint(g_sBar1.nMoveX, g_sBar1.nMoveY[0], "■");
	ScreenPrint(g_sBar1.nMoveX, g_sBar1.nMoveY[1], "■");
	ScreenPrint(g_sBar1.nMoveX, g_sBar1.nMoveY[2], "■");

	ScreenPrint(g_sBar2.nMoveX, g_sBar2.nMoveY[0], "■");
	ScreenPrint(g_sBar2.nMoveX, g_sBar2.nMoveY[1], "■");
	ScreenPrint(g_sBar2.nMoveX, g_sBar2.nMoveY[2], "■");

	ScreenPrint(0, 10, "↑");
	ScreenPrint(1, 11, "w");
	ScreenPrint(1, 13, "s");
	ScreenPrint(0, 14, "↓");

	ScreenPrint(78, 10, "↑");
	ScreenPrint(78, 11, "i");
	ScreenPrint(78, 13, "k");
	ScreenPrint(78, 14, "↓");
	
	ScreenFlipping();
}

// 해제
void Release()
{
}


int main(void)
{
	int nKey;
	clock_t CurTime;
	ScreenInit();  // 화면 초기화
	Init();

	while (1)
	{
		if (_kbhit())
		{
			nKey = _getch();
			switch (nKey)
			{
			case 'f':
			{
				g_sBall.nIsReady = 0;   //이동상태
			}
			break;
			case 'w':
				CurTime = clock();
				if (CurTime - g_sBar1.OldTime > g_sBar1.MoveTime)
				{
					g_sBar1.OldTime = CurTime;
					if (g_sBar1.nMoveY[0] > 0) //위쪽 경계영역 체크
					{
						g_sBar1.nMoveY[0]--;
						g_sBar1.nMoveY[1]--;
						g_sBar1.nMoveY[2]--;

						/*if (g_sBall.nIsReady == 1)
							g_sBall.nMoveY--;*/
					}
				}
				break;
			case 's':
				CurTime = clock();
				if (CurTime - g_sBar1.OldTime > g_sBar1.MoveTime)
				{
					g_sBar1.OldTime = CurTime;
					if (g_sBar1.nMoveY[2] < 24) //아래쪽 경계영역 체크
					{
						g_sBar1.nMoveY[0]++;
						g_sBar1.nMoveY[1]++;
						g_sBar1.nMoveY[2]++;

						/*if (g_sBall.nIsReady == 1)
							g_sBall.nMoveY++;*/
					}
				}
				break;
			case 'i':
				CurTime = clock();
				if (CurTime - g_sBar2.OldTime > g_sBar2.MoveTime)
				{
					g_sBar2.OldTime = CurTime;
					if (g_sBar2.nMoveY[0] > 0) //위쪽 경계영역 체크
					{
						g_sBar2.nMoveY[0]--;
						g_sBar2.nMoveY[1]--;
						g_sBar2.nMoveY[2]--;
					}
				}
				break;
			case 'k':
				CurTime = clock();
				if (CurTime - g_sBar2.OldTime > g_sBar2.MoveTime)
				{
					g_sBar2.OldTime = CurTime;
					if (g_sBar2.nMoveY[2] < 24) //아래쪽 경계영역 체크
					{
						g_sBar2.nMoveY[0]++;
						g_sBar2.nMoveY[1]++;
						g_sBar2.nMoveY[2]++;
					}
				}
				break;
			case '8':
				g_sBall.nDirect = TOP;
				break;
			case '9':
				g_sBall.nDirect = RIGHT_TOP;
				break;
			case '3':
				g_sBall.nDirect = RIGHT_DOWN;
				break;
			case '2':
				g_sBall.nDirect = DOWN;
				break;
			case '1':
				g_sBall.nDirect = LEFT_DOWN;
				break;
			case '7':
				g_sBall.nDirect = LEFT_TOP;
				break;
			}
		}
		Update();
		Render();
	}

	Release();
	ScreenRelease(); // 화면 해제   
	return 0;
}