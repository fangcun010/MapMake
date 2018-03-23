//���ڴ�С
#define WND_W			700
#define WND_H			500
//��ͼ����
#define MAP_FLOOR_COUNT	2
//��ͼ���ߴ�
#define MAX_MAP_W		99
#define MAX_MAP_H		99

//��ͼ��ߴ�
#define TITLE_W			16
#define TITLE_H			16

//������ͼ
#define VIEW_1_1		0
#define VIEW_1_2		1
#define VIEW_2_1		2

//��ͼ����
//Ǧ��
#define TOOL_PEN		0
//�ı���
#define TOOL_RECT		1
//��Բ
#define TOOL_ELLIPSE	2
//���
#define TOOL_FILL		3
//ѡ��
#define TOOL_SELECT		4

//�ָ���λ��
#define DIVSTRING_X		240

//����
#define KEY_T_UP		'W'
#define KEY_T_DOWN		'S'
#define KEY_T_LEFT		'A'
#define KEY_T_RIGHT		'D'

//ѡͼ������ɫ
//�ϲ��ͼ
#define CR_UP			0x00FF00
//�ײ��ͼ
#define CR_DOWN			0xFF0000
//�ϲ㸳ֵ����
#define CR_LOAD			0xFFF0F0

//��ͼ��͸��ɫ
#define TRANSPARENT_COLOR 0XFF00FF

extern HINSTANCE g_hInstance;//����ʵ�����
extern HWND g_hMainWnd;//�����ھ��
extern std::string g_strProjectName;//��������
extern std::string g_strMapDirName;//�ļ�·��
extern std::string g_strMapRlb;//Rlb�ļ�����
extern std::string g_strFileName;//�ļ���

extern HDC g_hTitleDC;//ͼ��DC
extern HDC g_hDrawDC;//��ͼDC
extern HDC g_hPageDC;//����ҳ��DC
extern HBITMAP g_hPageBitmap;//����ҳ��λͼ
extern HBITMAP g_hTitleBitmap;//ͼ��λͼ
extern HBITMAP g_hDrawBitmap;//��ͼ����λͼ

extern bool g_bCreate;//�Ƿ��ѽ�����ͼ

extern SRlb g_Rlb;//��ͼ����Դ

extern struct MapData
{
	int nMapW,nMapH;//��ͼ��С
	int nRlbID;
	int Map[MAP_FLOOR_COUNT][MAX_MAP_W][MAX_MAP_H];//��ͼ����
	COLORREF crBack;
	long TitleW,TitleH;//ͼ��ߴ�
	long LineX;//һ��ͼ����
} g_Map;

extern struct AppInfo
{
	POINT ptTitle;//ͼ����ʾλ��
	POINT ptDraw;//��ͼ��ʾλ��
	RECT rtSel[2];//ѡ��ͼ��λ��
	POINT ptDrawSel;//��ǰ����ѡ����
	int nView;//��ͼ��ʾ
	int nTool;//���߱�ʾ
	bool bInDraw;//���λ�ñ�ʾ
} g_AppInfo;

//�˵�ID
