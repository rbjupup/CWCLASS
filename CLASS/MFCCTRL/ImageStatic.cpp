// ImageStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ImageStatic.h"
#include "CLASS/CV/CvvImage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CImageStatic
int CImageStatic::m_nIndexEnum[4];
BOOL CImageStatic::m_bDrawRulerOut;
BOOL CImageStatic::m_bDrawRulerOut2;
BOOL CImageStatic::DrawDetRect[3];


CImageStatic::CImageStatic()
{
	m_pSencondWnd = NULL;
	ImageScale = 1;
	m_bFist = FALSE;
	Camera = 0;
	m_bDrawRuler = FALSE;
	m_bDrawRuler2 = FALSE;
	Clear = FALSE;
	m_bFlag = FALSE;
	m_pChipPos = NULL;
	m_bIsShowChipPos = FALSE;
	m_bFlagBox = FALSE;
	for (int i=0; i<4; i++)
	{
		m_bDrawRect[i] = FALSE;
		m_rtDraw[i].top = m_rtDraw[i].left = m_rtDraw[i].bottom = m_rtDraw[i].right= 0;
	}
	m_pen[0].CreatePen(PS_DASH, 1, RGB(255,128,0));
	m_pen[1].CreatePen(PS_DASH, 1, RGB(0,128,255));
	m_pen[2].CreatePen(PS_DASH, 1, RGB(255,0,0));
	m_pen[3].CreatePen(PS_DASH, 1, RGB(0,255,128));

	m_bNoShowBox = FALSE;
	m_bNoShowCoppeArea = FALSE;
	m_bNoShowSheetArea = FALSE;
	ImageScale2 = 1;

	m_bIsDrawHighARect = FALSE;
	m_bIsDrawCenter = false;
	Is3DView = 0;
	m_bIsHighTempDlg = false;

	m_ptLP = NULL;
	dScalx = NULL;
	dScaly = NULL;

	m_fjd = FALSE;

#ifdef STATIC_CV
	m_pImg = NULL;
#endif
}

CImageStatic::~CImageStatic()
{
#ifdef STATIC_CV
	if(m_pImg != NULL)
		cvReleaseImage(&m_pImg);
#endif
}


BEGIN_MESSAGE_MAP(CImageStatic, CStatic)
	ON_WM_PAINT()
ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageStatic message handlers

void CImageStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if(m_pSencondWnd != NULL && m_pSencondWnd->GetSafeHwnd() != NULL)
		m_pSencondWnd->Invalidate(FALSE);
	
	if(this->GetSafeHwnd() == NULL)
		return ;
	ShowImage();

	// Do not call CStatic::OnPaint() for painting messages
}

