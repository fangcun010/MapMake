//�������ƣ�MapMake
//���ߣ�����
//˵������
//���ڣ�2011-8-14
#include "stdafx.h"
#include "Globals.h"//ȫ�ֱ���

HINSTANCE g_hInstance;//����ʵ�����
HWND g_hMainWnd;//�����ھ��
std::string g_strProjectName;//��������
std::string g_strMapDirName;//�ļ�·��
std::string g_strMapRlb;//Rlb�ļ���Դ����
std::string g_strFileName;//�ļ���

HDC g_hTitleDC=NULL;//ͼ��DC
HDC g_hDrawDC=NULL;//��ͼDC
HDC g_hPageDC=NULL;//����ҳ��DC
HBITMAP g_hPageBitmap=NULL;//����ҳ��λͼ
HBITMAP g_hTitleBitmap=NULL;//ͼ��λͼ
HBITMAP g_hDrawBitmap=NULL;//��ͼ����λͼ
SRlb g_Rlb;//��ͼ����Դ

bool g_bCreate;//�Ƿ��ѽ�����ͼ

MapData g_Map;//��ͼ����

AppInfo g_AppInfo;//��������

LRESULT APIENTRY MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//������
bool RegClass(WNDCLASS &wc);//ע�������ڴ�����

void GetProjectName();//��ȡ��������

//�������ƣ�WinMain
//˵����������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-14
//�޸ļ�¼��
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG Msg;
	int x,y;

	g_strFileName="";//��ʼ���ļ���
	GetProjectName();

	g_bCreate=false;//����δ������ͼ

	g_hInstance=hInstance;

	if(!RegClass(wc))
	{
		MessageBox(NULL,"ע����������ʧ�ܣ�","MapMake",MB_OK);
		return 1;
	}

	x=(GetSystemMetrics(SM_CXSCREEN)-WND_W)/2;
	y=(GetSystemMetrics(SM_CYSCREEN)-WND_H)/2;

	hWnd=CreateWindow("MapMakeMainWnd","MapMake",WS_OVERLAPPEDWINDOW,
						x,y,//����λ��
						WND_W,WND_H,//���ڴ�С
						NULL,//���ڸ�����
						NULL,//���ڲ˵����
						hInstance,//����ʵ�����
						NULL);//������Ϣ�������Ĳ���

	g_Rlb.LoadRlb(g_strMapRlb.c_str());//����ͼ��

	if(!hWnd)
	{
		MessageBox(NULL,"�����ڴ���ʧ�ܣ�","MapMake",MB_OK);
		return 1;
	}

	g_hMainWnd=hWnd;

	ShowWindow(hWnd,nCmdShow);

	while(GetMessage(&Msg,NULL,NULL,NULL))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}


	return (int)Msg.wParam;
}


//�������ƣ�RegClass
//˵����ע�ᴰ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-15
//�޸ļ�¼��
bool RegClass(WNDCLASS &wc)
{
	wc.lpszClassName="MapMakeMainWnd";
	wc.lpszMenuName="MAINMENU";
	wc.hInstance=g_hInstance;
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=LoadIcon(g_hInstance,"ICON");
	wc.hbrBackground=(HBRUSH)COLOR_BACKGROUND;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.lpfnWndProc=MainWndProc;
	wc.style=CS_OWNDC|CS_VREDRAW|CS_HREDRAW;

	if(RegisterClass(&wc)) return true;
	else return false;
}


