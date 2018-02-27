#include "StdAfx.h"
#include "WSSZN.h"
//#include "3DCam.h"

CWSSZN::CWSSZN(void)
{
	init();
}


CWSSZN::~CWSSZN(void)
{
}

void CWSSZN::readscv()
{
	//if(m_pathcsv == "")
	m_pathcsv = GetDirPathByDialog("",NULL,1);
	m_cwcv.ParsePtSet(m_pathcsv,m_pathGray,0,NULL);
	
}

void CWSSZN::init()
{
	m_pathcsv = "";	//�㼯·��
	m_pathGray = GetAppPath()+"PIC\\GRAY.bmp";	//�㼯ת�Ҷ�ͼ��·��
	m_pathROI = GetAppPath()+"PIC\\ROI.bmp";	//�Ҷ�ͼ��ROI
	m_path3DBIN = GetAppPath()+"PIC\\BIN.bmp";//��ֵͼ��·��
	m_pathCon = GetAppPath()+"PIC\\CONTOUR.bmp";	//��������
	m_pathDIS = GetAppPath()+"PIC\\DIS.bmp";	//��������
	m_pathColorTemper = GetAppPath()+"PIC\\TEMPER.bmp";	//ɫ��ͼ
	m_pathBottomMask = GetAppPath()+"PIC\\BMask.bmp";	//��������
	m_pathMidHeight = GetAppPath()+"PIC\\MidHeight.bmp";	//����߶�ͼ
	m_pathTrueGray = GetAppPath()+"PIC\\IMGRAY.bmp";	//��.�Ҷ�ͼ
	CreateAllDirectories(GetAppPath()+"PIC");
	m_bneedFlip = FALSE;
}

BOOL CWSSZN::SetGrayImgROI()
{
	if(!FileExist(m_pathGray))
		return FALSE;
	m_cwcv.m_ssznpic.m_ROIGray = ROIrect;
	m_cwcv.SplitByROI(m_pathGray,m_pathROI,ROIrect.x,ROIrect.y,ROIrect.width,ROIrect.height);
	return TRUE;
}

BOOL CWSSZN::ROITo3D(double boarddis)
{
	if(!FileExist(m_pathROI))
		return FALSE;
	m_cwcv.FitPlane(m_pathROI,m_path3DBIN,ROIrectBoard.x,ROIrectBoard.y,ROIrectBoard.width,ROIrectBoard.height,boarddis,m_pathDIS);
	return TRUE;
}

BOOL CWSSZN::SetCon(int mincon)
{
	if(!FileExist(m_path3DBIN))
		return FALSE;
	m_cwcv.GetContour(m_path3DBIN,m_pathCon,mincon);
	return TRUE;
}

