#include "StdAfx.h"

#include "Globals.h"

//�½��Ի�������ַ�������
#define MAX_EDITSTRING			20

LRESULT WINAPI AboutBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//���ڶԻ�����Ϣ������
LRESULT WINAPI SelectTitleBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);//ѡ��ͼ��Ի�����Ϣ������

extern HINSTANCE g_hInstance;

static HWND hNewMapWnd;

//��������Ϣ������
void OnCreate();//WM_CREATE��Ϣ������
void OnDestroy();//WM_DESTROY��Ϣ������
void OnFileNew();//WM_COMMAND:ID_FILE_NEWI�˵�����Ϣ����
void OnFileOpen();//WM_COMMAND:ID_FILE_OPENI�˵�����Ϣ����
void OnFileSave();//WM_COMMAND:ID_FILE_SAVEI�˵�����Ϣ����
void OnFileOtherSave();//WM_COMMAND:ID_FILE_OTHERSAVEI�˵�����Ϣ����
void OnFillMap();//WM_COMMAND:ID_EDIT_FILL�˵�����Ϣ��������ͼ
void OnView11();//WM_COMMAND:ID_VIEW_1_1�˵�����Ϣ����
void OnView12();//WM_COMMAND:��ͼ->1/2�˵���Ӧ
void OnView21();//WM_COMMAND:��ͼ->2/1�˵���Ӧ
void OnRButtonDown(UINT nFlags,POINT pt);//WM_RBUTTONDOWN��Ϣ������
void OnLButtonDown(UINT nFlags,POINT pt);//WM_LBUTTONDOWN��Ϣ������
void OnMouseMove(UINT nFlags,POINT pt);//WM_MOUSEMOVE��Ϣ������
void OnKeyDown(int nKey);//WM_KEYUP��Ϣ������
void OnSize();//WM_SIZE��Ϣ������
void OnPaint();//WM_PAINT��Ϣ������
void UpdateViewMenu();//������ͼ�˵�
void UpdateDrawMenu();//���»�ͼ�˵�
void OnToolDeal(POINT pt,int nFloor);//���ߴ���
void OnSetOpp(POINT pt,int nFloor);//��������ȡ��

//ѡ��ͼ��Ի�����Ϣ������
void InitSlectTitleBox(HWND hWnd);//��ʼ��ѡ��ͼ��Ի���
void ShowPictureContent(HWND hWnd,int nIndex);//��ʾѡ�е�rlb��Ŀ����

//��ͼ���ƺ���
void DrawMap(POINT ptDest);//��ָ��λ�û���Ϊָ��ͼ��
void DrawTitleToPage();//��ͼ��ǰ̨����
void DrawAllMap();//����������ͼ����ͼ����
void DrawMapToPage();//���Ƶ�ͼͼ��ǰ̨����
void DrawCurSel();//���Ƶ�ǰѡ����
float GetFactor();//��ȡ���ű���

//ѡͼ
RECT GetDrawRect(RECT rt);//��ȡѡͼ����
RECT GetSelRect(int nFloor);//���غ������ѡ�о���



