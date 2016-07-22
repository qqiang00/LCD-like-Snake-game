#include "stdafx.h"
#include "GraphicsManager.h"

void GraphicsManager::PrepareLCDDots() {
	//绘制未激活的方块，保存至hmfInactive中
	hdcMeta = CreateMetaFile(NULL);
	hBrush = CreateSolidBrush(crInactive);
	hPen = CreatePen(PS_SOLID, 1, crInactive);
	SelectObject(hdcMeta, hPen);
	SelectObject(hdcMeta, hBrush);
	Rectangle(hdcMeta, 0, 0, LCDDOT_PIXELS, LCDDOT_PIXELS);
	//Rectangle(hdcMeta, INNER_CAP, INNER_CAP, LCDDOT_PIXELS - INNER_CAP, LCDDOT_PIXELS - INNER_CAP);
	hmfInactive = CloseMetaFile(hdcMeta);
	DeleteObject(hPen);
	DeleteObject(hBrush);
	//绘制激活的方块，保存至hmfActive中
	hdcMeta = CreateMetaFile(NULL);
	hPen = CreatePen(PS_SOLID, 1, crActive);
	SelectObject(hdcMeta, hPen);
	hBrush = CreateSolidBrush(crActive);
	SelectObject(hdcMeta, hBrush);
	Rectangle(hdcMeta, 0, 0, LCDDOT_PIXELS, LCDDOT_PIXELS);
	//Rectangle(hdcMeta, INNER_CAP, INNER_CAP, LCDDOT_PIXELS - INNER_CAP, LCDDOT_PIXELS - INNER_CAP);
	hmfActive = CloseMetaFile(hdcMeta);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}
// 根据方块的状态在制定的设备坐标处绘制方块
//
//
//
void GraphicsManager::DrawLCDDot(POINT p, BOOL isactive) {
	EnterCriticalSection(&cs);
	hdc = GetDC(hWnd);
	SetViewportOrgEx(hdc, p.x, p.y, NULL);
	if (isactive)
	{
		if (LCDDOT_PIXELS <= 2) {
			for (int col = 0; col < LCDDOT_PIXELS; col++) {
				for (int row = 0; row < LCDDOT_PIXELS; row++) {
					SetPixel(hdc, col, row, crActive);
				}
			}
		}
		else {
			PlayMetaFile(hdc, hmfActive);
		}
	}
	else {
		if (LCDDOT_PIXELS <= 2) {
			for (int col = 0; col < LCDDOT_PIXELS; col++) {
				for (int row = 0; row < LCDDOT_PIXELS; row++) {
					SetPixel(hdc, col, row, crInactive);
				}
			}
		}
		else {
			PlayMetaFile(hdc, hmfInactive);
		}
	}
	SetViewportOrgEx(hdc, 0, 0, NULL);
	ReleaseDC(hWnd, hdc);
	LeaveCriticalSection(&cs);
}
//按照Unit的性质绘制相应位置的Unit，索引为UNIT的索引

GraphicsManager::GraphicsManager() {
	InitializeCriticalSection(&cs);
	PrepareLCDDots();
}
GraphicsManager::GraphicsManager(HWND hwnd) {
	InitializeCriticalSection(&cs);
	this->hWnd = hwnd;
	PrepareLCDDots();
}
GraphicsManager::~GraphicsManager() {
	ReleaseDC(hWnd, hdc);
	DeleteCriticalSection(&cs);
}

void GraphicsManager::GetHWND(HWND hwnd) {
	this->hWnd = hwnd;
}


