#pragma once
/* ˫������Ľ�㶨�� */



/* ��ͷ����˫�������� */

class LinkList
{
private:
	LinkNode *head;
	LinkNode *tail;
	int length;
public:
	//�޲����Ĺ��캯��
	LinkList() { head = new LinkNode; tail = head; length = 1; }
	//�������Ĺ��캯��
	LinkList(UnitPosition item) { head = new LinkNode(item); tail = head; length = 1; }
	//�������캯��
	//LinkList(LinkList<T> &List) { head = List; tail = Locate(Length() - 1); }
	//��������
	~LinkList() { Clear(); }
	//���غ���:��ֵ
	//LinkList<T>& operator=(LinkList<T> &List);
	//�������
	void Clear() {
		LinkNode *p = NULL;
		//��������ÿ�ζ�ɾ��ͷ����next��㣬�����ͷ���
		while (NULL != head->next)
		{
			p = head->next;
			head->next = p->next; //���������ڵ�ĵ�ַ��ֵ��ͷ�ڵ��next
			delete p;//ÿ�ζ�ɾ��ͷ����next���
		}
		tail = head;
		length = 1;
	}
	int GetLength()  {
		return length;
	}
	//��ȡ������
	int Length() 
	{
		int count = 0;
		LinkNode *p = head->next;
		while (NULL != p)
		{
			p = p->next;
			++count;
		}
		return count;
	}
	//��ȡ����ͷ���
	LinkNode* GetHead() 
	{
		return head;
	}
	LinkNode* GetTail(){
		return tail;
	}
	//��������ͷ���
	//void SetHead(LinkNode<T> *p);
	//�������ݵ�λ�ã����ص�һ���ҵ����������ֵ�Ľ��ָ��
	//LinkNode<T>* Find(T &item);
	//��λָ����λ�ã����ظ�λ���ϵĽ��ָ��
	LinkNode* Locate(int pos)
	{
		int i = 0;
		LinkNode *p = head;

		if (pos < 0)
			return NULL;

		while (NULL != p && i < pos)
		{
			p = p->next;
			i++;
		}
		return p;
	}
	LinkNode* LocateFromTail(int pos) {
		int i = 0;
		LinkNode *p = tail;
		if (pos < 0)
			return NULL;
		while (NULL != p && i < pos)
		{
			p = p->last;
			i++;
		}
		return p;
	}
	//��Ϊͷ�ڵ����
	bool InsertAsHead(UnitPosition item) {
		LinkNode* node = new LinkNode(item);
		head->last = node;
		node->next = head;
		node->last = NULL;
		head = node;
		length += 1;
		return true;
	}
	//ɾ�����һ�������ظýڵ�
	LinkNode* RemoveLast() {
		if(tail->last != NULL)
			tail->last->next = NULL;
		LinkNode *p = tail;
		tail = tail->last;
		length -= 1;
		return p;
	}
	//��ӵ�β��
	bool AddToTail(UnitPosition item) {
		LinkNode* node = new LinkNode(item);
		tail->next = node;
		node->next = NULL;
		node->last = tail;
		tail = node;
		length += 1;
		return true;
	}
	////��ָ��λ��posǰ������ֵΪitem�Ľ�㣬ʧ�ܷ���false
	//bool Insert(T &item, int pos)
	//{
	//	//����һ���ڵ㣬ʹ���Ϊͷ�ڵ�
	//	if (pos == 0) {
	//		InsertAsHead(item);
	//	}
	//	else {
	//		LinkNode<T> *p = Locate(pos - 1);
	//		if (NULL == p)
	//			return false;

	//		LinkNode<T> *node = new LinkNode<T>(item);
	//		if (NULL == node)
	//		{
	//			//cerr << "�����ڴ�ʧ��!" << endl;
	//			exit(1);
	//		}
	//		p->next->last = node;
	//		node->last = p;
	//		node->next = p->next;
	//		p->next = node;
	//		return true;
	//	}
	//}

	////ɾ��ָ��λ��pos�ϵĽ�㣬item���Ǹý���ֵ��ʧ�ܷ���false
	//bool Remove(int pos, T &item)
	//{
	//	LinkNode<T> *p = Locate(pos);
	//	if (NULL == p || NULL == p->next)
	//		return false;

	//	LinkNode<T> *del = p->next;
	//	p->next = del->next;
	//	item = del->data;
	//	delete del;
	//	return true;
	//}
	//��ȡָ��λ��pos�Ľ���ֵ��ʧ�ܷ���false
	//bool GetData(int pos, T &item);
	//����ָ��λ��pos�Ľ���ֵ��ʧ�ܷ���false
	//bool SetData(int pos, T &item);
	//�ж������Ƿ�Ϊ��
	//bool IsEmpty() const;
	//��ӡ����
	//void Print() const
	//{
	//	int count = 0;
	//	LinkNode<T> *p = head;
	//	while (NULL != p->next)
	//	{
	//		p = p->next;
	//		std::cout << p->data << " ";
	//		if (++count % 10 == 0)  //ÿ��ʮ��Ԫ�أ����д�ӡ
	//			cout << std::endl;
	//	}
	//}


};