//�������ƣ�MainWndProc
//˵������Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-14
//�޸ļ�¼��
LRESULT WINAPI MainWndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int wmID;
	int wmEvent;
	POINT pt;

	switch(uMsg)
	{
	/**************************************************************\
	*WM_CREATE:��������һЩ��ʼ���Ĺ���
	\**************************************************************/
	case WM_CREATE:
		OnCreate();
		break;
	/**************************************************************\
	*WM_COMMAND:��������Ӧ�˵�
	\**************************************************************/
	case WM_COMMAND:
		wmID=LOWORD(wParam);
		wmEvent=HIWORD(wParam);
		switch(wmID)
		{
		case ID_FILE_NEWI://�ļ�->�½�
			OnFileNew();
			break;
		case ID_FILE_OPENI://�ļ�->��
			OnFileOpen();
			break;
		case ID_FILE_SAVEI://�ļ�->����
			OnFileSave();
			break;
		case ID_FILE_OTHERSAVEI://�ļ�->���Ϊ
			OnFileOtherSave();
			break;
		case ID_FILE_EXIT://�ļ�->�˳�
			SendMessage(hWnd,WM_CLOSE,NULL,NULL);
			break;
		case ID_EDIT_RESETPOINT:
			g_AppInfo.ptDraw.x=g_AppInfo.ptDraw.y=0;
			g_AppInfo.ptTitle.x=g_AppInfo.ptTitle.y=0;
			OnPaint();
			break;
		case ID_EDIT_FILL://�༭->����ͼ
			OnFillMap();
			break;
		case ID_VIEW_1_1://��ͼ->1/1
			g_AppInfo.nView=VIEW_1_1;
			UpdateViewMenu();
			OnPaint();
			break;
		case ID_VIEW_1_2://��ͼ->1/2
			g_AppInfo.nView=VIEW_1_2;
			UpdateViewMenu();
			OnPaint();
			break;
		case ID_VIEW_2_1://��ͼ->2/1
			g_AppInfo.nView=VIEW_2_1;
			UpdateViewMenu();
			OnPaint();
			break;
		case ID_HELP_ABOUT://����->����
			DialogBox(g_hInstance,"ABOUTBOX",hWnd,(DLGPROC)AboutBoxProc);
			break;
		}
		break;
	/**************************************************************\
	*WM_LBUTTONDOWN:��������Ӧ������
	\**************************************************************/
	case WM_LBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		OnLButtonDown(wParam,pt);
		break;
	/**************************************************************\
	*WM_RBUTTONDOWN:��������Ӧ������
	\**************************************************************/
	case WM_RBUTTONDOWN:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		OnRButtonDown(wParam,pt);
		break;
	/**************************************************************\
	*WM_MOUSEMOVE:��������Ӧ������
	\**************************************************************/
	case WM_MOUSEMOVE:
		pt.x=LOWORD(lParam);
		pt.y=HIWORD(lParam);
		OnMouseMove(wParam,pt);
		break;
	/**************************************************************\
	*WM_KEYDOWN:�����ﴦ����
	\**************************************************************/
	case WM_KEYDOWN:
		OnKeyDown(wParam);
		break;
	/**************************************************************\
	*WM_PAINT:�������ػ�����
	\**************************************************************/
	case WM_PAINT:
		OnPaint();
		break;
	/**************************************************************\
	*WM_SIZE:��������д������ź�Ĵ���
	\**************************************************************/
	case WM_SIZE:
		OnSize();
		break;
	/**************************************************************\
	*WM_CLOSE:���������˳�����Ĵ���
	\**************************************************************/
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	/**************************************************************\
	*WM_DESTROY:���������˳�����Ĵ���
	\**************************************************************/
	case WM_DESTROY:
		OnDestroy();
		break;
	}

	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


//�������ƣ�AboutBoxProc
//˵������Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-16
//�޸ļ�¼��
LRESULT WINAPI AboutBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	/**************************************************************\
	*WM_COMMAND:�������ʼ���Ի���
	\**************************************************************/
	case WM_INITDIALOG:
		return true;
		break;
	/**************************************************************\
	*WM_COMMAND:��������Ӧ����
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

//�������ƣ�NewMapBoxProc
//˵������Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-16
//�޸ļ�¼��
LRESULT WINAPI NewMapBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	HWND hCWnd;//��ʱ���ƾ��
	int nRet;
	switch(uMsg)
	{
	/**************************************************************\
	*WM_COMMAND:�������ʼ���Ի���
	\**************************************************************/
	case WM_INITDIALOG:
		//=================================================
		//��ʼ���Ի���ؼ�
		//=================================================
		hNewMapWnd=hWnd;

		hCWnd=GetDlgItem(hWnd,IDC_BACKCOLOR);
		
		//����ѡ����
		SendMessage(hCWnd,CB_INSERTSTRING,0,(LPARAM)"��ɫ");
		SendMessage(hCWnd,CB_INSERTSTRING,1,(LPARAM)"��ɫ");
		//����ѡ�е�һ��
		SendMessage(hCWnd,CB_SETCURSEL,0,NULL);
		//���õ�ͼ���ȱʡֵ
		hCWnd=GetDlgItem(hWnd,IDC_W);
		SetWindowText(hCWnd,"10");
		hCWnd=GetDlgItem(hWnd,IDC_H);
		SetWindowText(hCWnd,"10");
		hCWnd=GetDlgItem(hWnd,IDC_TID);
		SetWindowText(hCWnd,"1");
		return true;
		break;
	/**************************************************************\
	*WM_COMMAND:��������Ӧ����
	\**************************************************************/
	case WM_COMMAND:
		switch(wParam)
		{
		case IDC_CT://ѡ���ͼ��
			DialogBox(g_hInstance,"CHOOSEMAPPIC",hWnd,(DLGPROC)SelectTitleBoxProc);
			break;
		case IDOK://ȷ��
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
			if(!strcmp(str,"��ɫ"))
				crBack=0xFFFFFF;//��ɫ
			else
				crBack=0x000000;//��ɫ
			if(w>99 || h>99 || w<1 || h<1)
			{
				MessageBox(hWnd,"��ͼ��С���Ϸ���","MapMake",MB_OK);
				return true;
			}
			if(nRlbID>g_Rlb.GetCount() || nRlbID<1)
			{
				MessageBox(hWnd,"��ͼ��ԴID���Ϸ���","MapMake",MB_OK);
				return true;
			}
			g_Map.nRlbID=nRlbID;
			g_Map.nMapW=w;
			g_Map.nMapH=h;
			hCWnd=GetDlgItem(hWnd,IDC_BACKCOLOR);
			nRet=SendMessage(hCWnd,CB_GETCURSEL,NULL,NULL);
			if(nRet)
				g_Map.crBack=0x000000;//������ɫ
			else
				g_Map.crBack=0xFFFFFF;//������ɫ
		case IDCANCEL://ȡ��
			EndDialog(hWnd,wParam);
			return true;
			break;
		}
		break;
	}
	
	return false;
}