//根据LCD点阵的行列索引绘制方块
void GraphicsManager::DrawLCDDot(int column_index, int row_index, BOOL isactive) {
	POINT p;
	p.x = OFFSET + FRAME_LINE_WIDTH + CAP_PIXELS + column_index*(LCDDOT_PIXELS + CAP_PIXELS);
	p.y = OFFSET + FRAME_LINE_WIDTH + CAP_PIXELS + row_index*(LCDDOT_PIXELS + CAP_PIXELS);
	DrawLCDDot(p, isactive);
}
//绘制游戏区的线条边界
void GraphicsManager::DrawLineGameFrame() {
	hdc = GetDC(hWnd);
	SetViewportOrgEx(hdc, OFFSET, OFFSET, NULL);
	hPen = CreatePen(PS_SOLID, FRAME_LINE_WIDTH, crActive);
	SelectObject(hdc, hPen);
	Rectangle(hdc, 0, 0, LCD_COLS*(LCDDOT_PIXELS + CAP_PIXELS) + CAP_PIXELS + 2 * FRAME_LINE_WIDTH,
		LCD_ROWS*(LCDDOT_PIXELS + CAP_PIXELS) + CAP_PIXELS + 2 * FRAME_LINE_WIDTH);
	DeleteObject(hPen);
	ReleaseDC(hWnd, hdc);
}
//设置随机的LCD点阵值绘制游戏区
void GraphicsManager::DrawGameArea() {
	for (int x = 0; x < LCD_COLS; x++)
	{
		for (int y = 0; y < LCD_ROWS; y++)
		{
			BOOL status = (rand() % 2) * (rand() % 2);
			DrawLCDDot(x, y, status);
		}
	}
}
//绘制游戏区的LCD边界
void GraphicsManager::DrawLCDGameFrame() {
	int row = 0;
	int col = 0;
	for (row = 0, col = 0; col < LCD_COLS; col++) {
		DrawLCDDot(col, row, true);
	}
	for (row = LCD_ROWS - 1, col = 0; col < LCD_COLS; col++) {
		DrawLCDDot(col, row, true);
	}
	for (col = 0, row = 1; row < LCD_ROWS - 1; row++) {
		DrawLCDDot(col, row, true);
	}
	for (col = LCD_COLS - 1, row = 1; row < LCD_ROWS - 1; row++) {
		DrawLCDDot(col, row, true);
	}
}

void GraphicsManager::DrawUnit(int colindex, int rowindex, UnitType utype) {
	if (colindex < 0 || colindex >= UNIT_COLS || rowindex < 0 || rowindex >= UNIT_ROWS)
		return;//不在绘制范围内
	int col = 0, row = 0;
	switch (utype) {
	case UT_Libre:
		for (col = 0; col < UNIT_SIZE; col++) {
			for (row = 0; row < UNIT_SIZE; row++)
			{
				DrawLCDDot(1 + 1 + colindex*(UNIT_SIZE + 1) + col, 1 + 1 + rowindex*(UNIT_SIZE + 1) + row, false);
			}
		}break;
	case UT_Barrier:
	case UT_SnakeBody:
		for (col = 0; col < UNIT_SIZE; col++) {
			for (row = 0; row < UNIT_SIZE; row++)
			{
				DrawLCDDot(1 + 1 + colindex*(UNIT_SIZE + 1) + col, 1 + 1 + rowindex*(UNIT_SIZE + 1) + row, true);
			}
		}break;
	case UT_Food:
		for (col = 0; col < UNIT_SIZE; col++) {
			for (row = 0; row < UNIT_SIZE; row++)
			{
				if ((row + col) % 2 == 1)
					DrawLCDDot(1 + 1 + colindex*(UNIT_SIZE + 1) + col, 1 + 1 + rowindex*(UNIT_SIZE + 1) + row, true);
				else
					DrawLCDDot(1 + 1 + colindex*(UNIT_SIZE + 1) + col, 1 + 1 + rowindex*(UNIT_SIZE + 1) + row, false);
			}
		}break;
	case UT_OutOfRange:break;
	}
}
void GraphicsManager::DrawUnit(UnitPosition pos, UnitType utype) {
	DrawUnit(pos.ColIndex, pos.RowIndex, utype);
}
//绘制一个单元的某一个方向的间隙
void GraphicsManager::DrawUnitGap(UnitPosition p, Orientation o, bool isActive) {
	DrawUnitGap(p.ColIndex, p.RowIndex, o, isActive);
}
void GraphicsManager::DrawUnitGap(int colindex, int rowindex, Orientation o, bool isActive) {
	int i = 0;
	switch (o) {
	case Up:for (i = 0; i < 3; i++) { DrawLCDDot(2 + colindex*(UNIT_SIZE + 1) + i, 1 + rowindex*(UNIT_SIZE + 1), isActive); }break;
	case Right:for (i = 0; i < 3; i++) { DrawLCDDot(5 + colindex*(UNIT_SIZE + 1), 2 + rowindex*(UNIT_SIZE + 1) + i, isActive); }break;
	case Down:for (i = 0; i < 3; i++) { DrawLCDDot(2 + colindex*(UNIT_SIZE + 1) + i, 5 + rowindex*(UNIT_SIZE + 1), isActive); }break;
	case Left:for (i = 0; i < 3; i++) { DrawLCDDot(1 + colindex*(UNIT_SIZE + 1), 2 + rowindex*(UNIT_SIZE + 1) + i, isActive); }break;
	default:break;
	}
}
void GraphicsManager::DrawUnitGap(UnitPosition p1, UnitPosition p2, bool isActive) {
	if (abs(p1.ColIndex - p2.ColIndex) + abs(p1.RowIndex - p2.RowIndex)>1)
		return;
	if (p2.ColIndex - p1.ColIndex == 1) { DrawUnitGap(p1, Right, isActive); return; }
	if (p2.ColIndex - p1.ColIndex == -1) { DrawUnitGap(p1, Left, isActive); return; }
	if (p2.RowIndex - p1.RowIndex == 1) { DrawUnitGap(p1, Down, isActive); return; }
	if (p2.RowIndex - p1.RowIndex == -1) { DrawUnitGap(p1, Up, isActive); return; }
}

