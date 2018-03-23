//�����ƣ�SRlb
//���ߣ�����
//˵����ʵ�ֶ�rlb�ļ��Ķ�ȡ
//���ڣ�2011-8-17
#include "StdAfx.h"

//�������ƣ�SRlb::SRlb
//˵����SRlb�๹�캯��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
SRlb::SRlb()
{
	m_nCount=0;//��ʼ����Ŀ��
	m_Head.pNext=NULL;//��ʼ������ͷָ��
}

//�������ƣ�SRlb::~SRlb
//˵������������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
SRlb::~SRlb()
{
	Clear();
}

//�������ƣ�SRlb::LoadRlb
//˵������������
//���룺strFileName��Ҫ�����rlb�ļ��ļ���
//�����
//���أ�
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
bool SRlb::LoadRlb(const char *strFileName)
{
	FILE *fp;
	int nCount;
	int nAddress;
	FileName fn;
	int nSize;
	char *pMem=NULL;

	fp=fopen(strFileName,"rb");

	if(!fp)
	{
		MessageBox(NULL,"���ļ�ʧ�ܣ�","MapMake",MB_OK);
		return false;
	}

	//��ʼ������
	m_NameList.Clear();
	Clear();//���������

	fread(&nCount,sizeof(int),1,fp);

	int i;
	for(i=0;i<nCount;i++)
	{
		//��������λ��
		nAddress=sizeof(int)+nCount*sizeof(int)+i*SRLB_ITEMNAME_LEN;
		fseek(fp,nAddress,SEEK_SET);
		fread(fn.strName,SRLB_ITEMNAME_LEN,1,fp);
		fn.strName[SRLB_ITEMNAME_LEN]='\0';
		m_NameList.Add(fn);
		//��������λ��
		nAddress=(i+1)*sizeof(int);
		fseek(fp,nAddress,SEEK_SET);
		fread(&nAddress,sizeof(int),1,fp);
		fseek(fp,nAddress,SEEK_SET);
		fread(&nSize,sizeof(int),1,fp);

		pMem=new char[nSize];

		if(!pMem)//�ڴ�����ʧ��
		{
			fclose(fp);
			return false;
		}
		
		fread(pMem,nSize,1,fp);
		Insert(pMem,nSize,i+1);
		delete []pMem;
	}
	
	fclose(fp);

	return true;
}

//�������ƣ�SRlb::Insert
//˵������������
//���룺pMem�����ݿ飬nSize�����ݴ�С��nIndex�������λ�ã���1��ʼ��
//�����
//���أ�
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
void SRlb::Insert(const char *pMem,int nSize,int nIndex)
{
	Node *p;
	Node *pNew;

	if(nIndex<1 || nIndex>m_nCount+1)
	{
		MessageBox(NULL,"nIndexֵ�Ƿ�","SRlb",MB_OK);
		PostQuitMessage(1);//�����˳���Ϣ
	}

	p=&m_Head;

	int i;

	i=1;
	while(i<nIndex)
	{
		p=p->pNext;
		i++;
	}

	pNew=new Node;

	if(!pNew)
	{
		MessageBox(NULL,"�����ڴ�ʧ�ܣ�","SRlb",MB_OK);
		PostQuitMessage(1);//�����˳���Ϣ
	}

	pNew->pData=new char[nSize];

	if(!pNew->pData)
	{
		MessageBox(NULL,"�����ڴ�ʧ�ܣ�","SRlb",MB_OK);
		PostQuitMessage(1);//�����˳���Ϣ
	}
	
	pNew->nSize=nSize;
	memcpy(pNew->pData,pMem,nSize);
	pNew->pNext=p->pNext;
	p->pNext=pNew;

	m_nCount++;
}

//�������ƣ�SRlb::Delete
//˵����ɾ������
//���룺nIndex��Ҫɾ������Ŀ����
//�����
//���أ���Ŀ����ָ��
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
void SRlb::Delete(int nIndex)
{
	Node *p;
	Node *pTemp;

	if(nIndex<1 || nIndex>m_nCount)
	{
		MessageBox(NULL,"nIndexԽ��","SRlb",MB_OK);
		PostQuitMessage(1);
	}

	p=&m_Head;
	int i;
	for(i=1;i<nIndex;i++)
	{
		p=p->pNext;
	}

	pTemp=p->pNext;
	p->pNext=p->pNext->pNext;
	delete []pTemp->pData;
	delete pTemp;

	m_nCount--;
}

//�������ƣ�SRlb::Clear
//˵������ղ���
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
void SRlb::Clear()
{
	while(m_nCount)
	{
		Delete(1);
	}
}

//�������ƣ�SRlb::GetContent
//˵������ȡ��Ŀ����
//���룺nIndex��Ҫ��ȡ����Ŀ����
//�����
//���أ���Ŀ����ָ��
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
char *SRlb::GetContent(int nIndex)
{
	Node *p;

	if(nIndex<1 || nIndex>m_nCount)//�����Ƿ�
	{
		return NULL;
	}

	p=&m_Head;
	int i;
	for(i=0;i<nIndex;i++)
	{
		p=p->pNext;
	}

	return p->pData;
}

//�������ƣ�SRlb::GetName
//˵������ȡ��Ŀ����
//���룺nIndex��Ҫ��ȡ����Ŀ����
//�����
//���أ���Ŀ����ָ��
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
char *SRlb::GetName(int nIndex)
{
	if(nIndex<1 || nIndex>m_nCount)
	{
		return NULL;
	}

	return m_NameList.Get(nIndex)->strName;
}

//�������ƣ�SRlb::GetSize
//˵������ȡ��Ŀ����С
//���룺nIndex��Ҫ��ȡ����Ŀ����
//�����
//���أ���Ŀ����ָ��
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
int SRlb::GetSize(int nIndex)
{
	Node *p;

	if(nIndex<1 || nIndex>m_nCount)//�����Ƿ�
	{
		return -1;
	}

	p=&m_Head;
	int i;
	for(i=0;i<nIndex;i++)
	{
		p=p->pNext;
	}

	return p->nSize;
}

//�������ƣ�SRlb::GetCount
//˵������ȡrlb�ļ���Ŀ��
//���룺
//�����
//���أ�rlb��Ŀ��
//��ע��
//���ڣ�2011-8-17
//�޸ļ�¼��
int SRlb::GetCount()
{
	return m_nCount;
}
