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
public:
	CImageStatic();
	virtual ~CImageStatic();
	afx_msg void OnPaint();
	void GetMyHdc();
protected:
	DECLARE_MESSAGE_MAP()
public:

/************************************************************************/
/*								���Ĳ���	                            */
/************************************************************************/
//������ʾ��ͼƬ��
	IplImage * m_pImg;
	//��ʾһ��Iplimage,ע�ⲻҪ������,�ᵼ��Ұָ��
	void ChangeImg(IplImage* img);
	//��ʾ·���е�ͼƬ,ע�ⲻҪ������,�ᵼ��Ұָ��
	void ChangeImg(CString path);
	//��ʾ����������ͼƬ
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
	//�Ƿ��Ǹ߾��ȶ�λ�Ի���
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
	static int m_nIndexEnum[4];//0 ������˿��λ��� 1 ����Ƭ��λ��� 2�������
	CRect m_rtDraw[4];
	CPoint m_ptFlag;
	int m_nIndex;//����±�
	CRect   m_HighARect[10];		//ģ������
	CRect	m_HighDetRect[10];			//�������@whq 2016-5-10����ǰ�Ǽ�������ģ������һ�����ڷֳ���
	CPoint m_ptCenterTemp;
	CPoint m_ptCenter[2];


	CPen m_pen[4];
	double ImageScale;
	CCriticalSection m_g_cs;//ͬ����





/************************************************************************/
/*                          ����ƽ��ѡ��                                */
/************************************************************************/
	//ͼ��Ŵ����,ֵԽ��,�ӳ�ԽС,�ؼ���ͼ���ת����ϵ��Ϊ1/ImageScale2
	double ImageScale2;
	//��ʼ���ű���,�ոռ����ļ���ʱ��õ���
	double scalx,scaly;
	//���ͼ�������ROI
	CDRect m_ROIrect;
	//��¼�ϴΰ��µĵ�
	CPoint m_LDownPt;
	//��¼�ؼ��Ŀ��
	int Width();
	int Height();
	//�������ת��,0��ʾ�ӿؼ���ͼ��,1��֮
	void PointChange(CFloatPt srcpt,CFloatPt &dstpt,int changeType);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
/************************************************************************/
/*                          ����ƽ��ѡ�����                            */
/************************************************************************/


/************************************************************************/
/*                        ��ӻ���Ԫ��                                  */
/************************************************************************/
	vector<DRAWRECT> m_Rects[CameraNum]; //����
	vector<DRAWLINE> m_Lines[CameraNum]; //ֱ��
	vector<DRAWRECT> DetRects[CameraNum];//�������
	vector<DRAWFont> m_String[CameraNum];//����
	vector<DRAWCircle> m_Circle[CameraNum];//Բ
	vector<DRAWAngleArc> m_AngleArc[CameraNum];//Բ��
	vector <DRAWLINE> m_3Ddata;

	int myMark(C3FloatPt Pos,int w = 20,int h = 20,COLORREF crColor=/*RGB(207,221,238)*/RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myLine(C3FloatPt Pt1, C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE,BOOL IsShow = TRUE);
	int myAngleArc(C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor = RGB(255,0,0), int nWidth = 1,int nPenStyle = 0);
	int myCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor=RGB(0,255,0), int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int myRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsDraw = TRUE);
	int AddDetDect(LPCRECT lpRect,COLORREF crColor =RGB(255,0,0), int nWidth=1,int nPenStyle=1, BOOL IsDraw = TRUE);
	int AddString(CPoint Pt, CString strText ,int nAngle = 0, COLORREF crColor = RGB(255,0,0), 
		int lfHeight = 20, CString strFont = "����",  int nRadius = 1);
	int ClearDetDect();
	int ClearDraw();
	int ClearString();
/************************************************************************/
/*                        ��ӻ���Ԫ�����                              */
/************************************************************************/

/************************************************************************/
/*                        ����Ԫ�ؿ�ʼ                                  */
/************************************************************************/
	int myRectangle(CDC *pMemDC,LPCRECT lpRect,COLORREF crColor, int nWidth=1,int nPenStyle=0);
	int myCircle(CDC *pMemDC,int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth=1,int nPenStyle=0);
	int myLine(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,COLORREF crColor, int nWidth=1,int nPenStyle=0,BOOL IsShow = TRUE);
	int myAngleArc(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor, int nWidth,int nPenStyle);
	void Fun(CDC *pMemDC,int x, int y, CString strText, COLORREF crColor, int lfHeight, CString strFont , int nAngle, int nRadius);
	void DrawRuler(CDC *pMemDC);
	void DrawRectBox(CDC *pMemDC);

/************************************************************************/
/*                        ����Ԫ�����                                  */
/************************************************************************/	



};


#endif 
