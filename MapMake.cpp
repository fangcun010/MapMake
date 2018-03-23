//程序名称：MapMake
//作者：方寸
//说明：无
//日期：2011-8-14
#include "stdafx.h"
#include "Globals.h"//全局变量

HINSTANCE g_hInstance;//程序实例句柄
HWND g_hMainWnd;//主窗口句柄
std::string g_strProjectName;//工程名称
std::string g_strMapDirName;//文件路径
std::string g_strMapRlb;//Rlb文件资源名称
std::string g_strFileName;//文件名

HDC g_hTitleDC=NULL;//图块DC
HDC g_hDrawDC=NULL;//绘图DC
HDC g_hPageDC=NULL;//缓冲页面DC
HBITMAP g_hPageBitmap=NULL;//缓冲页面位图
HBITMAP g_hTitleBitmap=NULL;//图块位图
HBITMAP g_hDrawBitmap=NULL;//地图绘制位图
SRlb g_Rlb;//地图块资源

bool g_bCreate;//是否已建立地图

MapData g_Map;//地图数据

AppInfo g_AppInfo;//程序数据

LRESULT APIENTRY MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//主函数
bool RegClass(WNDCLASS &wc);//注册主窗口窗口类

void GetProjectName();//获取工程名称

//函数名称：WinMain
//说明：主函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-14
//修改记录：
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	MSG Msg;
	int x,y;

	g_strFileName="";//初始化文件名
	GetProjectName();

	g_bCreate=false;//设置未建立地图

	g_hInstance=hInstance;

	if(!RegClass(wc))
	{
		MessageBox(NULL,"注册主窗口类失败！","MapMake",MB_OK);
		return 1;
	}

	x=(GetSystemMetrics(SM_CXSCREEN)-WND_W)/2;
	y=(GetSystemMetrics(SM_CYSCREEN)-WND_H)/2;

	hWnd=CreateWindow("MapMakeMainWnd","MapMake",WS_OVERLAPPEDWINDOW,
						x,y,//窗口位置
						WND_W,WND_H,//窗口大小
						NULL,//窗口父窗口
						NULL,//窗口菜单句柄
						hInstance,//程序实例句柄
						NULL);//传给消息处理函数的参数

	g_Rlb.LoadRlb(g_strMapRlb.c_str());//载入图块

	if(!hWnd)
	{
		MessageBox(NULL,"主窗口创建失败！","MapMake",MB_OK);
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


//函数名称：RegClass
//说明：注册窗口类
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-15
//修改记录：
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


