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
/*                             图像分割开始                             */
/************************************************************************/
	//图像切分
	BOOL SplitByXYNum(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
	//阈值分割
	BOOL SplitByThreshold(CString InputImgPath,CString OutPutPath,int threshold);
	//ROI分割
	BOOL SplitByROI(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height);
	//某个方向上的动态阈值
	BOOL SplitByDirection(CString InputImgPath,CString OutPutPath,int diedaiwidth,int boardwidth,int direction,int seedThreshold);;
/************************************************************************/
/*                             图像分割结束                             */
/************************************************************************/
	//图像加减
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType = 0,BOOL bDir = FALSE);
	//解析csv点集，绘制灰度图
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type);
	//找轮廓
	void FindContour(CString inputPath,CString OutPutPath,int method);
	//漫水填充测试
	void FillFloodFillTest(CString InpuPath);
	//提取255的轮廓
	void GetContour(CString InputImgPath,CString OutPutPath,int minval);
	//深视智能测试
	void testsszn();
	//三维平面拟合
	void FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,int boarddis);

};