//�������ƣ�SelectTitleBoxProc
//˵������Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
LRESULT WINAPI SelectTitleBoxProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	int wmID,wmEvent;
	HWND hCWnd;
	int nRet;

	switch(uMsg)
	{
	/**************************************************************\
	*WM_INITDIALOG:�������ʼ���Ի���
	\**************************************************************/
	case WM_INITDIALOG://��ʼ��ѡ��ͼ��Ի���
		InitSlectTitleBox(hWnd);
		hCWnd=GetDlgItem(hWnd,IDC_CHOOSELIST);
		SendMessage(hCWnd,LB_SETCURSEL,0,NULL);//���õ�һ����ѡ��
		SendMessage(hCWnd,WM_PAINT,NULL,NULL);
		return true;
	/**************************************************************\
	*WM_COMMAND:��������Ӧ�Ի�������
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
			nRet=SendMessage(hCWnd,LB_GETCURSEL,NULL,NULL)+1;//ͼƬ��ԴID
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
		*WM_INITDIALOG:���������ͼ��
		\**************************************************************/
		case WM_PAINT:
			hCWnd=GetDlgItem(hWnd,IDC_CHOOSELIST);
			nRet=SendMessage(hCWnd,LB_GETCURSEL,NULL,NULL);
			ShowPictureContent(hWnd,nRet+1);
	}
	return false;
}

