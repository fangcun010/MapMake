//窗口大小
#define WND_W			700
#define WND_H			500
//地图层数
#define MAP_FLOOR_COUNT	2
//地图最大尺寸
#define MAX_MAP_W		99
#define MAX_MAP_H		99

//地图块尺寸
#define TITLE_W			16
#define TITLE_H			16

//程序视图
#define VIEW_1_1		0
#define VIEW_1_2		1
#define VIEW_2_1		2

//绘图工具
//铅笔
#define TOOL_PEN		0
//四边形
#define TOOL_RECT		1
//椭圆
#define TOOL_ELLIPSE	2
//填充
#define TOOL_FILL		3
//选择
#define TOOL_SELECT		4

//分割线位置
#define DIVSTRING_X		240

//按键
#define KEY_T_UP		'W'
#define KEY_T_DOWN		'S'
#define KEY_T_LEFT		'A'
#define KEY_T_RIGHT		'D'

//选图矩形颜色
//上层地图
#define CR_UP			0x00FF00
//底层地图
#define CR_DOWN			0xFF0000
//上层赋值矩形
#define CR_LOAD			0xFFF0F0

//地图块透明色
#define TRANSPARENT_COLOR 0XFF00FF

extern HINSTANCE g_hInstance;//程序实例句柄
extern HWND g_hMainWnd;//主窗口句柄
extern std::string g_strProjectName;//工程名称
extern std::string g_strMapDirName;//文件路径
extern std::string g_strMapRlb;//Rlb文件名称
extern std::string g_strFileName;//文件名

extern HDC g_hTitleDC;//图块DC
extern HDC g_hDrawDC;//绘图DC
extern HDC g_hPageDC;//缓冲页面DC
extern HBITMAP g_hPageBitmap;//缓冲页面位图
extern HBITMAP g_hTitleBitmap;//图块位图
extern HBITMAP g_hDrawBitmap;//地图绘制位图

extern bool g_bCreate;//是否已建立地图

extern SRlb g_Rlb;//地图块资源

extern struct MapData
{
	int nMapW,nMapH;//地图大小
	int nRlbID;
	int Map[MAP_FLOOR_COUNT][MAX_MAP_W][MAX_MAP_H];//地图数据
	COLORREF crBack;
	long TitleW,TitleH;//图块尺寸
	long LineX;//一行图块数
} g_Map;

extern struct AppInfo
{
	POINT ptTitle;//图块显示位置
	POINT ptDraw;//绘图显示位置
	RECT rtSel[2];//选中图块位置
	POINT ptDrawSel;//当前绘制选中区
	int nView;//视图标示
	int nTool;//工具标示
	bool bInDraw;//鼠标位置标示
} g_AppInfo;

//菜单ID
