// ImageStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ImageStatic.h"
#include "CvvImage.h"
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
	m_bDrawRuler = FALSE;
	m_bDrawRuler2 = FALSE;
	m_bFlag = FALSE;
	m_pChipPos = NULL;
	m_bIsShowChipPos = FALSE;
	m_bFlagBox = FALSE;
	for (int i=0; i<4; i++)
	{
		m_bDrawRect[i] = FALSE;
		m_rtDraw[i].top = m_rtDraw[i].left = m_rtDraw[i].bottom = m_rtDraw[i].right= 0;
		m_nIndex = i;
		ClearDraw();
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
	m_bIsHighTempDlg = false;



	//缩放平移选项
	m_ROIrect = CDRect(0,0,0,0);
	m_LDownPt = CPoint(-1,-1);

	m_nIndex = 0;
	m_move.m_show = this;
	m_bGL = FALSE;
	m_papa = NULL;
	m_bCanOutImage = FALSE;
	m_bChangeScalar = FALSE;
	m_bCanMove = TRUE;
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
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_MOUSEMOVE()
ON_WM_MOUSEWHEEL()
ON_WM_SIZE()
ON_WM_TIMER()
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
	if(m_bGL)
		ShowImageGL();
	else
		ShowImage();
	// Do not call CStatic::OnPaint() for painting messages
}

void CImageStatic::ShowImage()
{
	static int showing = FALSE;
	if(showing)
		return;
	showing = TRUE;
	int i  =0;
#ifdef STATIC_CV
	if(m_pImg != NULL){
		try
		{
			CRect rect;
			GetWindowRect(&rect);//获取控件相对于屏幕的位置
			ScreenToClient(rect);
			GetClientRect(&rect);  
			SetRect(rect,rect.left,rect.top,rect.right,rect.bottom);  
			CDC *pDC=GetDC();
			HDC hDC=pDC->GetSafeHdc();

			//初始化双缓冲绘图
			CPoint ptCenter;  
			ptCenter = rect.CenterPoint();  
			CDC dcMem; //用于缓冲作图的内存DC  
			CBitmap bmp; //内存中承载临时图象的位图  
			dcMem.CreateCompatibleDC(pDC); //依附窗口DC创建兼容内存DC  
			bmp.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图  
			dcMem.SelectObject(&bmp); //将位图选择进内存DC  
			dcMem.FillSolidRect(rect,pDC->GetBkColor());  //按原来背景填充客户区，不然会是黑色  

			//绘制图像
			if(m_bCanOutImage&&(m_ROIrect.x < 0 || m_ROIrect.y < 0 ||((m_ROIrect.x + m_ROIrect.width)>m_pImg->width) ||((m_ROIrect.y + m_ROIrect.height)>m_pImg->height)))
			{
				//创建一张和ROI一样的图片
				IplImage* tmpForShow = cvCreateImage(cvSize(m_ROIrect.width,m_ROIrect.height),m_pImg->depth,m_pImg->nChannels);
				cvZero(tmpForShow);
				//将原图的矩形转到新的坐标系
				//求两个矩形相交部分
				//转换回原来的坐标系
				CvRect imgSmallRoI;
				imgSmallRoI.x = m_ROIrect.x < 0?0:m_ROIrect.x;
				imgSmallRoI.y = m_ROIrect.y < 0?0:m_ROIrect.y;
				//右边减左边,如果(m_ROIrect.x + m_ROIrect.width)>m_pImg.width右边为m_pimg.width
				imgSmallRoI.width = (((m_ROIrect.x + m_ROIrect.width)>m_pImg->width) ? m_pImg->width:(m_ROIrect.x + m_ROIrect.width))
					- (m_ROIrect.x < 0?0:m_ROIrect.x);
				imgSmallRoI.height = (((m_ROIrect.y + m_ROIrect.height)>m_pImg->height)?m_pImg->height:(m_ROIrect.y + m_ROIrect.height))
					- (m_ROIrect.y < 0?0:m_ROIrect.y);
				imgSmallRoI.width = min(tmpForShow->width,imgSmallRoI.width);
				imgSmallRoI.height = min(tmpForShow->height,imgSmallRoI.height);
				if(imgSmallRoI.width>0&&imgSmallRoI.height>0)
				{
					CvRect imgBigROI;
					imgBigROI.x = m_ROIrect.x < 0?abs(m_ROIrect.x):0;
					imgBigROI.y = m_ROIrect.y < 0?abs(m_ROIrect.y):0;
					imgBigROI.width 	= imgSmallRoI.width ;
					imgBigROI.height 	= imgSmallRoI.height;
					CvRect tmprect = imgBigROI;
					tmprect = imgSmallRoI;
					tmprect = cvRect(m_ROIrect.x,m_ROIrect.y,m_ROIrect.width,m_ROIrect.height);
					cvResetImageROI(m_pImg);
					cvSetImageROI(tmpForShow,imgBigROI);
					cvSetImageROI(m_pImg,imgSmallRoI);
					cvCopy(m_pImg,tmpForShow);
					cvResetImageROI(tmpForShow);
				}
				CvvImage cimg;  
				if(tmpForShow->width>10000||tmpForShow->height>10000)
				{
					cvSetImageROI(tmpForShow,cvRect(0,0,min(tmpForShow->width,9999),min(tmpForShow->height,9999)));
				}
				cimg.CopyOf(tmpForShow);  
				cimg.DrawToHDC(dcMem.GetSafeHdc(),&rect);  
				if(tmpForShow != NULL)
				{
					cvReleaseImage(&tmpForShow);
					tmpForShow = NULL;
				}

			}
			else
			{
				if(m_ROIrect.width>0&&m_ROIrect.height>0)
				{
					CvvImage cimg;  
					cvResetImageROI(m_pImg);
					cvSetImageROI(m_pImg,cvRect(m_ROIrect.x,m_ROIrect.y,m_ROIrect.width,m_ROIrect.height));
					cimg.CopyOf(m_pImg);  
					cimg.DrawToHDC(dcMem.GetSafeHdc(),&rect);  
					cvResetImageROI(m_pImg);
				}
			}


			//绘制其它
			m_nIndex = 0;
			if (m_nIndex >=0 &&m_nIndex<CameraNum)
			{	
				for (i=0; i<m_Rects[m_nIndex].size(); i++)//绘制所有区域
				{
					if(m_bNoShowBox == TRUE)
						break;
					myRectangle(&dcMem,m_Rects[m_nIndex][i].m_Rect,m_Rects[m_nIndex][i].m_Color,m_Rects[m_nIndex][i].nWidth,m_Rects[m_nIndex][i].nPenStyle);
				}
				for (i=0; i<m_Lines[m_nIndex].size(); i++)//绘制所有直线
				{
					myLine(&dcMem,m_Lines[m_nIndex][i].Pt1,m_Lines[m_nIndex][i].Pt2,m_Lines[m_nIndex][i].m_Color,m_Lines[m_nIndex][i].nWidth,m_Lines[m_nIndex][i].nPenStyle,m_Lines[m_nIndex][i].IsShow);
				}
				for (i=0; i< m_Circle[m_nIndex].size(); i++)//绘制文字
				{	
					myCircle(&dcMem,
						m_Circle[m_nIndex][i].m_CircleCX,
						m_Circle[m_nIndex][i].m_CircleCY,
						m_Circle[m_nIndex][i].m_CircleR,
						m_Circle[m_nIndex][i].m_Color,
						m_Circle[m_nIndex][i].nWidth,
						m_Circle[m_nIndex][i].nPenStyle);

				}
				for (i=0; i< DetRects[m_nIndex].size(); i++)//绘制所有检测区域
				{
					if((m_bNoShowCoppeArea == TRUE) || (m_bNoShowSheetArea == TRUE))
						continue;
					myRectangle(&dcMem,DetRects[m_nIndex][i].m_Rect,DetRects[m_nIndex][i].m_Color,DetRects[m_nIndex][i].nWidth,DetRects[m_nIndex][i].nPenStyle);
				}
				for (i=0; i< m_String[m_nIndex].size(); i++)//绘制文字
				{	
					Fun(&dcMem,
						m_String[m_nIndex][i].Pt.x,
						m_String[m_nIndex][i].Pt.y,
						m_String[m_nIndex][i].strText,
						m_String[m_nIndex][i].crColor,
						m_String[m_nIndex][i].lfHeight,
						m_String[m_nIndex][i].strFont,
						m_String[m_nIndex][i].nAngle,
						m_String[m_nIndex][i].nRadius);
				}
				for (i=0; i< m_AngleArc[m_nIndex].size(); i++)//圆弧
				{	

					myAngleArc(&dcMem,
						m_AngleArc[m_nIndex][i].Pt1,
						m_AngleArc[m_nIndex][i].Pt2,
						m_AngleArc[m_nIndex][i].Pt3,
						m_AngleArc[m_nIndex][i].m_Color,
						m_AngleArc[m_nIndex][i].nWidth,
						m_AngleArc[m_nIndex][i].nPenStyle);

				}
			}// end of if (m_nIndex >=0 &&m_nIndex<CameraNum)

			//双缓冲绘图结束,释放资源
			pDC->BitBlt(0,0,rect.Width(),rect.Height(),  
				&dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台  
			dcMem.DeleteDC(); //删除DC  
			bmp.DeleteObject(); //删除位图  
			ReleaseDC(pDC); 
		}
		catch(cv::Exception e)
		{
			AfxMessageBox(e.msg.c_str());
		}
		}//end of if(m_pImg != NULL)
	else
	{
		CRect rect;
		GetWindowRect(&rect);//获取控件相对于屏幕的位置
		ScreenToClient(rect);
		GetClientRect(&rect);  
		CClientDC dc(this);  
		CBrush brush1(RGB(0,0,0));
		CBrush* pOldBrush=dc.SelectObject(&brush1);       
		dc.Rectangle(0,0,rect.Width(),rect.Height());  
		dc.SelectObject(pOldBrush);  
	}
	showing = FALSE;
	Sleep(100);
#endif
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
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	CFloatPt center,cpt2;
	CFloatPt pt = CFloatPt(m_CircleCX,m_CircleCY);
	PointChange(pt,center,1);
	m_CircleR *= ImageScale2; 
	//if(center.x>=0&&center.y>=0)
	pMemDC->Ellipse(center.x - m_CircleR, center.y - m_CircleR,center.x + m_CircleR, center.y + m_CircleR);
	pMemDC->SelectObject(oldPen);
	return 0;
}


int CImageStatic::myAngleArc(CDC *pMemDC,C3FloatPt Pt1,C3FloatPt Pt2,C3FloatPt Pt3,COLORREF crColor, int nWidth,int nPenStyle)
{
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);

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

	if (!IsShow)
	{
		return 0;
	}
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	CFloatPt pt = CFloatPt(Pt1.x,Pt1.y);
	CFloatPt cpt1,cpt2;
	PointChange(pt,cpt1,1);
	pt = CFloatPt(Pt2.x,Pt2.y);
	PointChange(pt,cpt2,1);
	pMemDC->MoveTo(CPoint(cpt1.x,cpt1.y));
	pMemDC->LineTo(CPoint(cpt2.x,cpt2.y));
	pMemDC->SelectObject(oldPen);
	return 0;
}



