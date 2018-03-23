#include "StdAfx.h"

#include "Globals.h"

//新建对话框最大字符串长度
#define MAX_EDITSTRING			20

LRESULT WINAPI AboutBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//关于对话框消息处理函数
LRESULT WINAPI SelectTitleBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//选择图块对话框消息处理函数

extern HINSTANCE g_hInstance;

static HWND hNewMapWnd;

//主窗口消息处理函数
void OnCreate();//WM_CREATE消息处理函数
void OnDestroy();//WM_DESTROY消息处函数
void OnFileNew();//WM_COMMAND:ID_FILE_NEWI菜单项消息处理
void OnFileOpen();//WM_COMMAND:ID_FILE_OPENI菜单项消息处理
void OnFileSave();//WM_COMMAND:ID_FILE_SAVEI菜单项消息处理
void OnFileOtherSave();//WM_COMMAND:ID_FILE_OTHERSAVEI菜单项消息处理
void OnFillMap();//WM_COMMAND:ID_EDIT_FILL菜单项消息处理填充地图
void OnView11();//WM_COMMAND:ID_VIEW_1_1菜单项消息处理
void OnView12();//WM_COMMAND:视图->1/2菜单响应
void OnView21();//WM_COMMAND:视图->2/1菜单响应
void OnRButtonDown(UINT nFlags,POINT pt);//WM_RBUTTONDOWN消息处理函数
void OnLButtonDown(UINT nFlags,POINT pt);//WM_LBUTTONDOWN消息处理函数
void OnMouseMove(UINT nFlags,POINT pt);//WM_MOUSEMOVE消息处理函数
void OnKeyDown(int nKey);//WM_KEYUP消息处理函数
void OnSize();//WM_SIZE消息处理函数
void OnPaint();//WM_PAINT消息处理函数
void UpdateViewMenu();//更新视图菜单
void UpdateDrawMenu();//更新绘图菜单
void OnToolDeal(POINT pt,int nFloor);//工具处理
void OnSetOpp(POINT pt,int nFloor);//设置数据取反

//选择图块对话框消息处理函数
void InitSlectTitleBox(HWND hWnd);//初始化选择图块对话框
void ShowPictureContent(HWND hWnd,int nIndex);//显示选中的rlb项目内容

//地图绘制函数
void DrawMap(POINT ptDest);//将指定位置绘制为指定图块
void DrawTitleToPage();//绘图到前台缓冲
void DrawAllMap();//绘制整个地图到地图缓冲
void DrawMapToPage();//复制地图图像到前台缓冲
void DrawCurSel();//绘制当前选中区
float GetFactor();//获取缩放倍数

//选图
RECT GetDrawRect(RECT rt);//获取选图矩形
RECT GetSelRect(int nFloor);//返回合理化后的选中矩形



//函数名称：MainWndProc
//说明：消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-14
//修改记录：
LRESULT WINAPI MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int wmID;
	int wmEvent;
	POINT pt;

	switch(uMsg)
	{
	/**************************************************************\
	*WM_CREATE:在这里做一些初始化的工作
	\**************************************************************/
	case WM_CREATE:
		OnCreate();
		break;
	/**************************************************************\
	*WM_COMMAND:在这里响应菜单
	\**************************************************************/
	case WM_COMMAND:
		wmID=LOWORD(wParam);
		wmEvent=HIWORD(wParam);
		switch(wmID)
		{
		case ID_FILE_NEWI://文件->新建
			OnFileNew();
			break;
		case ID_FILE_OPENI://文件->打开
			OnFileOpen();
			break;
		case ID_FILE_SAVEI://文件->保存
			OnFileSave();
			break;
		case ID_FILE_OTHERSAVEI://文件->另存为
			OnFileOtherSave();
			break;
		case ID_FILE_EXIT://文件->退出
			SendMessage(hWnd,WM_CLOSE,NULL,NULL);
			break;
		case ID_EDIT_RESETPOINT:
			g_AppInfo.ptDraw.x=g_AppInfo.ptDraw.y=0;
			g_AppInfo.ptTitle.x=g_AppInfo.ptTitle.y=0;
			OnPaint();
			break;
		case ID_EDIT_FILL://编辑->填充地图
			OnFillMap();
			break;
		case ID_VIEW_1_1://视图->1/1
			g_AppInfo.nView=VIEW_1_1;
			UpdateViewMenu();
			OnPaint();
			break;
		case ID_VIEW_1_2://视图->1/2
			g_AppInfo.nView=VIEW_1_2;
			UpdateViewMenu();
			OnPaint();
			break;
		case ID_VIEW_2_1://视图->2/1
			g_AppInfo.nView=VIEW_2_1;
			UpdateViewMenu();
			OnPaint();
			break;
		case ID_HELP_ABOUT://帮助->关于
			DialogBox(g_hInstance,"ABOUTBOX",hWnd,(DLGPROC)AboutBoxProc);
			break;
		}
		break;
	/**************************************************************\
	*WM_LBUTTONDOWN:在这里响应鼠标键击
	\**************************************************************/
	case WM_LBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		OnLButtonDown(wParam,pt);
		break;
	/**************************************************************\
	*WM_RBUTTONDOWN:在这里响应鼠标键击
	\**************************************************************/
	case WM_RBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		OnRButtonDown(wParam,pt);
		break;
	/**************************************************************\
	*WM_MOUSEMOVE:在这里响应鼠标键击
	\**************************************************************/
	case WM_MOUSEMOVE:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		OnMouseMove(wParam,pt);
		break;
	/**************************************************************\
	*WM_KEYDOWN:在这里处理按键
	\**************************************************************/
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;
	/**************************************************************\
	*WM_PAINT:在这里重画窗口
	\**************************************************************/
	case WM_PAINT:
		OnPaint();
		break;
	/**************************************************************\
	*WM_SIZE:在这里进行窗口缩放后的处理
	\**************************************************************/
	case WM_SIZE:
		OnSize();
		break;
	/**************************************************************\
	*WM_CLOSE:在这里做退出程序的处理
	\**************************************************************/
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	/**************************************************************\
	*WM_DESTROY:在这里做退出程序的处理
	\**************************************************************/
	case WM_DESTROY:
		OnDestroy();
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


//函数名称：AboutBoxProc
//说明：消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-16
//修改记录：
LRESULT WINAPI AboutBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	/**************************************************************\
	*WM_COMMAND:在这里初始化对话框
	\**************************************************************/
	case WM_INITDIALOG:
		return true;
		break;
	/**************************************************************\
	*WM_COMMAND:在这里响应命令
	\**************************************************************/
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd,wParam);
			return true;
			break;
		}
		break;
	}

	return false;
}

