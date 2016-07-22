#pragma once
/* 双向链表的结点定义 */



/* 带头结点的双向链表定义 */

class LinkList
{
private:
	LinkNode *head;
	LinkNode *tail;
	int length;
public:
	//无参数的构造函数
	LinkList() { head = new LinkNode; tail = head; length = 1; }
	//带参数的构造函数
	LinkList(UnitPosition item) { head = new LinkNode(item); tail = head; length = 1; }
	//拷贝构造函数
	//LinkList(LinkList<T> &List) { head = List; tail = Locate(Length() - 1); }
	//析构函数
	~LinkList() { Clear(); }
	//重载函数:赋值
	//LinkList<T>& operator=(LinkList<T> &List);
	//链表清空
	void Clear() {
		LinkNode *p = NULL;
		//遍历链表，每次都删除头结点的next结点，最后保留头结点
		while (NULL != head->next)
		{
			p = head->next;
			head->next = p->next; //将第三个节点的地址赋值给头节点的next
			delete p;//每次都删除头结点的next结点
		}
		tail = head;
		length = 1;
	}
	int GetLength()  {
		return length;
	}
	//获取链表长度
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
	//获取链表头结点
	LinkNode* GetHead() 
	{
		return head;
	}
	LinkNode* GetTail(){
		return tail;
	}
	//设置链表头结点
	//void SetHead(LinkNode<T> *p);
	//查找数据的位置，返回第一个找到的满足该数值的结点指针
	//LinkNode<T>* Find(T &item);
	//定位指定的位置，返回该位置上的结点指针
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
	//作为头节点插入
	bool InsertAsHead(UnitPosition item) {
		LinkNode* node = new LinkNode(item);
		head->last = node;
		node->next = head;
		node->last = NULL;
		head = node;
		length += 1;
		return true;
	}
	//删除最后一个并返回该节点
	LinkNode* RemoveLast() {
		if(tail->last != NULL)
			tail->last->next = NULL;
		LinkNode *p = tail;
		tail = tail->last;
		length -= 1;
		return p;
	}
	//添加到尾部
	bool AddToTail(UnitPosition item) {
		LinkNode* node = new LinkNode(item);
		tail->next = node;
		node->next = NULL;
		node->last = tail;
		tail = node;
		length += 1;
		return true;
	}
	////在指定位置pos前方插入值为item的结点，失败返回false
	//bool Insert(T &item, int pos)
	//{
	//	//插入一个节点，使其成为头节点
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
	//			//cerr << "分配内存失败!" << endl;
	//			exit(1);
	//		}
	//		p->next->last = node;
	//		node->last = p;
	//		node->next = p->next;
	//		p->next = node;
	//		return true;
	//	}
	//}

	////删除指定位置pos上的结点，item就是该结点的值，失败返回false
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
	//获取指定位置pos的结点的值，失败返回false
	//bool GetData(int pos, T &item);
	//设置指定位置pos的结点的值，失败返回false
	//bool SetData(int pos, T &item);
	//判断链表是否为空
	//bool IsEmpty() const;
	//打印链表
	//void Print() const
	//{
	//	int count = 0;
	//	LinkNode<T> *p = head;
	//	while (NULL != p->next)
	//	{
	//		p = p->next;
	//		std::cout << p->data << " ";
	//		if (++count % 10 == 0)  //每隔十个元素，换行打印
	//			cout << std::endl;
	//	}
	//}


};