int CImageStatic::myRectangle(CDC *pMemDC,LPCRECT lpRect,COLORREF crColor, int nWidth,int nPenStyle)
{
	CPen Pen;//　画笔	
	CPen *oldPen;
	Pen.CreatePen(nPenStyle,nWidth,crColor);
	oldPen = pMemDC->SelectObject(&Pen);
	pMemDC->SelectStockObject(NULL_BRUSH);
	CRect m_Rect = lpRect;
	CFloatPt tpt1,tpt2,tpt3;
	tpt1 = CFloatPt(((CRect)lpRect).TopLeft());
	PointChange(tpt1,tpt2,1);
	tpt1 = CFloatPt(((CRect)lpRect).BottomRight());
	PointChange(tpt1,tpt3,1);
	m_Rect = CRect(tpt2.x,tpt2.y,tpt3.x,tpt3.y);
	pMemDC->Rectangle(m_Rect);
	pMemDC->SelectObject(oldPen);
	return 0;
}

int CImageStatic::ChangingMyMark(C3FloatPt Pos,int w ,int h ,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw )
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
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
	for(int i = 0; i < 4; i++)
		PointChange(Pt[i],Pt[i],0);



	DRAWLINE m_DrawLine;
	m_DrawLine.Pt1 = Pt[0];
	m_DrawLine.Pt2 = Pt[1];
	m_DrawLine.m_Color = crColor;
	m_DrawLine.nWidth = nWidth;
	m_DrawLine.nPenStyle = nPenStyle;
	m_DrawLine.IsShow = IsDraw;
	m_Lines[m_nIndex][0] = m_DrawLine;
	m_DrawLine.Pt1 = Pt[2];
	m_DrawLine.Pt2 = Pt[3];
	m_Lines[m_nIndex][1] = m_DrawLine;
	ShowImage();
	return 0;
}
int CImageStatic::myMark(C3FloatPt Pos,int w ,int h ,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw )
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
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
	for(int i = 0; i < 4; i++)
		PointChange(Pt[i],Pt[i],0);
	DRAWLINE m_DrawLine;
	m_DrawLine.Pt1 = Pt[0];
	m_DrawLine.Pt2 = Pt[1];
	m_DrawLine.m_Color = crColor;
	m_DrawLine.nWidth = nWidth;
	m_DrawLine.nPenStyle = nPenStyle;
	m_DrawLine.IsShow = IsDraw;
	m_Lines[m_nIndex].push_back(m_DrawLine);
	m_DrawLine.Pt1 = Pt[2];
	m_DrawLine.Pt2 = Pt[3];
	m_Lines[m_nIndex].push_back(m_DrawLine);
	ShowImage();
	return 0;
}
int CImageStatic::ChangingMyCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw)
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWCircle m_DrawCircle;

	CFloatPt center,cpt2;
	CFloatPt pt = CFloatPt(m_CircleCX,m_CircleCY);
	PointChange(pt,center,0);

	m_DrawCircle.m_CircleCX = center.x;
	m_DrawCircle.m_CircleCY = center.y;
	m_DrawCircle.m_CircleR = m_CircleR/ImageScale2;
	m_DrawCircle.m_Color = crColor;
	m_DrawCircle.nPenStyle = nPenStyle;
	m_DrawCircle.nWidth = nWidth/ImageScale2;
	m_Circle[m_nIndex][0] = m_DrawCircle;
	return 0;
}
int CImageStatic::myCircle(int m_CircleCX,int m_CircleCY,int m_CircleR,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw)
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWCircle m_DrawCircle;

	CFloatPt center,cpt2;
	CFloatPt pt = CFloatPt(m_CircleCX,m_CircleCY);
	PointChange(pt,center,0);

	m_DrawCircle.m_CircleCX = center.x;
	m_DrawCircle.m_CircleCY = center.y;
	m_DrawCircle.m_CircleR = m_CircleR/ImageScale2;
	m_DrawCircle.m_Color = crColor;
	m_DrawCircle.nPenStyle = nPenStyle;
	m_DrawCircle.nWidth = nWidth/ImageScale2;
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
	CFloatPt cpt1,cpt2;
	CFloatPt pt = CFloatPt(Pt1.x,Pt1.y);
	PointChange(pt,cpt1,0);
	pt = CFloatPt(Pt2.x,Pt2.y);
	PointChange(pt,cpt2,0);
	m_DrawLine.Pt1 = C3FloatPt(cpt1.x,cpt1.y,0);
	m_DrawLine.Pt2 = C3FloatPt(cpt2.x,cpt2.y,0);
	m_DrawLine.m_Color = crColor;
	m_DrawLine.nWidth = nWidth;
	m_DrawLine.nPenStyle = nPenStyle;
	m_DrawLine.IsShow = IsShow;
	m_Lines[m_nIndex].push_back(m_DrawLine);
}
void CImageStatic::PointChange(C3FloatPt srcpt,C3FloatPt &dstpt,int changeType){
	if(scalx == 0|| scaly == 0 || ImageScale2 == 0)
	{
		return;
	}
	//假如是cv转crect
	if(changeType == 1){
		//减去ROI起始点,获得偏移量
		double dmovex = (srcpt.x - m_ROIrect.x);//<0? 0 : (pt.x - m_ROIrect.x);
		double dmovey = (srcpt.y - m_ROIrect.y);//<0? 0 : (pt.y - m_ROIrect.y);
		//计算这段距离等于多少的控件距离
		dstpt.x = dmovex*scalx*ImageScale2;
		dstpt.y = dmovey*scaly*ImageScale2;
	}else if(changeType == 0){//转cv
		double width = Width();
		//计算这段距离等于多少的控件距离
		dstpt.x = srcpt.x/scalx/ImageScale2 + m_ROIrect.x;
		dstpt.y = srcpt.y/scaly/ImageScale2 + m_ROIrect.y;
	}

}
void CImageStatic::PointChange(CFloatPt srcpt,CFloatPt &dstpt,int changeType){
	if(scalx == 0|| scaly == 0 || ImageScale2 == 0)
	{
		return;
	}
	//假如是cv转crect
	if(changeType == 1){
	//减去ROI起始点,获得偏移量
		double dmovex = (srcpt.x - m_ROIrect.x);//<0? 0 : (pt.x - m_ROIrect.x);
		double dmovey = (srcpt.y - m_ROIrect.y);//<0? 0 : (pt.y - m_ROIrect.y);
	//计算这段距离等于多少的控件距离
		dstpt.x = dmovex*scalx*ImageScale2;
		dstpt.y = dmovey*scaly*ImageScale2;
	}else if(changeType == 0){//转cv
		double width = Width();
	//计算这段距离等于多少的控件距离
		dstpt.x = srcpt.x/scalx/ImageScale2 + m_ROIrect.x;
		dstpt.y = srcpt.y/scaly/ImageScale2 + m_ROIrect.y;
	}

}