//�������ƣ�ShowPictureContent
//˵������ʾѡ�е�rlb��Ŀ����
//���룺hWnd�����ھ����Ҫ��ʾ����Ŀ����
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
void ShowPictureContent(HWND hWnd,int nIndex)
{
	HWND hCWnd;
	HDC hDestDC;
	HDC hDC;
	BITMAPINFO BmpInfo;
	long nValAddress;//ͼ�����ݿ�ʼλ��
	void *pvBits;//ͼ������
	HBITMAP hBmp;
	RECT rt;//��������

	hCWnd=GetDlgItem(hWnd,IDC_DRAWPAGE);
	hDestDC=GetWindowDC(hCWnd);
	GetClientRect(hCWnd,&rt);

	nValAddress=(long)*(g_Rlb.GetContent(nIndex)+10);

	hDC=CreateCompatibleDC(NULL);//��������Ļ���ݵ�DC

	ZeroMemory(&BmpInfo,sizeof(BITMAPINFO));//��ʼ��λͼ��Ϣ

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

//�������ƣ�InitSlectTitleBox
//˵������ʼ��ѡ��ͼ��Ի���
//���룺hWnd���Ի��򴰿ھ��
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
void InitSlectTitleBox(HWND hWnd)
{
	HWND hCWnd;//��ʱ���ھ��

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

//�������ƣ�OnCreate
//˵����WM_CREATE��Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-16
//�޸ļ�¼��
void OnCreate()
{
	
	int w,h;

	//��ȡ��Ļ��С
	w=GetSystemMetrics(SM_CXSCREEN);
	h=GetSystemMetrics(SM_CYSCREEN);

	//��������Ļ���ݵ�DC
	g_hPageDC=CreateCompatibleDC(NULL);//����ҳ��
	g_hTitleDC=CreateCompatibleDC(NULL);//ͼ��DC
	g_hDrawDC=CreateCompatibleDC(NULL);//��ͼDC
}

//�������ƣ�OnPaint
//˵����WM_PAINT��Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-16
//�޸ļ�¼��
void OnPaint()
{
	HDC hWndDC;//ǰ̨DC
	RECT crt;//�ͻ�����С
	RECT rt;//��ʱ�þ�������

	GetClientRect(g_hMainWnd,&crt);//��ȡ�ͻ�����С

	hWndDC=GetDC(g_hMainWnd);//ǰ̨DC

	rt=crt;

	rt.right=DIVSTRING_X;

	FillRect(g_hPageDC,&rt,(HBRUSH)COLOR_BACKGROUND);//���Ʊ���

	rt.left=DIVSTRING_X;
	rt.top=0;
	rt.right=DIVSTRING_X+5;
	rt.bottom=crt.bottom;

	DrawTitleToPage();//����ͼ��ǰ̨����

	FillRect(g_hPageDC,&rt,(HBRUSH)COLOR_BTNSHADOW);//���Ʒָ���

	rt=crt;
	rt.left=DIVSTRING_X+5;
	FillRect(g_hPageDC,&rt,(HBRUSH)COLOR_BACKGROUND);//���Ʊ���

	//���Ƶ�ǰ������

	DrawMapToPage();

	if(g_AppInfo.bInDraw)
		DrawCurSel();

	BitBlt(hWndDC,0,0,crt.right-crt.left,crt.bottom-crt.top,g_hPageDC,0,0,SRCCOPY);
}

//�������ƣ�OnFileNew
//˵����WM_COMMMAND:ID_FILE_NEWI�˵�����Ϣ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-16
//�޸ļ�¼��
void OnFileNew()
{
	BITMAPINFO BmpInfo;
	long nValAddress;//ͼ�����ݿ�ʼλ��
	void *pvBits;//ͼ������
	int nRet;
	
	if(g_bCreate)
	{
		nRet=MessageBox(g_hMainWnd,"�Ƿ��ȱ������½���","MapMake",MB_YESNOCANCEL);

		if(nRet==IDOK)
		{
		}
		else if(nRet=IDCANCEL)
		{
			return;
		}
	}
	
	//��ʾ�½��Ի���
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
		//����ͼ����Դ
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

		memset(&g_AppInfo,0,sizeof(AppInfo));//��ʼ����������

		g_AppInfo.rtSel[0].right=1;//�ײ�ѡ��
		g_AppInfo.rtSel[0].bottom=1;

		g_AppInfo.rtSel[1].right=1;//�ϲ�ѡ��
		g_AppInfo.rtSel[1].bottom=1;

		
		//��ʼ����ͼ
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

		g_bCreate=true;//���õ�ͼ�ѽ���
		UpdateViewMenu();
	}
	OnPaint();//����ͼ��ǰ̨����
}

//�������ƣ�OnKeyDown
//˵����WM_KEYDOWN��Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-19
//�޸ļ�¼��
void OnKeyDown(int nKey)
{
	if(nKey==KEY_T_UP)//��
	{
		g_AppInfo.ptTitle.y-=1;
	}
	else if(nKey==KEY_T_LEFT)//��
	{
		g_AppInfo.ptTitle.x-=1;
	}
	else if(nKey==KEY_T_RIGHT)//��
	{
		g_AppInfo.ptTitle.x+=1;
	}
	else if(nKey==KEY_T_DOWN)//��
	{
		g_AppInfo.ptTitle.y+=1;
	}
	else if(nKey==VK_UP)//��
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

//�������ƣ�OnLRuttonDown
//˵����WM_RBUTTONDOWN:��Ӧ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-21
//�޸ļ�¼��
void OnRButtonDown(UINT nFlags,POINT pt)
{
	int x,y;//������
	float Factor;

	Factor=GetFactor();//��ȡ���ű���

	if(nFlags==MK_RBUTTON)
	{
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
			&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
			&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
			&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//�����������ͼ����
		{
			x=pt.x/TITLE_W-g_AppInfo.ptTitle.x;
			y=pt.y/TITLE_H-g_AppInfo.ptTitle.y;
			g_AppInfo.rtSel[0].left=x;
			g_AppInfo.rtSel[0].top=y;
			g_AppInfo.rtSel[0].right=x+1;
			g_AppInfo.rtSel[0].bottom=y+1;
			
			OnPaint();	
		}
		else//����ڻ�ͼ��
		{
			x=(pt.x-DIVSTRING_X-5)/TITLE_W/Factor;
			y=pt.y/TITLE_H/Factor;
			
			if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;
				
				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,0);//�ϲ��ͼ
				
				g_AppInfo.bInDraw=true;
				OnPaint();
			}
			else//��겻�ڻ��Ƶ�ͼ��
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
			&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
		{
			POINT pt;
			char str[32];
			
			sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
			SetWindowText(g_hMainWnd,str);
			g_AppInfo.ptDrawSel.x=x;
			g_AppInfo.ptDrawSel.y=y;
			
			pt.x=x-g_AppInfo.ptDraw.x;
			pt.y=y-g_AppInfo.ptDraw.y;
			OnSetOpp(pt,0);//�ϲ��ͼ
			
			g_AppInfo.bInDraw=true;
			OnPaint();
		}
		else//��겻�ڻ��Ƶ�ͼ��
		{
			g_AppInfo.bInDraw=false;
			OnPaint();
		}
	}

}


//�������ƣ�OnLButtonDown
//˵����WM_LBUTTONDOWN:��Ӧ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-20
//�޸ļ�¼��
void OnLButtonDown(UINT nFlags,POINT pt)
{
	int x,y;//������
	float Factor;//ϵ��

	Factor=GetFactor();

	if(nFlags==MK_LBUTTON)//�����ѡͼ��
	{
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
					&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
					&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
					&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//�����������ͼ����
		{
			x=pt.x/TITLE_W-g_AppInfo.ptTitle.x;
			y=pt.y/TITLE_H-g_AppInfo.ptTitle.y;
			g_AppInfo.rtSel[1].left=x;
			g_AppInfo.rtSel[1].top=y;
			g_AppInfo.rtSel[1].right=x+1;
			g_AppInfo.rtSel[1].bottom=y+1;

			OnPaint();
		}
		else//����ڻ�ͼ��
		{
			x=(pt.x-DIVSTRING_X-5)/TITLE_W/Factor;
			y=pt.y/TITLE_H/Factor;
			
			if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;

				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,1);//�ϲ��ͼ

				g_AppInfo.bInDraw=true;
				OnPaint();
			}
			else//��겻�ڻ��Ƶ�ͼ��
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
			&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
		{
			POINT pt;
			char str[32];
			
			sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
			SetWindowText(g_hMainWnd,str);
			g_AppInfo.ptDrawSel.x=x;
			g_AppInfo.ptDrawSel.y=y;
			
			pt.x=x-g_AppInfo.ptDraw.x;
			pt.y=y-g_AppInfo.ptDraw.y;
			OnSetOpp(pt,1);//�ϲ��ͼ
			
			g_AppInfo.bInDraw=true;
			OnPaint();
		}
		else//��겻�ڻ��Ƶ�ͼ��
		{
			g_AppInfo.bInDraw=false;
			OnPaint();
		}
	}
}

