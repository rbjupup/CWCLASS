#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
#define CVTRYSTART try{
#define CVTRYEND 	}catch(cv::Exception err){\
CString opencverr;\
opencverr.Format("%s",err.msg);\
TRACE(opencverr);\
return FALSE;}
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);


public:
/************************************************************************/
/*                             ͼ��ָʼ                             */
/************************************************************************/
	//ͼ���з�
	BOOL SplitByXYNum(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
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
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type);
	//������
	void FindContour(CString inputPath,CString OutPutPath,int method);
	//��ˮ������
	void FillFloodFillTest(CString InpuPath);
	//��ȡ255������
	void GetContour(CString InputImgPath,CString OutPutPath,int minval);
	//�������ܲ���
	void testsszn();
	//��άƽ�����
	void FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,int boarddis);

};