int CImageStatic::myRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth,int nPenStyle,BOOL IsDraw )
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWRECT m_DrawRect;
	CFloatPt tmpcvpttl;
	CFloatPt tmpcvptrb;
	CFloatPt pt = CFloatPt(((CRect)lpRect).TopLeft());
	PointChange(pt,tmpcvpttl,0);
	pt = ((CRect)lpRect).BottomRight();
	PointChange(pt,tmpcvptrb,0);
	CRect cvre = CRect(tmpcvpttl.x,tmpcvpttl.y,tmpcvptrb.x,tmpcvptrb.y);
	
	m_DrawRect.m_Rect = cvre;
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
	m_Circle[m_nIndex].clear();
	m_AngleArc[m_nIndex].clear();

	m_Rects[m_nIndex].push_back(DRAWRECT());
	m_Circle[m_nIndex].push_back(DRAWCircle());
	m_Lines[m_nIndex].push_back(DRAWLINE());
	m_Lines[m_nIndex].push_back(DRAWLINE());
	m_Lines[m_nIndex].push_back(DRAWLINE());
	ClearString();
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

void CImageStatic::ChangeImg(IplImage* img,BOOL bchangescale)
{
	if(m_pImg != img&&m_pImg != NULL)
		cvReleaseImage(&m_pImg);
	m_pImg = img;
	CRect rect;
	GetClientRect(&rect);
	ImageScale2 = 1;
	m_bChangeScalar = bchangescale;
	if(bchangescale == FALSE){
		m_bCanOutImage = TRUE;
		BOOL widths = TRUE;
		for (int i = 0; i < 2;i++)
		{
			if(widths)
			{
				int roiwidth = m_pImg->width;
				scaly = scalx = (double)rect.Width()/(double)roiwidth;//m_pImg->width;
				m_ROIrect = CDRect(0,0,roiwidth,rect.Height()/scaly);
			}
			else
			{
				int roiwidth = m_pImg->height;
				scaly = scalx = (double)rect.Height()/(double)roiwidth;//m_pImg->width;
				m_ROIrect = CDRect(0,0,rect.Width()/scalx,roiwidth);
			}
			if(m_ROIrect.width > m_pImg->width||m_ROIrect.height > m_pImg->height)
			{
				widths = FALSE;
			}
			else
			{
				break;
			}
		}
	}
	else if(bchangescale == 2)
	{
		m_bCanOutImage = TRUE;
		scalx = 1.0;
		scaly = 1.0;
		m_ROIrect = CDRect(0,0,rect.Width(),rect.Height());
	}
	else
	{
		m_ROIrect = CDRect(0,0,m_pImg->width,m_pImg->height);
		scalx = (double)rect.Width()/m_pImg->width;
		scaly = (double)rect.Height()/m_pImg->height;
		m_bCanMove = FALSE;
	}
	ClearDraw();
	//m_ROIrect = CDRect(0,0,m_pImg->width,m_pImg->height);
	if(m_bGL)
		m_gl3D.ResizeBack();
	m_bGL = FALSE;

	ShowImage();
}

void CImageStatic::ChangeImg(CString path,BOOL bchangescale)
{
	IplImage *tmp = cvLoadImage(path);
	if(m_pImg != NULL)
		cvReleaseImage(&m_pImg);
	if(tmp != NULL){
		ChangeImg(tmp,bchangescale);
	}


}



void CImageStatic::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!CheckValue())
	{
		return;
	}
	if(m_bGL)
	{
		m_gl3D.OnLButtonDown(nFlags,point);
		return;
	}
		
	m_LDownPt = point;
}


void CImageStatic::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!CheckValue())
	{
		return;
	}
	if(m_bGL)
	{
		m_gl3D.OnLButtonUp(nFlags,point);
		return;
	}
	m_LDownPt = CPoint(-1,-1);
}


void CImageStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!CheckValue())
	{
		return;
	}
	if(m_bGL)
	{
		m_gl3D.OnMouseMove(nFlags,point);
		return;
	}
	if (m_LDownPt.x > 0 && m_LDownPt.y > 0)
	{
		int movex = point.x - m_LDownPt.x;
		int movey = point.y - m_LDownPt.y;
		MoveDis(movex, movey);

		m_LDownPt = point;

		ShowImage();
	}
}


BOOL CImageStatic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!CheckValue())
	{
		return FALSE;
	}
	if(m_bGL)
	{
		m_gl3D.OnMouseWheel(nFlags,zDelta,pt);
		return FALSE;
	}
	double tmpscale;
	int abs1 = 0;
	if(zDelta!=0)
		abs1 = zDelta/abs(zDelta);
	else
		return FALSE;
	tmpscale = ImageScale2*100.0 + abs1*sqrt(ImageScale2*100.0);
	tmpscale /= 100.0;
// 	if(tmpscale < 1)
// 		return FALSE;

	//计算新的ROI,前半部分表示放大缩小带来的宽度变化带来的图像像素变化

	double movex = -Width()/scalx /2.0/ tmpscale + Width()/scalx /2.0/ImageScale2 + m_ROIrect.x;
	double movey = -Height()/scaly /2.0/ tmpscale + Height()/scaly /2.0/ImageScale2 + m_ROIrect.y;
	double width = Width()/scalx / tmpscale;
	double height = Height()/scaly / tmpscale;

	//控件宽高表示的实际长度
	BOOL bmovex = FALSE;
	BOOL bmovey = TRUE;
	BOOL bValid = FALSE;
	if(movex + width <= m_pImg->width){//图像与右边界比较
		if (movey + height<=m_pImg->height)
		{
			if (movex >= 0)
			{
				if (movey >= 0)
				{
					bValid = TRUE;
				}
				else
				{
					bmovey = TRUE;
				}
			}
			else
			{
				bmovex = TRUE;
			}
		}
		else//移动y
		{
			bmovey = TRUE;
		}
	}
	else//移动x
	{
		bmovex = TRUE;
	}
	if(m_bCanOutImage||bValid)
	{
		if(width > 0&&height>0&&width < 9999&&height<9999)
		{
			m_ROIrect.x = movex;
			m_ROIrect.y = movey;
			m_ROIrect.width = width;
			m_ROIrect.height = height;
			ImageScale2 = tmpscale;
			ShowImage();
		}
		else
		{
			int a = 0;
		}
	}
	if(m_bChangeScalar&&(bmovex||bmovey))
	{
		//首先新的宽高要比原始的小
		if(width > m_pImg->width||height > m_pImg->height)
			return FALSE;
		//计算新的位置
		if(movex < 0)
			movex = 0;
		if (movex + width > m_pImg->width)
		{
			movex = m_pImg->width - width;
		}
		if(movey < 0)
			movey = 0;
		if (movey + height > m_pImg->height)
		{
			movey = m_pImg->height - height;
		}
		m_ROIrect.x = movex;
		m_ROIrect.y = movey;
		m_ROIrect.width = width;
		m_ROIrect.height = height;
		ImageScale2 = tmpscale;
		ShowImage();
	}

}