//�������ƣ�OnMouseMove
//˵����WM_MOUSEMOVE��Ϣ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-19
//�޸ļ�¼��
void OnMouseMove(UINT nFlags,POINT pt)
{
	static bool bLeft[2]={true,true};//ѡͼ�������϶���
	static bool bTop[2]={true,true};
	int x,y;//������
	int nFloor;
	float Factor;


	Factor=GetFactor();

	if(nFlags==MK_RBUTTON)
	{
		nFloor=0;//�ײ��ͼ
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
					&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
					&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
					&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//�����������ͼ����
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
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;

				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,0);//�ϲ��ͼ

				g_AppInfo.bInDraw=true;
				OnPaint();
			}
		}
	}
	else if(nFlags==MK_LBUTTON)
	{
		nFloor=1;//�ϲ��ͼ
		if(pt.x<DIVSTRING_X && pt.x/TITLE_W>=g_AppInfo.ptTitle.x
					&& pt.x/TITLE_W<g_AppInfo.ptTitle.x+g_Map.LineX
					&& pt.y/TITLE_H>=g_AppInfo.ptTitle.y
					&& pt.y/TITLE_W<g_AppInfo.ptTitle.y+g_Map.TitleH/TITLE_H)//�����������ͼ����
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
				&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
			{
				POINT pt;
				char str[32];
				
				sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
				SetWindowText(g_hMainWnd,str);
				g_AppInfo.ptDrawSel.x=x;
				g_AppInfo.ptDrawSel.y=y;

				pt.x=x-g_AppInfo.ptDraw.x;
				pt.y=y-g_AppInfo.ptDraw.y;
				OnToolDeal(pt,1);//�ϲ��ͼ

				g_AppInfo.bInDraw=true;
				OnPaint();
			}
		}
	}
	else if(nFlags==NULL && pt.x>DIVSTRING_X+5)//����ڻ�ͼ��
	{
		x=(pt.x-DIVSTRING_X-5)/(TITLE_W*Factor);
		y=pt.y/(TITLE_H*Factor);
		
		if(x>=g_AppInfo.ptDraw.x && x<g_AppInfo.ptDraw.x+g_Map.nMapW
			&& y>=g_AppInfo.ptDraw.y && y<g_AppInfo.ptDraw.y+g_Map.nMapH)//����ڻ��Ƶ�ͼ��
		{
			char str[32];

			sprintf(str,"MapMake (%d,%d)",x-g_AppInfo.ptDraw.x,y-g_AppInfo.ptDraw.y);
			SetWindowText(g_hMainWnd,str);
			g_AppInfo.ptDrawSel.x=x;
			g_AppInfo.ptDrawSel.y=y;
			g_AppInfo.bInDraw=true;
			OnPaint();
		}
		else//��겻�ڻ��Ƶ�ͼ��
		{
			g_AppInfo.bInDraw=false;
			OnPaint();
		}
	}
	else//��겻�ڻ��Ƶ�ͼ��
	{
		g_AppInfo.bInDraw=false;
		OnPaint();
	}
}