//函数名称：NewMapBoxProc
//说明：消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-16
//修改记录：
LRESULT WINAPI NewMapBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HWND hCWnd;//临时控制句柄
	int nRet;
	switch(uMsg)
	{
	/**************************************************************\
	*WM_COMMAND:在这里初始化对话框
	\**************************************************************/
	case WM_INITDIALOG:
		//=================================================
		//初始化对话框控件
		//=================================================
		hNewMapWnd=hWnd;

		hCWnd=GetDlgItem(hWnd,IDC_BACKCOLOR);
		
		//插入选择项
		SendMessage(hCWnd,CB_INSERTSTRING,0,(LPARAM)"白色");
		SendMessage(hCWnd,CB_INSERTSTRING,1,(LPARAM)"黑色");
		//设置选中第一项
		SendMessage(hCWnd,CB_SETCURSEL,0,NULL);
		//设置地图宽高缺省值
		hCWnd=GetDlgItem(hWnd,IDC_W);
		SetWindowText(hCWnd,"10");
		hCWnd=GetDlgItem(hWnd,IDC_H);
		SetWindowText(hCWnd,"10");
		hCWnd=GetDlgItem(hWnd,IDC_TID);
		SetWindowText(hCWnd,"1");
		return true;
		break;
	/**************************************************************\
	*WM_COMMAND:在这里响应命令
	\**************************************************************/
	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_CT://选择地图块
			DialogBox(g_hInstance,"CHOOSEMAPPIC",hWnd,(DLGPROC)SelectTitleBoxProc);
			break;
		case IDOK://确定
			int w,h;
			int nRlbID;
			COLORREF crBack;
			char str[MAX_EDITSTRING];
			hCWnd=GetDlgItem(hWnd,IDC_W);
			GetWindowText(hCWnd,str,MAX_EDITSTRING);
			w=atoi(str);
			hCWnd=GetDlgItem(hWnd,IDC_H);
			GetWindowText(hCWnd,str,MAX_EDITSTRING);
			h=atoi(str);
			hCWnd=GetDlgItem(hWnd,IDC_TID);
			GetWindowText(hCWnd,str,MAX_EDITSTRING);
			nRlbID=atoi(str);
			hCWnd=GetDlgItem(hWnd,IDC_BACKCOLOR);
			GetWindowText(hCWnd,str,MAX_EDITSTRING);
			if(!strcmp(str,"白色"))
				crBack=0xFFFFFF;//白色
			else
				crBack=0x000000;//黑色
			if(w>99 || h>99 || w<1 || h<1)
			{
				MessageBox(hWnd,"地图大小不合法！","MapMake",MB_OK);
				return true;
			}
			if(nRlbID>g_Rlb.GetCount() || nRlbID<1)
			{
				MessageBox(hWnd,"地图资源ID不合法！","MapMake",MB_OK);
				return true;
			}
			g_Map.nRlbID=nRlbID;
			g_Map.nMapW=w;
			g_Map.nMapH=h;
			hCWnd=GetDlgItem(hWnd,IDC_BACKCOLOR);
			nRet=SendMessage(hCWnd,CB_GETCURSEL,NULL,NULL);
			if(nRet)
				g_Map.crBack=0x000000;//背景白色
			else
				g_Map.crBack=0xFFFFFF;//背景黑色
		case IDCANCEL://取消
			EndDialog(hWnd,wParam);
			return true;
			break;
		}
		break;
	}
	
	return false;
}

//函数名称：SelectTitleBoxProc
//说明：消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
LRESULT WINAPI SelectTitleBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int wmID,wmEvent;
	HWND hCWnd;
	int nRet;

	switch(uMsg)
	{
	/**************************************************************\
	*WM_INITDIALOG:在这里初始化对话框
	\**************************************************************/
	case WM_INITDIALOG://初始化选择图块对话框
		InitSlectTitleBox(hWnd);
		hCWnd=GetDlgItem(hWnd,IDC_CHOOSELIST);
		SendMessage(hCWnd,LB_SETCURSEL,0,NULL);//设置第一个被选中
		SendMessage(hCWnd,WM_PAINT,NULL,NULL);
		return true;
	/**************************************************************\
	*WM_COMMAND:在这里响应对话框命令
	\**************************************************************/
	case WM_COMMAND:
		wmID=LOWORD(wParam);
		wmEvent=HIWORD(wParam);
		switch(wmID)
		{
		case IDC_CHOOSELIST:
			switch(wmEvent)
			{
			case LBN_SELCHANGE:
				nRet=SendMessage((HWND)lParam,LB_GETCURSEL,NULL,NULL);
				ShowPictureContent(hWnd,nRet+1);
				break;
			}
			break;
		case IDOK:
			char str[20];
			hCWnd=GetDlgItem(hWnd,IDC_CHOOSELIST);
			nRet=SendMessage(hCWnd,LB_GETCURSEL,NULL,NULL)+1;//图片资源ID
			itoa(nRet,str,10);
			hCWnd=GetDlgItem(hNewMapWnd,IDC_TID);
			SetWindowText(hCWnd,str);
		case IDCANCEL:
			EndDialog(hWnd,wParam);
			return true;
			break;
		}
		break;
		/**************************************************************\
		*WM_INITDIALOG:在这里绘制图像
		\**************************************************************/
		case WM_PAINT:
			hCWnd=GetDlgItem(hWnd,IDC_CHOOSELIST);
			nRet=SendMessage(hCWnd,LB_GETCURSEL,NULL,NULL);
			ShowPictureContent(hWnd,nRet+1);
	}
	return false;
}

//函数名称：ShowPictureContent
//说明：显示选中的rlb项目内容
//输入：hWnd：窗口句柄，要显示的项目索引
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void ShowPictureContent(HWND hWnd,int nIndex)
{
	HWND hCWnd;
	HDC hDestDC;
	HDC hDC;
	BITMAPINFO BmpInfo;
	long nValAddress;//图像数据开始位置
	void *pvBits;//图像数据
	HBITMAP hBmp;
	RECT rt;//窗口区域

	hCWnd=GetDlgItem(hWnd,IDC_DRAWPAGE);
	hDestDC=GetWindowDC(hCWnd);
	GetClientRect(hCWnd,&rt);

	nValAddress=(long)*(g_Rlb.GetContent(nIndex)+10);

	hDC=CreateCompatibleDC(NULL);//创建与屏幕兼容的DC

	ZeroMemory(&BmpInfo,sizeof(BITMAPINFO));//初始化位图信息

	memcpy(&BmpInfo,g_Rlb.GetContent(nIndex)+14,sizeof(BITMAPINFOHEADER));

	hBmp=CreateDIBSection(hDC,&BmpInfo,DIB_RGB_COLORS,&pvBits,
						NULL,0x0);
	memcpy(pvBits,&g_Rlb.GetContent(nIndex)[nValAddress],BmpInfo.bmiHeader.biSizeImage);

	SelectObject(hDC,hBmp);

	RedrawWindow(hCWnd,NULL,NULL,RDW_INVALIDATE);

	FillRect(hDestDC,&rt,(HBRUSH)COLOR_BACKGROUND);

	BitBlt(hDestDC,1,1,rt.right-rt.left,rt.bottom-rt.top,
				hDC,0,0,SRCCOPY);

	DeleteObject(hDC);

	DeleteObject(hBmp);
}

