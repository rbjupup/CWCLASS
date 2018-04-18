#if !defined(AFX_IMAGESTATIC_H__95842368_DB80_49AF_A58B_749F5A584461__INCLUDED_)
#define AFX_IMAGESTATIC_H__95842368_DB80_49AF_A58B_749F5A584461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageStatic.h : header file
//
#define STATIC_CV
#define USE_OPENGL 1

#include "def.h"
#include <afxmt.h>
#ifdef STATIC_CV
#include "cv.h"
#endif
#if USE_OPENGL == 1
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#ifdef _WIN64
#define GLUT_DISABLE_ATEXIT_HACK
#define GLUT_BUILDING_LIB 
#pragma comment(lib,"opengl32.lib")  
#pragma comment(lib,"glu32.lib")  
#pragma comment (lib, "glut64.lib")   
#pragma comment(lib,"glew32.lib")
#else
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")
#pragma comment(lib,"glaux.lib")
#endif

#endif
#include <math.h>
#include "VectorOperator.h"
#include "XFunCom.h"
class C3FloatPt 
{
public:
	double x, y ,z;

public:
	C3FloatPt(                    ) {x =         0.0F; y =         0.0F;z =         0.0F;};
	C3FloatPt(double x1, double y1, double z1) {x =           x1; y =           y1;z =           z1;};
	C3FloatPt(const C3FloatPt &pt  ) {x =         pt.x; y =         pt.y;z =         pt.z;};
	C3FloatPt(const CPoint &pt    ) {x = (double)pt.x; y = (double)pt.y;z=0.0;};
	C3FloatPt(const POINT & pt    ) {x = (double)pt.x; y = (double)pt.y;z=0.0;};
	C3FloatPt(const CFloatPt & pt    ) {x = (double)pt.x; y = (double)pt.y;z=0.0;};
	C3FloatPt* operator= (C3FloatPt*      pPt)
	{x =       pPt->x; y =       pPt->y;  z =       pPt->z;return  this;};
	C3FloatPt& operator= (const C3FloatPt& pt)
	{x =         pt.x; y =         pt.y;  z =         pt.z;return *this;};
	C3FloatPt& operator= (const CPoint&   pt)
	{x = (double)pt.x; y = (double)pt.y;z = 0.0; return *this;};
	C3FloatPt& operator= (const POINT&    pt)
	{x = (double)pt.x; y = (double)pt.y; z = 0.0;return *this;};
	C3FloatPt& operator- (C3FloatPt& pt)
	{x -= pt.x; y -= pt.y; z -= pt.z;return *this;};

	double Distance2Point( C3FloatPt p = C3FloatPt(0,0,0)) 
	{
		return sqrt( (x-p.x)* (x-p.x) + (y-p.y)* (y-p.y)+ (z-p.z)* (z-p.z) );
	} 
	double Distance2Point(double fX, double fY, double fZ)
	{
		return sqrt( (x-fX)* (x-fX) + (y-fY)* (y-fY) + (y-fZ)* (y-fZ));
	}
};

//#pragma comment(lib,"COpenGL.lib")

/////////////////////////////////////////////////////////////////////////////
// CImageStatic window
#define RULER_GAP 8
#define RULER_5 16
#define RULER_10 24

#define SUKER_SIZE 12
#define CHIP_SIZE 4
#define CameraNum 4
#include  <vector>
using namespace std;