// 绘制数字
void GraphicsManager::DrawDigit(HDC hdc, int iNumber)
{
	static BOOL  fSevenSegment[10][7] = {
		1, 1, 1, 0, 1, 1, 1,     // 0
		0, 0, 1, 0, 0, 1, 0,     // 1
		1, 0, 1, 1, 1, 0, 1,     // 2
		1, 0, 1, 1, 0, 1, 1,     // 3
		0, 1, 1, 1, 0, 1, 0,     // 4
		1, 1, 0, 1, 0, 1, 1,     // 5
		1, 1, 0, 1, 1, 1, 1,     // 6
		1, 0, 1, 0, 0, 1, 0,     // 7
		1, 1, 1, 1, 1, 1, 1,     // 8
		1, 1, 1, 1, 0, 1, 1 };  // 9
								//static POINT ptSegment[7][6] = {
								//	7,  6,  11,  2,  31,  2,  35,  6,  31, 10,  11, 10,
								//	6,  7,  10, 11,  10, 31,   6, 35,   2, 31,   2, 11,
								//	36,  7,  40, 11,  40, 31,  36, 35,  32, 31,  32, 11,
								//	7, 36,  11, 32,  31, 32,  35, 36,  31, 40,  11, 40,
								//	6, 37,  10, 41,  10, 61,   6, 65,   2, 61,   2, 41,
								//	36, 37,  40, 41,  40, 61,  36, 65,  32, 61,  32, 41,
								//	7, 66,  11, 62,  31, 62,  35, 66,  31, 70,  11, 70 };
	static POINT ptSegment[7][6] = {
		3,  2,  5,  1,  15,  1,  17,  2,  15, 4,  5, 4,
		2,  3,  4, 5,  4, 15,   2, 17,   0, 15,   0, 5,
		17,  3,  19, 5,  19, 15,  17, 17,  15, 15,  15, 5,
		3, 17,  5, 15,  15, 15,  17, 17,  15, 19,  5, 19,
		2, 18,  4, 20,  4, 30,   2, 32,   0, 30,   0, 20,
		17, 18,  19, 19,  19, 30,  17, 32,  15, 30,  15, 20,
		2, 32,  5, 30,  15, 30,  17, 32,  15, 34,  5, 34 };
	int          iSeg;

	for (iSeg = 0; iSeg < 7; iSeg++)
		if (fSevenSegment[iNumber][iSeg])
			Polygon(hdc, ptSegment[iSeg], 6);
}

void GraphicsManager::DrawTwoDigits(HDC hdc, int iNumber, BOOL fSuppress)
{
	if (!fSuppress || (iNumber / 10 != 0))
		DrawDigit(hdc, iNumber / 10);

	OffsetWindowOrgEx(hdc, -20, 0, NULL);
	DrawDigit(hdc, iNumber % 10);
	OffsetWindowOrgEx(hdc, -20, 0, NULL);
}

void GraphicsManager::DrawColon(HDC hdc)
{
	POINT ptColon[2][4] = { 2,  21,  6,  17,  10, 21,  6, 25,
		2,  51,  6,  47,  10, 51,  6, 55 };

	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);

	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}

void GraphicsManager::DrawTime(HDC hdc, BOOL f24Hour, BOOL fSuppress)
{
	SYSTEMTIME st;

	GetLocalTime(&st);

	if (f24Hour)
		DrawTwoDigits(hdc, st.wHour, fSuppress);
	else
		DrawTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);

	DrawColon(hdc);
	DrawTwoDigits(hdc, st.wMinute, FALSE);
	DrawColon(hdc);
	DrawTwoDigits(hdc, st.wSecond, FALSE);
}

