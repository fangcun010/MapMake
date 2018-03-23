//�����ƣ�SRlb
//���ߣ�����
//˵����ʵ�ֶ�rlb�ļ��Ķ�ȡ�����ص�ָ��DCָ��λ��
//���ڣ�2011-8-17
#include "SChain.h"

#define SRLB_ITEMNAME_LEN			32

class SRlb
{
//Data
private:
	struct Node
	{
		char *pData;
		Node *pNext;
		int nSize;
	} m_Head;
	int m_nCount;
	struct FileName
	{
		char strName[SRLB_ITEMNAME_LEN+1];
	};
	SChain<FileName> m_NameList;
//Function
public:
	SRlb();//���캯��
	~SRlb();//��������
	bool LoadRlb(const char *strFileName);//����rlb�ļ�
	void Insert(const char *pMem,int nSize,int nIndex);//������Ŀ
	void Delete(int nIndex);//ɾ������
	void Clear();//��ղ���
	char *GetContent(int nIndex);//��ȡ��Ŀ����
	char *GetName(int nIndex);//��ȡ��Ŀ����
	int GetSize(int nIndex);//��ȡ��Ŀ��С
	int GetCount();//��ȡ��Ŀ��
};