int CImageStatic::Width()
{
	CRect m_ShowRC;
	GetWindowRect(m_ShowRC);
	ScreenToClient(m_ShowRC);
	return m_ShowRC.Width();
}

int CImageStatic::Height()
{
	CRect m_ShowRC;
	GetWindowRect(m_ShowRC);
	ScreenToClient(m_ShowRC);
	return m_ShowRC.Height();
}

void CImageStatic::ChangingInit()
{
	m_Rects[m_nIndex][0] = DRAWRECT();
	m_Circle[m_nIndex][0] = DRAWCircle();
	m_Lines[m_nIndex][0] = DRAWLINE();
	m_Lines[m_nIndex][1] = DRAWLINE();
	m_Lines[m_nIndex][2] = DRAWLINE();
	ShowImage();
}

int CImageStatic::ChangingMyLine(C3FloatPt Pt1, C3FloatPt Pt2,COLORREF crColor, int nWidth/*=1*/,int nPenStyle/*=0*/,BOOL IsDraw /*= TRUE*/,BOOL IsShow /*= TRUE*/)
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWLINE m_DrawLine;
	CFloatPt cpt1,cpt2;
	CFloatPt pt = CFloatPt(Pt1.x,Pt1.y);
	PointChange(pt,cpt1,0);
	pt = CFloatPt(Pt2.x,Pt2.y);
	PointChange(pt,cpt2,0);
	m_DrawLine.Pt1 = C3FloatPt(cpt1.x,cpt1.y,0);
	m_DrawLine.Pt2 = C3FloatPt(cpt2.x,cpt2.y,0);
	m_DrawLine.m_Color = crColor;
	m_DrawLine.nWidth = nWidth;
	m_DrawLine.nPenStyle = nPenStyle;
	m_DrawLine.IsShow = IsShow;
	m_Lines[m_nIndex][2] = m_DrawLine;
}

int CImageStatic::ChangingMyRectangle(LPCRECT lpRect,COLORREF crColor, int nWidth/*=1*/,int nPenStyle/*=0*/,BOOL IsDraw /*= TRUE*/)
{
	if (m_nIndex<0 ||m_nIndex>CameraNum-1)
	{
		return 0;
	}
	DRAWRECT m_DrawRect;
	CFloatPt tmpcvpttl;
	CFloatPt tmpcvptrb;
	CFloatPt pt = CFloatPt(((CRect)lpRect).TopLeft());
	PointChange(pt,tmpcvpttl,0);
	pt = ((CRect)lpRect).BottomRight();
	PointChange(pt,tmpcvptrb,0);
	CRect cvre = CRect(tmpcvpttl.x,tmpcvpttl.y,tmpcvptrb.x,tmpcvptrb.y);

	m_DrawRect.m_Rect = cvre;
	m_DrawRect.m_Color = crColor;
	m_DrawRect.nWidth = nWidth;
	m_DrawRect.nPenStyle = nPenStyle;
	m_Rects[m_nIndex][0] = m_DrawRect;
	ShowImage();
}

void CImageStatic::ChangingStyle(int changingType)
{
	m_move.m_bDrawingDeteRC = changingType;
}

void CImageStatic::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
// #if USE_OPENGL == 1
// 	m_gl3D.OnSize(cx,cy);
// #endif
	
}



void MovePara::MyLButtonDown(UINT nFlags, CWnd* papa,CPoint point)
{
	CRect PicRect;
	if (m_ShowRC.PtInRect(point))
	{
		if(m_bDrawingDeteRC){
			point.x-=m_ShowRC.left;
			point.y-=m_ShowRC.top;
			m_bLbtnDown = TRUE;
			m_stPt.x=m_endPt.x=point.x;///m_show.ImageScale2+0.5;
			m_stPt.y=m_endPt.y=point.y;///m_show.ImageScale2+0.5;

			m_show->ShowImage();
			return;
		}
		else if(TranParentToChild(*m_show,papa, point))
			m_show->OnLButtonDown(nFlags,point);

	} 
}

void MovePara::MyMouseMove(UINT nFlags, CWnd* papa, CPoint point)
{
	int radi = 0;
	double dislength  = 0;
	if (m_ShowRC.PtInRect(point))
	{
		switch (m_bDrawingDeteRC)
		{
		case 0:
			if(TranParentToChild(*m_show,papa, point)){
				m_show->OnMouseMove(nFlags,point);
			}
			break;
		case 1:
			point.x-=m_ShowRC.left;
			point.y-=m_ShowRC.top;
			m_endPt.x=point.x;///m_show.ImageScale2+0.5;
			m_endPt.y=point.y;///m_show.ImageScale2+0.5;
			m_show->ChangingMyMark(C3FloatPt(m_endPt.x,m_endPt.y,0));
			//m_show.myRectangle(CRect(m_stPt,m_endPt),RGB(255,255,255));
			radi = min((m_endPt.x-m_stPt.x)/2.0,(m_endPt.y-m_stPt.y)/2.0);
			dislength = sqrt(pow((m_endPt.x-m_stPt.x),2.0)+pow((m_endPt.y-m_stPt.y),2.0));
			if(m_bLbtnDown)
				m_show->ChangingMyCircle(m_stPt.x,m_stPt.y,dislength);
				//m_show->ChangingMyCircle(m_stPt.x + radi,m_stPt.y + radi,radi);
			break;
		case 2:
			point.x-=m_ShowRC.left;
			point.y-=m_ShowRC.top;
			m_endPt.x=point.x;///m_show.ImageScale2+0.5;
			m_endPt.y=point.y;///m_show.ImageScale2+0.5;
			if(m_bLbtnDown)
				m_show->ChangingMyRectangle(CRect(m_stPt,m_endPt),RGB(255,0,0));
			break;
		}

	}
	else if(m_bLbtnDown){
		if(TranParentToChild(*m_show,papa, point))
			m_show->OnLButtonUp(nFlags,point);
	}
}

void MovePara::MyLButtonUp(UINT nFlags, CWnd* papa , CPoint point)
{
	double radi = 0;
	if(m_bLbtnDown&&m_bDrawingDeteRC){
		CRect rect(m_stPt,m_endPt);

		m_show->ChangingInit();
		switch(m_bDrawingDeteRC){
		case 0:
			break;
		case 1:
			radi = min((m_endPt.x-m_stPt.x)/2.0,(m_endPt.y-m_stPt.y)/2.0);
			radi = sqrt(pow((m_endPt.x-m_stPt.x),2.0)+pow((m_endPt.y-m_stPt.y),2.0));
			m_show->myCircle(m_stPt.x,m_stPt.y,radi);
			//m_show->myCircle(m_stPt.x + radi,m_stPt.y + radi,radi);
			break;
		case 2:
			m_show->myRectangle(CRect(m_stPt,m_endPt),RGB(255,0,0));
			break;
		}
		m_bLbtnDown = FALSE;
		m_bDrawingDeteRC = FALSE;
	}

	if(TranParentToChild(*m_show,papa, point)){
		m_show->OnLButtonUp(nFlags,point);
		m_show->ChangingInit();
	}
}

BOOL MovePara::MyMouseWheel(UINT nFlags, CWnd* papa , short zDelta, CPoint pt)
{
	if(TranParentToChild(*m_show,papa, pt,1))
		m_show->OnMouseWheel(nFlags, zDelta, pt);
	return TRUE;
}

void MovePara::MyInit(CWnd* papa)
{
	m_show->GetWindowRect(m_ShowRC);
	papa->ScreenToClient(m_ShowRC);
};


/************************************************************************/
/*                           opengl 3D显示                              */
/************************************************************************/

void CImageStatic::ChangeImgGL(cv::Mat img)
{
	if(CV_8U == img.depth())
	{
		m_gl3D.m_pic = img.clone();
	}
	else if(CV_32F == img.depth())
	{
		m_gl3D.m_pic3D = img.clone();
	}
	else
	{
		return;
	}
	m_gl3D.GetMinMax();
	if(!m_bGL)
		m_gl3D.Resize();
	m_bGL = TRUE;
	ClearDraw();
	ShowImageGL();
}

void CImageStatic::ShowImageGL()
{
	m_gl3D.DrawImage();
}