//�������ƣ�OnSize
//˵����WM_SIZE:���ź�Ĵ���
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-19
//�޸ļ�¼��
void OnSize()
{
	RECT rt;

	GetClientRect(g_hMainWnd,&rt);//��ȡ�����ڿͻ�����С
	
	if(g_hPageBitmap)
	{
		DeleteObject(g_hPageBitmap);
	}
	g_hPageBitmap=CreateCompatibleBitmap(GetDC(NULL),rt.right-rt.left,rt.bottom-rt.top);
	SelectObject(g_hPageDC,g_hPageBitmap);
}

//�������ƣ�OnDestroy
//˵���������˳�����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-19
//�޸ļ�¼��
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

//�������ƣ�DrawMap
//˵������ָ��λ�û���Ϊָ��ͼ��
//���룺ptDest:Ŀ��λ�ã�ptSource:Դλ��
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
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
		//���Ƶײ��ͼ
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

//�������ƣ�DrawTitleToPage
//˵�������Ƶ�ǰ̨���崰��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
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
	
	//����ѡͼ��
	x=g_AppInfo.ptTitle.x*TITLE_W;
	y=g_AppInfo.ptTitle.y*TITLE_H;
	w=g_Map.TitleW;
	h=g_Map.TitleH;
	if(w+x>DIVSTRING_X)//Ҫ��ʾ�Ŀ�ȳ����ָ���
	{
		w=DIVSTRING_X-x;
	}
	BitBlt(hDC,x,y,w,h,g_hTitleDC,0,0,SRCCOPY);
	//����ѡͼ�����
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
	
	//ɾ����ˢ
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
	
	
	SelectObject(hDC,htBr);//��ԭDC
	SelectObject(hDC,htPen);
	DeleteObject(hBr);
}


//�������ƣ�DrawAllMap()
//˵��������������ͼ����ͼ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
void DrawAllMap()
{
	int x,y;
	POINT ptDest;
	//�����ϲ��ͼ
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

//�������ƣ�DrawMapToPage()
//˵�������Ƶ�ͼ��ǰ̨����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-18
//�޸ļ�¼��
void DrawMapToPage()
{
	int sx,sy;
	int x,y;
	int w,h;
	int sw,sh;
	float Factor;//ϵ��

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

	//��������
	if(x<DIVSTRING_X+5)
	{
		w=w-(DIVSTRING_X+5-x);
		sx=(DIVSTRING_X+5-x)/Factor;
		sw-=sx;
		x=DIVSTRING_X+5;
		if(w<0) w=0;
	}
	

	StretchBlt(g_hPageDC,x,y,w,h,
				g_hDrawDC,sx,sy,sw,sh,SRCCOPY);//���Ƶ�ͼͼ��ǰ̨����
}

//�������ƣ�GetDrawRect
//˵����ѡ�о�������ƾ��ε�ת��
//���룺rt:ѡ�о���
//�����
//���أ�
//��ע��
//���ڣ�2011-8-21
//�޸ļ�¼��
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

//�������ƣ�GetDrawRect
//˵�������غ������ѡ�о��Σ�left<right,top<bottomΪ������
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-21
//�޸ļ�¼��
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

//�������ƣ�DrawCurSel()
//˵�������Ƶ�ǰѡ����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-21
//�޸ļ�¼��
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
	
	
	lbr.lbStyle=BS_HOLLOW;//���Ļ�ˢ
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
	SelectObject(g_hPageDC,htPen);//��ԭDC
	SelectObject(g_hPageDC,htBr);
	
}