//函数名称：InitSlectTitleBox
//说明：初始化选择图块对话框
//输入：hWnd：对话框窗口句柄
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void InitSlectTitleBox(HWND hWnd)
{
	HWND hCWnd;//临时窗口句柄

	hCWnd=GetDlgItem(hWnd,IDC_CHOOSELIST);;
	
	int i;

	for(i=1;i<=g_Rlb.GetCount();i++)
	{
		if(i==19)
		{
			char *p=g_Rlb.GetName(i);
		}
		SendMessage(hCWnd,LB_ADDSTRING,NULL,(LPARAM)g_Rlb.GetName(i));
	}
}

//函数名称：OnCreate
//说明：WM_CREATE消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-16
//修改记录：
void OnCreate()
{
	
	int w,h;

	//获取屏幕大小
	w=GetSystemMetrics(SM_CXSCREEN);
	h=GetSystemMetrics(SM_CYSCREEN);

	//创建与屏幕兼容的DC
	g_hPageDC=CreateCompatibleDC(NULL);//缓冲页面
	g_hTitleDC=CreateCompatibleDC(NULL);//图块DC
	g_hDrawDC=CreateCompatibleDC(NULL);//绘图DC
}

//函数名称：OnPaint
//说明：WM_PAINT消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-16
//修改记录：
void OnPaint()
{
	HDC hWndDC;//前台DC
	RECT crt;//客户区大小
	RECT rt;//临时用矩形数据

	GetClientRect(g_hMainWnd,&crt);//获取客户区大小

	hWndDC=GetDC(g_hMainWnd);//前台DC

	rt=crt;

	rt.right=DIVSTRING_X;

	FillRect(g_hPageDC,&rt,(HBRUSH)COLOR_BACKGROUND);//绘制背景

	rt.left=DIVSTRING_X;
	rt.top=0;
	rt.right=DIVSTRING_X+5;
	rt.bottom=crt.bottom;

	DrawTitleToPage();//画地图到前台窗口

	FillRect(g_hPageDC,&rt,(HBRUSH)COLOR_BTNSHADOW);//绘制分割线

	rt=crt;
	rt.left=DIVSTRING_X+5;
	FillRect(g_hPageDC,&rt,(HBRUSH)COLOR_BACKGROUND);//绘制背景

	//绘制当前绘制区

	DrawMapToPage();

	if(g_AppInfo.bInDraw)
		DrawCurSel();

	BitBlt(hWndDC,0,0,crt.right-crt.left,crt.bottom-crt.top,g_hPageDC,0,0,SRCCOPY);
}

//函数名称：OnFileNew
//说明：WM_COMMMAND:ID_FILE_NEWI菜单项消息处理
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-16
//修改记录：
void OnFileNew()
{
	BITMAPINFO BmpInfo;
	long nValAddress;//图像数据开始位置
	void *pvBits;//图像数据
	int nRet;
	
	if(g_bCreate)
	{
		nRet=MessageBox(g_hMainWnd,"是否先保存再新建？","MapMake",MB_YESNOCANCEL);

		if(nRet==IDOK)
		{
		}
		else if(nRet=IDCANCEL)
		{
			return;
		}
	}
	
	//显示新建对话框
	if(DialogBox(g_hInstance,"NEWMAP",g_hMainWnd,(DLGPROC)NewMapBoxProc)==IDOK)
	{
		if(g_hDrawBitmap)
		{
			DeleteObject(g_hDrawBitmap);
		}
		if(g_hTitleBitmap)
		{
			DeleteObject(g_hTitleBitmap);
		}
		//载入图块资源
		ZeroMemory(&BmpInfo,sizeof(BITMAPINFO));

		nValAddress=(long)*(g_Rlb.GetContent(g_Map.nRlbID)+10);
		memcpy(&BmpInfo,g_Rlb.GetContent(g_Map.nRlbID)+14,sizeof(BITMAPINFOHEADER));

		g_hTitleBitmap=CreateDIBSection(g_hTitleDC,&BmpInfo,DIB_RGB_COLORS,&pvBits,
						NULL,0x0);
		memcpy(pvBits,&g_Rlb.GetContent(g_Map.nRlbID)[nValAddress],BmpInfo.bmiHeader.biSizeImage);

		SelectObject(g_hTitleDC,g_hTitleBitmap);

		g_hDrawBitmap=CreateCompatibleBitmap(GetDC(NULL),g_Map.nMapW*TITLE_W,g_Map.nMapH*TITLE_H);
		SelectObject(g_hDrawDC,g_hDrawBitmap);

		g_Map.TitleW=BmpInfo.bmiHeader.biWidth;
		g_Map.TitleH=BmpInfo.bmiHeader.biHeight;
		g_Map.LineX=g_Map.TitleW/TITLE_W;

		memset(&g_AppInfo,0,sizeof(AppInfo));//初始化程序数据

		g_AppInfo.rtSel[0].right=1;//底层选中
		g_AppInfo.rtSel[0].bottom=1;

		g_AppInfo.rtSel[1].right=1;//上层选中
		g_AppInfo.rtSel[1].bottom=1;

		
		//初始化地图
		int x,y;
		POINT pt;
		pt.x=0;
		pt.y=0;
		for(y=0;y<g_Map.nMapH;y++)
		{
			for(x=0;x<g_Map.nMapW;x++)
			{
				g_Map.Map[0][x][y]=0;
				g_Map.Map[1][x][y]=0;
			}
		}
		DrawAllMap();

		g_bCreate=true;//设置地图已建立
		UpdateViewMenu();
	}
	OnPaint();//画地图到前台窗口
}

//函数名称：OnKeyDown
//说明：WM_KEYDOWN消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-19
//修改记录：
void OnKeyDown(int nKey)
{
	if(nKey==KEY_T_UP)//上
	{
		g_AppInfo.ptTitle.y-=1;
	}
	else if(nKey==KEY_T_LEFT)//左
	{
		g_AppInfo.ptTitle.x-=1;
	}
	else if(nKey==KEY_T_RIGHT)//右
	{
		g_AppInfo.ptTitle.x+=1;
	}
	else if(nKey==KEY_T_DOWN)//下
	{
		g_AppInfo.ptTitle.y+=1;
	}
	else if(nKey==VK_UP)//上
	{
		g_AppInfo.ptDraw.y-=1;
	}
	else if(nKey==VK_DOWN)
	{
		g_AppInfo.ptDraw.y+=1;
	}
	else if(nKey==VK_LEFT)
	{
		g_AppInfo.ptDraw.x-=1;
	}
	else if(nKey==VK_RIGHT)
	{
		g_AppInfo.ptDraw.x+=1;
	}

	OnPaint();
}