void CImageStatic::ShowImage()
{

	int i  =0;
#ifdef STATIC_CV
	if(m_pImg != NULL){
	  		HDC hdc;
	 		CDC *MemDC = GetDC();//CDC::FromHandle(hdc);
	 		CRect rect;
	 		GetWindowRect(&rect);//获取控件相对于屏幕的位置
	 		ScreenToClient(rect);
	 		CRgn Rgn;
	 		Rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	 		MemDC->SelectClipRgn(&Rgn);
			m_nIndex = 0;
	 		if (m_nIndex >=0 &&m_nIndex<CameraNum)
	 		{	
	 			for (i=0; i<m_Rects[m_nIndex].size(); i++)//绘制所有区域
	 			{
	 				if(m_bNoShowBox == TRUE)
	 					break;
	 				myRectangle(MemDC,m_Rects[m_nIndex][i].m_Rect,m_Rects[m_nIndex][i].m_Color,m_Rects[m_nIndex][i].nWidth,m_Rects[m_nIndex][i].nPenStyle);
	 			}
	 			for (i=0; i<m_Lines[m_nIndex].size(); i++)//绘制所有直线
	 			{
	 				myLine(MemDC,m_Lines[m_nIndex][i].Pt1,m_Lines[m_nIndex][i].Pt2,m_Lines[m_nIndex][i].m_Color,m_Lines[m_nIndex][i].nWidth,m_Lines[m_nIndex][i].nPenStyle,m_Lines[m_nIndex][i].IsShow);
	 			}
	 			for (i=0; i< DetRects[m_nIndex].size(); i++)//绘制所有检测区域
	 			{
	 				if((0 == i && m_bNoShowCoppeArea == TRUE) || (1 == i && m_bNoShowSheetArea == TRUE))
	 					continue;
	 				myRectangle(MemDC,DetRects[m_nIndex][i].m_Rect,DetRects[m_nIndex][i].m_Color,DetRects[m_nIndex][i].nWidth,DetRects[m_nIndex][i].nPenStyle);
	 			}
	 			for (i=0; i< m_String[m_nIndex].size(); i++)//绘制文字
	 			{	
	 				Fun(MemDC,
	 					m_String[m_nIndex][i].Pt.x,
	 					m_String[m_nIndex][i].Pt.y,
	 					m_String[m_nIndex][i].strText,
	 					m_String[m_nIndex][i].crColor,
	 					m_String[m_nIndex][i].lfHeight,
	 					m_String[m_nIndex][i].strFont,
	 					m_String[m_nIndex][i].nAngle,
	 					m_String[m_nIndex][i].nRadius);
	 			}
	 			for (i=0; i< m_Circle[m_nIndex].size(); i++)//绘制文字
	 			{	
	 				myCircle(MemDC,
	 					m_Circle[m_nIndex][i].m_CircleCX,
	 					m_Circle[m_nIndex][i].m_CircleCY,
	 					m_Circle[m_nIndex][i].m_CircleR,
	 					m_Circle[m_nIndex][i].m_Color,
	 					m_Circle[m_nIndex][i].nWidth,
	 					m_Circle[m_nIndex][i].nPenStyle);
	 
	 			}
	 			for (i=0; i< m_AngleArc[m_nIndex].size(); i++)//圆弧
	 			{	
	 
	 				myAngleArc(MemDC,
	 					m_AngleArc[m_nIndex][i].Pt1,
	 					m_AngleArc[m_nIndex][i].Pt2,
	 					m_AngleArc[m_nIndex][i].Pt3,
	 					m_AngleArc[m_nIndex][i].m_Color,
	 					m_AngleArc[m_nIndex][i].nWidth,
	 					m_AngleArc[m_nIndex][i].nPenStyle);
	 
	 			}
	 		}
	 		ReleaseDC(MemDC);
	 		m_g_cs.Unlock();//解锁@whq
	 		Sleep(1);//@whq
	     GetWindowRect(&rect);//获取控件相对于屏幕的位置
	 	ScreenToClient(rect);
		CDC *pDC=GetDC();
		HDC hDC=pDC->GetSafeHdc();
		GetClientRect(&rect);  
		SetRect(rect,rect.left,rect.top,rect.right,rect.bottom);  
		CvvImage cimg;  
		cimg.CopyOf(m_pImg);  
		cimg.DrawToHDC(hDC,&rect);  
		ReleaseDC(pDC); 


		}
#endif

// 	if (Is3DView == 2)
// 	{
//  		if (NULL == m_pImageDeal)
//  		{
//  			m_g_cs.Unlock();//解锁@whq
//  			Sleep(1);//@whq
//  			return;
//  		}
//  		HDC hdc;
//  		if (( FALSE == m_pImageDeal->GetSurfaceDC(hdc) ))
//  		{
//  			m_pImageDeal->ReleaseSurfaceDC(hdc);
//  			m_g_cs.Unlock();//解锁@whq
//  			Sleep(1);//@whq
//  			return ;
//  		}
// 		CDC *MemDC /*= new CDC */= CDC::FromHandle(hdc);
// 		CRect rect;
// 		GetWindowRect(&rect);//获取控件相对于屏幕的位置
// 		ScreenToClient(rect);
// 		CRgn Rgn;
// 		Rgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
// 		MemDC->SelectClipRgn(&Rgn);
// 		if (m_nIndex >=0 &&m_nIndex<CameraNum)
// 		{	
// 			for (i=0; i<m_Rects[m_nIndex].size(); i++)//绘制所有区域
// 			{
// 				if(m_bNoShowBox == TRUE)
// 					break;
// 				myRectangle(MemDC,m_Rects[m_nIndex][i].m_Rect,m_Rects[m_nIndex][i].m_Color,m_Rects[m_nIndex][i].nWidth,m_Rects[m_nIndex][i].nPenStyle);
// 			}
// 			for (i=0; i<m_Lines[m_nIndex].size(); i++)//绘制所有直线
// 			{
// 				myLine(MemDC,m_Lines[m_nIndex][i].Pt1,m_Lines[m_nIndex][i].Pt2,m_Lines[m_nIndex][i].m_Color,m_Lines[m_nIndex][i].nWidth,m_Lines[m_nIndex][i].nPenStyle,m_Lines[m_nIndex][i].IsShow);
// 			}
// 			for (i=0; i< DetRects[m_nIndex].size(); i++)//绘制所有检测区域
// 			{
// 				if((0 == i && m_bNoShowCoppeArea == TRUE) || (1 == i && m_bNoShowSheetArea == TRUE))
// 					continue;
// 				myRectangle(MemDC,DetRects[m_nIndex][i].m_Rect,DetRects[m_nIndex][i].m_Color,DetRects[m_nIndex][i].nWidth,DetRects[m_nIndex][i].nPenStyle);
// 			}
// 			for (i=0; i< m_String[m_nIndex].size(); i++)//绘制文字
// 			{	
// 				Fun(MemDC,
// 					m_String[m_nIndex][i].Pt.x,
// 					m_String[m_nIndex][i].Pt.y,
// 					m_String[m_nIndex][i].strText,
// 					m_String[m_nIndex][i].crColor,
// 					m_String[m_nIndex][i].lfHeight,
// 					m_String[m_nIndex][i].strFont,
// 					m_String[m_nIndex][i].nAngle,
// 					m_String[m_nIndex][i].nRadius);
// 			}
// 			for (i=0; i< m_Circle[m_nIndex].size(); i++)//绘制文字
// 			{	
// 				myCircle(MemDC,
// 					m_Circle[m_nIndex][i].m_CircleCX,
// 					m_Circle[m_nIndex][i].m_CircleCY,
// 					m_Circle[m_nIndex][i].m_CircleR,
// 					m_Circle[m_nIndex][i].m_Color,
// 					m_Circle[m_nIndex][i].nWidth,
// 					m_Circle[m_nIndex][i].nPenStyle);
// 
// 			}
// 			for (i=0; i< m_AngleArc[m_nIndex].size(); i++)//圆弧
// 			{	
// 
// 				myAngleArc(MemDC,
// 					m_AngleArc[m_nIndex][i].Pt1,
// 					m_AngleArc[m_nIndex][i].Pt2,
// 					m_AngleArc[m_nIndex][i].Pt3,
// 					m_AngleArc[m_nIndex][i].m_Color,
// 					m_AngleArc[m_nIndex][i].nWidth,
// 					m_AngleArc[m_nIndex][i].nPenStyle);
// 
// 			}
// 		}
// 		ReleaseDC(MemDC);
//  		m_pImageDeal->ReleaseSurfaceDC(hdc);
// 		m_g_cs.Unlock();//解锁@whq
// 		Sleep(1);//@whq
// 		return;
// 	}
// 	CDC *pDC=GetDC();
// 	//LT
// 	if (pDC == NULL)
// 	{
// 		m_g_cs.Unlock();//解锁@whq
// 		return;
// 	}
   //获得矩形框架大小
// 	CRect rect;
//     GetWindowRect(&rect);//获取控件相对于屏幕的位置
// 	ScreenToClient(rect);
//     CDC		MemDC;					//首先定义一个显示设备对象 
// 	CBitmap MemBitmap;				//定义一个位图对象 
// 	MemDC.CreateCompatibleDC(pDC); //随后建立与屏幕显示兼容的内存显示设备
// 	if (MemDC == NULL)
// 	{
// 		AfxMessageBox("显示设备对象出错");
// 	}
//    
// 	//显示保存在m_pImage中的图片
// 	MemBitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
// 	MemDC.SelectObject(&MemBitmap);
// 	if (NULL != m_pImage)
// 	{
// 		m_pImage->ShowCurrentImage(&MemDC, 0, 0, rect.Width(), rect.Height());
// 		//抗锯齿
// 		//m_pImage->ShowPartialImage(&MemDC, rect, CPoint(0,0), CPoint(m_pImage->GetImageWidth(),m_pImage->GetImageHeight()));
// 		//ImageScale = (double)rect.Width()/m_pImage->GetImageWidth();
// 		//LT 20151010  在page上显示大相机和小相机图像时缩放倍率始终无法传进来
//  		if (ImageScale > 0.99)
//  		{
// //  			double scale1 = rect.Width()/float(SMALL_CAM_WIDTH);
// //  			double scale2 = rect.Height()/float(SMALL_CAM_HIGHT);
// 			double scale1 = rect.Width()/float(m_pImage->GetImageWidth());
// 			double scale2 = rect.Height()/float(m_pImage->GetImageHeight());
//  			ImageScale = scale1<scale2?scale1:scale2;
//  		}
// 	}
// 	else
// 	{
// 		//MemDC.FillSolidRect(0, 0, rect.Width(), rect.Height(),/* RGB(0,85,241)*/RGB(85,123,205));
// 	}
// 
//  	if (m_bDrawRulerOut == TRUE)
//  	{
//  		int i  = 0;
//  		i++;
//  	}
//  
//  	if (m_fjd == TRUE)
//  	{
//  		int i  = 0;
//  		i++;
//  	}
//  
// 	DrawRuler(&MemDC);
//  	if (m_bDrawRuler == TRUE && m_nIndexEnum[2] == m_nIndex && m_bFist == FALSE)
//  	{
//  		DrawRuler(&MemDC);
//  	}
//  	if (m_bDrawRulerOut == TRUE && m_nIndexEnum[2] == m_nIndex /*&& m_bFist == TRUE*/)
//  	{
//  		DrawRuler(&MemDC);
//  	}
//  
//  	if (m_bDrawRuler2 == TRUE && m_nIndexEnum[3] == m_nIndex && m_bFist == FALSE)
//  	{
//  		DrawRuler(&MemDC);
//  	}
//  	if (m_bDrawRulerOut2 == TRUE && m_nIndexEnum[3] == m_nIndex /*&& m_bFist == TRUE*/)
//  	{
//  		DrawRuler(&MemDC);
//  	}
// 	if (m_pImage != NULL)
// 	{
// 		ImageScale2 = m_pImage->GetShowScale(); 
// 	}
// 	else
// 	{
// 		if(rect.Width()>0)
// 		{
// 			ImageScale2 = (double)(rect.Width() / 1280.0);
// 		}
// 		else
// 		{
// 			ImageScale2 = 1;
// 		}
// 	}
// 
// 	MemDC.SelectStockObject(NULL_BRUSH);
// 	for (i=0; i<4; i++)
// 	{
// 		MemDC.SelectObject(&m_pen[i]);
// 		if(m_bDrawRect[i] == TRUE)
// 		{
// 			MemDC.Rectangle(m_rtDraw[i]);
// 		}
// 	}
// 
// 	if (m_nIndex >=0 &&m_nIndex<CameraNum)
// 	{	
// 		for (i=0; i<m_Rects[m_nIndex].size(); i++)//绘制所有区域
// 		{
// 			if(m_bNoShowBox == TRUE)
// 				break;
// 			myRectangle(&MemDC,m_Rects[m_nIndex][i].m_Rect,m_Rects[m_nIndex][i].m_Color,m_Rects[m_nIndex][i].nWidth,m_Rects[m_nIndex][i].nPenStyle);
// 		}
// 		//LT
//  		for (i=0; i<m_Lines[m_nIndex].size(); i++)//绘制所有直线//@whq624注释
//  		{
//  			myLine(&MemDC,m_Lines[m_nIndex][i].Pt1,m_Lines[m_nIndex][i].Pt2,m_Lines[m_nIndex][i].m_Color,m_Lines[m_nIndex][i].nWidth,m_Lines[m_nIndex][i].nPenStyle,m_Lines[m_nIndex][i].IsShow);
//  		}
// 		//
// 		for (i=0; i< DetRects[m_nIndex].size(); i++)//绘制所有检测区域
// 		{
// 			if((0 == i && m_bNoShowCoppeArea == TRUE) || (1 == i && m_bNoShowSheetArea == TRUE))
// 				continue;
// 			myRectangle(&MemDC,DetRects[m_nIndex][i].m_Rect,DetRects[m_nIndex][i].m_Color,DetRects[m_nIndex][i].nWidth,DetRects[m_nIndex][i].nPenStyle);
// 		}
// 		for (i=0; i< m_String[m_nIndex].size(); i++)//绘制文字
// 		{	
// 			Fun(&MemDC,
// 				m_String[m_nIndex][i].Pt.x,
// 				m_String[m_nIndex][i].Pt.y,
// 				m_String[m_nIndex][i].strText,
// 				m_String[m_nIndex][i].crColor,
// 				m_String[m_nIndex][i].lfHeight,
// 				m_String[m_nIndex][i].strFont,
// 				m_String[m_nIndex][i].nAngle,
// 				m_String[m_nIndex][i].nRadius);
// 		}
// 		for (i=0; i< m_Circle[m_nIndex].size(); i++)//绘制文字
// 		{	
// 			myCircle(&MemDC,
// 				m_Circle[m_nIndex][i].m_CircleCX,
// 				m_Circle[m_nIndex][i].m_CircleCY,
// 				m_Circle[m_nIndex][i].m_CircleR,
// 				m_Circle[m_nIndex][i].m_Color,
// 				m_Circle[m_nIndex][i].nWidth,
// 				m_Circle[m_nIndex][i].nPenStyle);
// 			
// 		}
// 		for (i=0; i< m_AngleArc[m_nIndex].size(); i++)//圆弧
// 		{	
// 			
// 			myAngleArc(&MemDC,
// 				m_AngleArc[m_nIndex][i].Pt1,
// 				m_AngleArc[m_nIndex][i].Pt2,
// 				m_AngleArc[m_nIndex][i].Pt3,
// 				m_AngleArc[m_nIndex][i].m_Color,
// 				m_AngleArc[m_nIndex][i].nWidth,
// 				m_AngleArc[m_nIndex][i].nPenStyle);
// 
// 		}
// 	}
// 	if (m_bFlag == TRUE)
// 	{
// 		CPen Pen;//　画笔	
// 		CPen *oldPen;
// 		Pen.CreatePen(PS_DASH,1,RGB(0,255,28));
// 		oldPen = MemDC.SelectObject(&Pen);
// 		MemDC.SelectStockObject(NULL_BRUSH);
// 		CPoint m_ptStart, m_ptEnd,m_ptStart2, m_ptEnd2;
// 		
// 		m_ptStart.x = m_ptFlag.x;
// 		m_ptStart.y = m_ptFlag.y - SUKER_SIZE/2;
// 		m_ptEnd.x = m_ptFlag.x;
// 		m_ptEnd.y = m_ptFlag.y + SUKER_SIZE/2;
// 		
// 		m_ptStart2.x = m_ptFlag.x - SUKER_SIZE/2;
// 		m_ptStart2.y = m_ptFlag.y;
// 		m_ptEnd2.x = m_ptFlag.x + SUKER_SIZE/2;
// 		m_ptEnd2.y = m_ptFlag.y;
// 		
// 		MemDC.MoveTo(m_ptStart);
// 		MemDC.LineTo(m_ptEnd);
// 		MemDC.MoveTo(m_ptStart2);
// 		MemDC.LineTo(m_ptEnd2);
// 		MemDC.SelectObject(oldPen);
// 	}
// 
// 	if (m_bIsShowChipPos == TRUE)
// 	{
// 		DrawRectBox(&MemDC);
// 	}
// 	
// 	if (m_bFlagBox ==  TRUE)
// 	{
// 		CPoint m_ptStart, m_ptEnd,m_ptStart2, m_ptEnd2;
// 		CPoint pt;
// 		CPen pen;
// 		CPen *oldPen;
// 		pen.CreatePen(PS_SOLID,1,RGB(0,0,255));
// 		oldPen = MemDC.SelectObject(&pen);
// 		MemDC.SelectStockObject(NULL_BRUSH);
// 		for (int i=0; i<4; i++)
// 		{
// 			pt = m_ptFlagBox[i];
// 			if (pt.x == 0 && pt.y == 0)
// 			{
// 				break;
// 			}
// 			m_ptStart.x = pt.x;
// 			m_ptStart.y = pt.y - 10/2;
// 			m_ptEnd.x = pt.x;
// 			m_ptEnd.y = pt.y + 10/2;
// 			
// 			m_ptStart2.x = pt.x - 10/2;
// 			m_ptStart2.y = pt.y;
// 			m_ptEnd2.x = pt.x + 10/2;
// 			m_ptEnd2.y = pt.y;
// 			
// 			MemDC.MoveTo(m_ptStart);
// 			MemDC.LineTo(m_ptEnd);
// 			MemDC.MoveTo(m_ptStart2);
// 			MemDC.LineTo(m_ptEnd2);
// 		}
// 		MemDC.SelectObject(oldPen);
// 	}
// 
// 	if (TRUE == m_bIsDrawHighARect)
// 	{
// 		CRect rt[MAX_CHILD_CONTROL_NUM];
// 		//画出检测区域矩形
// 		CPen Pen;//　画笔	
// 		CPen *oldPen;
// 		Pen.CreatePen(PS_DASH,1,RGB(255,0,0));
// 		oldPen = MemDC.SelectObject(&Pen);
// 		MemDC.SelectStockObject(NULL_BRUSH);
// 		int w ;
// 		int h ;
// 		for (int i=0;i<MAX_CHILD_CONTROL_NUM;i++)
// 		{
//  			if (!m_HighARect[i].IsRectEmpty())
//  			{
//  				w = abs(m_HighARect[i].right - m_HighARect[i].left) ;
//  				h = abs(m_HighARect[i].bottom - m_HighARect[i].top) ;
//   				rt[i].left = (m_ptCenter[i].x - w/2.0)/**ImageScale*/;
//   				rt[i].top = (m_ptCenter[i].y - h/2.0)/**ImageScale*/;
//   				rt[i].right = (m_ptCenter[i].x + w/2.0)/**ImageScale*/;
//   				rt[i].bottom = (m_ptCenter[i].y + h/2.0)/**ImageScale*/;
//  				MemDC.Rectangle(rt[i]);
//  				if (w < 10 )
//  				{
//  					w = w/2 ;
//  				}
//  				else
//  				{
//  					w = 10;
//  				}
//  				if (h < 10)
//  				{
//  					h = h/2;
//  				}
//  				else
//  				{
//  					h = 10;
//  				}
//  				MemDC.MoveTo(m_ptCenter[i].x, m_ptCenter[i].y - h);
//  				MemDC.LineTo(m_ptCenter[i].x, m_ptCenter[i].y + h);
//  				MemDC.MoveTo(m_ptCenter[i].x - w, m_ptCenter[i].y );
//  				MemDC.LineTo(m_ptCenter[i].x + w, m_ptCenter[i].y);
//  
//  			}
// 		}
// 
// 		//whq
// 		CRect rtS;
// //   		w = abs(m_HighSelectRect[0].right - m_HighSelectRect[0].left) ;
// //   		h = abs(m_HighSelectRect[0].bottom - m_HighSelectRect[0].top) ;
//    		rtS.left		= m_HighSelectRect.left;//(m_ptCenterTemp.x - w)/**ImageScale*/;
//    		rtS.top		= m_HighSelectRect.top;//(m_ptCenterTemp.y - h)/**ImageScale*/;
//    		rtS.right		= m_HighSelectRect.right;//(m_ptCenterTemp.x + w)/**ImageScale*/;
//    		rtS.bottom	= m_HighSelectRect.bottom;//(m_ptCenterTemp.y + h)/**ImageScale*/;
//  
//  
//  
//  		MemDC.Rectangle(rtS);
// // 		if (w < 10 )
// // 		{
// // 			w = w/2 ;
// // 		}
// // 		else
// // 		{
// // 			w = 10;
// // 		}
// // 		if (h < 10)
// // 		{
// // 			h = h/2;
// // 		}
// // 		else
// // 		{
// // 			h = 10;
// // 		}
// //  		MemDC.MoveTo(m_ptCenterTemp.x, m_ptCenterTemp.y - h);
// //  		MemDC.LineTo(m_ptCenterTemp.x, m_ptCenterTemp.y + h);
// //  		MemDC.MoveTo(m_ptCenterTemp.x - w, m_ptCenterTemp.y );
// //  		MemDC.LineTo(m_ptCenterTemp.x + w, m_ptCenterTemp.y);
// 
// 
// 		//////////////////////////////////////////////////////////////////////////
// 		//显示高精度相机的检测区域
// 		//@whq 2016-5-10
// 		//////////////////////////////////////////////////////////////////////////
// 		CPen GreenPen;
// 		GreenPen.CreatePen(PS_DASH,1,RGB(0,255,0));
// 		MemDC.SelectObject(&GreenPen);
// 		for (int i=0;i<MAX_FIND_POINT;i++)
// 		{
// 			if (!m_HighDetRect[i].IsRectEmpty())
// 			{
// 				rt[i].left = m_HighDetRect[i].left;
// 				rt[i].top = m_HighDetRect[i].top;
// 				rt[i].right = m_HighDetRect[i].right;
// 				rt[i].bottom = m_HighDetRect[i].bottom;
// 				MemDC.Rectangle(rt[i]);
// 			}
// 		}
// 
// 		MemDC.SelectObject(oldPen);
// 	}
// 	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
// 	MemBitmap.DeleteObject(); 
// 	MemDC.DeleteDC();
// 	ReleaseDC(pDC);
// 	m_g_cs.Unlock();//解锁@whq
// 	//TRACE("showimage:index= %d,Unlock:lockcount=%d\n",m_nIndex,m_g_cs.m_sect.LockCount);
// 	Sleep(1);//@whq
}
void CImageStatic::GetMyHdc()
{
	//m_hdc =this->GetDC()->GetSafeHdc();
}
void CImageStatic::DrawRectBox(CDC *pMemDC)
{
	if(m_pChipPos == NULL)
	{
		return;
	}
	
	CRect rt;
	CPoint pt;
	int i = 0;
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(PS_SOLID,1,RGB(255,0,128));
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	while(i != m_pChipPos->GetSize())
	{
		pt.x = m_pChipPos->GetAt(i).x * ImageScale;
		pt.y = m_pChipPos->GetAt(i).y * ImageScale;
		rt.top = pt.y - CHIP_SIZE;
		rt.bottom = pt.y + CHIP_SIZE;
		rt.left = pt.x - CHIP_SIZE;
		rt.right = pt.x + CHIP_SIZE;
		pMemDC->Rectangle(rt);
		i++;
	}
	pMemDC->SelectObject(oldPen);
}

