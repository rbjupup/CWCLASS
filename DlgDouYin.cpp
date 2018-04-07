// DlgDouYin.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgDouYin.h"
#include "afxdialogex.h"

using namespace std;
using namespace cv;

// CDlgDouYin 对话框

IMPLEMENT_DYNAMIC(CDlgDouYin, CDialogEx)

CDlgDouYin::CDlgDouYin(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgDouYin::IDD, pParent)
	, m_nZmaxchange(10)
	, m_nchangeLength(20)
	, m_nChangeTo(10)
	, m_extentLength(20)
{

}

CDlgDouYin::~CDlgDouYin()
{
}

void CDlgDouYin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SOU, m_picctlSource);
	DDX_Control(pDX, IDC_STATIC_FACE, m_picctlFace);
	DDX_Control(pDX, IDC_STATIC_CHANGE, m_picctlChange);
	DDX_Slider(pDX, IDC_SLIDER1, m_nZmaxchange);
	DDX_Slider(pDX, IDC_SLIDER2, m_nchangeLength);
	DDX_Slider(pDX, IDC_SLIDER3, m_nChangeTo);
	DDX_Slider(pDX, IDC_SLIDER4, m_extentLength);
}


BEGIN_MESSAGE_MAP(CDlgDouYin, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CAM, &CDlgDouYin::OnBnClickedButtonTestCam)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CAM2, &CDlgDouYin::OnBnClickedButtonTestCam2)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CAM3, &CDlgDouYin::OnBnClickedButtonTestCam3)
	ON_BN_CLICKED(IDC_BUTTON_TEST_CAM4, &CDlgDouYin::OnBnClickedButtonTestCam4)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CDlgDouYin::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER2, &CDlgDouYin::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER3, &CDlgDouYin::OnNMReleasedcaptureSlider1)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER4, &CDlgDouYin::OnNMReleasedcaptureSlider1)
END_MESSAGE_MAP()


// CDlgDouYin 消息处理程序


void CDlgDouYin::OnBnClickedButtonTestCam()
{
  

	cvNamedWindow("mp4");      
    /*CvCapture结构体，保存图像信息，cvCreateCameraCapture(-1)读入摄像头视频， 
     *                           cvCreateFileCapture("路径")读入视频文件*/  
    CvCapture * capture=cvCreateCameraCapture(-1);  
      
    IplImage* frame;  
      
    while(1)  
    {  
        /*cvQueryFrame()，读入CvCapture结构体指针，将下一帧加载到内存*/  
        frame=cvQueryFrame(capture);  
        if(!frame)  
            break;  
  
        /*cvShowImage()在窗口中显示*/  
        cvShowImage("mp4",frame);  
        /*cvWaitKey()触发按键、控制帧率*/  
        char c =cvWaitKey(33);  
        if(c==27)  
            break;  
    }  
    /*cvReleaseCapture()释放结构体指针*/  
    cvReleaseCapture(&capture);  

}


BOOL CDlgDouYin::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetTimer(0,100,0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgDouYin::OnBnClickedButtonTestCam2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgDouYin::OnBnClickedButtonTestCam3()
{
	m_cwcv.m_face.FindAndDraw("",NULL,1);
}


void CDlgDouYin::OnBnClickedButtonTestCam4()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDlgDouYin::OnTimer(UINT_PTR nIDEvent)
{
	try{
		switch(nIDEvent)
		{
		case 0:
			KillTimer(0);
			IplImage *frame = NULL;
			m_cwcv.opencwcam();
			while(1)
			{
				m_cwcv.GetOneFrame(frame);
				if(!frame)  
					break;  
				m_picctlSource.ChangeImg(frame,FALSE);
				IplImage *tmppic = cvCloneImage(frame);
				m_cwcv.m_face.FindAndDraw(tmppic,TRUE);
				m_picctlFace.ChangeImg(tmppic,FALSE);
				cvReleaseImage(&tmppic);
				tmppic = NULL;

				tmppic = cvCloneImage(frame);
				cvZero(tmppic);
				for(int rectIndex = 0; rectIndex < m_cwcv.m_face.facedata.Faceplace.size();rectIndex++)
				{
					CvRect rect = m_cwcv.m_face.facedata.Faceplace[rectIndex];
					rect.x -= m_extentLength;
					rect.y -= m_extentLength;
					rect.width += 2*m_extentLength;
					rect.height += 2*m_extentLength;
					if (rect.x < 0)
					{
						rect.x = 0;
					}
					if (rect.y < 0)
					{
						rect.y = 0;
					}
					if(rect.x + rect.width > frame->width)
					{
						rect.width = frame->width - rect.x - 1;
					}
					if(rect.y + rect.height > frame->height)
					{
						rect.height = frame->height - rect.x - 1;
					}
					int halfHeight = rect.height / 2;
					int halfwidth = rect.width / 2;
					int nmaxN = m_nchangeLength*halfwidth/100; 
					int nmaxK = m_nChangeTo*halfwidth/100; 
					int nminN = m_nChangeTo*halfwidth/100; 
					int nminK = m_nChangeTo*halfwidth/100; 
					double scalarNU = (nmaxN - nminN)/(double)m_nZmaxchange; 
					double scalarND = (nmaxN - nminN)/(double)(rect.height - m_nZmaxchange); 
					for (int j = 0; j < rect.height;j++)
					{
						for (int i = 0; i < rect.width;i++)
						{
							//计算当前行的NK
							int newN = 0;
							int newK = 0;
							int newX = 0;
							double ScalarSmall = 0;
							double ScalarBig = 0;
							newK = nmaxK;
							if(j < m_nZmaxchange)
							{
								newN = scalarNU*j + nminN;

							}
							else
							{
								newN = nmaxN - scalarND* (j - m_nZmaxchange);
							}
							ScalarSmall = newN / newK;
							ScalarBig = (halfwidth - newN)/(halfwidth - newK);
							//对当前行进行变形操作
							if(i < newN)
							{
								newX = ScalarSmall*i;
							}
							if(i >= newN && i < rect.width - newN)
							{
								newX = ScalarBig*(i-newN)+newN;
							}
							if(i >= rect.width - newN)
							{
								newX = ScalarSmall*(i-rect.width + newN)+rect.width - newN;
							}
							YX_BYTE_R(tmppic,j + rect.y,i + rect.x) = YX_BYTE_R(frame,j + rect.y,newX + rect.x);
							YX_BYTE_G(tmppic,j + rect.y,i + rect.x) = YX_BYTE_G(frame,j + rect.y,newX + rect.x);
							YX_BYTE_B(tmppic,j + rect.y,i + rect.x) = YX_BYTE_B(frame,j + rect.y,newX + rect.x);
						}
					}


				}
				m_picctlChange.ChangeImg(tmppic,FALSE);
				cvReleaseImage(&tmppic);
				tmppic = NULL;


				XWaitTime(0.01);
			}
			m_cwcv.releasecwcam();
			break;
		}


		CDialogEx::OnTimer(nIDEvent);
	}
	catch(cv::Exception s)
	{
		AfxMessageBox(s.err.c_str());
	}

}


void CDlgDouYin::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	*pResult = 0;
}