//函数名称：OnLRuttonDown
//说明：WM_RBUTTONDOWN:响应鼠标左键
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-21
//修改记录：
void OnRButtonDown(UINT nFlags,POINT pt)
{
	int x,y;//换算用
	float Factor;

	Factor=GetFactor();//获取缩放倍数

	if(nFlags==MK_RBUTTON)
	{
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
			&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
			&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
			&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//鼠标左键点击在图块上
		{
			x=pt.x/TITLE_W-g_AppInfo.ptTitle.x;
			y=pt.y/TITLE_H-g_AppInfo.ptTitle.y;
			g_AppInfo.rtSel[0].left=x;
			g_AppInfo.rtSel[0].top=y;
			g_AppInfo.rtSel[0].right=x+1;
			g_AppInfo.rtSel[0].bottom=y+1;
			
			OnPaint();	
		}
		else//点击在绘图区
		{
			x=(pt.x-DIVSTRING_X-5)/TITLE_W/Factor;
			y=pt.y/TITLE_H/Factor;
			
			if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;
				
				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,0);//上层地图
				
				g_AppInfo.bInDraw=true;
				OnPaint();
			}
			else//鼠标不在绘制地图上
			{
				g_AppInfo.bInDraw=false;
				OnPaint();
			}
		}
	}	else if(nFlags==(MK_RBUTTON|MK_CONTROL))
	{
		x=(pt.x-DIVSTRING_X-5)/TITLE_W/Factor;
		y=pt.y/TITLE_H/Factor;
		
		if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
			&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
		{
			POINT pt;
			char str[32];
			
			sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
			SetWindowText(g_hMainWnd,str);
			g_AppInfo.ptDrawSel.x=x;
			g_AppInfo.ptDrawSel.y=y;
			
			pt.x=x-g_AppInfo.ptDraw.x;
			pt.y=y-g_AppInfo.ptDraw.y;
			OnSetOpp(pt,0);//上层地图
			
			g_AppInfo.bInDraw=true;
			OnPaint();
		}
		else//鼠标不在绘制地图上
		{
			g_AppInfo.bInDraw=false;
			OnPaint();
		}
	}

}


//函数名称：OnLButtonDown
//说明：WM_LBUTTONDOWN:响应鼠标左键
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-20
//修改记录：
void OnLButtonDown(UINT nFlags,POINT pt)
{
	int x,y;//换算用
	float Factor;//系数

	Factor=GetFactor();

	if(nFlags==MK_LBUTTON)//点击在选图区
	{
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
					&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
					&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
					&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//鼠标左键点击在图块上
		{
			x=pt.x/TITLE_W-g_AppInfo.ptTitle.x;
			y=pt.y/TITLE_H-g_AppInfo.ptTitle.y;
			g_AppInfo.rtSel[1].left=x;
			g_AppInfo.rtSel[1].top=y;
			g_AppInfo.rtSel[1].right=x+1;
			g_AppInfo.rtSel[1].bottom=y+1;

			OnPaint();
		}
		else//点击在绘图区
		{
			x=(pt.x-DIVSTRING_X-5)/TITLE_W/Factor;
			y=pt.y/TITLE_H/Factor;
			
			if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;

				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,1);//上层地图

				g_AppInfo.bInDraw=true;
				OnPaint();
			}
			else//鼠标不在绘制地图上
			{
				g_AppInfo.bInDraw=false;
				OnPaint();
			}
		}
	}
	else if(nFlags==(MK_LBUTTON|MK_CONTROL))
	{
		x=(pt.x-DIVSTRING_X-5)/TITLE_W/Factor;
		y=pt.y/TITLE_H/Factor;
		
		if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
			&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
		{
			POINT pt;
			char str[32];
			
			sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
			SetWindowText(g_hMainWnd,str);
			g_AppInfo.ptDrawSel.x=x;
			g_AppInfo.ptDrawSel.y=y;
			
			pt.x=x-g_AppInfo.ptDraw.x;
			pt.y=y-g_AppInfo.ptDraw.y;
			OnSetOpp(pt,1);//上层地图
			
			g_AppInfo.bInDraw=true;
			OnPaint();
		}
		else//鼠标不在绘制地图上
		{
			g_AppInfo.bInDraw=false;
			OnPaint();
		}
	}
}

//函数名称：OnMouseMove
//说明：WM_MOUSEMOVE消息处理函数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-19
//修改记录：
void OnMouseMove(UINT nFlags,POINT pt)
{
	static bool bLeft[2]={true,true};//选图矩形右上顶点
	static bool bTop[2]={true,true};
	int x,y;//换算用
	int nFloor;
	float Factor;


	Factor=GetFactor();

	if(nFlags==MK_RBUTTON)
	{
		nFloor=0;//底层地图
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
					&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
					&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
					&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//鼠标左键点击在图块上
		{
			x=pt.x/TITLE_W-g_AppInfo.ptTitle.x;
			y=pt.y/TITLE_H-g_AppInfo.ptTitle.y;

			if(x>=g_AppInfo.rtSel[nFloor].left)
			{
				g_AppInfo.rtSel[nFloor].right=x+1;
				if(!bLeft[nFloor])
				{
					g_AppInfo.rtSel[nFloor].left--;
					bLeft[nFloor]=true;
				}
			}
			if(y>=g_AppInfo.rtSel[nFloor].top)
			{
				g_AppInfo.rtSel[nFloor].bottom=y+1;
				if(!bTop[nFloor])
				{
					g_AppInfo.rtSel[nFloor].top--;
					bTop[nFloor]=true;
				}
			}
			if(x<g_AppInfo.rtSel[nFloor].left)
			{
				g_AppInfo.rtSel[nFloor].right=x;
				
				if(bLeft[nFloor])
				{
					g_AppInfo.rtSel[nFloor].left++;
					bLeft[nFloor]=false;
				}
			}
			if(y<g_AppInfo.rtSel[nFloor].top)
			{
				g_AppInfo.rtSel[nFloor].bottom=y;
				if(bTop[nFloor])
				{
					g_AppInfo.rtSel[nFloor].top++;
					bTop[nFloor]=false;
				}
			}
			OnPaint();
		}
		else
		{
			x=(pt.x-DIVSTRING_X-5)/(TITLE_W*Factor);
			y=pt.y/(TITLE_H*Factor);
			
			if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;

				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,0);//上层地图

				g_AppInfo.bInDraw=true;
				OnPaint();
			}
		}
	}
	else if(nFlags==MK_LBUTTON)
	{
		nFloor=1;//上层地图
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
					&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
					&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
					&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//鼠标左键点击在图块上
		{
			x=pt.x/TITLE_W-g_AppInfo.ptTitle.x;
			y=pt.y/TITLE_H-g_AppInfo.ptTitle.y;

			if(x>=g_AppInfo.rtSel[nFloor].left)
			{
				g_AppInfo.rtSel[nFloor].right=x+1;
				if(!bLeft[nFloor])
				{
					g_AppInfo.rtSel[nFloor].left--;
					bLeft[nFloor]=true;
				}
			}
			if(y>=g_AppInfo.rtSel[nFloor].top)
			{
				g_AppInfo.rtSel[nFloor].bottom=y+1;
				if(!bTop[nFloor])
				{
					g_AppInfo.rtSel[nFloor].top--;
					bTop[nFloor]=true;
				}
			}
			if(x<g_AppInfo.rtSel[nFloor].left)
			{
				g_AppInfo.rtSel[nFloor].right=x;
				
				if(bLeft[nFloor])
				{
					g_AppInfo.rtSel[nFloor].left++;
					bLeft[nFloor]=false;
				}
			}
			if(y<g_AppInfo.rtSel[nFloor].top)
			{
				g_AppInfo.rtSel[nFloor].bottom=y;
				if(bTop[nFloor])
				{
					g_AppInfo.rtSel[nFloor].top++;
					bTop[nFloor]=false;
				}
			}
			OnPaint();
		}
		else
		{
			x=(pt.x-DIVSTRING_X-5)/(TITLE_W*Factor);
			y=pt.y/(TITLE_H*Factor);
			
			if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;

				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,1);//上层地图

				g_AppInfo.bInDraw=true;
				OnPaint();
			}
		}
	}
	else if(nFlags==NULL && pt.x>DIVSTRING_X+5)//鼠标在绘图区
	{
		x=(pt.x-DIVSTRING_X-5)/(TITLE_W*Factor);
		y=pt.y/(TITLE_H*Factor);
		
		if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
			&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//鼠标在绘制地图上
		{
			char str[32];

			sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
			SetWindowText(g_hMainWnd,str);
			g_AppInfo.ptDrawSel.x=x;
			g_AppInfo.ptDrawSel.y=y;
			g_AppInfo.bInDraw=true;
			OnPaint();
		}
		else//鼠标不在绘制地图上
		{
			g_AppInfo.bInDraw=false;
			OnPaint();
		}
	}
	else//鼠标不在绘制地图上
	{
		g_AppInfo.bInDraw=false;
		OnPaint();
	}
}


