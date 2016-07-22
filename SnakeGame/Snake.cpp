#include "stdafx.h"
#include "Snake.h"

Snake::Snake() {
	head = new LinkNode();
	tail = head;
	Length = 1;
	}
	//����һ��ȷ��ͷ��λ�á����з����Լ��ڶγ��ȵ�̰ʳ��
Snake::Snake(UnitPosition headunit, Orientation movedirection, int length) {
	head =new LinkNode(headunit);
	tail = head;
	Length = 1;
	moveDirection = movedirection;
	if (length < 1)
		length = 1;		//������һ��ͷ
	for (int i = 1; i < length; i++) {
		UnitPosition bodypos = UnitPosition(headunit.ColIndex,headunit.RowIndex);
		bodypos.Move(movedirection, -1 * i);
		AddToTail(bodypos);
	}		
}
UnitPosition Snake::GetHeadPos() {
	return head->Data;
}
UnitPosition Snake::GetTailPos() {
	return tail->Data;
}
bool Snake::AddToTail(UnitPosition unit) {
	LinkNode* node =new LinkNode(unit);
	tail->next = node;
	node->last = tail;
	tail = node;
	Length += 1;
	return true;
}
bool Snake::Eat(UnitPosition unit) {
	LinkNode* node = new LinkNode(unit);
	head->last = node;
	node->next = head;
	node->last = NULL;
	head = node;
	Length += 1;
	return true;
}
UnitPosition Snake::RemoveTail() {
	//if (tail == NULL)
	//	return;
	LinkNode* p = tail;
	tail = p->last;
	if (tail != NULL)
	{
		tail->next = NULL;
	}
	Length -= 1;
	return UnitPosition(p->Data.ColIndex, p->Data.RowIndex);
}
int Snake::GetLength() {
	return Length;
}
bool Snake::InsertAsHead(UnitPosition unit) {
	Eat(unit);
	return true;
}
//���ƶ�һ�Σ�����������β������
UnitPosition Snake::Move() {
	UnitPosition pos = UnitPosition(head->Data.ColIndex, head->Data.RowIndex);
	pos.Move(moveDirection);
	InsertAsHead(pos);
	return RemoveTail();
}
void Snake::Clear() {
	LinkNode *p = NULL;
	//��������ÿ�ζ�ɾ��ͷ����next��㣬�����ͷ���
	while (NULL != head->next)
	{
		p = head->next;
		head->next = p->next; //���������ڵ�ĵ�ַ��ֵ��ͷ�ڵ��next
		delete p;//ÿ�ζ�ɾ��ͷ����next���
	}
	tail = head;
	Length = 1;
}
Snake::~Snake() {
	//printf("in deconstruce function");
	//Clear();
}