void GL3DPara::OnSize(int X,int Y)
{
	m_viewx=X;
	m_viewy=Y;
}
void GL3DPara::LM()
{
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);

	GLfloat lightambient[]={0.1f, 0.1f, 0.1f, 0.0f};
	GLfloat lightdiffuse[]={0.1f, 0.1f, 0.1f, 0.0f};
	GLfloat lightposition[]={0.0,0.0,0.0,0.0};
	GLfloat lightSpecular[]={0.1f, 0.1f, 0.1f, 0.0f};

	glLightfv(GL_LIGHT0,GL_AMBIENT,lightambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightdiffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightSpecular); 
	glLightfv(GL_LIGHT0,GL_POSITION,lightposition); 


	glDisable(GL_LIGHTING);

	GLfloat mat_specular[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat mat_diffuse[] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat mat_shininess[] = { 10000.f };
	GLfloat mat_ambient[] = { 0.8f, 0.8f, 0.8f, 1.f };
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient );

	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
	glShadeModel( GL_SMOOTH );

	glDisable(GL_LIGHTING);

}
HFONT hFont; 
void myFont()
{
	CFont *m_pFontSelected=NULL; 
	LOGFONT lf ; 
	memset(&lf,0,sizeof(LOGFONT)); 
	lf.lfHeight = 0 ; 
	lf.lfWidth = 0 ; 
	lf.lfEscapement = 0 ; 
	lf.lfOrientation = 0 ; 
	lf.lfWeight = FW_NORMAL ; 
	lf.lfItalic = FALSE ; 
	lf.lfUnderline = FALSE ; 
	lf.lfStrikeOut = FALSE ; 
	lf.lfCharSet = ANSI_CHARSET ; 
	lf.lfOutPrecision = OUT_TT_PRECIS ; 
	lf.lfClipPrecision= CLIP_DEFAULT_PRECIS ; 
	lf.lfQuality = PROOF_QUALITY ; 
	lf.lfPitchAndFamily = VARIABLE_PITCH | TMPF_TRUETYPE | FF_MODERN ; 
	lstrcpy (lf.lfFaceName, "Arial") ; 
	//设置当前字体
	hFont = CreateFontIndirect(&lf);
}
void   glDrawString(char *str)     
{  

	SelectObject(wglGetCurrentDC(),hFont);
	GLYPHMETRICSFLOAT agmf[128]; 
	unsigned int ich=0; 
	unsigned int j=0; 
	char COText[128]; 
	const GLuint FListBase=1; 
	unsigned int i=0; 
	int nListNum;

	strcpy(COText,str); 
	i=0; 

	while(i<strlen((char   *)COText)){ ///判断是否为双字节 
		if (IsDBCSLeadByte(COText[i])){
			ich=COText[i]; 
			ich=(ich<<8)+256; ////256为汉字内码“偏移量” 
			ich=ich+COText[i+1]; 
			i++;
		} 
		else{ 
			ich=COText[i];
		}
		nListNum = glGenLists(1);
		wglUseFontOutlines(wglGetCurrentDC(), /////字体轮廓设备联系DC 
			ich, /////要转换为显示列表的第一个字符 
			1, /////要转换为显示列表的字符数 
			nListNum , /////显示列表的基数 
			0.0f, /////指定与实际轮廓的最大偏移量 
			0.05f, /////在Z轴负方向的值 
			1, /////指定显示列表线段或多边形 
			&agmf[1]); /////接受字符的地址 
		i++; 
		glCallList(nListNum); glDeleteLists(nListNum, 1);
	}
}
void GL3DPara::DrawCoordinate()
{
	GLUquadricObj* quadObj;
	float dLen;
	dLen = 100.0;
	// 	glLineWidth(1.0f);
	glColor3f(0.0,1.0,0.0);
	//坐标线
	glBegin(GL_LINES);
	glColor3f(0.0,0.0,1.0);
	glVertex3d(0.0,0.0,0.0);
	glVertex3d(0.0,0.0,dLen);

	glColor3f(1.0,0.0,0.0);    
	glVertex3d(0.0,0.0,0.0);
	glVertex3d(dLen,0.0,0.0);

	glColor3f(0.0f,0.80f,0.8f); 
	glVertex3d(0.0,0.0,0.0);
	glVertex3d(0.0,dLen,0.0);
	glEnd();
	//***********************标记x,y,z轴********************************//
	glPushMatrix();
	glTranslatef(dLen+10,0.0,0.0);
	glRotatef(m_RotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_RotY, 0.0f, 1.0f, 0.0f);
	// 		double tempx;
	// 		if(m_RotX != 0)
	// 			tempx = sqrt(fabs(m_RotZ*m_RotZ - m_RotX*m_RotX)) * m_RotX / fabs(m_RotX);
	// 		else
	// 			tempx = m_RotZ;
	// 		glRotatef(tempx, 1.0f, 0.0f, 0.0f);
	// 		double tempy;
	// 		if(m_RotY != 0)
	// 			tempy = sqrt(fabs(m_RotZ*m_RotZ - m_RotY*m_RotY)) * m_RotY / fabs(m_RotY);
	// 		else
	// 			tempy = m_RotZ;
	// 		glRotatef(tempy, 0.0f, 1.0f, 0.0f);
	//		glRotatef(sqrt(fabs(m_RotZ*m_RotZ - m_RotY*m_RotY)), 0.0f, 1.0f, 0.0f);
	//		glRotatef(90,1.f,0.f,0.f);
	glScalef(20.0,20.0,20.0);
	glColor3f(0.0f,0.80f,0.0f); // 根据字体位置设置颜色
	glDrawString("x");
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(0.0,dLen+10,0.0);
	glRotatef(m_RotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_RotY, 0.0f, 1.0f, 0.0f);
	//		glRotatef(m_RotZ, 0.0f, 1.0f, 0.0f);
	//		glRotatef(90,1.f,0.f,0.f);
	//		glRotatef(180,0.f,1.f,0.f);
	glScalef(20.0,20.0,20.0);
	glColor3f(0.0f,0.80f,0.8f); // 根据字体位置设置颜色
	glDrawString("y");
	glPopMatrix(); 

	glPushMatrix();
	glTranslatef(0.0,-10.0,dLen+10);
	//		glRotatef(-90,1.f,0.f,0.f);
	glRotatef(m_RotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_RotY, 0.0f, 1.0f, 0.0f);
	//		glRotatef(m_RotZ, 0.0f, 1.0f, 0.0f);
	glScalef(20.0,20.0,20.0);
	glColor3f(0.0f,0.80f,0.0f); // 根据字体位置设置颜色
	glDrawString("z");
	glPopMatrix(); 
	//********************************************************************//
	//圆锥
	//z方向
	glPushMatrix();
	glColor3f(0.0,0.0,1.0);
	glTranslatef(0.0f,0.0f,dLen);
	quadObj=gluNewQuadric();
	gluQuadricDrawStyle(quadObj,GLU_FILL);
	gluQuadricNormals(quadObj,GLU_FLAT);
	gluCylinder(quadObj,2.0,0.0,10.0,6,6);
	gluDeleteQuadric(quadObj);
	glPopMatrix();
	//y方向
	glPushMatrix();
	glColor3f(0.0f,0.80f,0.8f); 
	glTranslatef(0.0,dLen,0.0);
	glRotatef(-90.0,1.0,0.0,0.0);
	quadObj=gluNewQuadric();
	gluQuadricDrawStyle(quadObj,GLU_FILL);
	gluQuadricNormals(quadObj,GLU_FLAT);
	gluCylinder(quadObj,2.0,0.0,10.0,6,6);
	gluDeleteQuadric(quadObj);
	glPopMatrix();
	//x方向
	glPushMatrix();
	glColor3f(1.0,0.0,0.0);
	glTranslatef(dLen,0.0,0.0);
	glRotatef(90.0,0.0,1.0,0.0);
	quadObj=gluNewQuadric();
	gluQuadricDrawStyle(quadObj,GLU_FILL);
	gluQuadricNormals(quadObj,GLU_FLAT);
	gluCylinder(quadObj,2.0,0.0,10.0,6,6);
	gluDeleteQuadric(quadObj);
	glPopMatrix();

}
void GL3DPara::DrawImage()
{
	static BOOL  bBusy;		//检测是否正在绘制
	if(bBusy) 	return;
	bBusy = TRUE;
	DrawBar();
	OnSize(papaRect.Width(),papaRect.Height());
	glClearColor(1.0,1.0,1.0,1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);      //clear color and depth buffers
	glClearDepth(1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);


	glViewport(0, 0, m_viewx, m_viewy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0*(GLfloat)m_viewx/(GLfloat)m_viewy, 1.0, 20000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();  	
	//reset modelview matrix
	gluLookAt( m_camera.vEye.x,		
		m_camera.vEye.y,		
		m_camera.vEye.z,
		m_camera.vCenter.x,	
		m_camera.vCenter.y,	
		m_camera.vCenter.z,
		m_camera.vUp.x,		
		m_camera.vUp.y,		
		m_camera.vUp.z);
	LM();	
	glPushMatrix();
	DrawCoordinate();
	DrawObj();	
	glPopMatrix();
	glFinish();
	SwapBuffers(wglGetCurrentDC());  
	//m_father->MoveWindow(papaRect);
	bBusy = FALSE;
}
BOOL GL3DPara::bSetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL|            // support OpenGL
		PFD_DOUBLEBUFFER,             // double buffered,			
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		32,                             // 32-bit z-buffer
		1,                              // no stencil buffer
		1,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		1,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	int pixelformat;

	if ( (pixelformat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd)) == 0 )
	{
		AfxMessageBox("ChoosePixelFormat failed");
		return FALSE;
	}

	if (SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd) == FALSE)
	{
		AfxMessageBox("SetPixelFormat failed");
		return FALSE;
	}

	return TRUE;

}
void GL3DPara::Init(CWnd *father)
{
	m_father = father;
	PIXELFORMATDESCRIPTOR pfd;
	int         n;
	HGLRC		hrc;
	m_pDC = new CClientDC(m_father);
	ASSERT(m_pDC != NULL);


	if (!bSetupPixelFormat())
		return;

	n =::GetPixelFormat(m_pDC->GetSafeHdc());
	::DescribePixelFormat(m_pDC->GetSafeHdc(), n, sizeof(pfd), &pfd);
	hrc = wglCreateContext(m_pDC->GetSafeHdc());
	wglMakeCurrent(m_pDC->GetSafeHdc(), hrc);
	myFont();
	hHandWantGrab		= LoadCursorFromFile("res\\cursor1.cur");
	return;
}