class CImageStatic;
struct MovePara{
	CRect m_ShowRC;
	BOOL m_bLbtnDown;
	CPoint m_stPt;
	CPoint m_endPt;
	BOOL m_bDrawingDeteRC;
	CImageStatic* m_show;
	MovePara(){
		m_bDrawingDeteRC = FALSE;
		m_bLbtnDown = FALSE;
		m_show = NULL;
	}
	void MyLButtonDown(UINT nFlags,CWnd* papa, CPoint point);
	void MyMouseMove(UINT nFlags, CWnd* papa , CPoint point);
	void MyLButtonUp(UINT nFlags, CWnd* papa , CPoint point);
	BOOL MyMouseWheel(UINT nFlags, CWnd* papa , short zDelta, CPoint pt);
	void MyInit(CWnd* papa);
};
struct DRAWRECT//绘区域结构体
{
	CRect m_Rect;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
	DRAWRECT(){
		m_Rect = CRect(0,0,0,0);
		m_Color = RGB(0,0,0);
		nWidth = 0;
		nPenStyle = 0;
	}
};
struct DRAWLINE//绘直线结构体
{
	C3FloatPt Pt1;
	C3FloatPt Pt2;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
	BOOL IsShow ;
	DRAWLINE(){
		Pt1 = C3FloatPt(0,0,0);
		Pt2 = C3FloatPt(0,0,0);
		m_Color = RGB(0,0,0);
		nWidth = 0;
		nPenStyle = 0;
		IsShow = TRUE;
	}
};
struct DRAWCircle//绘圆结构体
{
	int m_CircleCX;
	int m_CircleCY;
	int m_CircleR;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
	DRAWCircle(){
		m_CircleCX = 0;
		m_CircleCY = 0;
		m_CircleR = 0;
		m_Color = RGB(0,0,0);
		nWidth = 0;
		nPenStyle = 0;
	}
};
struct DRAWFont//绘文字结构体
{
	CPoint Pt;
	CString strText ;
	COLORREF crColor;
	int lfHeight;
	CString strFont; 
	int nAngle;
	int nRadius ;
};
struct DRAWAngleArc//绘圆弧结构体
{
	C3FloatPt Pt1;
	C3FloatPt Pt2;
	C3FloatPt Pt3;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
};

typedef struct tagCAMERA{
	VECTOR vEye;
	VECTOR vCenter;
	VECTOR vUp;
}CAMERA;
struct GL3DPara
{

	cv::Mat m_pic3D;		//小数点图像
	cv::Mat m_pic;			//没有小数点的图像
	double m_valmax,m_valmin;	//转灰度图的时候需要知道最大最小值
	double co1,co0;	//最大最小值转换成深度的计算公式
	//gray =（val - min）*（255/（max - min）） = val *255/（max - min） - min*255 /（max - min）
	//     = val*co1+co0

	CWnd *m_father;			//父亲的指针
	CWnd *m_Grandfather;	//爷爷的指针
	int m_viewx,m_viewy;	//显示大小
	CAMERA	m_camera;		//视角
	CAMERA	m_cameraOld;
	double   m_RotX, m_RotY, m_RotZ;				//旋转
	double   m_oldRotX, m_oldRotY, m_oldRotZ;
	CClientDC	*m_pDC;
	bool m_bDown;			// = true;左键是否按下
	CPoint m_pointDown;		//左键按下的位置= point;
	float	m_theta,m_thetaOld;		//=m_theta;
	HCURSOR			hHandWantGrab;
	BOOL ColorRG0;		//颜色阶梯,有2和6两种模式,若该值为真,使用RG模式
	CStatic *m_cs;//显示条条
	CRect papaRect;		//opengl绘制位置
	CRect papaSRect;		//opengl绘制位置
	GL3DPara()
	{
		co1 = co0  = -1;
		m_cs = NULL;
		ColorRG0 = FALSE;
		m_father = NULL;
		m_pDC = NULL;
		m_bDown = FALSE;
		m_theta = m_thetaOld = 0;
		m_viewx = m_viewy = 0;
		m_camera.vCenter.x = 0;
		m_camera.vCenter.y = 0;
		m_camera.vCenter.z = 0;

		m_camera.vEye.x = 0;
		m_camera.vEye.y = 0;
		m_camera.vEye.z = 400;

		m_camera.vUp.x = 0;
		m_camera.vUp.y = 1;
		m_camera.vUp.z = 0;

		m_RotX = 0;
		m_RotY = 0;
		m_RotZ = 0;
		m_oldRotX = m_oldRotY = m_oldRotZ = 0;
		m_valmax = 2.0;
		m_valmin = 0.0;
	}
	//初始化
	void Init(CWnd *papa);
	//使用窗体初始化控件
	void InitGrandPa(CWnd *grandpa);
	//释放资源
	void Release();
	//绘制图像
	void DrawImage();
	//重新设置窗口的位置
	void Resize();
	void ResizeBack();
	//设置OPenGL绘制区域
	void OnSize(int X,int Y);
	//左键按下
	void OnLButtonDown(UINT nFlags, CPoint point);
	//鼠标移动
	void OnMouseMove(UINT nFlags, CPoint point); 
	//左键弹起
	void OnLButtonUp(UINT nFlags, CPoint point);
	//滚轮滚动
	void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) ;
	//绘制旁边显示用的表示深度的颜色条
	void DrawBar();
	//设置打光之类的参数
	void LM();	
	//绘制坐标轴
	void DrawCoordinate();
	//设置像素的格式
	BOOL bSetupPixelFormat();
	//放大缩小
	void ZoomIn(int abs);
	//绘制图像
	void DrawObj();
	//获取颜色
	void GetColor(double r, double &red, double &green, double &blue);
	//获取最大最小值,用来做最后的调色
	void GetMinMax();
	//绘制分块颜色
	void DrawColorBlock(CDC* pDC,CRect r, COLORREF cr1, COLORREF cr2);
	//设置Co
	void SetCo(double co1,double co0);

};
class CImageStatic : public CStatic
{
public:
	CImageStatic();
	virtual ~CImageStatic();
	afx_msg void OnPaint();
	void GetMyHdc();
protected:
	DECLARE_MESSAGE_MAP()
public:

/************************************************************************/
/*								控制选项开始	                        */
/************************************************************************/

