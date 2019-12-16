#include<windows.h>
#include<time.h>
#include"resource.h"
#pragma comment(lib, "Msimg32.lib")

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("김태헌_2014032050");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	srand((unsigned)time(0));

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 710, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return Message.wParam;

}

#define HEIGHT 25
#define WIDTH 14
#define X_START 47
#define Y_START 30
#define SIZE 22
#define X_NEXTBLOCK 20
#define Y_NEXTBLOCK 60

TCHAR C_Score[50];
TCHAR C_BlockCount[50];
TCHAR C_Level[50];
TCHAR C_Lineleft[50];

int m_cy, m_cx, m_Form, m_Rotation, m_NextBlock, m_Score, m_Level, m_BlockCount, m_DownSpeed, m_NextLevel;
bool b_NowGame;
int m_Field[HEIGHT][WIDTH] = { 0, };
int m_ColorField[HEIGHT][WIDTH] = { 0, };
int m_Block[7][4][4][4] = {
	//  ■
	//  ■
	//  ■
	//  ■
	{
	{{ 1,1,1,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 }},
	{{ 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 }},
	{{ 1,1,1,1 },{ 0,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 }},
	{{ 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 },{ 0,0,1,0 }}
	},

	//  ■■
	//  ■■
	{
	{ { 1,1,0,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,0,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,0,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,0,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } }
	},

	//  ■■
	//    ■■
	{
	{ { 1,1,0,0 },{ 0,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,0,0 },{ 1,1,0,0 },{ 1,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,0,0 },{ 0,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,0,0 },{ 1,1,0,0 },{ 1,0,0,0 },{ 0,0,0,0 } }
	},

	//    ■■
	//  ■■
	{
	{ { 0,1,1,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,0,0,0 },{ 1,1,0,0 },{ 0,1,0,0 },{ 0,0,0,0 } },
	{ { 0,1,1,0 },{ 1,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,0,0,0 },{ 1,1,0,0 },{ 0,1,0,0 },{ 0,0,0,0 } }
	},

	//  ■
	//  ■■■
	{
	{ { 1,0,0,0 },{ 1,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,0,0 },{ 1,0,0,0 },{ 1,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,1,0 },{ 0,0,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,0,0 },{ 0,1,0,0 },{ 1,1,0,0 },{ 0,0,0,0 } }
	},

	//      ■
	//  ■■■
	{
	{ { 0,0,1,0 },{ 1,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,0,0,0 },{ 1,0,0,0 },{ 1,1,0,0 },{ 0,0,0,0 } },
	{ { 1,1,1,0 },{ 1,0,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,0,0 },{ 0,1,0,0 },{ 0,1,0,0 },{ 0,0,0,0 } }
	},

	//    ■
	//  ■■■
	{
	{ { 0,1,0,0 },{ 1,1,1,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 1,0,0,0 },{ 1,1,0,0 },{ 1,0,0,0 },{ 0,0,0,0 } },
	{ { 1,1,1,0 },{ 0,1,0,0 },{ 0,0,0,0 },{ 0,0,0,0 } },
	{ { 0,1,0,0 },{ 1,1,0,0 },{ 0,1,0,0 },{ 0,0,0,0 } }
	}
};


void init_Field()
{
	for (int i = 0; i < HEIGHT; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (j == 1 || j == 0 || j == 12 || j == 13)
				m_Field[i][j] = -1;
			else
				m_Field[i][j] = 0;
		}
	}
}

void LevelCheck(HWND hWnd)
{
	if (m_BlockCount >= m_NextLevel)
	{
		m_DownSpeed -= 30;
		m_Level++;
		m_NextLevel += m_Level * 5;
		KillTimer(hWnd, 2);
		SetTimer(hWnd, 2, m_DownSpeed, NULL);
	}
}

void CheckEnd(HWND hWnd)
{
	b_NowGame = FALSE;
	for (int i = 2; i < WIDTH - 2; i++)
	{
		if (m_Field[2][i] == -1)
		{
			
			KillTimer(hWnd, 1);
			KillTimer(hWnd, 2);
			if (MessageBox(hWnd, TEXT("다시하시겠습니까?"), TEXT("GameOver"), MB_YESNO) == IDYES)
			{
				SetTimer(hWnd, 1, 25, NULL);
				return;
			}
			else
			{
				KillTimer(hWnd, 3);
				PostQuitMessage(0);
			}
		}

	}
	b_NowGame = TRUE;
}