//�������ƣ�GetFactor()
//˵������ȡ���ű���
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-21
//�޸ļ�¼��
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

//�������ƣ�UpdateViewMenu
//˵����������ͼ�˵�
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-21
//�޸ļ�¼��
void UpdateViewMenu()
{
	int nRet;
	HMENU hMenu;

	hMenu=GetMenu(g_hMainWnd);//��ȡ�˵�

	if(g_AppInfo.nView==VIEW_1_1)
	{
		nRet=GetMenuState(hMenu,ID_VIEW_1_1,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)//δ���
		{
			CheckMenuItem(hMenu,ID_VIEW_1_1,MF_CHECKED);
			//���������
			CheckMenuItem(hMenu,ID_VIEW_1_2,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_VIEW_2_1,MF_UNCHECKED);
		}
		g_AppInfo.nView=VIEW_1_1;
	}
	else if(g_AppInfo.nView==VIEW_1_2)
	{
		nRet=GetMenuState(hMenu,ID_VIEW_1_2,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)//δ���
		{
			CheckMenuItem(hMenu,ID_VIEW_1_2,MF_CHECKED);
			//���������
			CheckMenuItem(hMenu,ID_VIEW_1_1,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_VIEW_2_1,MF_UNCHECKED);
		}
		g_AppInfo.nView=VIEW_1_2;
	}
	else if(g_AppInfo.nView==VIEW_2_1)
	{
		nRet=GetMenuState(hMenu,ID_VIEW_2_1,MF_BYCOMMAND);
		if(nRet==MF_UNCHECKED)//δ���
		{
			CheckMenuItem(hMenu,ID_VIEW_2_1,MF_CHECKED);
			//���������
			CheckMenuItem(hMenu,ID_VIEW_1_1,MF_UNCHECKED);
			CheckMenuItem(hMenu,ID_VIEW_1_2,MF_UNCHECKED);
		}
		g_AppInfo.nView=VIEW_2_1;
	}

	OnPaint();
}