//函数名称：OnSize
//说明：WM_SIZE:缩放后的处理
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-19
//修改记录：
void OnSize()
{
	RECT rt;

	GetClientRect(g_hMainWnd,&rt);//获取主窗口客户区大小
	
	if(g_hPageBitmap)
	{
		DeleteObject(g_hPageBitmap);
	}
	g_hPageBitmap=CreateCompatibleBitmap(GetDC(NULL),rt.right-rt.left,rt.bottom-rt.top);
	SelectObject(g_hPageDC,g_hPageBitmap);
}

//函数名称：OnDestroy
//说明：程序退出处理
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-19
//修改记录：
void OnDestroy()
{
	if(g_hPageDC)
	{
		DeleteDC(g_hPageDC);
	}
	if(g_hPageBitmap)
	{
		DeleteObject(g_hPageBitmap);
	}
	if(g_hDrawDC)
	{
		DeleteDC(g_hDrawDC);
	}
	if(g_hDrawBitmap)
	{
		DeleteObject(g_hDrawBitmap);
	}
	if(g_hTitleDC)
	{
		DeleteDC(g_hTitleDC);
	}
	if(g_hTitleBitmap)
	{
		DeleteObject(g_hTitleBitmap);
	}
}

//函数名称：DrawMap
//说明：将指定位置绘制为指定图块
//输入：ptDest:目标位置，ptSource:源位置
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void DrawMap(POINT ptDest)
{
	int dx,dy;
	int sx,sy;
	HBRUSH hBr;
	RECT rt;

	hBr=CreateSolidBrush(g_Map.crBack);

	rt.left=ptDest.x*TITLE_W;
	rt.right=rt.left+TITLE_W;
	rt.top=ptDest.y*TITLE_H;
	rt.bottom=rt.top+TITLE_H;

	FillRect(g_hDrawDC,&rt,hBr);

	DeleteObject(hBr);

	dx=ptDest.x*TITLE_W;
	dy=ptDest.y*TITLE_H;

	sx=abs((g_Map.Map[0][ptDest.x][ptDest.y]%g_Map.LineX)*TITLE_W);
	sy=abs((g_Map.Map[0][ptDest.x][ptDest.y]/g_Map.LineX)*TITLE_H);

	if(sx!=0 || sy!=0)
	{
		//绘制底层地图
		TransparentBlt(g_hDrawDC,dx,dy,TITLE_W,TITLE_H,g_hTitleDC,sx,sy,TITLE_W,TITLE_H,TRANSPARENT_COLOR);
	}

	sx=abs((g_Map.Map[1][ptDest.x][ptDest.y]%g_Map.LineX)*TITLE_W);
	sy=abs((g_Map.Map[1][ptDest.x][ptDest.y]/g_Map.LineX)*TITLE_H);

	if(sx!=0 || sy!=0)
	{
		TransparentBlt(g_hDrawDC,dx,dy,TITLE_W,TITLE_H,g_hTitleDC,sx,sy,TITLE_W,TITLE_H,TRANSPARENT_COLOR);
	}
	
	if(g_Map.Map[0][ptDest.x][ptDest.y]<0)
	{
		TransparentBlt(g_hDrawDC,dx,dy,TITLE_W,TITLE_H,g_hTitleDC,0,0,TITLE_W,TITLE_H,TRANSPARENT_COLOR);
	}

	if(g_Map.Map[1][ptDest.x][ptDest.y]<0)
	{
		LOGBRUSH lBr;
		HBRUSH hBr;
		HPEN hPen;
		HBRUSH htBr;
		HPEN htPen;

		lBr.lbStyle=BS_HOLLOW;
		hBr=CreateBrushIndirect(&lBr);
		hPen=CreatePen(PS_SOLID,3,CR_LOAD);

		htBr=(HBRUSH)SelectObject(g_hDrawDC,hBr);
		htPen=(HPEN)SelectObject(g_hDrawDC,hPen);

		Rectangle(g_hDrawDC,dx+3,dy+3,dx+TITLE_W-3,dy+TITLE_H-3);

		SelectObject(g_hDrawDC,htBr);
		SelectObject(g_hDrawDC,htPen);

		DeleteObject(hBr);
		DeleteObject(hPen);
	}
}

//函数名称：DrawTitleToPage
//说明：绘制到前台缓冲窗口
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void DrawTitleToPage()
{
	int x,y;
	int w,h;
	HBRUSH htBr;
	HPEN htPen;
	HBRUSH hBr;
	HPEN hPen;
	HDC hDC;
	
	if(!g_bCreate) return;
	
	hDC=g_hPageDC;
	
	//绘制选图区
	x=g_AppInfo.ptTitle.x*TITLE_W;
	y=g_AppInfo.ptTitle.y*TITLE_H;
	w=g_Map.TitleW;
	h=g_Map.TitleH;
	if(w+x>DIVSTRING_X)//要显示的宽度超过分割线
	{
		w=DIVSTRING_X-x;
	}
	BitBlt(hDC,x,y,w,h,g_hTitleDC,0,0,SRCCOPY);
	//绘制选图块矩形
	RECT rt;
	rt=g_AppInfo.rtSel[0];
	rt.top+=g_AppInfo.ptTitle.y;
	rt.bottom+=g_AppInfo.ptTitle.y;
	rt.left+=g_AppInfo.ptTitle.x;
	rt.right+=g_AppInfo.ptTitle.x;
	LOGBRUSH lbr;
	
	lbr.lbStyle=BS_HOLLOW;
	lbr.lbColor=0xFFFFFF;
	hBr=CreateBrushIndirect(&lbr);
	
	htBr=(HBRUSH)SelectObject(hDC,hBr);
	
	hPen=CreatePen(PS_SOLID,3,CR_DOWN);
	
	htPen=(HPEN)SelectObject(hDC,hPen);
	
	Rectangle(hDC,rt.left*TITLE_W,rt.top*TITLE_H,
		rt.right*TITLE_W,rt.bottom*TITLE_H);
	
	//删除画刷
	DeleteObject(hPen);
	
	hPen=CreatePen(PS_SOLID,3,CR_UP);
	SelectObject(hDC,hPen);
	rt=g_AppInfo.rtSel[1];
	rt.top+=g_AppInfo.ptTitle.y;
	rt.bottom+=g_AppInfo.ptTitle.y;
	rt.left+=g_AppInfo.ptTitle.x;
	rt.right+=g_AppInfo.ptTitle.x;
	
	Rectangle(hDC,rt.left*TITLE_W,rt.top*TITLE_H,
		rt.right*TITLE_W,rt.bottom*TITLE_H);
	
	
	SelectObject(hDC,htBr);//还原DC
	SelectObject(hDC,htPen);
	DeleteObject(hBr);
}


