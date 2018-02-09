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
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);
	Mat m_matDis;
	Mat m_matDisSide;
	CvMat *m_matHeightdata;
	CvRect m_ROIGray;
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
	BOOL SplitByROI(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height);
	//ĳ�������ϵĶ�̬��ֵ
	BOOL SplitByDirection(CString InputImgPath,CString OutPutPath,int diedaiwidth,int boardwidth,int direction,int seedThreshold);;
/************************************************************************/
/*                             ͼ��ָ����                             */
/************************************************************************/
	//ͼ��Ӽ�
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType = 0,BOOL bDir = FALSE);
	//����csv�㼯�����ƻҶ�ͼ
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type,double *co = NULL);
	void OutPutGrayPic(double * co, CString OutPutPath);
	//������
	void FindContour(CString inputPath,CString OutPutPath,int method);
	//��ˮ������
	void FillFloodFillTest(CString InpuPath);
	//��ȡ255������
	void GetContour(CString InputImgPath,CString OutPutPath,int minval);
	//�������ܲ���
	void testsszn(CString pathColorTemper,SSZNDATA &data);

	void sortByXY(int numrows);

	//��άƽ�����
	void FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,double boarddis,CString disImgPath);
	void FitPlane(CString InputImgPath,CString backmaskPath,CString disImgPath);
/************************************************************************/
/*                       ��ͼ����		                                */
/************************************************************************/
	//ͼ��ȡ��
	BOOL ImgNot(CString InputImgPath,CString OutPutPath);
};