void Fix_Block(int height)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] == 1)
			{
				m_Field[i + m_cy][j + m_cx] = -1;
			}
		}
	}
	m_Score = m_Score + (HEIGHT - height - 1) * 2;
	m_cy = 0;
	m_cx = 5;
	m_Form = m_NextBlock;
	m_NextBlock = rand() % 7;
	m_Rotation = 0;
}

void All_Block_Down(int h)
{
	for (int i = h; i > 1; i--)
	{
		for (int j = 2; j < WIDTH - 2 ; j++)
		{
			m_Field[i][j] = m_Field[i-1][j];
			m_ColorField[i][j] = m_ColorField[i - 1][j];
		}
	}
}
 
void Remove_Block()
{
	int temp, count = 0;
	for (int i = HEIGHT; i > 0; i--)
	{
		temp = 0;
		for (int j = 2; j < WIDTH - 2; j++)
		{
			if (m_Field[i][j] == -1)
			{
				temp++;
				if (temp == 10)
				{
					for (int k = 2; k < WIDTH - 2; k++)
					{
						m_Field[i][k] = 0;
					}
					m_BlockCount++;
					All_Block_Down(i++);
					count++;
				}
			}
		}
	}
	if (count == 1)
		m_Score += 50;
	if (count == 2)
		m_Score += 150;
	if (count == 3)
		m_Score += 400;
	if (count == 4)
		m_Score += 900;
}

int Block_Down()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] == 1)
			{
				if (m_Field[i + m_cy + 1][j + m_cx] == -1 || (i + m_cy + 1) == HEIGHT - 1)
				{
					Fix_Block(i + m_cy);
					Remove_Block();
					return 1;
				}
			}
		}
	}
	
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] != -1)
			{
				m_Field[i + m_cy][j + m_cx] = 0;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy + 1][j + m_cx] != -1)
			{
				m_Field[i + m_cy + 1][j + m_cx] = m_Block[m_Form][m_Rotation][i][j];
				if(m_Block[m_Form][m_Rotation][i][j] == 1)
					m_ColorField[i + m_cy + 1][j + m_cx] = m_Form;
			}
		}
	}

	m_cy++;

	
	
}

void Hard_Drop()
{
	while (true)
	{
		if (Block_Down() == 1)
			return;
	}
}

void Rotate_Block()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Block[m_Form][(m_Rotation + 1) % 4][i][j] == 1)
			{
				if (m_Field[i + m_cy][j + m_cx] == -1)
					return ;
			}
		}
	}
	m_Rotation = (m_Rotation + 1) % 4;
}

void Move_Left_Block()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] == 1)
			{
				if (m_Field[i + m_cy][j + m_cx - 1] == -1 || (j + m_cx - 1) < 0)
				{
					return ;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] != -1)
			{
				m_Field[i + m_cy][j + m_cx] = 0;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx - 1] != -1)
			{
				m_Field[i + m_cy][j + m_cx - 1] = m_Block[m_Form][m_Rotation][i][j];
				if (m_Block[m_Form][m_Rotation][i][j] == 1)
					m_ColorField[i + m_cy][j + m_cx - 1] = m_Form;
			}
		}
	}
	m_cx--;
}

void Move_Right_Block()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] == 1)
			{
				if (m_Field[i + m_cy][j + m_cx + 1] == -1 || (j + m_cx + 1) > WIDTH - 1)
				{
					return ;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx] != -1)
			{
				m_Field[i + m_cy][j + m_cx] = 0;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (m_Field[i + m_cy][j + m_cx + 1] != -1)
			{
				m_Field[i + m_cy][j + m_cx + 1] = m_Block[m_Form][m_Rotation][i][j];
				if (m_Block[m_Form][m_Rotation][i][j] == 1)
					m_ColorField[i + m_cy][j + m_cx + 1] = m_Form;
			}
		}
	}
	m_cx++;
}