void CImageStatic::DrawRuler(CDC *pMemDC)
{
	int nMWidth = ImageScale*110/2;
	int nMHeight = ImageScale*110/2;
	int nCount = 0;
	int nFlag = 0;
	CPoint ptStart(nMWidth, nMHeight);
	CPoint ptEnd(nMWidth, nMHeight);
	CPoint ptStart2(nMWidth, nMHeight);
	CPoint ptEnd2(nMWidth, nMHeight);
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(PS_SOLID,1,RGB(255,0,0));
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	
	pMemDC->MoveTo(CPoint(nMWidth, 0));
	pMemDC->LineTo(CPoint(nMWidth, nMHeight*2));
	pMemDC->MoveTo(CPoint(0, nMHeight));
	pMemDC->LineTo(CPoint(nMWidth*2, nMHeight));
	ptStart.x = ptStart2.x = ptEnd.x = ptEnd2.x = nMWidth;
	ptStart.y = ptStart2.y = ptEnd.y = ptEnd2.y = nMHeight;
	while (1)
	{
		ptEnd.x = ptStart.x += RULER_GAP;
		ptEnd2.x = ptStart2.x -= RULER_GAP;
		if (ptStart.x > 110 || ptStart2.x < 0)
		{
			break;
		}
		nFlag++;
		nCount++;
		
		if (0 == nFlag % 10)
		{
			ptEnd.y = ptEnd2.y = nMHeight - RULER_10;
			nFlag = 0;
		}
		else if (0 == nFlag % 5)
		{
			ptEnd.y = ptEnd2.y = nMHeight - RULER_5;
		}
		else
		{
			ptEnd.y = ptEnd2.y = nMHeight - RULER_GAP;	
		}
		pMemDC->MoveTo(ptStart);
		pMemDC->LineTo(ptEnd);
		pMemDC->MoveTo(ptStart2);
		pMemDC->LineTo(ptEnd2);
	}
	ptStart.x = ptStart2.x = ptEnd.x = ptEnd2.x = nMWidth;
	ptStart.y = ptStart2.y = ptEnd.y = ptEnd2.y = nMHeight;
	while (1)
	{
		ptEnd.y = ptStart.y += RULER_GAP;
		ptEnd2.y = ptStart2.y -= RULER_GAP;
		if (ptStart.y > 110 || ptStart2.y < 0)
		{
			break;
		}
		nFlag++;
		nCount++;
		
		if (0 == nFlag % 10)
		{
			ptEnd.x = ptEnd2.x = nMWidth + RULER_10;
			nFlag = 0;
		}
		else if (0 == nFlag % 5)
		{
			ptEnd.x = ptEnd2.x = nMWidth + RULER_5;
		}
		else
		{
			ptEnd.x = ptEnd2.x = nMWidth + RULER_GAP;	
		}
		pMemDC->MoveTo(ptStart);
		pMemDC->LineTo(ptEnd);
		pMemDC->MoveTo(ptStart2);
		pMemDC->LineTo(ptEnd2);
	}
	pMemDC->SelectObject(oldPen);
}
int CImageStatic::myCircle(CDC *pMemDC,int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth,int nPenStyle)
{
	if (Is3DView == 1)
	{
		return 0;
	}
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	m_CircleCX *= ImageScale2; 
	m_CircleCY *= ImageScale2; 
	m_CircleR *= ImageScale2; 
	pMemDC->Ellipse(m_CircleCX - m_CircleR, m_CircleCY - m_CircleR,m_CircleCX + m_CircleR, m_CircleCY + m_CircleR);
	pMemDC->SelectObject(oldPen);
	return 0;
}


