//类名称：SRlb
//作者：方寸
//说明：实现对rlb文件的读取
//日期：2011-8-17
#include "StdAfx.h"

//函数名称：SRlb::SRlb
//说明：SRlb类构造函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-17
//修改记录：
SRlb::SRlb()
{
	m_nCount=0;//初始化项目数
	m_Head.pNext=NULL;//初始化链表头指针
}

//函数名称：SRlb::~SRlb
//说明：析构函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
SRlb::~SRlb()
{
	Clear();
}

//函数名称：SRlb::LoadRlb
//说明：插入新项
//输入：strFileName：要载入的rlb文件文件名
//输出：
//返回：
//备注：
//日期：2011-8-17
//修改记录：
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
		MessageBox(NULL,"打开文件失败！","MapMake",MB_OK);
		return false;
	}

	//初始化链表
	m_NameList.Clear();
	Clear();//清空数据项

	fread(&nCount,sizeof(int),1,fp);

	int i;
	for(i=0;i<nCount;i++)
	{
		//计算名称位置
		nAddress=sizeof(int)+nCount*sizeof(int)+i*SRLB_ITEMNAME_LEN;
		fseek(fp,nAddress,SEEK_SET);
		fread(fn.strName,SRLB_ITEMNAME_LEN,1,fp);
		fn.strName[SRLB_ITEMNAME_LEN]='\0';
		m_NameList.Add(fn);
		//计算数据位置
		nAddress=(i+1)*sizeof(int);
		fseek(fp,nAddress,SEEK_SET);
		fread(&nAddress,sizeof(int),1,fp);
		fseek(fp,nAddress,SEEK_SET);
		fread(&nSize,sizeof(int),1,fp);

		pMem=new char[nSize];

		if(!pMem)//内存申请失败
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

//函数名称：SRlb::Insert
//说明：插入新项
//输入：pMem：数据块，nSize：数据大小，nIndex：插入的位置（从1开始）
//输出：
//返回：
//备注：
//日期：2011-8-17
//修改记录：
void SRlb::Insert(const char *pMem,int nSize,int nIndex)
{
	Node *p;
	Node *pNew;

	if(nIndex<1 || nIndex>m_nCount+1)
	{
		MessageBox(NULL,"nIndex值非法","SRlb",MB_OK);
		PostQuitMessage(1);//发送退出消息
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
		MessageBox(NULL,"申请内存失败！","SRlb",MB_OK);
		PostQuitMessage(1);//发送退出消息
	}

	pNew->pData=new char[nSize];

	if(!pNew->pData)
	{
		MessageBox(NULL,"申请内存失败！","SRlb",MB_OK);
		PostQuitMessage(1);//发送退出消息
	}
	
	pNew->nSize=nSize;
	memcpy(pNew->pData,pMem,nSize);
	pNew->pNext=p->pNext;
	p->pNext=pNew;

	m_nCount++;
}

//函数名称：SRlb::Delete
//说明：删除操作
//输入：nIndex：要删除的项目索引
//输出：
//返回：项目内容指针
//备注：
//日期：2011-8-18
//修改记录：
void SRlb::Delete(int nIndex)
{
	Node *p;
	Node *pTemp;

	if(nIndex<1 || nIndex>m_nCount)
	{
		MessageBox(NULL,"nIndex越界","SRlb",MB_OK);
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

//函数名称：SRlb::Clear
//说明：清空操作
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void SRlb::Clear()
{
	while(m_nCount)
	{
		Delete(1);
	}
}

//函数名称：SRlb::GetContent
//说明：获取项目内容
//输入：nIndex：要获取的项目索引
//输出：
//返回：项目内容指针
//备注：
//日期：2011-8-17
//修改记录：
char *SRlb::GetContent(int nIndex)
{
	Node *p;

	if(nIndex<1 || nIndex>m_nCount)//索引非法
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

//函数名称：SRlb::GetName
//说明：获取项目名称
//输入：nIndex：要获取的项目索引
//输出：
//返回：项目内容指针
//备注：
//日期：2011-8-17
//修改记录：
char *SRlb::GetName(int nIndex)
{
	if(nIndex<1 || nIndex>m_nCount)
	{
		return NULL;
	}

	return m_NameList.Get(nIndex)->strName;
}

//函数名称：SRlb::GetSize
//说明：获取项目代大小
//输入：nIndex：要获取的项目索引
//输出：
//返回：项目内容指针
//备注：
//日期：2011-8-17
//修改记录：
int SRlb::GetSize(int nIndex)
{
	Node *p;

	if(nIndex<1 || nIndex>m_nCount)//索引非法
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

//函数名称：SRlb::GetCount
//说明：获取rlb文件项目数
//输入：
//输出：
//返回：rlb项目数
//备注：
//日期：2011-8-17
//修改记录：
int SRlb::GetCount()
{
	return m_nCount;
}
