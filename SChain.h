//模板类名称：SChain
//作者：方寸
//说明：链表类
//日期：2011-8-18

template<class T>
class SChain
{
//Data
private:
	struct Node
	{
		T Data;
		Node *pNext;
	} m_Head;
	int m_nCount;
//Function
public:
	//函数名称：SChain
	//说明：构造函数
	//输入：
	//输出：
	//返回：
	//备注：
	//日期：2011-8-18
	//修改记录：
	SChain()
	{
		m_nCount=0;//初始化项目数
		m_Head.pNext=NULL;//初始化头结点
	}

	//函数名称：Insert
	//说明：插入项目
	//输入：nIndex:要插入到的项目索引,Item:项目数据
	//输出：
	//返回：成功1;失败0
	//备注：
	//日期：2011-8-18
	//修改记录：
	bool Insert(int nIndex,T &Item)
	{
		Node *p;
		Node *pNew;

		if(nIndex<1 || nIndex>m_nCount+1)//nIndex索引非法
		{
			return false;
		}

		p=&m_Head;
		int i;
		for(i=1;i<nIndex;i++)//查找nIndex的前一项
		{
			p=p->pNext;
		}

		pNew=new Node;
		pNew->Data=Item;
		pNew->pNext=p->pNext;
		p->pNext=pNew;

		m_nCount++;

		return true;
	}

	//函数名称：Add
	//说明：向链表尾部添加项目
	//输入：Item:项目数据
	//输出：
	//返回：成功1;失败0
	//备注：
	//日期：2011-8-18
	//修改记录：
	bool Add(T &Item)
	{
		return Insert(m_nCount+1,Item);
	}

	//函数名称：Delete
	//说明：删除项目
	//输入：
	//输出：
	//返回：成功1;失败0
	//备注：
	//日期：2011-8-18
	//修改记录：
	bool Delete(int nIndex)
	{
		Node *p;
		Node *pTemp;

		if(nIndex<1 || nIndex>m_nCount)//nIndex非法
		{
			return false;
		}

		p=&m_Head;
		int i;
		for(i=1;i<m_nCount;i++)
		{
			p=p->pNext;
		}

		pTemp=p->pNext;
		p->pNext=p->pNext->pNext;

		delete pTemp;
		m_nCount--;

		return true;
	}
	
	//函数名称：IsEmpty
	//说明：判空
	//输入：
	//输出：
	//返回：空1;非空0
	//备注：
	//日期：2011-8-18
	//修改记录：
	bool IsEmpty()
	{
		return m_nCount ? 0:1;
	}

	//函数名称：Clear
	//说明：清空
	//输入：
	//输出：
	//返回：
	//备注：
	//日期：2011-8-18
	//修改记录：
	void Clear()
	{
		while(!IsEmpty())
			Delete(1);
	}
	

	//函数名称：Get
	//说明：获取项目
	//输入：
	//输出：
	//返回：找到的项目数据指针
	//备注：
	//日期：2011-8-18
	//修改记录：
	T *Get(int nIndex)
	{
		Node *p;

		if(nIndex<1 || nIndex>m_nCount)//nIndex非法
		{
			return NULL;
		}

		p=&m_Head;
		int i;
		for(i=0;i<nIndex;i++)//查找到nIndex项
		{
			p=p->pNext;
		}

		return &p->Data;
	}

	//函数名称：GetCount
	//说明：获取项目数
	//输入：
	//输出：
	//返回：项目数
	//备注：
	//日期：2011-8-18
	//修改记录
	int GetCount()
	{
		return m_nCount;
	}

	//函数名称：~SChain
	//说明：析构函数
	//输入：
	//输出：
	//返回：
	//备注：
	//日期：2011-8-18
	//修改记录
	~SChain()
	{
		Clear();
	}
};