//函数名称：DrawAllMap()
//说明：绘制整个地图到地图缓冲
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void DrawAllMap()
{
	int x,y;
	POINT ptDest;
	//绘制上层地图
	for(y=0;y<g_Map.nMapH;y++)
	{
		for(x=0;x<g_Map.nMapW;x++)
		{
			ptDest.x=x;
			ptDest.y=y;

			DrawMap(ptDest);
		}
	}
}

//函数名称：DrawMapToPage()
//说明：绘制地图到前台缓冲
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-18
//修改记录：
void DrawMapToPage()
{
	int sx,sy;
	int x,y;
	int w,h;
	int sw,sh;
	float Factor;//系数

	if(!g_bCreate) return;
	
	Factor=GetFactor();

	x=DIVSTRING_X+5+g_AppInfo.ptDraw.x*TITLE_W*Factor;
	y=g_AppInfo.ptDraw.y*TITLE_H*Factor;
	w=g_Map.nMapW*TITLE_W*Factor;
	h=g_Map.nMapH*TITLE_H*Factor;

	sx=0;
	sy=0;
	sw=g_Map.nMapW*TITLE_W;
	sh=g_Map.nMapH*TITLE_H;

	//修正坐标
	if(x<DIVSTRING_X+5)
	{
		w=w-(DIVSTRING_X+5-x);
		sx=(DIVSTRING_X+5-x)/Factor;
		sw-=sx;
		x=DIVSTRING_X+5;
		if(w<0) w=0;
	}
	

	StretchBlt(g_hPageDC,x,y,w,h,
				g_hDrawDC,sx,sy,sw,sh,SRCCOPY);//复制地图图像到前台缓冲
}

//函数名称：GetDrawRect
//说明：选中矩形与绘制矩形的转换
//输入：rt:选中矩形
//输出：
//返回：
//备注：
//日期：2011-8-21
//修改记录：
RECT GetDrawRect(RECT rt)
{
	int t;

	if(rt.left>rt.right)
	{
		t=rt.left;
		rt.left=rt.right;
		rt.right=t;
	}
	if(rt.top>rt.bottom)
	{
		t=rt.top;
		rt.top=rt.bottom;
		rt.bottom=t;
	}
	rt.right-=rt.left;
	rt.bottom-=rt.top;
	rt.left=0;
	rt.top=0;

	return rt;
}

//函数名称：GetDrawRect
//说明：返回合理化后的选中矩形（left<right,top<bottom为合理化）
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-21
//修改记录：
RECT GetSelRect(int nFloor)
{
	int t;
	RECT rt;

	rt=g_AppInfo.rtSel[nFloor];
	if(rt.left>rt.right)
	{
		t=rt.left;
		rt.left=rt.right;
		rt.right=t;
	}
	if(rt.bottom<rt.top)
	{
		t=rt.bottom;
		rt.bottom=rt.top;
		rt.top=t;
	}

	return rt;
}

//函数名称：DrawCurSel()
//说明：绘制当前选中区
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-21
//修改记录：
void DrawCurSel()
{
	int x,y;
	LOGBRUSH lbr;
	RECT rt;
	HBRUSH hBr;
	HBRUSH htBr;
	HPEN hPen;
	HPEN htPen;
	float Factor;

	Factor=GetFactor();
	
	x=g_AppInfo.ptDrawSel.x;
	y=g_AppInfo.ptDrawSel.y;
	
	rt=GetDrawRect(g_AppInfo.rtSel[0]);
	
	rt.top=(y+rt.top)*TITLE_H*Factor;
	rt.bottom=(y+rt.bottom)*TITLE_H*Factor;
	rt.left=(x+rt.left)*TITLE_W*Factor+DIVSTRING_X+5;
	rt.right=(x+rt.right)*TITLE_W*Factor+DIVSTRING_X+5;
	
	
	lbr.lbStyle=BS_HOLLOW;//空心画刷
	hBr=CreateBrushIndirect(&lbr);
	
	hPen=CreatePen(PS_SOLID,3,CR_DOWN);
	
	htPen=(HPEN)SelectObject(g_hPageDC,hPen);
	htBr=(HBRUSH)SelectObject(g_hPageDC,hBr);
	
	Rectangle(g_hPageDC,rt.left,
		rt.top,rt.right,
		rt.bottom);
	
	DeleteObject(hPen);
	
	rt=GetDrawRect(g_AppInfo.rtSel[1]);
	
	rt.top=(y+rt.top)*TITLE_H*Factor;
	rt.bottom=(y+rt.bottom)*TITLE_H*Factor;
	rt.left=(x+rt.left)*TITLE_W*Factor+DIVSTRING_X+5;
	rt.right=(x+rt.right)*TITLE_W*Factor+DIVSTRING_X+5;
	
	hPen=CreatePen(PS_SOLID,3,CR_UP);
	
	SelectObject(g_hPageDC,hPen);
	
	Rectangle(g_hPageDC,rt.left,
		rt.top,rt.right,
		rt.bottom);
	
	RECT crt;
	GetClientRect(g_hMainWnd,&crt);
	BitBlt(GetDC(g_hMainWnd),0,0,crt.right-crt.left,crt.bottom-crt.top,g_hPageDC,0,0,SRCCOPY);
	DeleteObject(hBr);
	DeleteObject(hPen);
	SelectObject(g_hPageDC,htPen);//还原DC
	SelectObject(g_hPageDC,htBr);
	
}


//函数名称：GetFactor()
//说明：获取缩放倍数
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-21
//修改记录：
float GetFactor()
{
	float Ret;

	if(g_AppInfo.nView==VIEW_1_1)
	{
		Ret=1;
	}
	else if(g_AppInfo.nView==VIEW_1_2)
	{
		Ret=0.5;
	}
	else if(g_AppInfo.nView==VIEW_2_1)
	{
		Ret=2;
	}

	return Ret;
}

