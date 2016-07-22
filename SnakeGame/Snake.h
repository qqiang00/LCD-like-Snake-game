#pragma once
struct LinkNode
{
	UnitPosition Data;
	LinkNode *next;
	LinkNode *last;

	LinkNode(LinkNode *pnext = NULL, LinkNode *plast = NULL) { next = pnext; last = plast; }
	LinkNode(UnitPosition item, LinkNode *pnext = NULL, LinkNode *plast = NULL)
		//函数参数表中的形参允许有默认值，但是带默认值的参数需要放后面
	{
		next = pnext;
		last = plast;
		Data = item;
	}
};

class Snake
{
public:
	LinkNode* head;
	LinkNode* tail;
	Orientation moveDirection;
private:
	int Length;
	bool InsertAsHead(UnitPosition unit);
public:
	Snake();
	//生成一个确定头的位置、运行方向以及节段长度的贪食蛇
	Snake(UnitPosition headunit, Orientation movedirection, int length);
	UnitPosition GetHeadPos();
	UnitPosition GetTailPos();
	bool AddToTail(UnitPosition unit);
	bool Eat(UnitPosition unit);
	int GetLength();
	void Clear();
	UnitPosition Move();
	UnitPosition RemoveTail();
	~Snake();
};

