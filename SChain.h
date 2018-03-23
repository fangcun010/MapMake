//ģ�������ƣ�SChain
//���ߣ�����
//˵����������
//���ڣ�2011-8-18

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
	//�������ƣ�SChain
	//˵�������캯��
	//���룺
	//�����
	//���أ�
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	SChain()
	{
		m_nCount=0;//��ʼ����Ŀ��
		m_Head.pNext=NULL;//��ʼ��ͷ���
	}

	//�������ƣ�Insert
	//˵����������Ŀ
	//���룺nIndex:Ҫ���뵽����Ŀ����,Item:��Ŀ����
	//�����
	//���أ��ɹ�1;ʧ��0
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	bool Insert(int nIndex,T &Item)
	{
		Node *p;
		Node *pNew;

		if(nIndex<1 || nIndex>m_nCount+1)//nIndex�����Ƿ�
		{
			return false;
		}

		p=&m_Head;
		int i;
		for(i=1;i<nIndex;i++)//����nIndex��ǰһ��
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

	//�������ƣ�Add
	//˵����������β�������Ŀ
	//���룺Item:��Ŀ����
	//�����
	//���أ��ɹ�1;ʧ��0
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	bool Add(T &Item)
	{
		return Insert(m_nCount+1,Item);
	}

	//�������ƣ�Delete
	//˵����ɾ����Ŀ
	//���룺
	//�����
	//���أ��ɹ�1;ʧ��0
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	bool Delete(int nIndex)
	{
		Node *p;
		Node *pTemp;

		if(nIndex<1 || nIndex>m_nCount)//nIndex�Ƿ�
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
	
	//�������ƣ�IsEmpty
	//˵�����п�
	//���룺
	//�����
	//���أ���1;�ǿ�0
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	bool IsEmpty()
	{
		return m_nCount ? 0:1;
	}

	//�������ƣ�Clear
	//˵�������
	//���룺
	//�����
	//���أ�
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	void Clear()
	{
		while(!IsEmpty())
			Delete(1);
	}
	

	//�������ƣ�Get
	//˵������ȡ��Ŀ
	//���룺
	//�����
	//���أ��ҵ�����Ŀ����ָ��
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼��
	T *Get(int nIndex)
	{
		Node *p;

		if(nIndex<1 || nIndex>m_nCount)//nIndex�Ƿ�
		{
			return NULL;
		}

		p=&m_Head;
		int i;
		for(i=0;i<nIndex;i++)//���ҵ�nIndex��
		{
			p=p->pNext;
		}

		return &p->Data;
	}

	//�������ƣ�GetCount
	//˵������ȡ��Ŀ��
	//���룺
	//�����
	//���أ���Ŀ��
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼
	int GetCount()
	{
		return m_nCount;
	}

	//�������ƣ�~SChain
	//˵������������
	//���룺
	//�����
	//���أ�
	//��ע��
	//���ڣ�2011-8-18
	//�޸ļ�¼
	~SChain()
	{
		Clear();
	}
};