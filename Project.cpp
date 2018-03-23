#include "StdAfx.h"
#include "Globals.h"


//�������ƣ�GetProjectName
//˵������ȡ��������
//���룺
//�����
//���أ�
//���ڣ�2011-8-17
//��ע��
//�޸ļ�¼��
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
	strProjectDat+="\\RPG��Դ\\GameName.dat";

	fp=fopen(strProjectDat.c_str(),"rb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"��ȡ��������ʧ�ܣ�","MapMake",MB_OK|MB_ICONSTOP);
		PostQuitMessage(1);
	}

	fseek(fp,0,SEEK_END);
	nLen=ftell(fp);
	rewind(fp);

	pMem=new char[nLen+1];

	if(!pMem)
	{
		MessageBox(g_hMainWnd,"�����ڴ�ʧ�ܣ�","MapMake",MB_OK|MB_ICONSTOP);
		PostQuitMessage(1);
	}

	fread(pMem,nLen,1,fp);

	pMem[nLen]='\0';

	fclose(fp);

	g_strProjectName=pMem;
	delete []pMem;
	g_strMapDirName=strExeDir;
	g_strMapDirName+="\\RPG��Դ\\";
	g_strMapDirName+=g_strProjectName;
	g_strMapRlb=g_strMapDirName;
	g_strMapRlb+="\\DT.rlb";
	g_strMapDirName+="\\Map";
}