int CImageStatic::myAngleArc(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor, int nWidth,int nPenStyle)
{
	if (Is3DView == 1)
	{
		return 0;
	}
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	
	ChargeImagePt(Pt1);
	ChargeImagePt(Pt2);
	ChargeImagePt(Pt3);

	Pt1.y = - Pt1.y;
	Pt2.y = - Pt2.y;
	Pt3.y = - Pt3.y;

	Pt1.x *= ImageScale2;
	Pt1.y *= ImageScale2;
	Pt2.x *= ImageScale2;
	Pt2.y *= ImageScale2;
	Pt3.x *= ImageScale2;
	Pt3.y *= ImageScale2;
	

	C3FloatPt m_Center;
	double k1,k2,b1;
	double k3,k4,b3,b4;
	int Dir = 1;
/*	double Angle1,Angle2;*/
	double m_CircleR;

	k1 = (Pt2.y - Pt1.y)/(Pt2.x - Pt1.x);
	k2 = (Pt3.y - Pt2.y)/(Pt3.x - Pt2.x);
	b1 = Pt1.y - k1*Pt1.x;

	k3 = -1/k1;
	k4 = -1/k2;
	b3 = (Pt2.y+Pt1.y)/2 - k3*(Pt2.x +Pt1.x)/2;
	b4 = (Pt3.y+Pt2.y)/2 - k4*(Pt3.x +Pt2.x)/2 ;

	m_Center.x = (b4 - b3)/(k3 - k4);
	m_Center.y = k3*(m_Center.x)+b3;

// 	Angle1 = atan2((Pt1.y - m_Center.y), Pt1.x - m_Center.x)/PI*180;
// 	Angle2 = atan2((Pt3.y - m_Center.y), Pt3.x - m_Center.x)/PI*180;
	m_CircleR = m_Center.Distance2Point(Pt2);


	/*
	double Errs = m_Center.y - (k1*m_Center.x+b1);
// 	Errs = Errs*atan2((Pt2.y -Pt1.y)*-1 ,Pt2.x - Pt1.x );
	double a = Pt2.x - Pt1.x;
	double b = Pt2.y - Pt1.y;
	*/
	double Errs = Pt3.y - (k1*Pt3.x+b1);
	double a = Pt2.x - Pt1.x;
	double b = Pt2.y - Pt1.y;

 	double c = atan2(b,a);
	if (a>=0&&b>=0)
	{
		c = 1;
	}
	else if(a<=0&&b>=0)
	{
		c = -1;
	}
	else if(a<=0&&b<=0)
	{
		c = -1;
	}
	else if(a>=0&&b<=0)
	{
		c = 1;
	}
	Errs = Errs*c;
	m_Center.y  = - m_Center.y;
	CRect m_Rect (m_Center.x - m_CircleR,m_Center.y - m_CircleR,m_Center.x + m_CircleR,m_Center.y + m_CircleR);

	if (Errs<0)//判断方向 顺时针 逆时针
/*	if(dd.direct)*/
	{
		Dir = 1;
		/*pMemDC->SetArcDirection(AD_COUNTERCLOCKWISE);*/
		pMemDC->Arc(m_Rect,CPoint(Pt3.x ,  - Pt3.y),CPoint(Pt1.x,   - Pt1.y));
/*		pMemDC->Arc(m_Rect,CPoint(Pt1.x,  - Pt1.y),CPoint(Pt3.x ,  - Pt3.y));*/
	}
	else
	{
 		Dir = -1;
		/*pMemDC->SetArcDirection(AD_CLOCKWISE);*/
 		pMemDC->Arc(m_Rect,CPoint(Pt1.x,  - Pt1.y),CPoint(Pt3.x ,  - Pt3.y));
		
	}
	pMemDC->SelectObject(oldPen);
	return 0;
}
int CImageStatic::myLine(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsShow )
{
	if (Is3DView == 1)
	{
		return 0;
	}

	if (!IsShow)
	{
		return 0;
	}
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);

	if (Is3DView==0)
	{
		Pt1.x *= ImageScale2; 
		Pt1.y *= ImageScale2; 
		Pt2.x *= ImageScale2; 
		Pt2.y *= ImageScale2;
	}
	
	ChargeImagePt(Pt1);
	ChargeImagePt(Pt2);
	pMemDC->MoveTo(CPoint(Pt1.x,Pt1.y));
	pMemDC->LineTo(CPoint(Pt2.x,Pt2.y));
	pMemDC->SelectObject(oldPen);
	return 0;
}


