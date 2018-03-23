#include "StdAfx.h"
#include "Globals.h"


//函数名称：GetProjectName
//说明：获取工程名称
//输入：
//输出：
//返回：
//日期：2011-8-17
//备注：
//修改记录：
void GetProjectName()
{
	FILE *fp;
	char *pMem=NULL;
	long nLen;
	char strExeDir[MAX_PATH];
	std::string strProjectDat;

	GetModuleFileName(NULL,strExeDir,MAX_PATH);

	int i=strlen(strExeDir)-1;

	while(strExeDir[i]!='\\')
		i--;
	strExeDir[i]='\0';

	strProjectDat=strExeDir;
	strProjectDat+="\\RPG资源\\GameName.dat";

	fp=fopen(strProjectDat.c_str(),"rb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"获取工程名称失败！","MapMake",MB_OK|MB_ICONSTOP);
		PostQuitMessage(1);
	}

	fseek(fp,0,SEEK_END);
	nLen=ftell(fp);
	rewind(fp);

	pMem=new char[nLen+1];

	if(!pMem)
	{
		MessageBox(g_hMainWnd,"分配内存失败！","MapMake",MB_OK|MB_ICONSTOP);
		PostQuitMessage(1);
	}

	fread(pMem,nLen,1,fp);

	pMem[nLen]='\0';

	fclose(fp);

	g_strProjectName=pMem;
	delete []pMem;
	g_strMapDirName=strExeDir;
	g_strMapDirName+="\\RPG资源\\";
	g_strMapDirName+=g_strProjectName;
	g_strMapRlb=g_strMapDirName;
	g_strMapRlb+="\\DT.rlb";
	g_strMapDirName+="\\Map";
}