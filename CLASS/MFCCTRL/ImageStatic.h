#if !defined(AFX_IMAGESTATIC_H__95842368_DB80_49AF_A58B_749F5A584461__INCLUDED_)
#define AFX_IMAGESTATIC_H__95842368_DB80_49AF_A58B_749F5A584461__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ImageStatic.h : header file
//
#define STATIC_CV
#include "def.h"
#include <afxmt.h>
#ifdef STATIC_CV
#include "cv.h"
#endif
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



struct DRAWRECT//绘区域结构体
{
	CRect m_Rect;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
};
struct DRAWLINE//绘直线结构体
{
	C3FloatPt Pt1;
	C3FloatPt Pt2;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
	BOOL IsShow ;
};
struct DRAWCircle//绘圆结构体
{
	int m_CircleCX;
	int m_CircleCY;
	int m_CircleR;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
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
/*								核心操作	                            */
/************************************************************************/
//保存显示的图片用
	IplImage * m_pImg;
	//显示一张Iplimage,注意不要析构掉,会导致野指针
	void ChangeImg(IplImage* img);
	//显示路径中的图片,注意不要析构掉,会导致野指针
	void ChangeImg(CString path);
	//显示照相机里面的图片
	void ShowImage();


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
	//记录控件的宽高
	int Width();
	int Height();
	//点的坐标转换,0表示从控件到图像,1反之
	void PointChange(CFloatPt srcpt,CFloatPt &dstpt,int changeType);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
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

	int myMark(C3FloatPt Pos,int w = 20,int h = 20,COLORREF crColor=/*RGB(207,221,238)*/RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myLine(C3FloatPt Pt1, C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE,BOOL IsShow = TRUE);
	int myAngleArc(C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor = RGB(255,0,0), int nWidth = 1,int nPenStyle = 0);
	int myCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor=RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
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



};


#endif 