BOOL CWSSZN::GetRes(int numOfLie,double maxThannum)
{
	SSZNDATA tmpdata;
	tmpdata.m_rownum = numOfLie;
	tmpdata.m_dThresholdDeep = maxThannum;
	m_cwcv.testsszn(m_pathColorTemper,tmpdata);
	m_Lastdata.push_back(tmpdata);
	return TRUE;
}
UINT Capture(LPVOID param){
#if USE_3D_CAM == 1
	SSZNCAMOP* ssznn = (SSZNCAMOP*)param;
	ssznn->releaseCam();
	ssznn->connectByIp();
	ssznn->StartBatch();

#endif
	return 0;
}
BOOL CWSSZN::Snap(double waitTimeMill)
{
	//���߳�ȥ��ȡͼƬ����
	m_ssznCam.m_capFinish = false;
	m_ssznCam.m_dmostWaitTime = waitTimeMill;
	AfxBeginThread(Capture,&m_ssznCam);
	if(!WaitForBOOL(m_ssznCam.m_capFinish,TRUE,waitTimeMill))
		return FALSE;
	if(m_ssznCam.m_capRes == FALSE)
		return FALSE;
	//�ȵ����ݺ�ŵ��߶Ⱦ���
	m_ssznCam.Copy2Data(m_cwcv.m_ssznpic.m_matHeightdata);
	if(m_cwcv.m_ssznpic.m_matHeightdata == NULL)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CWSSZN::FitPlaneByMask()
{
	m_cwcv.m_ssznpic.m_ROIGray = cvRect(0,0,0,0);
	if(!(FileExist(m_pathBottomMask)))
		return FALSE;
	m_cwcv.FitPlane(m_pathBottomMask);
	return TRUE;
}

BOOL CWSSZN::GetMidHeight(CvMat *matHeightdata)
{
	if(matHeightdata == NULL || matHeightdata->cols == 0 || matHeightdata->rows == 0)
	{
		return FALSE;
	}
	vector<float> vecLineData;
	//���ȶ�ȡ�м�һ�е�����
	for(int i = 0; i < matHeightdata->height;i++)
	{
		vecLineData.push_back(cvmGet(matHeightdata,i,matHeightdata->width/2));
	}
	//��ȡ֮��ת����0-255
	double minval = 10000; 
	double maxval = -10000;
	//grayval = ��fdata - co0(��Сֵ)��*co1���������ӣ�
	double co0 = 0;
	double co1 = 0;
	for (int i = 0 ;i < vecLineData.size();i++)
	{
		if(vecLineData[i] < minval)
		{
			minval = vecLineData[i]; 
		}
		if(vecLineData[i] > maxval)
		{
			maxval = vecLineData[i]; 
		}
	}
	co0 = minval;
	co1 = 255/(maxval - minval);
	//Ȼ����Ƶ�ͼ�񣨿� = matrows �� = 255��
	Mat pMidHeight = Mat::zeros(256,vecLineData.size(), CV_8UC1);
	for (int i = 0; i < vecLineData.size();i++)
	{
		//cvmSet(pMidHeight,i,(vecLineData[i]-co0)*co1,255);
		int valheight = (vecLineData[i]-co0)*co1;
		assert(valheight < 256);
		//�м�߶ȸ�ֵ,Խ�߱�ʾԽ��
		pMidHeight.at<uchar> (255 - valheight,i) = 255;
	}
	imwrite(m_pathMidHeight.GetBuffer(),pMidHeight);

	return TRUE;
}

//���������ת�ɻҶ����ݲ�����
BOOL CWSSZN::RenewGray(CString savePath)
{
	BOOL res = m_ssznCam.CopyGray2Data(m_cwcv.m_ssznpic.m_matTrueGray);
	if(res)
	{
		if(savePath == _T(" "))
		{
			savePath = m_pathTrueGray;
		}
		imwrite(savePath.GetBuffer(),m_cwcv.m_ssznpic.m_matTrueGray);
		m_pathTrueGray.ReleaseBuffer();
	}
	return res;
}
//���������ת��ͼ�����ݲ�����
BOOL CWSSZN::RenewHeight(IplImage* pImg)
{
	m_cwcv.FitPlane(" ");
	return m_cwcv.PRenewHeightPic(NULL,pImg,2);//2��ʾʹ��matside�����߶�����
}
BOOL CWSSZN::RenewHeight(CString savePath)
{
	if(savePath == _T(" "))
	{
		savePath = m_pathGray;
	}
	m_cwcv.FitPlane(" ");
	return m_cwcv.RenewHeightPic(NULL,savePath,2);//2��ʾʹ��matside�����߶�����
}

#if USE_3D_CAM == 1

BOOL SSZNCAMOP::connectByIp()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	//�������ýṹ��
	SR7IF_ETHERNET_CONFIG SREthernetConFig;             //Ethernet communication settings
	//��ȡIP�ؼ���ַ4��ֵ
	//�ӽ����ȡIP��ַ--����IP����ֵ
 	SREthernetConFig.abyIpAddress[0] = IpAddr[0];
 	SREthernetConFig.abyIpAddress[1] = IpAddr[1];
 	SREthernetConFig.abyIpAddress[2] = IpAddr[2];
 	SREthernetConFig.abyIpAddress[3] = IpAddr[3];


	/// <summary>Device ID (fixed to 0)</summary> �豸ID
	const int DEVICE_ID = 0;             //The communication device to communicate with.

	//�������
	int ret = SR7IF_EthernetOpen(DEVICE_ID, &SREthernetConFig);
	if (ret  ==  0)
	{		b_connect = true;
			SaveDataToFile();    
			return TRUE;
	}
	else
	{
		printf("����ʧ�ܣ�\n");
		AfxMessageBox(_T("����ʧ�ܣ�"), MB_OKCANCEL | MB_ICONQUESTION);
		return FALSE;
	}
}

void SSZNCAMOP::SaveDataToFile()
{
	//���浱ǰIP
	std::fstream file("IpSet.bin", std::ios::out);
	//IPд���ļ�
	file << IpAddr[0] << "," << IpAddr[1] 
	<< "," << IpAddr[2] << "," << IpAddr[3] << "," << "\n";
	file.close();
}

