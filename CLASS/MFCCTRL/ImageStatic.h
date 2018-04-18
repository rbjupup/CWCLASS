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
struct DRAWRECT//������ṹ��
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
struct DRAWLINE//��ֱ�߽ṹ��
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
struct DRAWCircle//��Բ�ṹ��
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

typedef struct tagCAMERA{
	VECTOR vEye;
	VECTOR vCenter;
	VECTOR vUp;
}CAMERA;
struct GL3DPara
{

	cv::Mat m_pic3D;		//С����ͼ��
	cv::Mat m_pic;			//û��С�����ͼ��
	double m_valmax,m_valmin;	//ת�Ҷ�ͼ��ʱ����Ҫ֪�������Сֵ
	double co1,co0;	//�����Сֵת������ȵļ��㹫ʽ
	//gray =��val - min��*��255/��max - min���� = val *255/��max - min�� - min*255 /��max - min��
	//     = val*co1+co0

	CWnd *m_father;			//���׵�ָ��
	CWnd *m_Grandfather;	//үү��ָ��
	int m_viewx,m_viewy;	//��ʾ��С
	CAMERA	m_camera;		//�ӽ�
	CAMERA	m_cameraOld;
	double   m_RotX, m_RotY, m_RotZ;				//��ת
	double   m_oldRotX, m_oldRotY, m_oldRotZ;
	CClientDC	*m_pDC;
	bool m_bDown;			// = true;����Ƿ���
	CPoint m_pointDown;		//������µ�λ��= point;
	float	m_theta,m_thetaOld;		//=m_theta;
	HCURSOR			hHandWantGrab;
	BOOL ColorRG0;		//��ɫ����,��2��6����ģʽ,����ֵΪ��,ʹ��RGģʽ
	CStatic *m_cs;//��ʾ����
	CRect papaRect;		//opengl����λ��
	CRect papaSRect;		//opengl����λ��
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
	//��ʼ��
	void Init(CWnd *papa);
	//ʹ�ô����ʼ���ؼ�
	void InitGrandPa(CWnd *grandpa);
	//�ͷ���Դ
	void Release();
	//����ͼ��
	void DrawImage();
	//�������ô��ڵ�λ��
	void Resize();
	void ResizeBack();
	//����OPenGL��������
	void OnSize(int X,int Y);
	//�������
	void OnLButtonDown(UINT nFlags, CPoint point);
	//����ƶ�
	void OnMouseMove(UINT nFlags, CPoint point); 
	//�������
	void OnLButtonUp(UINT nFlags, CPoint point);
	//���ֹ���
	void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) ;
	//�����Ա���ʾ�õı�ʾ��ȵ���ɫ��
	void DrawBar();
	//���ô��֮��Ĳ���
	void LM();	
	//����������
	void DrawCoordinate();
	//�������صĸ�ʽ
	BOOL bSetupPixelFormat();
	//�Ŵ���С
	void ZoomIn(int abs);
	//����ͼ��
	void DrawObj();
	//��ȡ��ɫ
	void GetColor(double r, double &red, double &green, double &blue);
	//��ȡ�����Сֵ,���������ĵ�ɫ
	void GetMinMax();
	//���Ʒֿ���ɫ
	void DrawColorBlock(CDC* pDC,CRect r, COLORREF cr1, COLORREF cr2);
	//����Co
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
/*								����ѡ�ʼ	                        */
/************************************************************************/

	BOOL m_bCanOutImage;		//���������ͼ����������ROI���кڱ����ROI֮��ĵط�
	BOOL m_bChangeScalar;		//�Ƿ���Ըı䵱ǰ�ı���
	BOOL m_bCanMove;			//�Ƿ�����ͼ���ƶ�

/************************************************************************/
/*								����ѡ�����                            */
/************************************************************************/
/************************************************************************/
/*								���Ĳ���	                            */
/************************************************************************/
//������ʾ��ͼƬ��
	IplImage * m_pImg;
	//��ʾһ��Iplimage,ע�ⲻҪ������,�ᵼ��Ұָ��
	void ChangeImg(IplImage* img,BOOL bchangescale = FALSE);
	//��ʾ·���е�ͼƬ,ע�ⲻҪ������,�ᵼ��Ұָ��
	void ChangeImg(CString path,BOOL bchangescale = FALSE);
	//��ʾ����������ͼƬ
	void ShowImage();
	//���浽�ļ�
	BOOL SaveToFile(CString path);
	//��ʼ������
	void Init(CWnd *papa);
	//������
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
/*                           opengl 3D��ʾ                              */
/************************************************************************/
	GL3DPara m_gl3D;//����������gl��3D��ʾ�йصı���
	//����һ��Mat��ͼ��,֮������Mat����Ϊ�����Զ������ڴ�
	void ChangeImgGL(cv::Mat img);
	//��opengl����3Dͼ��
	void ShowImageGL();
	//�Ƿ���3D
	BOOL m_bGL;

/************************************************************************/
/*                           opengl 3D��ʾ ����                         */
/************************************************************************/



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
	MovePara m_move;
	//��¼�ؼ��Ŀ��
	int Width();
	int Height();
	//�������ת��,0��ʾ�ӿؼ���ͼ��,1��֮
	void PointChange(CFloatPt srcpt,CFloatPt &dstpt,int changeType);
	void PointChange(C3FloatPt srcpt,C3FloatPt &dstpt,int changeType);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	void MoveDis(int movex, int movey);

	BOOL CheckValue();

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
	//��̬���,��ӵ�Ԫ���ٵ���һ�κ�����
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



	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual void PreSubclassWindow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};


#endif 
