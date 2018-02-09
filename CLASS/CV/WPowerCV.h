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
	vector<int> m_vecindex;		//孔编号
	vector<double> m_veca;		//平均值
	vector<double> m_max;		//最大值
	vector<double> m_diff;		//均方差
	double m_dis;				//铜面的方差
	int m_rownum;				//孔阵列的行数
	double m_dThresholdDeep;	//最小深度阈值
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
/*                             图像分割开始                             */
/************************************************************************/
	//图像切分
	BOOL SplitByXYNum(CString InputImgPath,CString OutPutDir,int XNum,int YNum);
	//点云切分
	BOOL SplitScvByXYNum(CString InputscvPath,CString OutPutDir,int XNum,int YNum);
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
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type,double *co = NULL);
	void OutPutGrayPic(double * co, CString OutPutPath);
	//找轮廓
	void FindContour(CString inputPath,CString OutPutPath,int method);
	//漫水填充测试
	void FillFloodFillTest(CString InpuPath);
	//提取255的轮廓
	void GetContour(CString InputImgPath,CString OutPutPath,int minval);
	//深视智能测试
	void testsszn(CString pathColorTemper,SSZNDATA &data);

	void sortByXY(int numrows);

	//三维平面拟合
	void FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,double boarddis,CString disImgPath);
	void FitPlane(CString InputImgPath,CString backmaskPath,CString disImgPath);
/************************************************************************/
/*                       单图操作		                                */
/************************************************************************/
	//图像取反
	BOOL ImgNot(CString InputImgPath,CString OutPutPath);
};