void CImageStatic::ChargeImagePt(C3FloatPt &Pt)
{
	if(m_ptLP == NULL || dScalx == NULL || dScaly == NULL)
		return ;
	Pt.x = (Pt.x - m_ptLP->left)*(*dScalx);
	Pt.y = (Pt.y - m_ptLP->top)*(*dScaly);


}

int CImageStatic::myRectangle(CDC *pMemDC,LPCRECT lpRect,COLORREF crColor, int nWidth,int nPenStyle)
{
	if (Is3DView == 1)
	{
		return 0;
	}
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	CRect m_Rect = lpRect;
	m_Rect.bottom *= ImageScale2; 
	m_Rect.right *= ImageScale2;
	m_Rect.top *= ImageScale2;
	m_Rect.left *= ImageScale2;
	C3FloatPt pt1, pt2;
	pt1.x = m_Rect.left;
	pt1.y = m_Rect.top;
	pt2.x = m_Rect.right;
	pt2.y = m_Rect.bottom;
	ChargeImagePt(pt1);
	ChargeImagePt(pt2);
	m_Rect.left		=pt1.x ;
	m_Rect.top		=pt1.y ;
	m_Rect.right	=pt2.x ;
	m_Rect.bottom	=pt2.y ;
	pMemDC->Rectangle(m_Rect);
	pMemDC->SelectObject(oldPen);
	return 0;
}