void GL3DPara::Release()
{
	HGLRC	hrc;
	hrc = ::wglGetCurrentContext();
	::wglMakeCurrent(NULL,  NULL);
	if (hrc)
		::wglDeleteContext(hrc);
	if (m_pDC)
		delete m_pDC;
	m_father = NULL;
	if(m_cs!=NULL)
	{
		delete m_cs;
		m_cs = NULL;
	}
}

void GL3DPara::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	SetCursor( hHandWantGrab ); 
	m_bDown = true;
	m_pointDown= point;
	m_thetaOld=m_theta;
	m_cameraOld=m_camera;
	m_oldRotX = m_RotX;
	m_oldRotY = m_RotY;
	m_oldRotZ = m_RotZ;
	m_father->SetCapture();	
}

void GL3DPara::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	double theta;
	VECTOR PxOld, axis, x1, y1, p;

	//////////////////////////////视点旋转//////////////////////////////////////////////	
	if(m_bDown)
		if(1/*m_bRotate*/)
		{
			m_camera = m_cameraOld;
			PxOld = (m_cameraOld.vCenter-m_cameraOld.vEye)%m_cameraOld.vUp;
			PxOld = PxOld/Module(PxOld);
			y1 = (point.y-m_pointDown.y)*m_cameraOld.vUp;
			x1 = -(point.x-m_pointDown.x)*PxOld;
			p = x1+y1;
			theta =sqrt(p.y*p.y + p.x*p.x)/100;
			if(theta<0.001)return ;
			axis = (x1+y1)%(m_cameraOld.vCenter-m_cameraOld.vEye);
			axis = axis/Module(axis);//旋转轴
			RotateAxis(axis, m_camera.vCenter, theta, &m_camera.vEye);
			RotateAxis(axis, theta, &m_camera.vUp);
			m_RotX = m_oldRotX + RAD2ANG(theta) * axis.x;
			m_RotY = m_oldRotY + RAD2ANG(theta) * axis.y;
			m_RotZ = m_oldRotZ + RAD2ANG(theta) * axis.z;
			if(m_RotX > 360 || m_RotX < -360)
				m_RotX = (fabs(m_RotX) - 360)*m_RotX / fabs(m_RotX);
			if(m_RotY > 360 || m_RotY < -360)
				m_RotY = (fabs(m_RotY) - 360)*m_RotY / fabs(m_RotY);
			if(m_RotZ > 360 || m_RotZ < -360)
				m_RotZ = (fabs(m_RotZ) - 360)*m_RotZ / fabs(m_RotZ);
		}	
		
}

void GL3DPara::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bDown = false;
	ReleaseCapture();
}
void GL3DPara::ZoomIn(int abs)
{
	double EyeR,EyeB,EyeL;

	EyeR=sqrt(m_camera.vEye.x*m_camera.vEye.x+
		m_camera.vEye.y*m_camera.vEye.y+m_camera.vEye.z*m_camera.vEye.z);
	EyeB=atan2(m_camera.vEye.z,sqrt(m_camera.vEye.x*m_camera.vEye.x+m_camera.vEye.y*m_camera.vEye.y));
	EyeL=atan2(m_camera.vEye.y,m_camera.vEye.x);
	EyeR=EyeR-abs*sqrt(EyeR);
	if(EyeR<40000&&EyeR>1)
	{
		m_camera.vEye.x=EyeR*cos(EyeB)*cos(EyeL);	
		m_camera.vEye.y=EyeR*cos(EyeB)*sin(EyeL);
		m_camera.vEye.z=EyeR*sin(EyeB);
	}

}

void TestDrawCube()
{
  	glBegin(GL_QUAD_STRIP);//填充凸多边形
  	glVertex3f(0.0f, 0.0f, 0.0f);  
  	glVertex3f(0.0f, 1.0f, 0.0f);  
  	glVertex3f(1.0f, 0.0f, 0.0f);  
  	glVertex3f(1.0f, 1.0f, 0.0f);  
  	glVertex3f(1.0f, 0.0f, 1.0f);  
  	glVertex3f(1.0f, 1.0f, 1.0f);  
  	glVertex3f(0.0f, 0.0f, 1.0f);  
  	glVertex3f(0.0f, 1.0f, 1.0f);  
  	glVertex3f(0.0f, 0.0f, 0.0f);  
  	glVertex3f(0.0f, 1.0f, 0.0f);  
  	glEnd();  
  	glBegin(GL_QUAD_STRIP);  
  	glVertex3f(0.0f, 0.0f, 0.0f);
  	glVertex3f(1.0f, 0.0f, 0.0f);
  	glVertex3f(0.0f, 0.0f, 1.0f);
  	glVertex3f(1.0f, 0.0f, 1.0f);
  	glVertex3f(0.0f, 1.0f, 0.0f);
  	glVertex3f(1.0f, 1.0f, 0.0f);
  	glVertex3f(0.0f, 1.0f, 1.0f);
  	glVertex3f(1.0f, 1.0f, 1.0f);
  	glEnd();  
}
void GL3DPara::GetColor(double r, double &red, double &green, double &blue)
{
	double maxRadius = 0;
	double minRadius = 0;

	maxRadius= m_valmax;
	minRadius= m_valmin;
	double ave = (maxRadius - minRadius)/6;
	double ratio = (r - minRadius)/ave;
	int flat = int(ratio);
	double temp = ratio - flat;
	if(ColorRG0)
	{
		temp = (r - minRadius)/(maxRadius - minRadius);
		red = 1-temp;
		green = temp;
		blue = 0;
		return;
	}


	switch(flat) 
	{
	case 0:
		{

			red = 128 * (1-temp)/255.0;
			green = 0;
			blue = 128/255.0;
			break;
		}
	case 1:
		{
			red = 0;
			green = 0;
			blue = 128/255.0 + 128/255.0*temp;
			break;
		}
	case 2:
		{
			red = 0;
			green = 1*temp;
			blue = 1;
			break;
		}
	case 3:
		{
			red = 0;
			green = 1;
			blue = 1*(1-temp);
			break;
		}
	case 4:
		{
			red = 1*temp;
			green = 1;
			blue = 0;
			break;
		}
	case 5:
		{
			red = 1;
			green = 1*(1-temp);
			blue = 0;
			break;
		}
	default:
		{
			red = 1;
			green = 0;
			blue = 0;
			break;
		}
	}
}