void SSZNCAMOP::ReadDataFromFile()
{
	int IpSet[4];
	IpSet[0] = 192;
	IpSet[1] = 168;
	IpSet[2] = 0;
	IpSet[3] = 131;  //Ĭ��ֵ

	//��ȡ�����IP��ַ�ļ�
	FILE *inputfile;
	if ((inputfile = fopen("IpSet.bin", "r")) == NULL)  //��ȡ�����ļ�
	{
		IpAddr[0] = IpSet[0];
		IpAddr[1] = IpSet[1];
		IpAddr[2] = IpSet[2];
		IpAddr[3] = IpSet[3];
		return;
	}
	int addr_d = 0;
	fscanf(inputfile, "%d,", &addr_d);
	IpAddr[0] = addr_d;
	fscanf(inputfile, "%d,", &addr_d);
	IpAddr[1] = addr_d;
	fscanf(inputfile, "%d,", &addr_d);
	IpAddr[2] = addr_d;
	fscanf(inputfile, "%d,", &addr_d);
	IpAddr[3] = addr_d;
}

void SSZNCAMOP::StartBatch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SR7IF_Data DataObject = NULL;

	const int DEVICE_ID = 0;        //The communication device to communicate with.
#if IOTRIGGER
	int reT = SR7IF_StartTriggerBatch(DEVICE_ID);    //Ӳ��IO������ʼ������
#else
	int reT = SR7IF_StartMeasure(DEVICE_ID);         //��ʼ������--��ʼɨ��
#endif
	printf("StartBatch:%d\n", reT);
	m_capRes = false;
	//�ȴ����ݽ������,������ɽ�����һ����20000�����ڽ�������Զ�������һ��������Ҫ�ȴ�20000���룻����ʱ�䳬��20000���룬��ʱ
	int ret = SR7IF_ReceiveData(0, DataObject, 2000);

	if (ret >= 0)  //�ɹ�
	{
		// ��ȡ����������
		batchPoint = SR7IF_ProfilePointCount(DataObject);
		printf("BatchPointCount:%d\n", batchPoint);

		//��ȡ��������
		int m_DataWidth = SR7IF_ProfileDataWidth(DataObject);
		printf("BatchWidth:%d\n", m_DataWidth);
		pdwBatchW = m_DataWidth;

		//����x������(mm)
		double m_XPitch = SR7IF_ProfileData_XPitch(DataObject);    
		printf("Data_XPitch:%f\n", m_XPitch);
		xPitch = m_XPitch;

		//��ȡ�߶�����
		//����߶����ݻ���
		if(pdwBatchData)
			delete[] pdwBatchData;
		pdwBatchData = new int[batchPoint * pdwBatchW];
		//����Ҷ����ݻ���
		if(grayData)
			delete[] grayData;
		grayData = new unsigned char[batchPoint * pdwBatchW];
		int m_curNo = 0;       //��ǰ��ȡ����
		int m_TotalNo = 0;     //�ܹ���ȡ������

		//��ȡ�߶�����
		m_curNo = SR7IF_GetProfileData(DataObject, pdwBatchData);
		int reTT = SR7IF_GetIntensityData(DataObject, grayData);

// 		if (C_DataType.GetCurSel() == 0)   //��ʾ�߶�ͼ
// 			BatchDataShow(pdwBatchData, 8.4, -8.4, 255, pdwBatchW, batchPoint);  //��ʾ�߶�����
// 		else if (C_DataType.GetCurSel() == 1) //��ʾ�Ҷ�ͼ
// 			GrayDataShow(grayData, pdwBatchW, batchPoint);

#ifdef _DEBUG  
		//��ȡ������ֵ����
		unsigned int* Encoder = new unsigned int[batchPoint];
		SR7IF_GetEncoder(DataObject, Encoder);

		for (int i = 0; i < batchPoint; i++)
			printf("Encoder:%d  %d\n", i, Encoder[i]);

		delete[] Encoder;
#endif

		//��ȡ����������
		if (LasetWidthData)
			delete[] LasetWidthData;
		LasetWidthData = new unsigned char[batchPoint * pdwBatchW];
		int RET = SR7IF_GetLaserWidthData(DataObject, LasetWidthData, batchPoint);
		printf("GetLaserWidthData ret:%d\n", RET);
		m_capRes = TRUE;
	}
	m_capFinish = TRUE;
