#pragma once

//方块的大小
#define LCDDOT_PIXELS	3
//方块之间的间隙宽度
#define CAP_PIXELS		1
//单个方块内部边界与实体时间的间隙
#define INNER_CAP		1
//每一个UNIT为正方形，它的边长所占据的LCD点阵数
#define UNIT_SIZE		3
//游戏区所包含的UNIT尺寸
#define UNIT_ROWS		20
#define UNIT_COLS		30
//游戏区LCD点阵的行数和列数（边界宽1个LCD，边界与UNIT的间隙1个LCD
//
#define LCD_ROWS			((UNIT_SIZE+1)*UNIT_ROWS+3)		//1：标识UNIT之间有一个LCD间隔
#define LCD_COLS			((UNIT_SIZE+1)*UNIT_COLS+3)		//3：指的是两个边界和一个边界与UNIT之间的间隔

//框架宽度
#define FRAME_LINE_WIDTH	1
//游戏区距离客户区原点的偏移量
#define OFFSET			1
//控制区的宽度
#define CTLAREA_WIDTH	8*LCDDOT_PIXELS
//与游戏有关的全局变量
enum Orientation {
	Up = 1,
	Right = 2,
	Down = -1,
	Left = -2,
	Unknow = 9
};
enum UnitType {
	UT_Libre = 0,			//空地
	UT_SnakeBody = 1,		//蛇的身体
	UT_Barrier = 2,		//障碍物或边界
	UT_Food = 3,			//可食用的食物
	UT_OutOfRange = 4		//不在游戏区范围内
};
struct UnitPosition {
public:
	int ColIndex;
	int RowIndex;
	void SetPosition(UnitPosition unit) {
		this->ColIndex = unit.ColIndex;
		this->RowIndex = unit.RowIndex;
	}
	void SetPosition(int colindex, int rowindex) {
		this->ColIndex = colindex;
		this->RowIndex = rowindex;
	}
	UnitPosition(int colindex, int rowindex) {
		SetPosition(colindex, rowindex);
	}
	UnitPosition() {
		//放在最底下一行，第0列
		SetPosition(0, UNIT_ROWS - 1);
	}
	//晁某一个方向移动一格
	void Move(Orientation o) {
		Move(o, 1);
	}
	//朝某一个方向移动多少格
	void Move(Orientation o, int step) {
		switch (o) {
		case Up:this->RowIndex -= step; break;
		case Right:this->ColIndex += step; break;
		case Down:this->RowIndex += step; break;
		case Left:this->ColIndex -= step; break;
		default:break;
		}
	}
	bool IsInGameArea() {
		if (ColIndex >= 0 && ColIndex < UNIT_COLS && RowIndex >= 0 && RowIndex < UNIT_ROWS)
			return true;
		return false;
	}
};
//用于同步线程的临界区变量
static CRITICAL_SECTION cs;


class GraphicsManager
{
private:

	HDC hdc;
	HDC hdcMeta;
	HBRUSH hBrush;
	HPEN hPen;
	HMETAFILE hmfInactive,hmfActive;
	HWND hWnd;

	COLORREF crInactive = RGB(255, 255, 255);
	COLORREF crActive = RGB(75, 75, 75);
	//COLORREF crBackground = RGB(255, 255, 225);
private:
	// 函数:在内存里绘制激活的和未激活的方块单元
	//
	//
	//
	void PrepareLCDDots();
	// 根据方块的状态在制定的设备坐标处绘制方块
	//
	//
	//
	void DrawLCDDot(POINT p, BOOL isactive);
	//按照Unit的性质绘制相应位置的Unit，索引为UNIT的索引
public:
	COLORREF crBackground = RGB(255, 255, 225);
	GraphicsManager();
	GraphicsManager(HWND hwnd);
	~GraphicsManager();

	void GetHWND(HWND hwnd);


	//根据LCD点阵的行列索引绘制方块
	void DrawLCDDot(int column_index, int row_index, BOOL isactive);
	//绘制游戏区的线条边界
	void DrawLineGameFrame();
	//设置随机的LCD点阵值绘制游戏区
	void DrawGameArea();
	//绘制游戏区的LCD边界
	void DrawLCDGameFrame();
	void DrawUnit(UnitPosition unit, UnitType utype);
	void DrawUnit(int colindex, int rowindex, UnitType utype);
	//绘制一个单元的某一个方向的间隙
	void DrawUnitGap(UnitPosition p1, Orientation o, bool isActive);
	void DrawUnitGap(int colindex, int rowindex, Orientation o, bool isActive);
	void DrawUnitGap(UnitPosition p1, UnitPosition p2, bool isActive);
	// 绘制数字
	void DrawDigit(HDC hdc, int iNumber);

	void DrawTwoDigits(HDC hdc, int iNumber, BOOL fSuppress);
	void DrawColon(HDC hdc);

	void DrawTime(HDC hdc, BOOL f24Hour, BOOL fSuppress);

};