	BOOL m_bCanOutImage;		//假如可以在图像外面设置ROI会有黑边填充ROI之外的地方
	BOOL m_bChangeScalar;		//是否可以改变当前的比例
	BOOL m_bCanMove;			//是否允许图像移动

/************************************************************************/
/*								控制选项结束                            */
/************************************************************************/
/************************************************************************/
/*								核心操作	                            */
/************************************************************************/
//保存显示的图片用
	IplImage * m_pImg;
	//显示一张Iplimage,注意不要析构掉,会导致野指针
	void ChangeImg(IplImage* img,BOOL bchangescale = FALSE);
	//显示路径中的图片,注意不要析构掉,会导致野指针
	void ChangeImg(CString path,BOOL bchangescale = FALSE);
	//显示照相机里面的图片
	void ShowImage();
	//保存到文件
	BOOL SaveToFile(CString path);
	//初始化操作
	void Init(CWnd *papa);
	//父窗口
	CWnd *m_papa;





	BOOL m_bNoShowBox;
	BOOL m_bNoShowCoppeArea;
	BOOL m_bNoShowSheetArea;
	BOOL m_bFlag;
	BOOL m_bDrawRuler;
	BOOL m_bDrawRect[CameraNum];
	BOOL m_bFist;
	bool m_bIsDrawHighARect;
	bool m_bIsDrawCenter;
	//是否是高精度对位对话框
	bool m_bIsHighTempDlg;
	BOOL m_bFlagBox;
	BOOL m_bIsShowChipPos;
	static BOOL m_bDrawRulerOut;
	static BOOL m_bDrawRulerOut2;
	//	static BOOL m_bDrawRulerModify;
	static BOOL DrawDetRect[3];
	BOOL m_bDrawRuler2;
	CWnd * m_pSencondWnd;


	CRect m_HighSelectRect;
	CArray <CFloatPt, CFloatPt> *m_pChipPos;
	CPoint m_ptFlagBox[4];
	static int m_nIndexEnum[4];//0 代表螺丝定位相机 1 代表晶片定位相机 2修正相机
	CRect m_rtDraw[4];
	CPoint m_ptFlag;
	int m_nIndex;//相机下标
	CRect   m_HighARect[10];		//模板区域
	CRect	m_HighDetRect[10];			//检测区域@whq 2016-5-10，以前是检测区域和模板区域一起，现在分出来
	CPoint m_ptCenterTemp;
	CPoint m_ptCenter[2];