// 비트맵그리는 함수
void DrawBitmap(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	// 비트맵의 정보(크기)를 저장
	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	// 비트맵 고속복사
	BitBlt(hdc, x, y, bx, by, MemDC, 0, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

int frame;
void DrawTitle(HDC hdc, int x, int y, HBITMAP hBit)
{
	HDC MemDC;
	HBITMAP OldBitmap;
	int bx, by;
	BITMAP bit;

	MemDC = CreateCompatibleDC(hdc);
	OldBitmap = (HBITMAP)SelectObject(MemDC, hBit);

	// 비트맵의 정보(크기)를 저장
	GetObject(hBit, sizeof(BITMAP), &bit);
	bx = bit.bmWidth;
	by = bit.bmHeight;

	// 비트맵 고속복사
	BitBlt(hdc, x, y, 600, by, MemDC, 600*frame, 0, SRCCOPY);

	SelectObject(MemDC, OldBitmap);
	DeleteDC(MemDC);
}

void Game_Start(HWND hWnd)
{
	b_NowGame = TRUE;
	frame = 0;
	m_cy = 0;
	m_cx = 5;
	m_Score = 0;
	m_BlockCount = 0;
	m_Level = 1;
	m_NextLevel = 5;
	m_Form = rand() % 7;
	m_NextBlock = rand() % 7;
	m_DownSpeed = 500;
	m_Rotation = 0;
	init_Field();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m_Field[i + m_cy][j + m_cx] = m_Block[m_Form][m_Rotation][i][j];
			m_ColorField[i + m_cy][j + m_cx] = m_Form;
		}
	}

	
	SetTimer(hWnd, 2, m_DownSpeed, NULL);
}

HBITMAP hBit, hBit2, RedBlock, GreenBlock, BlueBlock, YellowBlock, PurpleBlock, SkyBlueBlock, OrangeBlock, Title;

