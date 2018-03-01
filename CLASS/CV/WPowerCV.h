#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
#define CVTRYSTART try{
#define CVTRYEND 	}catch(cv::Exception err){\
CString opencverr;\
opencverr.Format("%s",err.msg);\
TRACE(opencverr);\
return FALSE;}
struct SSZNDATA{
	vector<int> m_vecindex;		//�ױ��
	vector<double> m_veca;		//ƽ��ֵ
	vector<double> m_max;		//���ֵ
	vector<double> m_diff;		//������
	double m_dis;				//ͭ��ķ���
	int m_rownum;				//�����е�����
	double m_dThresholdDeep;	//��С�����ֵ
};
//�ɼ�ͼ������м���Ҫͬʱ��ȡ�Ĳ���
struct SSZNPRPJECT{
	//gray =��val - min��*��255/��max - min���� = val *255/��max - min�� - min*255 /��max - min��
	//     = val*co1+co0
	double co0,co1;
	//����õĿ׵�λ�÷ֲ�
	vector<CvRect> hole;
	//�Ҷ�ͼ·��
	CString graypath;
	//�Ҷ�ͼROI
	CvRect m_ROIGray;
	//ROI ·��
	CString roipath;
	//��·��
	CString holepath;
	//������е㵽ƽ��ľ���ľ���
	Mat m_matDis;
	//�������������ϵĵ㵽ƽ�����ľ���
	Mat m_matDisSide;
	//��ȡ����ԭʼ�������
	CvMat *m_matHeightdata;
	//��ȡ����ԭʼ�������
	Mat m_matTrueGray;
	SSZNPRPJECT()
	{
		co0 = 0;
		co1 = 0;
		m_matHeightdata = NULL;
		m_ROIGray = cvRect(0,0,1,1);
	}
} ;
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);
	SSZNPRPJECT	m_ssznpic;
	//���������Ͳ�νṹ
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
public:
/************************************************************************/
/*                             ͼ��ָʼ                             */
/************************************************************************/
	//ͼ���з�
	BOOL SplitByXYNum(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
	//�����з�
	BOOL SplitScvByXYNum(CString InputscvPath,CString OutPutDir,int XNum,int YNum);
	//��ֵ�ָ�
	BOOL SplitByThreshold(CString InputImgPath,CString OutPutPath,int threshold);
	//ROI�ָ�
	BOOL SplitByROI(CString InputImgPath,CString OutPutPath,int x,int y, int width,
		int height);
	//ĳ�������ϵĶ�̬��ֵ
	BOOL SplitByDirection(CString InputImgPath,CString OutPutPath,int diedaiwidth,
		int boardwidth,int direction,int seedThreshold);
/************************************************************************/
/*                             ͼ��ָ����                             */
/************************************************************************/
/************************************************************************/
/*                             ͼ���ò�����ʼ                         */
/************************************************************************/
	//ͼ��Ӽ�
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,
		int YMove,int CalType = 0,BOOL bDir = FALSE);
	//ͼ��ȡ��
	BOOL ImgNot(CString InputImgPath,CString OutPutPath);
	//������������
	void FindContour(CString inputPath,CString OutPutPath,int method);
	//��ˮ���
	void FillFloodFill(CString InpuPath);
	//ͼ��ƥ��
	BOOL MatchImage(CString inputPath,CString TemplatePath,CString OutPutPath);
	//��һ��(��ֵͶ�䵽һ������)
	BOOL Normalize(Mat *srcdata, double lownum,double highnum);
	//����任
	BOOL CwWarp(vector<CvPoint2D32f> realCenterPt,vector<CvPoint2D32f> stdCenterPt,CvMat* resmatrix);
	//����任Mat
	BOOL CwWarpMat(vector<CvPoint2D32f> RealCenterPt,vector<CvPoint2D32f> StdCenterPt ,double *m_AffinePrjPara,double* m_AffinePrjParaInv);

/************************************************************************/
/*                             ͼ���ò�������                         */
/************************************************************************/

/************************************************************************/
/*                       For3D���������ʼ                              */
/************************************************************************/
	//��άƽ�����
	void FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,double boarddis,CString disImgPath);
	void FitPlane(CString backmaskPath);

	void FitPlane(IplImage* pBackMaskImg);

	//�������ܲ���
	void testsszn(CString pathColorTemper,SSZNDATA &data);
	//����3D����Ŀ�
	void sortByXY(int numrows);
	//��ȡ255������,�����µ�hole[]����
	BOOL GetContour(CString InputImgPath,CString OutPutPath,int minval);
	BOOL GetContour(IplImage* pImg,double areath);
	//����csv�㼯�����ƻҶ�ͼ
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type,double *co = NULL);
	//ˢ�����ͼ
	BOOL RenewHeightPic(double * co, CString OutPutPath,int type = 1);
	BOOL PRenewHeightPic(double * co, IplImage* pImg,int nInputType = 1);
	//��ȡ���ս��
	BOOL GetHoleDeep(IplImage* pImghole,CString pathColorTemper,SSZNDATA &sszndata);
/************************************************************************/
/*                       For3D�����������                              */
/************************************************************************/
/************************************************************************/
/*                       ���Կ�ʼ                                       */
/************************************************************************/
	BOOL test_cwcv();
	BOOL test_cwcvEdge();
	BOOL test_cwcvFront();
	BOOL test_cwcvBack();
	BOOL test_match();
	BOOL test_Contour();
	BOOL test_CwWarp();
/************************************************************************/
/*                       ���Խ���                                       */
/************************************************************************/
};
