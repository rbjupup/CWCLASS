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
//采集图像过程中间需要同时获取的参数
struct SSZNPRPJECT{
	//gray =（val - min）*（255/（max - min）） = val *255/（max - min） - min*255 /（max - min）
	//     = val*co1+co0
	double co0,co1;
	//最后获得的孔的位置分布
	vector<CvRect> hole;
	//灰度图路径
	CString graypath;
	//灰度图ROI
	CvRect m_ROIGray;
	//ROI 路径
	CString roipath;
	//孔路径
	CString holepath;
	//存放所有点到平面的距离的矩阵
	Mat m_matDis;
	//存放所有用来拟合的点到平面距离的矩阵
	Mat m_matDisSide;
	//读取到的原始深度数据
	CvMat *m_matHeightdata;
	//读取到的原始深度数据
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
	//定义轮廓和层次结构
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
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
	BOOL SplitByROI(CString InputImgPath,CString OutPutPath,int x,int y, int width,
		int height);
	//某个方向上的动态阈值
	BOOL SplitByDirection(CString InputImgPath,CString OutPutPath,int diedaiwidth,
		int boardwidth,int direction,int seedThreshold);
/************************************************************************/
/*                             图像分割结束                             */
/************************************************************************/
/************************************************************************/
/*                             图像常用操作开始                         */
/************************************************************************/
	//图像加减
	BOOL ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,
		int YMove,int CalType = 0,BOOL bDir = FALSE);
	//图像取反
	BOOL ImgNot(CString InputImgPath,CString OutPutPath);
	//索贝尔找轮廓
	void FindContour(CString inputPath,CString OutPutPath,int method);
	//漫水填充
	void FillFloodFill(CString InpuPath);
	//图像匹配
	BOOL MatchImage(CString inputPath,CString TemplatePath,CString OutPutPath);
	//归一化(将值投射到一个区间)
	BOOL Normalize(Mat *srcdata, double lownum,double highnum);
	//仿射变换
	BOOL CwWarp(vector<CvPoint2D32f> realCenterPt,vector<CvPoint2D32f> stdCenterPt,CvMat* resmatrix);
	//仿射变换Mat
	BOOL CwWarpMat(vector<CvPoint2D32f> RealCenterPt,vector<CvPoint2D32f> StdCenterPt ,double *m_AffinePrjPara,double* m_AffinePrjParaInv);

/************************************************************************/
/*                             图像常用操作结束                         */
/************************************************************************/

/************************************************************************/
/*                       For3D相机操作开始                              */
/************************************************************************/
	//三维平面拟合
	void FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,double boarddis,CString disImgPath);
	void FitPlane(CString backmaskPath);

	void FitPlane(IplImage* pBackMaskImg);

	//深视智能测试
	void testsszn(CString pathColorTemper,SSZNDATA &data);
	//排序3D相机的孔
	void sortByXY(int numrows);
	//提取255的轮廓,并更新到hole[]里面
	BOOL GetContour(CString InputImgPath,CString OutPutPath,int minval);
	BOOL GetContour(IplImage* pImg,double areath);
	//解析csv点集，绘制灰度图
	BOOL ParsePtSet(CString inputPath,CString OutPutPath,int type,double *co = NULL);
	//刷新深度图
	BOOL RenewHeightPic(double * co, CString OutPutPath,int type = 1);
	BOOL PRenewHeightPic(double * co, IplImage* pImg,int nInputType = 1);
	//获取最终结果
	BOOL GetHoleDeep(IplImage* pImghole,CString pathColorTemper,SSZNDATA &sszndata);
/************************************************************************/
/*                       For3D相机操作结束                              */
/************************************************************************/
/************************************************************************/
/*                       测试开始                                       */
/************************************************************************/
	BOOL test_cwcv();
	BOOL test_cwcvEdge();
	BOOL test_cwcvFront();
	BOOL test_cwcvBack();
	BOOL test_match();
	BOOL test_Contour();
	BOOL test_CwWarp();
/************************************************************************/
/*                       测试结束                                       */
/************************************************************************/
};