int CImageStatic::myMark(C3FloatPt Pos,int w ,int h ,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw )
{
	if (Is3DView ==2)
	{
		h =  h/ (*dScaly);
		w =  w/ (*dScalx);
	}


	C3FloatPt Pt[4];
	Pt[0].x = Pos.x - w;
	Pt[0].y = Pos.y;
	Pt[0].z = Pos.z;

	Pt[1].x = Pos.x + w;
	Pt[1].y = Pos.y;
	Pt[1].z = Pos.z;

	Pt[2].x = Pos.x ;
	Pt[2].y = Pos.y - h;
	Pt[2].z = Pos.z;

	Pt[3].x = Pos.x ;
	Pt[3].y = Pos.y + h;
	Pt[3].z = Pos.z;
	myLine(Pt[0],Pt[1],crColor,nWidth,nPenStyle,IsDraw);
	myLine(Pt[2],Pt[3],crColor,nWidth,nPenStyle,IsDraw);
	return 0;
}
int CImageStatic::myCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw)
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWCircle m_DrawCircle;
	m_DrawCircle.m_CircleCX = m_CircleCX;
	m_DrawCircle.m_CircleCY = m_CircleCY;
	m_DrawCircle.m_CircleR = m_CircleR;
	m_DrawCircle.m_Color = crColor;
	m_DrawCircle.nPenStyle = nPenStyle;
	m_DrawCircle.nWidth = nWidth;
	m_Circle[m_nIndex].push_back(m_DrawCircle);
	return 0;
}
int CImageStatic::myLine(C3FloatPt Pt1,C3FloatPt Pt2,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw,BOOL IsShow  )
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWLINE m_DrawLine;
	m_DrawLine.Pt1 = Pt1;
	m_DrawLine.Pt2 = Pt2;
	m_DrawLine.m_Color = crColor;
	m_DrawLine.nWidth = nWidth;
	m_DrawLine.nPenStyle = nPenStyle;
	m_DrawLine.IsShow = IsShow;
	m_Lines[m_nIndex].push_back(m_DrawLine);
}


