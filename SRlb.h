//类名称：SRlb
//作者：方寸
//说明：实现对rlb文件的读取，加载到指定DC指定位置
//日期：2011-8-17
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
	SRlb();//构造函数
	~SRlb();//析构函数
	bool LoadRlb(const char *strFileName);//载入rlb文件
	void Insert(const char *pMem,int nSize,int nIndex);//插入项目
	void Delete(int nIndex);//删除操作
	void Clear();//清空操作
	char *GetContent(int nIndex);//获取项目内容
	char *GetName(int nIndex);//获取项目名称
	int GetSize(int nIndex);//获取项目大小
	int GetCount();//获取项目数
};