// 	C_SaveData.EnableWindow(true);
// 	C_SaveGrayData.EnableWindow(true);
// 	C_BatchStart.EnableWindow(true);
// 	C_BatchStop.EnableWindow(false);
}

void SSZNCAMOP::Copy2Data(CvMat* &matdata)
{
	if(matdata == NULL || matdata->rows != batchPoint||matdata->cols != pdwBatchW)
	{
		cvReleaseMat(&matdata);
		matdata= cvCreateMat(pdwBatchW , batchPoint, CV_32FC1);
		//cvZero(matdata);
	}
	for (int i= 0; i< pdwBatchW ; i++)
	{
		for (int j = 0 ; j < batchPoint; j++)
		{
			cvmSet( matdata, i, j, pdwBatchData[j * pdwBatchW + i ] / 100000.0 );
		}
	}
}

void SSZNCAMOP::releaseCam()
{
	FreeConsole();
	if(pdwBatchData)
	{
		delete[] pdwBatchData;
		pdwBatchData = NULL;
	}
		
	if(grayData)
	{
		delete[] grayData;
		grayData = NULL;
	}
		
// 	if(b_connect)
// 	{
// 		SR7IF_CommClose(0);
// 		b_connect = FALSE;
// 	}
		
}

BOOL SSZNCAMOP::CopyGray2Data(cv::Mat &matdata)
{
	if(grayData == NULL)
	{
		return FALSE;
	}
	if(matdata.rows != batchPoint||matdata.cols != pdwBatchW)
	{
		matdata.release();
		matdata= Mat::zeros(batchPoint,pdwBatchW, CV_8UC1);
		//cvZero(matdata);
	}
	int grayval = 0;
	//���Ƶ��µľ�����
	for (int i= 0; i< pdwBatchW ; i++)
	{
		for (int j = 0 ; j < batchPoint; j++)
		{
			grayval = grayData[j * pdwBatchW + i ];
			matdata.at<uchar>(j,i) = grayval;
		}
	}
	return TRUE;
}

#endif
//ʹ��ȫ�������ƽ��
//��ֵ�ָ���ͨ��
//ƥ�����תƽ�ƾ���
//��׼ͼ������תƽ��,��ƥ�䵽ʵʱͼ
//��ñ���ͼ�Ϳ�ͼ
//��ȡȱ�ݿ�
//���ȱ�ݿ��ĵ�
BOOL THREAD_LINER_DATA::StartProcess()
{
	CString strSavePath;
	//m_grabImg->RenewHeight();
	//m_pImgHeightData = Mat(m_grabImg->m_cwcv.m_ssznpic.m_matHeightdata,TRUE);


	//ʹ��ȫ�������ƽ��
	IplImage* pHeightImg = cvCreateImage(cvSize(m_grabImg->m_cwcv.m_ssznpic.m_matHeightdata->rows,
		m_grabImg->m_cwcv.m_ssznpic.m_matHeightdata->cols),8,1);
	strSavePath.Format("D:\\tmp\\Height%d.bmp",m_nCurNum);
	m_grabImg->RenewHeight(pHeightImg);
	cvSaveImage(strSavePath,pHeightImg);

	//��ֵ�ָ�
	m_dThreadshold = m_grabImg->m_cwcv.m_ssznpic.co1*m_dThreadshold + m_grabImg->m_cwcv.m_ssznpic.co0;
	cvThreshold(pHeightImg,pHeightImg,m_dThreadshold,255,CV_THRESH_BINARY);
	cvSaveImage(strSavePath,pHeightImg);

	//ȡ��,���͸�ʴ
	cvNot(pHeightImg,pHeightImg);
	cvDilate(pHeightImg,pHeightImg,NULL,3);
	cvErode(pHeightImg,pHeightImg,NULL,3);
	cvSaveImage(strSavePath,pHeightImg);

	strSavePath.Format("D:\\tmp\\Gray%d.bmp",m_nCurNum);
	m_grabImg->RenewGray(strSavePath);
	strSavePath.Format("D:\\tmp\\Temp%d.bmp",m_nCurNum);
	imwrite(strSavePath.GetBuffer(),m_pImgTemp);
	strSavePath.Format("D:\\tmp\\Hole%d.bmp",m_nCurNum);
	imwrite(strSavePath.GetBuffer(),m_pImgHole);
	return TRUE;
}