int CImageStatic::myRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw )
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWRECT m_DrawRect;
	m_DrawRect.m_Rect = lpRect;
	m_DrawRect.m_Color = crColor;
	m_DrawRect.nWidth = nWidth;
	m_DrawRect.nPenStyle = nPenStyle;
	m_Rects[m_nIndex].push_back(m_DrawRect);	

}

int CImageStatic::ClearDraw()
{
	m_g_cs.Lock();//加锁@whq
	//TRACE("ClearDraw:index = %d , lockcount=%d\n",m_nIndex,m_g_cs.m_sect.LockCount);
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		m_g_cs.Unlock();//解锁@whq
		Sleep(1);//@whq
		return 0;
	}
	m_Rects[m_nIndex].clear();
	m_Lines[m_nIndex].clear();
	Clear = TRUE;
	ClearString();
	m_Circle[m_nIndex].clear();
	m_AngleArc[m_nIndex].clear();
	m_g_cs.Unlock();//解锁@whq
	//TRACE("cleardraw:index= %d , Unlock:lockcount=%d\n",m_nIndex,m_g_cs.m_sect.LockCount);
	Sleep(1);//@whq
	return 0;
}
int CImageStatic::AddDetDect(LPCRECT lpRect,COLORREF crColor , int nWidth,int nPenStyle, BOOL IsDraw )
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWRECT m_DrawRect;
	m_DrawRect.m_Rect = lpRect;
	m_DrawRect.m_Color = crColor;
	m_DrawRect.nWidth = nWidth;
	m_DrawRect.nPenStyle = nPenStyle;
	DetRects[m_nIndex].push_back(m_DrawRect);
	return 0;
}
int CImageStatic::ClearDetDect()
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DetRects[m_nIndex].clear();
	return 0;
}

