#pragma once

//����Ĵ�С
#define LCDDOT_PIXELS	3
//����֮��ļ�϶���
#define CAP_PIXELS		1
//���������ڲ��߽���ʵ��ʱ��ļ�϶
#define INNER_CAP		1
//ÿһ��UNITΪ�����Σ����ı߳���ռ�ݵ�LCD������
#define UNIT_SIZE		3
//��Ϸ����������UNIT�ߴ�
#define UNIT_ROWS		20
#define UNIT_COLS		30
//��Ϸ��LCD������������������߽��1��LCD���߽���UNIT�ļ�϶1��LCD
//
#define LCD_ROWS			((UNIT_SIZE+1)*UNIT_ROWS+3)		//1����ʶUNIT֮����һ��LCD���
#define LCD_COLS			((UNIT_SIZE+1)*UNIT_COLS+3)		//3��ָ���������߽��һ���߽���UNIT֮��ļ��

//��ܿ��
#define FRAME_LINE_WIDTH	1
//��Ϸ������ͻ���ԭ���ƫ����
#define OFFSET			1
//�������Ŀ��
#define CTLAREA_WIDTH	8*LCDDOT_PIXELS
//����Ϸ�йص�ȫ�ֱ���
enum Orientation {
	Up = 1,
	Right = 2,
	Down = -1,
	Left = -2,
	Unknow = 9
};
enum UnitType {
	UT_Libre = 0,			//�յ�
	UT_SnakeBody = 1,		//�ߵ�����
	UT_Barrier = 2,		//�ϰ����߽�
	UT_Food = 3,			//��ʳ�õ�ʳ��
	UT_OutOfRange = 4		//������Ϸ����Χ��
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
		//���������һ�У���0��
		SetPosition(0, UNIT_ROWS - 1);
	}
	//��ĳһ�������ƶ�һ��
	void Move(Orientation o) {
		Move(o, 1);
	}
	//��ĳһ�������ƶ����ٸ�
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
//����ͬ���̵߳��ٽ�������
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
	// ����:���ڴ�����Ƽ���ĺ�δ����ķ��鵥Ԫ
	//
	//
	//
	void PrepareLCDDots();
	// ���ݷ����״̬���ƶ����豸���괦���Ʒ���
	//
	//
	//
	void DrawLCDDot(POINT p, BOOL isactive);
	//����Unit�����ʻ�����Ӧλ�õ�Unit������ΪUNIT������
public:
	COLORREF crBackground = RGB(255, 255, 225);
	GraphicsManager();
	GraphicsManager(HWND hwnd);
	~GraphicsManager();

	void GetHWND(HWND hwnd);


	//����LCD����������������Ʒ���
	void DrawLCDDot(int column_index, int row_index, BOOL isactive);
	//������Ϸ���������߽�
	void DrawLineGameFrame();
	//���������LCD����ֵ������Ϸ��
	void DrawGameArea();
	//������Ϸ����LCD�߽�
	void DrawLCDGameFrame();
	void DrawUnit(UnitPosition unit, UnitType utype);
	void DrawUnit(int colindex, int rowindex, UnitType utype);
	//����һ����Ԫ��ĳһ������ļ�϶
	void DrawUnitGap(UnitPosition p1, Orientation o, bool isActive);
	void DrawUnitGap(int colindex, int rowindex, Orientation o, bool isActive);
	void DrawUnitGap(UnitPosition p1, UnitPosition p2, bool isActive);
	// ��������
	void DrawDigit(HDC hdc, int iNumber);

	void DrawTwoDigits(HDC hdc, int iNumber, BOOL fSuppress);
	void DrawColon(HDC hdc);

	void DrawTime(HDC hdc, BOOL f24Hour, BOOL fSuppress);

};

