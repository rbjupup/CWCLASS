#pragma once
#include "WPowerCV.h"
#include "XFunCom.h"
#if USE_3D_CAM == 1
#include "Lib\SR7Link.h"
#include "Lib\SR3dexe.h"
#pragma  comment(lib, "SR7Link.lib") /*����ws2_32.lib��̬���ӿ�*/  
#endif
#include <fstream>
#if USE_3D_CAM == 1
struct SSZNCAMOP{
	CRect m_rect;                                  //���ڿؼ���С��λ�ñ仯
	unsigned int batchPoint;                       //����������
	double xPitch;                                 //x������ֵ
	int pdwBatchW;                                 //һ�����ݸ���
	int *pdwBatchData;                             //���������ݴ洢Buffer
	int IpAddr[4];                                 //IP��ַ���
	bool BatchMode;                                //������ģʽ 0���Ǵ��� 1��������   ��������ģʽ�²��ܽ���ɨ�账��
	CStatusBar m_Statusbar;                        //״̬��
	unsigned char* grayData;                       //�Ҷ����ݻ���
	bool b_connect;                                //������ӳɹ���־
	unsigned char* LasetWidthData;                 //�����߿������
	volatile BOOL m_capFinish;					   //ͼ��ɼ����
	BOOL m_capRes;								   //ͼ��ɼ����
	double m_dmostWaitTime;						   //�ɼ�����ͼ����ȴ�ʱ��
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
		pdwBatchW = 3200;         //һ���������������ݵ�������--�̶�Ϊ3200����
		BatchMode = true;         //ture : ������ģʽ  �� ��������ģʽ�²��ܽ���ɨ�账��
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

//����
	BOOL connectByIp();
//�������ݵ��ļ���
	void SaveDataToFile();
//���ļ��л�ȡ����
	void ReadDataFromFile();
//��ʼ������
	void StartBatch();
//������ת����һ��CvMat��������,������ڴ���
	void Copy2Data(CvMat* &matdata);
//���Ҷ�����ת����һ��Cv::Mat��������,������ڴ���
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
	//��������Ľ���߶�ͼ��
	BOOL GetMidHeight(CvMat *matHeightdata);
	//ˢ�»Ҷ�����
	BOOL RenewGray(CString path);
	//ˢ�»Ҷ�����
	BOOL RenewHeight(CString path);
	BOOL RenewHeight(IplImage* pImg);
public:
	CWPowerCV m_cwcv;	//opencv���
	CvRect ROIrect;		//	����ROI��λ��
	CvRect ROIrectBoard;		//	����ROI��λ��
	vector<SSZNDATA> m_Lastdata;	//���յõ�������
	BOOL m_bneedFlip;				//������ɨͼ��������ʵ��ͼ����ұ�,������Ϊ��
#if USE_3D_CAM == 1
	SSZNCAMOP m_ssznCam;		//���������������
#endif
/************************************************************************/
/*							����·��				                    */
/************************************************************************/
	CString m_pathcsv;			//�㼯·��
	CString m_pathGray;			//�㼯ת�Ҷ�ͼ��·��
	CString m_pathROI;			//�Ҷ�ͼ��ROI
	CString m_path3DBIN;		//��ֵͼ��·��
	CString m_pathCon;			//��������
	CString m_pathDIS;			//���ͼ
	CString m_pathColorTemper;	//ɫ��ͼ
	CString m_pathBottomMask;	//����maskͼ
	CString m_pathMidHeight;	//�е�ĸ߶�ͼ
	CString m_pathTrueGray;		//ǰ��ĻҶ�ͼ�Ǹ߶�ͼת��,����������



};



/************************************************************************/
/*                         ����ɨ���                                   */
/************************************************************************/
//����ṹ�帺������ȿ�����
//�Եõ�ÿ���׵���Ȳ����浽�ļ���
//�Թ���ȡ
struct THREAD_LINER_DATA
{
	//����Ҷ�����
	cv::Mat m_pImgGray;
	//�����������
	cv::Mat m_pImgHeightData;
	//�����׼ͼ����
	cv::Mat m_pImgTemp;
	//�����ͼ����
	cv::Mat m_pImgHole;
	//��ǰ�߳����ڴ����л��Ǵ������
	enum {CWWORKING,CWFINISH};
	//��ǰ�����
	CWSSZN *m_grabImg;
	BOOL m_CurStatue;
	//��ǰ�����ı��
	int m_nCurNum;
	//����ͨ����ֵ
	double m_dThreadshold;
	//�����������ݵ��ļ���
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
	//��ʼ����ǰ��
	BOOL StartProcess();
};

static UINT THREAD_TOTAL_LINER(LPVOID lparam)
{
	THREAD_LINER_DATA* thisblock = (THREAD_LINER_DATA*)lparam;
	thisblock->StartProcess();
	return 255;
}