void GL3DPara::DrawObj()
{
	int i,j;
	double x,y,z,radius;
	double toppointx=0,toppointy=0,toppointz=0;
	double bottompointx=0,bottompointy=0,bottompointz=0;
	double colorr,colorg,colorb;
	colorr = colorg = colorr = 0;
	for (int i = 1; i < m_pic.rows;i++)
	{
 		glBegin(GL_QUAD_STRIP);  
		for (int j = 0; j < m_pic.cols;j++)
		{
			double val = YX_BYTE_MAT(m_pic,i,j);
			double vals = YX_BYTE_MAT(m_pic,i - 1,j);
// 			if(val == 0||vals ==0)
// 				continue;
			GetColor(val,colorr,colorg,colorb);
			glColor3f(colorr, colorg, colorb);
			glVertex3f(j, i, val);

			GetColor(vals,colorr,colorg,colorb);
			glColor3f(colorr, colorg, colorb);
			glVertex3f(j, i - 1, vals);
		}
		glEnd(); 
	}

	///////////////////////////////////////绘制经纬线///////////////////////////////////////////
	glPushMatrix();

	glPopMatrix();
	/////////////////////////////////////////////////////////////////////////////////////////*/
}
void GL3DPara::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta!=0)
	{
		ZoomIn((zDelta/abs(zDelta)));
	}
		
}

void GL3DPara::GetMinMax()
{
	int tmpmin = INT_MAX;
	int tmpmax = -1;

	for (int i = 0 ; i < m_pic.rows;i++)
	{
		for (int j = 0 ; j < m_pic.cols;j++)
		{
			int val = YX_BYTE_MAT(m_pic,i,j);
			if(val == 0)
				continue;
			tmpmax = val > tmpmax?val:tmpmax;
			tmpmin = val < tmpmin?val:tmpmin;
		}
	}
	m_valmin = tmpmin;
	m_valmax = tmpmax;
}
void GL3DPara::DrawColorBlock(CDC* pDC,CRect r, COLORREF cr1, COLORREF cr2)
{
	int r1=GetRValue(cr1);
	int g1=GetGValue(cr1);
	int b1=GetBValue(cr1);
	int r2=GetRValue(cr2);
	int g2=GetGValue(cr2);
	int b2=GetBValue(cr2);

	int rdeta=r2-r1;
	int gdeta=g2-g1;
	int bdeta=b2-b1;
	int nHeight=r.Height();
	int rvariate,gvariate,bvariate;
	CRect drawrect;

	for(int i=0;i<nHeight;i++)
	{
		rvariate=r1+MulDiv(i,rdeta,nHeight);
		gvariate=g1+MulDiv(i,gdeta,nHeight);
		bvariate=b1+MulDiv(i,bdeta,nHeight);
		drawrect.SetRect(r.left,r.top+i,r.right,r.top+i+1);
		pDC->FillSolidRect(drawrect,RGB(rvariate,gvariate,bvariate));
	}
}