//函数名称：UpdateViewMenu
//说明：更新视图菜单
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-21
//修改记录：
void UpdateViewMenu()
{
	int nRet;
	HMENU hMenu;

	hMenu=GetMenu(g_hMainWnd);//获取菜单

	if(g_AppInfo.nView==VIEW_1_1)
	{
		nRet=GetMenuState(hMenu,ID_VIEW_1_1,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)//未标记
		{
			CheckMenuItem(hMenu,ID_VIEW_1_1,MF_CHECKED);
			//清除其它项
			CheckMenuItem(hMenu,ID_VIEW_1_2,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_VIEW_2_1,MF_UNCHECKED);
		}
		g_AppInfo.nView=VIEW_1_1;
	}
	else if(g_AppInfo.nView==VIEW_1_2)
	{
		nRet=GetMenuState(hMenu,ID_VIEW_1_2,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)//未标记
		{
			CheckMenuItem(hMenu,ID_VIEW_1_2,MF_CHECKED);
			//清除其它项
			CheckMenuItem(hMenu,ID_VIEW_1_1,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_VIEW_2_1,MF_UNCHECKED);
		}
		g_AppInfo.nView=VIEW_1_2;
	}
	else if(g_AppInfo.nView==VIEW_2_1)
	{
		nRet=GetMenuState(hMenu,ID_VIEW_2_1,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)//未标记
		{
			CheckMenuItem(hMenu,ID_VIEW_2_1,MF_CHECKED);
			//清除其它项
			CheckMenuItem(hMenu,ID_VIEW_1_1,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_VIEW_1_2,MF_UNCHECKED);
		}
		g_AppInfo.nView=VIEW_2_1;
	}

	OnPaint();
}


//函数名称：UpdateDrawMenu
//说明：更新绘图菜单
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
/*void UpdateDrawMenu()
{
	int nRet;
	HMENU hMenu;

	hMenu=GetMenu(g_hMainWnd);

	if(g_AppInfo.nTool==TOOL_PEN)//铅笔
	{
		nRet=GetMenuState(hMenu,ID_DRAW_PEN,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)
		{
			CheckMenuItem(hMenu,ID_DRAW_PEN,MF_CHECKED);

			CheckMenuItem(hMenu,ID_DRAW_RECT,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_FILL,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_SELECT,MF_UNCHECKED);
		}
	}
	else if(g_AppInfo.nTool==TOOL_RECT)//四边形
	{
		nRet=GetMenuState(hMenu,ID_DRAW_RECT,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)
		{
			CheckMenuItem(hMenu,ID_DRAW_RECT,MF_CHECKED);

			CheckMenuItem(hMenu,ID_DRAW_PEN,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_FILL,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_SELECT,MF_UNCHECKED);
		}
	}
	else if(g_AppInfo.nTool==TOOL_ELLIPSE)//椭圆
	{
		nRet=GetMenuState(hMenu,ID_DRAW_ELLIPSE,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)
		{
			CheckMenuItem(hMenu,ID_DRAW_ELLIPSE,MF_CHECKED);

			CheckMenuItem(hMenu,ID_DRAW_RECT,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_PEN,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_FILL,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_SELECT,MF_UNCHECKED);
		}
	}
	else if(g_AppInfo.nTool==TOOL_FILL)//填充
	{
		nRet=GetMenuState(hMenu,ID_DRAW_FILL,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)
		{
			CheckMenuItem(hMenu,ID_DRAW_FILL,MF_CHECKED);

			CheckMenuItem(hMenu,ID_DRAW_RECT,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_PEN,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_SELECT,MF_UNCHECKED);
		}
	}
	else if(g_AppInfo.nTool==TOOL_SELECT)//选择
	{
		nRet=GetMenuState(hMenu,ID_DRAW_SELECT,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)
		{
			CheckMenuItem(hMenu,ID_DRAW_SELECT,MF_CHECKED);

			CheckMenuItem(hMenu,ID_DRAW_RECT,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_ELLIPSE,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_PEN,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_DRAW_FILL,MF_UNCHECKED);
		}
	}

}*/


//函数名称：OnToolDeal
//说明：工具处理
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
void OnToolDeal(POINT pt,int nFloor)
{
	RECT rt;
	int x,y;
	int ex,ey;
	POINT ptDest;

	rt=GetSelRect(nFloor);
	x=pt.x;
	y=pt.y;
	ex=rt.right-rt.left+x;
	ey=rt.bottom-rt.top+y;

	for(y=pt.y;y<ey && y<g_Map.nMapH;y++)
	{
		for(x=pt.x;x<ex && x<g_Map.nMapW;x++)
		{
			g_Map.Map[nFloor][x][y]=(rt.top+y-pt.y)*g_Map.LineX+(rt.left+x-pt.x);
			ptDest.x=x;
			ptDest.y=y;
			DrawMap(ptDest);
		}
	}
}

//函数名称：OnSetOpp
//说明：设置地图数据取反
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
void OnSetOpp(POINT pt,int nFloor)
{
	RECT rt;
	int x,y;
	int ex,ey;
	POINT ptDest;

	rt=GetSelRect(nFloor);
	x=pt.x;
	y=pt.y;
	ex=rt.right-rt.left+x;
	ey=rt.bottom-rt.top+y;

	for(y=pt.y;y<ey && y<g_Map.nMapH;y++)
	{
		for(x=pt.x;x<ex && x<g_Map.nMapW;x++)
		{
			g_Map.Map[nFloor][x][y]*=-1;//设置数据取反
			ptDest.x=x;
			ptDest.y=y;
			DrawMap(ptDest);
		}
	}
}


//函数名称：OnFileSave
//说明：文件保存
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
void OnFileSave()
{
	char strFileName[MAX_PATH];

	if(!g_bCreate)
	{
		MessageBox(g_hMainWnd,"请先新建地图！","MapMake",MB_OK);
		return;
	}

	if(!strcmp(g_strFileName.c_str(),""))
	{
		OPENFILENAME op;//保存对话框信息

		ZeroMemory(&op,sizeof(OPENFILENAME));

		op.lpstrInitialDir=g_strMapDirName.c_str();
		op.lpstrDefExt=".Map";
		op.lpstrFilter="地图文件(*.map)\0*.map\0";
		op.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
		op.lStructSize=sizeof(OPENFILENAME);
		op.lpstrFile=strFileName;
		op.nMaxFile=MAX_PATH;

		strFileName[0]='\0';
		if(GetSaveFileName(&op))
		{
			g_strFileName=strFileName;
		}
		else
			return;
	}
	
	//保存文件
	FILE *fp;
	int tInt;

	fp=fopen(g_strFileName.c_str(),"wb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"保存文件失败！","MapMake",MB_OK);
		return;
	}

	fwrite(&g_Map.nRlbID,sizeof(int),1,fp);//写入rlb资源ID
	if(g_Map.crBack==0x000000)//黑色
		tInt=1;
	else
		tInt=0;

	fwrite(&tInt,sizeof(int),1,fp);//写入背景
	//写入地图大小
	fwrite(&g_Map.nMapW,sizeof(int),1,fp);
	fwrite(&g_Map.nMapH,sizeof(int),1,fp);

	//写入地图
	int x,y;
	for(y=0;y<g_Map.nMapH;y++)
	{
		for(x=0;x<g_Map.nMapW;x++)
		{
			fwrite(&g_Map.Map[0][x][y],sizeof(int),1,fp);
			fwrite(&g_Map.Map[1][x][y],sizeof(int),1,fp);
		}
	}

	fclose(fp);

	MessageBox(g_hMainWnd,"保存成功！","MapMake",MB_OK);
}

