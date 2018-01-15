#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "stdafx.h"
#define CVTRYSTART try{
#define CVTRYEND 	}catch(cv::Exception err){\
CString opencverr;\
opencverr.Format("%s",err.msg);\
AfxMessageBox(opencverr);\
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
/************************************************************************/
/*                             ͼ��ָ����                             */
/************************************************************************/
	//ͼ��Ӽ�
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType = 0,BOOL bDir = FALSE);
	//����csv�㼯�����ƻҶ�ͼ
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type);
	//������
	void FindContour(CString InputImgPath,CString OutPutPath,int method);
	//��ˮ������
	void FillFloodFillTest(CString InpuPath);
};