int  CImageStatic::AddString(CPoint Pt, CString strText ,int nAngle , COLORREF crColor , 
	int lfHeight , CString strFont ,  int nRadius )
{
	DRAWFont StringTemp;
	StringTemp.Pt = Pt;
	StringTemp.strText = strText;
	StringTemp.nAngle = nAngle;
	StringTemp.crColor = crColor;
	StringTemp.lfHeight = lfHeight;
	StringTemp.strFont = strFont;
	StringTemp.nRadius = nRadius;
	m_String[m_nIndex].push_back(StringTemp);
	return 0;

}
int  CImageStatic::ClearString()
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	m_String[m_nIndex].clear();
	return 0;
}

int CImageStatic::myAngleArc(C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor , int nWidth ,int nPenStyle )
{
	DRAWAngleArc m_AngleArcTemp;
	m_AngleArcTemp.Pt1 = Pt1;
	m_AngleArcTemp.Pt2 = Pt2;
	m_AngleArcTemp.Pt3 = Pt3;
	m_AngleArcTemp.m_Color = crColor;
	m_AngleArcTemp.nWidth = nWidth;
	m_AngleArcTemp.nPenStyle = nPenStyle;
	m_AngleArc[m_nIndex].push_back(m_AngleArcTemp);
	return 1;
}

void CImageStatic::Fun(CDC *pMemDC,int x, int y, CString strText, COLORREF crColor, 
	int lfHeight, CString strFont , int nAngle, int nRadius)
{

	if (Is3DView == 1)
	{
		return ;
	}
	x  *= ImageScale2;
	y  *= ImageScale2;
	pMemDC->SetBkMode(TRANSPARENT);
	pMemDC->SetTextColor(crColor);
	CFont font;
	LOGFONT stFont;
	memset(&stFont,0,sizeof(stFont));
	stFont.lfHeight=lfHeight;
	stFont.lfWeight=FW_NORMAL;
	stFont.lfClipPrecision=CLIP_LH_ANGLES;
	strcpy(stFont.lfFaceName,strFont);
	stFont.lfEscapement=nAngle*10;
	font.CreateFontIndirect(&stFont);
	CFont *oldFont;
	oldFont=pMemDC->SelectObject(&font);
	if(nAngle*10<900)
		pMemDC->TextOut(x+nRadius*cos(nAngle*10/1800*3.14),
		y-nRadius*sin(nAngle*10/1800*3.14),strText);
	if(nAngle*10>=900&&nAngle*10<1800)
		pMemDC->TextOut(x+nRadius*cos(nAngle*10/1800*3.14),
		y-nRadius*sin(nAngle*10/1800*3.14),strText);
	if(nAngle*10>=1800&&nAngle*10<2700)
		pMemDC->TextOut(x+nRadius*cos(nAngle*10/1800*3.14),
		y-nRadius*sin(nAngle*10/1800*3.14),strText);
	if(nAngle*10>=2700&&nAngle*10<=3600)
		pMemDC->TextOut(x+nRadius*cos(nAngle*10/1800*3.14),
		y-nRadius*sin(nAngle*10/1800*3.14),strText);
	pMemDC->SelectObject(oldFont);
	font.DeleteObject ();   
}

void CImageStatic::ChangeImg(IplImage* img)
{
	if(m_pImg != NULL)
		cvReleaseImage(&m_pImg);
	m_pImg = img;
}