	CPen m_pen[4];
	double ImageScale;
	CCriticalSection m_g_cs;//同步锁


/************************************************************************/
/*                           opengl 3D显示                              */
/************************************************************************/
	GL3DPara m_gl3D;//用来保存与gl和3D显示有关的变量
	//传入一张Mat的图像,之所以用Mat是因为可以自动管理内存
	void ChangeImgGL(cv::Mat img);
	//用opengl绘制3D图像
	void ShowImageGL();
	//是否是3D
	BOOL m_bGL;

/************************************************************************/
/*                           opengl 3D显示 结束                         */
/************************************************************************/



/************************************************************************/
/*                          缩放平移选项                                */
/************************************************************************/
	//图像放大比例,值越大,视场越小,控件到图像的转化关系变为1/ImageScale2
	double ImageScale2;
	//初始缩放倍数,刚刚加载文件的时候得到的
	double scalx,scaly;
	//最后图像上面的ROI
	CDRect m_ROIrect;
	//记录上次按下的点
	CPoint m_LDownPt;
	MovePara m_move;
	//记录控件的宽高
	int Width();
	int Height();
	//点的坐标转换,0表示从控件到图像,1反之
	void PointChange(CFloatPt srcpt,CFloatPt &dstpt,int changeType);
	void PointChange(C3FloatPt srcpt,C3FloatPt &dstpt,int changeType);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void MoveDis(int movex, int movey);

	BOOL CheckValue();

	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
/************************************************************************/
/*                          缩放平移选项结束                            */
/************************************************************************/


/************************************************************************/
/*                        添加绘制元素                                  */
/************************************************************************/
	vector<DRAWRECT> m_Rects[CameraNum]; //区域
	vector<DRAWLINE> m_Lines[CameraNum]; //直线
	vector<DRAWRECT> DetRects[CameraNum];//检测区域
	vector<DRAWFont> m_String[CameraNum];//文字
	vector<DRAWCircle> m_Circle[CameraNum];//圆
	vector<DRAWAngleArc> m_AngleArc[CameraNum];//圆弧
	vector <DRAWLINE> m_3Ddata;
	//动态添加,添加的元素再调用一次后会更新
	void ChangingInit();
	void ChangingStyle(int changingType);
	int ChangingMyCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor=RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int ChangingMyMark(C3FloatPt Pos,int w = 20,int h = 20,COLORREF crColor=/*RGB(207,221,238)*/RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int ChangingMyLine(C3FloatPt Pt1, C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE,BOOL IsShow = TRUE);
	int ChangingMyRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);


	int myMark(C3FloatPt Pos,int w = 20,int h = 20,COLORREF crColor=/*RGB(207,221,238)*/RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myLine(C3FloatPt Pt1, C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE,BOOL IsShow = TRUE);
	int myCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor=RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myAngleArc(C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor = RGB(255,0,0), int nWidth = 1,int nPenStyle = 0);
	int AddDetDect(LPCRECT lpRect,COLORREF crColor =RGB(255,0,0), int nWidth=1,int nPenStyle=1, BOOL IsDraw = TRUE);
	int AddString(CPoint Pt, CString strText ,int nAngle = 0, COLORREF crColor = RGB(255,0,0), 
		int lfHeight = 20, CString strFont = "宋体",  int nRadius = 1);
	int ClearDetDect();
	int ClearDraw();
	int ClearString();
/************************************************************************/
/*                        添加绘制元素完成                              */
/************************************************************************/

/************************************************************************/
/*                        绘制元素开始                                  */
/************************************************************************/
	int myRectangle(CDC *pMemDC,LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0);
	int myCircle(CDC *pMemDC,int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth=1,int nPenStyle=0);
	int myLine(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsShow = TRUE);
	int myAngleArc(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor, int nWidth,int nPenStyle);
	void Fun(CDC *pMemDC,int x, int y, CString strText, COLORREF crColor, int lfHeight, CString strFont , int nAngle, int nRadius);
	void DrawRuler(CDC *pMemDC);
	void DrawRectBox(CDC *pMemDC);
	/************************************************************************/
/*                        绘制元素完成                                  */
/************************************************************************/	



	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PreSubclassWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


#endif 
