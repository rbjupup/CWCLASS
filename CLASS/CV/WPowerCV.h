#pragma once
#include "opencv2/opencv.hpp"
#include "opencv/cv.h"
#include "stdafx.h"
#define CVTRYSTART try{
#define CVTRYEND 	}catch(cv::Exception err){\
CString opencverr;\
opencverr.Format("%s",err.msg);\
AfxMessageBox(opencverr);}
class CWPowerCV
{
public:
	CWPowerCV(void);
	~CWPowerCV(void);


public:
	//ͼ���з�
	BOOL SplitIMG(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
	//ͼ��Ӽ�
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType = 0,BOOL bDir = FALSE);
	//����csv�㼯�����ƻҶ�ͼ
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type);
	//��ֵ�ָ�
	BOOL SplitByThreshold(CString InputImgPath,CString OutPutPath,int threshold);
};

