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



struct DRAWRECT//������ṹ��
{
	CRect m_Rect;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
};
struct DRAWLINE//��ֱ�߽ṹ��
{
	C3FloatPt Pt1;
	C3FloatPt Pt2;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
	BOOL IsShow ;
};
struct DRAWCircle//��Բ�ṹ��
{
	int m_CircleCX;
	int m_CircleCY;
	int m_CircleR;
	COLORREF m_Color;
	int nWidth;
	int nPenStyle;
};
struct DRAWFont//�����ֽṹ��
{
	CPoint Pt;
	CString strText ;
	COLORREF crColor;
	int lfHeight;
	CString strFont; 
	int nAngle;
	int nRadius ;
};
struct DRAWAngleArc//��Բ���ṹ��
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
// Construction
public:
	CFloatPt m_ptAreaCenter;
	BOOL m_fjd;
	double *dScalx;
	double *dScaly; 
	CRect *m_ptLP;
	void ChargeImagePt(C3FloatPt &Pt);
	float m_ClearColorRed;
	float m_ClearColorGreen;
	float m_ClearColorBlue;
	HGLRC m_hRC;
	CDC* m_pDC;
	BOOL InitializeOpenGL(void);
	BOOL SetupPixelFormat(void);
	void RenderScene(void);

	BOOL m_bNoShowBox;
	BOOL m_bNoShowCoppeArea;
	BOOL m_bNoShowSheetArea;
	CPen m_pen[4];
	BOOL m_bDrawRect[4];
	CRect m_rtDraw[4];
	BOOL m_bFlagBox;
	CPoint m_ptFlagBox[4];
	BOOL m_bIsShowChipPos;
	CArray <CFloatPt, CFloatPt> *m_pChipPos;
	void DrawRectBox(CDC *pMemDC);

	CPoint m_ptFlag;
	BOOL m_bFlag;
	BOOL m_bDrawRuler;
	static BOOL m_bDrawRulerOut;
	BOOL m_bDrawRuler2;
	static BOOL m_bDrawRulerOut2;
//	static BOOL m_bDrawRulerModify;
	BOOL m_bFist;//
	static int m_nIndexEnum[4];//0 ������˿��λ��� 1 ����Ƭ��λ��� 2�������
//	static int m_nDraw;
	static BOOL DrawDetRect[3];
	int Camera;
	CImageStatic();
	int m_nIndex;//����±�
	double ImageScale;
	double ImageScale2;
	CWnd * m_pSencondWnd;
	void DrawRuler(CDC *pMemDC);

	CRect   m_HighARect[10];		//ģ������
	CRect	m_HighDetRect[10];			//�������@whq 2016-5-10����ǰ�Ǽ�������ģ������һ�����ڷֳ���


	bool	m_bIsDrawHighARect;
	//�Ƿ��Ǹ߾��ȶ�λ�Ի���
	bool m_bIsHighTempDlg;
	CRect m_HighSelectRect;
	bool m_bIsDrawCenter;
	CPoint m_ptCenterTemp;
	CPoint m_ptCenter[2];
	int Is3DView;//0�� 2d  1�� 3d  3��magnify
	CCriticalSection m_g_cs;//ͬ����
// Attributes

public:
	void ShowImage();//��ʾ����������ͼƬ
	//LT 
	void GetMyHdc();
	virtual ~CImageStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CImageStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	vector<DRAWRECT> m_Rects[CameraNum]; //����
	vector<DRAWLINE> m_Lines[CameraNum]; //ֱ��
	vector<DRAWRECT> DetRects[CameraNum];//�������
	vector<DRAWFont> m_String[CameraNum];//����
	vector<DRAWCircle> m_Circle[CameraNum];//Բ
	vector<DRAWAngleArc> m_AngleArc[CameraNum];//Բ��
	vector <DRAWLINE> m_3Ddata;

	int myMark(C3FloatPt Pos,int w = 20,int h = 20,COLORREF crColor=/*RGB(207,221,238)*/RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myLine(C3FloatPt Pt1, C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE,BOOL IsShow = TRUE);
	int myLine(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsShow = TRUE);
	int myAngleArc(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor, int nWidth,int nPenStyle);
	int myAngleArc(C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor = RGB(255,0,0), int nWidth = 1,int nPenStyle = 0);

	int myRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);

	int myCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor=RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);

	
	int myRectangle(CDC *pMemDC,LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0);
	int myCircle(CDC *pMemDC,int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth=1,int nPenStyle=0);

	int AddDetDect(LPCRECT lpRect,COLORREF crColor =RGB(255,0,0), int nWidth=1,int nPenStyle=1, BOOL IsDraw = TRUE);
	int ClearDetDect();
	int ClearDraw();
	BOOL Clear;
	int AddString(CPoint Pt, CString strText ,int nAngle = 0, COLORREF crColor = RGB(255,0,0), 
		int lfHeight = 20, CString strFont = "����",  int nRadius = 1);
	int ClearString();
	void Fun(CDC *pMemDC,int x, int y, CString strText, COLORREF crColor, int lfHeight, CString strFont , int nAngle, int nRadius);
	void ChangeImg(IplImage* img);
private:
#ifdef STATIC_CV
	IplImage * m_pImg;
#endif
};


#endif 
