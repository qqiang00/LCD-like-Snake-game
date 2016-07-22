#pragma once
struct LinkNode
{
	UnitPosition Data;
	LinkNode *next;
	LinkNode *last;

	LinkNode(LinkNode *pnext = NULL, LinkNode *plast = NULL) { next = pnext; last = plast; }
	LinkNode(UnitPosition item, LinkNode *pnext = NULL, LinkNode *plast = NULL)
		//�����������е��β�������Ĭ��ֵ�����Ǵ�Ĭ��ֵ�Ĳ�����Ҫ�ź���
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
	//����һ��ȷ��ͷ��λ�á����з����Լ��ڶγ��ȵ�̰ʳ��
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

