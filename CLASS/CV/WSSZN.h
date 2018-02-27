#pragma once
#include "WPowerCV.h"
#include "XFunCom.h"
#if USE_3D_CAM == 1
#include "Lib\SR7Link.h"
#include "Lib\SR3dexe.h"
#pragma  comment(lib, "SR7Link.lib") /*链接ws2_32.lib动态链接库*/  
#endif
#include <fstream>
#if USE_3D_CAM == 1
struct SSZNCAMOP{
	CRect m_rect;                                  //用于控件大小及位置变化
	unsigned int batchPoint;                       //批处理行数
	double xPitch;                                 //x方向间距值
	int pdwBatchW;                                 //一行数据个数
	int *pdwBatchData;                             //批处理数据存储Buffer
	int IpAddr[4];                                 //IP地址存放
	bool BatchMode;                                //批处理模式 0：非处理 1：批处理   非批处理模式下不能进行扫描处理
	CStatusBar m_Statusbar;                        //状态栏
	unsigned char* grayData;                       //灰度数据缓存
	bool b_connect;                                //相机连接成功标志
	unsigned char* LasetWidthData;                 //激光线宽度数据
	volatile BOOL m_capFinish;					   //图像采集完成
	BOOL m_capRes;								   //图像采集完成
	double m_dmostWaitTime;						   //采集不到图像最长等待时间
	double C_LV;
	double C_HV;
	double C_ZScale;
	double C_YInterv;  
	SSZNCAMOP(){
		C_LV = -8.4;
		C_HV = 8.4;
		C_ZScale = 1.0;
		C_YInterv = 0.1;
		batchPoint = 0;    
		xPitch = 0;
		pdwBatchW = 3200;         //一条轮廓包含的数据点数设置--固定为3200个点
		BatchMode = true;         //ture : 批处理模式  ， 非批处理模式下不能进行扫描处理
		pdwBatchData = NULL;
		grayData = NULL;
		LasetWidthData = NULL;
		b_connect = false;
		m_capFinish = false;
		m_capRes = TRUE;
		ReadDataFromFile();
		m_dmostWaitTime = 2000;
	}
	~SSZNCAMOP(){
		releaseCam();

	}

	void releaseCam();

//功能
	BOOL connectByIp();
//保存数据到文件中
	void SaveDataToFile();
//从文件中获取数据
	void ReadDataFromFile();
//开始批处理
	void StartBatch();
//将数据转化到一个CvMat数组里面,方面后期处理
	void Copy2Data(CvMat* &matdata);
//将灰度数据转化到一个Cv::Mat数组里面,方面后期处理
	BOOL CopyGray2Data(cv::Mat &matdata);
};
#endif

class CWSSZN
{
public:
	CWSSZN(void);
	~CWSSZN(void);
public:
	void init();
	void readscv();
	BOOL SetGrayImgROI();
	BOOL ROITo3D(double boarddis);
	BOOL SetCon(int mincon);
	BOOL GetRes(int numOfLie,double maxThannum);
	BOOL Snap(double waitTimeMill);
	BOOL FitPlaneByMask();
	//输出点击点的截面高度图像
	BOOL GetMidHeight(CvMat *matHeightdata);
	//刷新灰度数据
	BOOL RenewGray(CString path);
	//刷新灰度数据
	BOOL RenewHeight(CString path);
	BOOL RenewHeight(IplImage* pImg);
public:
	CWPowerCV m_cwcv;	//opencv相关
	CvRect ROIrect;		//	保存ROI的位置
	CvRect ROIrectBoard;		//	保存ROI的位置
	vector<SSZNDATA> m_Lastdata;	//最终得到的数据
	BOOL m_bneedFlip;				//假如线扫图像的左边是实际图像的右边,这里置为真
#if USE_3D_CAM == 1
	SSZNCAMOP m_ssznCam;		//深视智能相机操作
#endif
/************************************************************************/
/*							各种路径				                    */
/************************************************************************/
	CString m_pathcsv;			//点集路径
	CString m_pathGray;			//点集转灰度图的路径
	CString m_pathROI;			//灰度图加ROI
	CString m_path3DBIN;		//二值图的路径
	CString m_pathCon;			//轮廓区域
	CString m_pathDIS;			//深度图
	CString m_pathColorTemper;	//色温图
	CString m_pathBottomMask;	//背景mask图
	CString m_pathMidHeight;	//中点的高度图
	CString m_pathTrueGray;		//前面的灰度图是高度图转的,这个才是真的



};



/************************************************************************/
/*                         纯线扫相机                                   */
/************************************************************************/
//这个结构体负责处理深度块数据
//以得到每个孔的深度并保存到文件中
//以供读取
struct THREAD_LINER_DATA
{
	//保存灰度数据
	cv::Mat m_pImgGray;
	//保存深度数据
	cv::Mat m_pImgHeightData;
	//保存标准图数据
	cv::Mat m_pImgTemp;
	//保存孔图数据
	cv::Mat m_pImgHole;
	//当前线程是在处理中还是处理完成
	enum {CWWORKING,CWFINISH};
	//当前块相机
	CWSSZN *m_grabImg;
	BOOL m_CurStatue;
	//当前处理块的编号
	int m_nCurNum;
	//保存通孔阈值
	double m_dThreadshold;
	//保存最终数据到文件中
	void SaveDataToFIle();

	THREAD_LINER_DATA()
	{
		m_grabImg = new CWSSZN;
		m_grabImg->init();
		m_dThreadshold = 0;
	}
	THREAD_LINER_DATA(const THREAD_LINER_DATA& t)
	{
		this->m_grabImg = new CWSSZN;
		this->m_grabImg->init();
		this->m_pImgGray = t.m_pImgGray;
		this->m_pImgHeightData = t.m_pImgHeightData;
		this->m_pImgTemp = t.m_pImgTemp;
		this->m_pImgHole = t.m_pImgHole;
		this->m_CurStatue = t.m_CurStatue;
		this->m_nCurNum = t.m_nCurNum;
		this->m_dThreadshold = t.m_dThreadshold;
	}
	~THREAD_LINER_DATA()
	{
		if(NULL != m_grabImg)
		{
			delete m_grabImg;
			m_grabImg = NULL;
		}

	}
	//开始处理当前块
	BOOL StartProcess();
};

static UINT THREAD_TOTAL_LINER(LPVOID lparam)
{
	THREAD_LINER_DATA* thisblock = (THREAD_LINER_DATA*)lparam;
	thisblock->StartProcess();
	return 255;
}