//函数名称：OnFileOpen
//说明：打开文件
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
void OnFileOpen()
{
	int nRet;

	if(g_bCreate)
	{
		nRet=MessageBox(g_hMainWnd,"是否保存再打开？","MapMake",MB_YESNOCANCEL);
		if(nRet==IDYES)
			OnFileSave();
		else if(nRet==IDCANCEL)
			return;
	}

	char strFileName[MAX_PATH];

	OPENFILENAME op;//保存对话框信息
	
	ZeroMemory(&op,sizeof(OPENFILENAME));
	
	op.lpstrInitialDir=g_strMapDirName.c_str();
	op.lpstrDefExt=".Map";
	op.lpstrFilter="地图文件(*.map)\0*.map\0";
	op.Flags=OFN_HIDEREADONLY|OFN_FILEMUSTEXIST;
	op.lStructSize=sizeof(OPENFILENAME);
	op.lpstrFile=strFileName;
	op.nMaxFile=MAX_PATH;
	
	strFileName[0]='\0';

	if(!GetOpenFileName(&op))
		return;

	FILE *fp;
	int tInt;

	fp=fopen(strFileName,"rb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"打开文件失败！","MapMake",MB_OK);
	}

	fread(&g_Map.nRlbID,sizeof(int),1,fp);//写入rlb资源ID
	
	fread(&tInt,sizeof(int),1,fp);//写入背景

	if(tInt==1)//黑色
		g_Map.crBack=0x000000;
	else
		g_Map.crBack=0xFFFFFF;
	//写入地图大小
	fread(&g_Map.nMapW,sizeof(int),1,fp);
	fread(&g_Map.nMapH,sizeof(int),1,fp);

	//写入地图
	int x,y;
	for(y=0;y<g_Map.nMapH;y++)
	{
		for(x=0;x<g_Map.nMapW;x++)
		{
			fread(&g_Map.Map[0][x][y],sizeof(int),1,fp);
			fread(&g_Map.Map[1][x][y],sizeof(int),1,fp);
		}
	}

	fclose(fp);
	
	g_strFileName=strFileName;
	
	if(g_hDrawBitmap)
	{
		DeleteObject(g_hDrawBitmap);
	}
	if(g_hTitleBitmap)
	{
		DeleteObject(g_hTitleBitmap);
	}
	BITMAPINFO BmpInfo;
	long nValAddress;
	void *pvBits;

	//载入图块资源
	ZeroMemory(&BmpInfo,sizeof(BITMAPINFO));
	
	nValAddress=(long)*(g_Rlb.GetContent(g_Map.nRlbID)+10);
	memcpy(&BmpInfo,g_Rlb.GetContent(g_Map.nRlbID)+14,sizeof(BITMAPINFOHEADER));
	
	g_hTitleBitmap=CreateDIBSection(g_hTitleDC,&BmpInfo,DIB_RGB_COLORS,&pvBits,
		NULL,0x0);
	memcpy(pvBits,&g_Rlb.GetContent(g_Map.nRlbID)[nValAddress],BmpInfo.bmiHeader.biSizeImage);
	
	SelectObject(g_hTitleDC,g_hTitleBitmap);
	
	g_hDrawBitmap=CreateCompatibleBitmap(GetDC(NULL),g_Map.nMapW*TITLE_W,g_Map.nMapH*TITLE_H);
	SelectObject(g_hDrawDC,g_hDrawBitmap);
	
	g_Map.TitleW=BmpInfo.bmiHeader.biWidth;
	g_Map.TitleH=BmpInfo.bmiHeader.biHeight;
	g_Map.LineX=g_Map.TitleW/TITLE_W;
	
	memset(&g_AppInfo,0,sizeof(AppInfo));//初始化程序数据
	
	g_AppInfo.rtSel[0].right=1;//底层选中
	g_AppInfo.rtSel[0].bottom=1;
	
	g_AppInfo.rtSel[1].right=1;//上层选中
	g_AppInfo.rtSel[1].bottom=1;
	
	g_bCreate=true;
	//初始化地图

	DrawAllMap();
	UpdateViewMenu();
	
	OnPaint();//画地图到前台窗口
}

//函数名称：OnFillMap
//说明：填充地图
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
void OnFillMap()
{
	int x,y;

	for(y=0;y<g_Map.nMapH;y++)
	{
		for(x=0;x<g_Map.nMapW;x++)
		{
			g_Map.Map[0][x][y]=g_AppInfo.rtSel[0].top*g_Map.LineX+g_AppInfo.rtSel[0].left;
			g_Map.Map[1][x][y]=0;
		}
	}

	DrawAllMap();
	OnPaint();
}


//函数名称：OnFileOtherSave
//说明：填充地图
//输入：
//输出：
//返回：
//备注：
//日期：2011-8-22
//修改记录：
void OnFileOtherSave()
{
	char strFileName[MAX_PATH];
	OPENFILENAME op;//保存对话框信息

	if(!g_bCreate)
	{
		MessageBox(g_hMainWnd,"请先新建地图！","MapMake",MB_OK);
		return;
	}
	
	ZeroMemory(&op,sizeof(OPENFILENAME));
	
	op.lpstrInitialDir=g_strMapDirName.c_str();
	op.lpstrDefExt=".Map";
	op.lpstrFilter="地图文件(*.map)\0*.map\0";
	op.Flags=OFN_HIDEREADONLY|OFN_PATHMUSTEXIST;
	op.lStructSize=sizeof(OPENFILENAME);
	op.lpstrFile=strFileName;
	op.nMaxFile=MAX_PATH;
	
	strFileName[0]='\0';
	if(GetSaveFileName(&op))
	{
		g_strFileName=strFileName;
	}
	else
		return;
	
	//保存文件
	FILE *fp;
	int tInt;

	fp=fopen(g_strFileName.c_str(),"wb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"保存文件失败！","MapMake",MB_OK);
		return;
	}

	fwrite(&g_Map.nRlbID,sizeof(int),1,fp);//写入rlb资源ID
	if(g_Map.crBack==0x000000)//黑色
		tInt=1;
	else
		tInt=0;

	fwrite(&tInt,sizeof(int),1,fp);//写入背景
	//写入地图大小
	fwrite(&g_Map.nMapW,sizeof(int),1,fp);
	fwrite(&g_Map.nMapH,sizeof(int),1,fp);

	//写入地图
	int x,y;
	for(y=0;y<g_Map.nMapH;y++)
	{
		for(x=0;x<g_Map.nMapW;x++)
		{
			fwrite(&g_Map.Map[0][x][y],sizeof(int),1,fp);
			fwrite(&g_Map.Map[1][x][y],sizeof(int),1,fp);
		}
	}

	fclose(fp);

	MessageBox(g_hMainWnd,"保存成功！","MapMake",MB_OK);
}