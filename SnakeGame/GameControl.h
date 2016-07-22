#pragma once
//游戏控制类，该类封装了GraphicsManager类的绘制功能
//
enum GameStatus {
	GS_PrepareForStart = 0,
	GS_Running = 1,
	GS_Paused = 2,
	GS_GameOver = 4
};
#define MAX_COLLIDE_TIMES	20
class GameControl
{
private:
	UnitType gameArea[UNIT_COLS][UNIT_ROWS];//记录游戏区域所有方块状态，false标识未激活占用
	Snake snake1;
	long score;//当前得分
	long hiScore;//最高分
	double speed;//运行速度
	bool lock = false;
	UnitPosition foodPos;
	int collideTimes = 0;

public:
	GraphicsManager graphicsMgr;//绘图管理对象
	GameStatus curStatus = GS_GameOver;
	Orientation userComOrient;//使用者给出的方向
	int level;//当前等级
private:
	//初始化个参数
	void Initiate() {
		for (int row = 0; row <UNIT_ROWS; row++) {
			for (int col = 0; col <UNIT_COLS; col++) {
				gameArea[col][row] = UT_Libre;
				//graphicsMgr.DrawBrick(x, y, gameArea[x][y]);
			}
		}
		score = 0;
		hiScore = 0;
		level = 9;
		userComOrient = Right;
		snake1 = Snake(UnitPosition(4, UNIT_ROWS - 1), Right, 5);
		LinkNode* p = snake1.head;
		while (p != NULL) {
			gameArea[p->Data.ColIndex][p->Data.RowIndex] = UT_SnakeBody;
			p = p->next;
		}
		foodPos.ColIndex = UNIT_COLS/2;
		foodPos.RowIndex = UNIT_ROWS/2;
		gameArea[foodPos.ColIndex][foodPos.RowIndex] = UT_Food;
		collideTimes = 0;
		//CreateFood();
		GameStatus curStatus = GS_GameOver;
	}

public:
	GameControl() {
		Initiate();
	}
	~GameControl() {};
	void GetHWND(HWND hwnd) {
		graphicsMgr.GetHWND(hwnd);
	}
	void DisplayGameBorder() {
		graphicsMgr.DrawLCDGameFrame();
	}
	void CreateAndDisplayBarriers(int num) {
		if (num <= 0)return;
		UnitPosition unit;
		for (int i = 0; i < num; i++) {
			unit = FindUnitLibre();
			gameArea[unit.ColIndex][unit.RowIndex] = UT_Barrier;
			graphicsMgr.DrawUnit(unit, UT_Barrier);
		}
	}
	void DisplaySnake() {
		LinkNode* p = snake1.head;
		while (p != NULL) {
			graphicsMgr.DrawUnit(p->Data,UT_SnakeBody);
			if (p->next != NULL) {
				graphicsMgr.DrawUnitGap(p->Data, p->next->Data, true);
			}
			p = p->next;
		}
	}
	void EraseSnake() {
		LinkNode* p = snake1.tail;
		while (p != NULL) {
			UnitPosition tailPos = snake1.RemoveTail();
			gameArea[tailPos.ColIndex][tailPos.RowIndex] = UT_Libre;
			graphicsMgr.DrawUnit(p->Data, UT_Libre);
			if (p->last != NULL) {
				graphicsMgr.DrawUnitGap(tailPos, p->last->Data, false);
			}
			p = snake1.tail;
			//Sleep(50);
		}
	}
	void DisplayFood() {
		graphicsMgr.DrawUnit(foodPos, UT_Food);
	}
	void EraseFood() {
		graphicsMgr.DrawUnit(foodPos, UT_Libre);
	}
	//从游戏区上方到下方先全部激活再全部失活
	void ClearGameAreaRowByRow(PVOID pvoid) {
		int row = 0;
		for (row = 0; row < UNIT_ROWS; row++) {
			//SetRowStatus(row, true);
			Sleep(20);
		}
		for (row = UNIT_ROWS - 1; row >= 0; row--) {
			//SetRowStatus(row, false);
			Sleep(20);
		}
	}
	SYSTEMTIME st;
	UnitPosition FindUnitLibre() {
		int col, row;
		do {
			GetLocalTime(&st);
			col = (st.wMilliseconds*rand()) % UNIT_COLS;
			GetLocalTime(&st);
			row = (st.wMilliseconds*rand()) % UNIT_ROWS;
		} while (gameArea[col][row] != UT_Libre);
		return UnitPosition(col, row);
	}
	void CreateFood() {
		foodPos = FindUnitLibre();
		//foodPos.ColIndex = col;
		//foodPos.RowIndex = row;
		gameArea[foodPos.ColIndex][foodPos.RowIndex] = UT_Food;
	}
	//根据贪食蛇当前移动方向和用户给出的指令方向判断贪食蛇后续移动方向
	void UpdateSnakeMoveDirection() {
		if ((userComOrient != snake1.moveDirection) && (userComOrient + snake1.moveDirection != 0)) {
			snake1.moveDirection = userComOrient;
		}
	}
	//试图去移动，如果可以则移动，如果不行则返回false
	bool TryToMove() {
		UpdateSnakeMoveDirection();
		UnitPosition nextpos = UnitPosition(snake1.head->Data.ColIndex, snake1.head->Data.RowIndex);
		nextpos.Move(snake1.moveDirection);
		if (nextpos.IsInGameArea()) {
			UnitType targetUType = gameArea[nextpos.ColIndex][nextpos.RowIndex];
			if (targetUType == UT_Libre) {
				//可以移动,先把尾部去除
				UnitPosition tailPos = snake1.Move();
				graphicsMgr.DrawUnit(tailPos, UT_Libre);
				graphicsMgr.DrawUnitGap(tailPos, snake1.tail->Data, false);
				gameArea[tailPos.ColIndex][tailPos.RowIndex] = UT_Libre;
				//加画头部
				UnitPosition headPos = snake1.head->Data;
				graphicsMgr.DrawUnit(headPos, UT_SnakeBody);
				graphicsMgr.DrawUnitGap(headPos, snake1.head->next->Data, true);
				gameArea[headPos.ColIndex][headPos.RowIndex] = UT_SnakeBody;
				curStatus = GS_Running;
				collideTimes = 0;
				return true;
			}
			else if (targetUType == UT_Food)
			{
				snake1.Eat(nextpos);
				Beep(5000, 30);
				UnitPosition headPos = snake1.head->Data;
				graphicsMgr.DrawUnit(headPos, UT_SnakeBody);
				graphicsMgr.DrawUnitGap(headPos, snake1.head->next->Data, true);
				gameArea[headPos.ColIndex][headPos.RowIndex] = UT_SnakeBody;
				curStatus = GS_Running;
				if (snake1.GetLength() == UNIT_COLS*UNIT_ROWS) {
					curStatus = GS_GameOver;
					return true;
				}
				CreateFood();
				DisplayFood();
				collideTimes = 0;
				return true;
			}
			else {
				collideTimes += 1;
				if (collideTimes >= MAX_COLLIDE_TIMES)
				{
					curStatus = GS_GameOver;
				}
				if(collideTimes>1)
					Beep(1000, 30);
				return false;
			}
		}
		else {//撞到边界
			collideTimes += 1;
			if (collideTimes >= MAX_COLLIDE_TIMES)
			{
				curStatus = GS_GameOver;
			}
			if (collideTimes>1)
				Beep(1000, 30);
			return false;
		}
	}


	void Running() {
		TryToMove();
	}
	void Reset() {
		Initiate();
	}

	//准备开始前的绘制
	void PrepareStart() {
		EraseFood();
		EraseSnake();
		Initiate();
		DisplaySnake();
		DisplayFood();
		//CreateAndDisplayBarriers(30);
		curStatus = GS_PrepareForStart;
	}
	void Start() {
		curStatus = GS_Running;
	}
	void Pause()
	{
		curStatus = GS_Paused;
	}

	void Resume() {
		curStatus = GS_Running;
	}
	//提高级别，当提高最高级别时，级别被设定为最低
	void SpeedUp() {
		if (level == 9)
			level = 1;
		else
			level += 1;
	}
	//降低级别，当降低最低级别是，级别被设定为最高
	void SpeedDown() {
		if (level == 0)
			level = 9;
		else
			level -= 1;
	}
};