//�������ƣ�UpdateDrawMenu
//˵�������»�ͼ�˵�
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
/*void UpdateDrawMenu()
{
	int nRet;
	HMENU hMenu;

	hMenu=GetMenu(g_hMainWnd);

	if(g_AppInfo.nTool==TOOL_PEN)//Ǧ��
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
	else if(g_AppInfo.nTool==TOOL_RECT)//�ı���
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
	else if(g_AppInfo.nTool==TOOL_ELLIPSE)//��Բ
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
	else if(g_AppInfo.nTool==TOOL_FILL)//���
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
	else if(g_AppInfo.nTool==TOOL_SELECT)//ѡ��
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


//�������ƣ�OnToolDeal
//˵�������ߴ���
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
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

//�������ƣ�OnSetOpp
//˵�������õ�ͼ����ȡ��
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
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
			g_Map.Map[nFloor][x][y]*=-1;//��������ȡ��
			ptDest.x=x;
			ptDest.y=y;
			DrawMap(ptDest);
		}
	}
}


//�������ƣ�OnFileSave
//˵�����ļ�����
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
void OnFileSave()
{
	char strFileName[MAX_PATH];

	if(!g_bCreate)
	{
		MessageBox(g_hMainWnd,"�����½���ͼ��","MapMake",MB_OK);
		return;
	}

	if(!strcmp(g_strFileName.c_str(),""))
	{
		OPENFILENAME op;//����Ի�����Ϣ

		ZeroMemory(&op,sizeof(OPENFILENAME));

		op.lpstrInitialDir=g_strMapDirName.c_str();
		op.lpstrDefExt=".Map";
		op.lpstrFilter="��ͼ�ļ�(*.map)\0*.map\0";
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
	
	//�����ļ�
	FILE *fp;
	int tInt;

	fp=fopen(g_strFileName.c_str(),"wb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"�����ļ�ʧ�ܣ�","MapMake",MB_OK);
		return;
	}

	fwrite(&g_Map.nRlbID,sizeof(int),1,fp);//д��rlb��ԴID
	if(g_Map.crBack==0x000000)//��ɫ
		tInt=1;
	else
		tInt=0;

	fwrite(&tInt,sizeof(int),1,fp);//д�뱳��
	//д���ͼ��С
	fwrite(&g_Map.nMapW,sizeof(int),1,fp);
	fwrite(&g_Map.nMapH,sizeof(int),1,fp);

	//д���ͼ
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

	MessageBox(g_hMainWnd,"����ɹ���","MapMake",MB_OK);
}

//�������ƣ�OnFileOpen
//˵�������ļ�
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
void OnFileOpen()
{
	int nRet;

	if(g_bCreate)
	{
		nRet=MessageBox(g_hMainWnd,"�Ƿ񱣴��ٴ򿪣�","MapMake",MB_YESNOCANCEL);
		if(nRet==IDYES)
			OnFileSave();
		else if(nRet==IDCANCEL)
			return;
	}

	char strFileName[MAX_PATH];

	OPENFILENAME op;//����Ի�����Ϣ
	
	ZeroMemory(&op,sizeof(OPENFILENAME));
	
	op.lpstrInitialDir=g_strMapDirName.c_str();
	op.lpstrDefExt=".Map";
	op.lpstrFilter="��ͼ�ļ�(*.map)\0*.map\0";
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
		MessageBox(g_hMainWnd,"���ļ�ʧ�ܣ�","MapMake",MB_OK);
	}

	fread(&g_Map.nRlbID,sizeof(int),1,fp);//д��rlb��ԴID
	
	fread(&tInt,sizeof(int),1,fp);//д�뱳��

	if(tInt==1)//��ɫ
		g_Map.crBack=0x000000;
	else
		g_Map.crBack=0xFFFFFF;
	//д���ͼ��С
	fread(&g_Map.nMapW,sizeof(int),1,fp);
	fread(&g_Map.nMapH,sizeof(int),1,fp);

	//д���ͼ
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

	//����ͼ����Դ
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
	
	memset(&g_AppInfo,0,sizeof(AppInfo));//��ʼ����������
	
	g_AppInfo.rtSel[0].right=1;//�ײ�ѡ��
	g_AppInfo.rtSel[0].bottom=1;
	
	g_AppInfo.rtSel[1].right=1;//�ϲ�ѡ��
	g_AppInfo.rtSel[1].bottom=1;
	
	g_bCreate=true;
	//��ʼ����ͼ

	DrawAllMap();
	UpdateViewMenu();
	
	OnPaint();//����ͼ��ǰ̨����
}

//�������ƣ�OnFillMap
//˵��������ͼ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
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


//�������ƣ�OnFileOtherSave
//˵��������ͼ
//���룺
//�����
//���أ�
//��ע��
//���ڣ�2011-8-22
//�޸ļ�¼��
void OnFileOtherSave()
{
	char strFileName[MAX_PATH];
	OPENFILENAME op;//����Ի�����Ϣ

	if(!g_bCreate)
	{
		MessageBox(g_hMainWnd,"�����½���ͼ��","MapMake",MB_OK);
		return;
	}
	
	ZeroMemory(&op,sizeof(OPENFILENAME));
	
	op.lpstrInitialDir=g_strMapDirName.c_str();
	op.lpstrDefExt=".Map";
	op.lpstrFilter="��ͼ�ļ�(*.map)\0*.map\0";
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
	
	//�����ļ�
	FILE *fp;
	int tInt;

	fp=fopen(g_strFileName.c_str(),"wb");

	if(!fp)
	{
		MessageBox(g_hMainWnd,"�����ļ�ʧ�ܣ�","MapMake",MB_OK);
		return;
	}

	fwrite(&g_Map.nRlbID,sizeof(int),1,fp);//д��rlb��ԴID
	if(g_Map.crBack==0x000000)//��ɫ
		tInt=1;
	else
		tInt=0;

	fwrite(&tInt,sizeof(int),1,fp);//д�뱳��
	//д���ͼ��С
	fwrite(&g_Map.nMapW,sizeof(int),1,fp);
	fwrite(&g_Map.nMapH,sizeof(int),1,fp);

	//д���ͼ
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

	MessageBox(g_hMainWnd,"����ɹ���","MapMake",MB_OK);
}