void GL3DPara::DrawBar()
{
//	CStatic barstatic;
	CRect PicRect;
	//获取父窗口大小
	papaRect = CWGetWndRect(m_father);
	double height = papaRect.Height() *0.85/6.0; //PicRect.Height() / 6.0;
	double width = height*0.1;
	double rulerwidth = 12.0 * width;
	CPoint pt;
	pt.x = papaRect.Width() - 8.0*width;
	pt.y = papaRect.Height() *0.07;
	CPoint ltpt, rbpt;
	CDC* dc = NULL;
	CDC *dcMem = NULL; //用于缓冲作图的内存DC  

	CRect rect;
	CBitmap bmp; //内存中承载临时图象的位图  
	CDC dctmp;
	if(1)//双缓冲,已经放弃
	{
		
		if(m_cs == NULL)
			return;
		dc = m_cs->GetDC();
		CDC* tmpfatherdc = m_father->GetDC();
		dcMem = &dctmp; //用于缓冲作图的内存DC  
		CRect papainGrandFather;
		m_father->GetWindowRect(&papainGrandFather);//获取控件相对于屏幕的位置
		m_Grandfather->ScreenToClient(papainGrandFather);
		ltpt.x = papaRect.Width() + papainGrandFather.left;
		ltpt.y = papainGrandFather.top;
		rbpt.x = ltpt.x + rulerwidth;
		rbpt.y = ltpt.y + papaRect.Height();

		rect.SetRect(ltpt, rbpt);			//初始化双缓冲绘图
		//m_cs->MoveWindow(&rect);
		dcMem->CreateCompatibleDC(dc); //依附窗口DC创建兼容内存DC  
		bmp.CreateCompatibleBitmap(dc,rect.Width(),rect.Height());//创建兼容位图  
		dcMem->SelectObject(&bmp); //将位图选择进内存DC  
		dcMem->FillSolidRect(CRect(0,0,rect.Width(),rect.Height()),RGB(255,255,255));  //按原来背景填充客户区，不然会是黑色  
		m_father->ReleaseDC(tmpfatherdc);
		pt.x = 0;
		pt.y = papaRect.Height() *0.07;
	}
	else
	{
		dc = m_father->GetDC();
		dcMem = dc;
	}



	///1
	ltpt.x = pt.x;
	ltpt.y = pt.y;
	rbpt.x = pt.x + width;
	rbpt.y = int(pt.y + height);
	CRect rect1;
	rect1.SetRect(ltpt, rbpt);
	DrawColorBlock(dcMem, rect1, RGB(255,0,0), RGB(255,255,0));
	///2
	ltpt.y = rbpt.y;
	rbpt.y = int(pt.y + height*2);
	rect1.SetRect(ltpt, rbpt);
	DrawColorBlock(dcMem, rect1, RGB(255,255,0), RGB(0,255,0));
	///3
	ltpt.y = rbpt.y;
	rbpt.y = int(pt.y + height*3);
	rect1.SetRect(ltpt, rbpt);
	DrawColorBlock(dcMem, rect1, RGB(0,255,0), RGB(0,255,255));
	///4
	ltpt.y = rbpt.y;
	rbpt.y = int(pt.y + height*4);
	rect1.SetRect(ltpt, rbpt);
	DrawColorBlock(dcMem, rect1, RGB(0,255,255), RGB(0,0,255));
	///5
	ltpt.y = rbpt.y;
	rbpt.y = int(pt.y + height*5);
	rect1.SetRect(ltpt, rbpt);
	DrawColorBlock(dcMem, rect1, RGB(0,0,255), RGB(0,0,128));
	///6
	ltpt.y = rbpt.y;
	rbpt.y = int(pt.y + height*6);
	rect1.SetRect(ltpt, rbpt);
	DrawColorBlock(dcMem, rect1, RGB(0,0,128), RGB(128,0,128));



	//以下画半径值
	double R_max = 0;
	double R_min = 0;
	if(co1 != -1&&co0 != -1)
	{
		if(co1 != 0)
		{
			R_max = (m_valmax-co0)/co1;
			R_min = (m_valmin-co0)/co1;
		}
	}
	if(R_max == 0&&R_min == 0)
	{
		R_max = m_valmax;
		R_min = m_valmin;
	}
	CRect TextRect = CRect(pt.x + 2.0*width,pt.y,pt.x + rulerwidth,papaRect.Height());
	CStatic* TextStaic = NULL;
	dcMem->SetBkMode( TRANSPARENT );
	dcMem->SetTextColor( RGB(0, 0, 0) );
	double TextHeight = height * 6.0 / 8.0;
	double tempR = (R_max - R_min) / 8.0;
	double temptop = TextRect.top;
	CString str;
	//1
	str.Format("%.4f", double(R_max));
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//2
	str.Format("%.4f", double(R_max-tempR));
	TextRect.top = int(temptop + TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//3
	str.Format("%.4f", double(R_max-2*tempR));
	TextRect.top = int(temptop + 2*TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//4
	str.Format("%.4f", double(R_max-3*tempR));
	TextRect.top = int(temptop + 3*TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//5
	str.Format("%.4f", double(R_max-4*tempR));
	TextRect.top = int(temptop + 4*TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//6
	str.Format("%.4f", double(R_max-5*tempR));
	TextRect.top = int(temptop + 5*TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//7
	str.Format("%.4f", double(R_max-6*tempR));
	TextRect.top = int(temptop + 6*TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//8
	str.Format("%.4f", double(R_max-7*tempR));
	TextRect.top = int(temptop + 7*TextHeight);
	dcMem->DrawText(str, TextRect, DT_LEFT);
	//9
	str.Format("%.4f", double(R_min));
	TextRect.top = int(temptop + 8*TextHeight);//APP->m_nHeight;
	dcMem->DrawText(str, TextRect, DT_LEFT);
	if(1)
	{
		dc->BitBlt(0,0,rect.Width(),rect.Height(),  
			dcMem,0,0,SRCCOPY);//将内存DC上的图象拷贝到前台  
		dcMem->DeleteDC(); //删除DC  
		bmp.DeleteObject(); //删除位图  
	}

// 	str.Format("XROT = %.4f\tYROT = %.4f\tZROT = %.4f", m_RotX,m_RotY,m_RotZ);
// 	TextRect.left = 0.05*papaRect.Width();
// 	TextRect.right = 0.95*papaRect.Width();
// 	TextRect.top = 0.90*papaRect.Height();
// 	TextRect.bottom= papaRect.Height();
// 	dcMem->DrawText(str, TextRect, DT_LEFT);


// 	CRect PowerRect;
// 	CStatic* PowerStaic = (CStatic*)GetDlgItem( IDC_POWERTEXT );
// 	CFont vertFont;
// 	CString strText=" Power(dBm) "; 
// 	CPaintDC dc2( PowerStaic );
// 	vertFont.CreateFont(16, 0, 900, 900, FW_BOLD,0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
// 		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
// 	CFont *pOldFont = dc2.SelectObject(&vertFont);
// 	PowerStaic->GetClientRect( PowerRect );
// 	dc2.SetBkMode( TRANSPARENT );
// 	dc2.SetTextColor( RGB(0, 0, 0) );
// 	//	dc2.DrawText(strText, PowerRect, DT_CENTER);
	// 	dc2.TextOut(PowerRect.left, int(APP->m_nHeight/2+50), strText);
	m_father->ReleaseDC(dc);
}

void GL3DPara::InitGrandPa(CWnd *grandpa)
{
	if(m_cs!= NULL)
	{
		delete m_cs;
		m_cs = NULL;
	}
	m_cs = new CStatic();  
	m_Grandfather = grandpa;
	m_cs->Create(" ", WS_OVERLAPPED|WS_VISIBLE|SS_CENTER, CRect(0,0,1,1),grandpa);  
	m_cs->ShowWindow(SW_SHOW);
}

void GL3DPara::Resize()
{
	if(m_cs == NULL)
	{
		return;
	}
	CPoint ltpt,rbpt;
	CRect papaRect1 = CWGetWndRect(m_father);
	CRect papainGrandFather,rect;
	double height = papaRect1.Height() *0.85/6.0; //PicRect.Height() / 6.0;
	double width = height*0.1;
	int rulerwidth = 12.0*width;
	m_father->GetWindowRect(&papainGrandFather);//获取控件相对于屏幕的位置
	m_Grandfather->ScreenToClient(papainGrandFather);
	m_father->MoveWindow(CRect(papainGrandFather.left,papainGrandFather.top,papainGrandFather.right - rulerwidth,papainGrandFather.bottom));
	ltpt.x = papaRect1.Width() + papainGrandFather.left - rulerwidth;
	ltpt.y = papainGrandFather.top;
	rbpt.x = ltpt.x + rulerwidth;
	rbpt.y = ltpt.y + papaRect1.Height();

	rect.SetRect(ltpt, rbpt);			//初始化双缓冲绘图
	m_cs->MoveWindow(&rect);
	m_cs->ShowWindow(SW_SHOW);
}

void GL3DPara::ResizeBack()
{
	if(m_cs == NULL)
	{
		return;
	}
	CPoint ltpt,rbpt;
	CRect papaRect1 = CWGetWndRect(m_father);
	CRect papainGrandFather,rect;
	double height = papaRect1.Height() *0.85/6.0; //PicRect.Height() / 6.0;
	double width = height*0.1;
	int rulerwidth = 12.0*width;
	m_father->GetWindowRect(&papainGrandFather);//获取控件相对于屏幕的位置
	m_Grandfather->ScreenToClient(papainGrandFather);
	m_father->MoveWindow(CRect(papainGrandFather.left,papainGrandFather.top,papainGrandFather.right + rulerwidth,papainGrandFather.bottom));
// 	ltpt.x = papaRect1.Width() + papainGrandFather.left - 9.0*width;
// 	ltpt.y = papainGrandFather.top;
// 	rbpt.x = ltpt.x + 9.0*width;
// 	rbpt.y = ltpt.y + papaRect1.Height();
// 
// 	rect.SetRect(ltpt, rbpt);			//初始化双缓冲绘图
//	m_cs->MoveWindow(&rect);
	m_cs->ShowWindow(SW_HIDE);
}

void GL3DPara::SetCo(double co1q,double co0q)
{
	co1 =co1q;
	co0 = co0q;
}


/************************************************************************/
/*                           opengl 3D显示 结束                         */
/************************************************************************/



void CImageStatic::PreSubclassWindow()
{

#if USE_OPENGL == 1
	m_gl3D.Init(this);
	SetTimer(1, 30, NULL);
#endif
	CStatic::PreSubclassWindow();
}


void CImageStatic::OnTimer(UINT_PTR nIDEvent)
{
	
	Invalidate(false);
	CStatic::OnTimer(nIDEvent);
}

BOOL CImageStatic::CheckValue()
{
	if(m_pImg == NULL&&m_bGL == FALSE&&m_bCanMove == TRUE)
		return FALSE;
}

void CImageStatic::MoveDis(int movex, int movey)
{
	//计算新的额ROI
	if(m_bGL == FALSE)
	{
		double dmovex = -movex/scalx/ImageScale2 + m_ROIrect.x;
		double dmovey = -movey/scaly/ImageScale2 + m_ROIrect.y;
		double width = Width()/scalx / ImageScale2;
		double height = Height()/scaly / ImageScale2;
		//控件宽高表示的实际长度
		BOOL bValid = FALSE;
		if(dmovex + width <= m_pImg->width){//图像与右边界比较
			if (dmovey + height<=m_pImg->height)
			{
				if (dmovex >= 0)
				{
					if (dmovey >= 0)
					{
						bValid = TRUE;
					}
				}
			}
		}
		if(m_bCanOutImage||bValid)
		{
			if(width > 0&&height>0&&width < 9999&&height<9999)
			{
			m_ROIrect.x = dmovex;
			m_ROIrect.y = dmovey;
			m_ROIrect.width = width;
			m_ROIrect.height = height;
			ShowImage();
			}
			else
			{
				int a = 0;
			}
		}
	}
}

BOOL CImageStatic::SaveToFile(CString path)
{
	//CWnd* bmpShow = GetDlgItem(ID);  
	CDC *pdc = GetDC();  
	CImage  imag;  
	CRect rect;  
	GetClientRect(&rect);        //获取画布大小  
	GetWindowRect(&rect);  
	imag.Create(rect.Width(), rect.Height(), 32);  
	::BitBlt(imag.GetDC(), 0, 0, rect.Width(), rect.Height(), pdc->m_hDC, 0, 0, SRCCOPY);  
// 	TCHAR szFilter[] = _T("jpg file(*.jpg)|*.jpg|bmp file(*.bmp)|*.bmp|所有文件(*.*)|*.*||");  //文件格式过滤  
// 	// 构造保存文件对话框      
// 	CFileDialog fileDlg(FALSE, _T("jpg"), _T("*.jpg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);  
// 	fileDlg.m_ofn.lpstrTitle = _T("保存直方图");  //保存对话窗口标题名  
// 	CString picturePath;  
// 	if (IDOK == fileDlg.DoModal())  //按下确认键  
// 	{  
// 		picturePath = fileDlg.GetPathName();  //文件路径  
// 	}  

	HRESULT hResult = imag.Save(path); //保存图片  
	ReleaseDC(pdc);  
	imag.ReleaseDC();  
	return TRUE;
}

void CImageStatic::Init(CWnd* papa)
{
	m_papa = papa;
	m_move.MyInit(papa);
	m_gl3D.Init(this);
	m_gl3D.InitGrandPa(papa);
}