void OnTimer(HWND hWnd)
{
	RECT crt;
	HDC hdc, hMemDC;
	HBITMAP OldBit;
	HFONT myFont, oldFont;
	GetClientRect(hWnd, &crt);
	hdc = GetDC(hWnd);



	if (hBit == NULL) {
		hBit = CreateCompatibleBitmap(hdc, crt.right, crt.bottom);	// hdc와 호환할 수 있는 비트맵 생성
	}
	hMemDC = CreateCompatibleDC(hdc);	// hdc와 호환할 수 있는 DC생성

										// 호환할수 있는 dc와 비트맵이 같으면 selectObject로 연결하였을때 둘은 서로 연결된다.
										// hbit는 전역변수, WM_PAINT에 있는 DrawBitmap 연결되서 비트맵을 뿌려짐
	OldBit = (HBITMAP)SelectObject(hMemDC, hBit);		// hMemDC에 hbit를 할당 (hMemDC에 비트맵을 뿌리면 hBit에 저장이된다.) 후 oldBit에 옛날 비트맵 저장
	myFont = CreateFont(30, 40, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, TEXT("ArcadeClassic"));
	oldFont = (HFONT)SelectObject(hMemDC, myFont);
	if (!b_NowGame)
	{
		DrawTitle(hMemDC, 0, 0, Title);
		SetTextColor(hMemDC, RGB(255, 255, 255));
		SetBkMode(hMemDC, TRANSPARENT);
		TextOut(hMemDC, 170, 350, TEXT("Press SPACE bar"), 15);
	}
	else
	{
		DrawBitmap(hMemDC, 0, 0, hBit2);


		for (int i = 0; i < HEIGHT - 1; i++)
		{
			for (int j = 2; j < WIDTH - 2; j++)
			{
				if (m_Field[i][j] == 1 || m_Field[i][j] == -1)
				{
					//Rectangle(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, j * SIZE + SIZE + X_START, i * SIZE + SIZE + Y_START);
					if (m_ColorField[i][j] == 0)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, RedBlock);
					if (m_ColorField[i][j] == 1)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, BlueBlock);
					if (m_ColorField[i][j] == 2)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, OrangeBlock);
					if (m_ColorField[i][j] == 3)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, SkyBlueBlock);
					if (m_ColorField[i][j] == 4)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, YellowBlock);
					if (m_ColorField[i][j] == 5)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, PurpleBlock);
					if (m_ColorField[i][j] == 6)
						DrawBitmap(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, GreenBlock);

				}
			}
		}

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (m_Block[m_NextBlock][0][i][j] == 1)
				{
					//Rectangle(hMemDC, j * SIZE + X_START, i * SIZE + Y_START, j * SIZE + SIZE + X_START, i * SIZE + SIZE + Y_START);
					if (m_NextBlock == 0)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, RedBlock);
					if (m_NextBlock == 1)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, BlueBlock);
					if (m_NextBlock == 2)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, OrangeBlock);
					if (m_NextBlock == 3)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, SkyBlueBlock);
					if (m_NextBlock == 4)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, YellowBlock);
					if (m_NextBlock == 5)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, PurpleBlock);
					if (m_NextBlock == 6)
						DrawBitmap(hMemDC, j * SIZE + X_NEXTBLOCK, i * SIZE + Y_NEXTBLOCK, GreenBlock);

				}
			}
		}

		LevelCheck(hWnd);
		if (b_NowGame)
		{
			CheckEnd(hWnd);
		}

		

		SetTextColor(hMemDC, RGB(255, 0, 0));
		SetBkMode(hMemDC, TRANSPARENT);
		wsprintf(C_Score, TEXT("SCORE        %d"), m_Score);
		TextOut(hMemDC, 70, 600, C_Score, lstrlen(C_Score));
		wsprintf(C_BlockCount, TEXT("Lines        %d"), m_BlockCount);
		TextOut(hMemDC, 70, 620, C_BlockCount, lstrlen(C_BlockCount));

		SetTextColor(hMemDC, RGB(0, 0, 255));
		wsprintf(C_Level, TEXT("ROUND    %d"), m_Level);
		TextOut(hMemDC, 350, 570, C_Level, lstrlen(C_Level));
		wsprintf(C_Lineleft, TEXT("LINES LEFT %d"), m_NextLevel - m_BlockCount);
		TextOut(hMemDC, 350, 520, C_Lineleft, lstrlen(C_Lineleft));
		TextOut(hMemDC, 350, 620, TEXT("CREDITS    0"), 12);
		
	}
	
	SelectObject(hMemDC, oldFont);
	DeleteObject(myFont);
	SelectObject(hMemDC, OldBit);
	DeleteDC(hMemDC);
	ReleaseDC(hWnd, hdc);
	InvalidateRect(hWnd, NULL, FALSE);
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static HBITMAP backBitmap = NULL;
	switch (iMessage)
	{
	case WM_CREATE:
		hBit2 = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
		RedBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP5));
		GreenBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP3));
		BlueBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
		YellowBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP7));
		PurpleBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
		SkyBlueBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP6));
		OrangeBlock = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP8));
		Title = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP9));
		SetTimer(hWnd, 1, 25, NULL);
		SetTimer(hWnd, 3, 100, NULL);
		return 0;

	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			OnTimer(hWnd);
			break;

		case 2:
			Block_Down();
			InvalidateRect(hWnd, NULL, FALSE);
			break;

		case 3:
			++frame;
			frame %= 28;
			InvalidateRect(hWnd, NULL, FALSE);
			break;
		}
		return 0;

	case WM_KEYDOWN:
		if (b_NowGame)
		{
			switch (wParam)
			{
			case VK_UP:
				Rotate_Block();
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case VK_DOWN:
				Block_Down();
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case VK_LEFT:
				Move_Left_Block();
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case VK_RIGHT:
				Move_Right_Block();
				InvalidateRect(hWnd, NULL, FALSE);
				break;

			case VK_SPACE:
				Hard_Drop();
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			}
			return 0;
		}
		else
		{
			switch (wParam)
			{
			case VK_SPACE:
				Game_Start(hWnd);
				InvalidateRect(hWnd, NULL, FALSE);
				break;
			}
			return 0;
		}

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		if (hBit) DrawBitmap(hdc, 0, 0, hBit);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		if (hBit)
		{
			DeleteObject(hBit);
		}
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

