#include "StdAfx.h"
#include "WPowerCV.h"
#include "XFunCom.h"
CWPowerCV::CWPowerCV(void)
{
}


CWPowerCV::~CWPowerCV(void)
{
}

BOOL CWPowerCV::SplitByXYNum(CString InputImgPath,CString OutPutDir,int XNum,int YNum)
{
	//加载图像
	if (!FileExist(InputImgPath)||XNum <= 0|| YNum <= 0)
		return FALSE;
	IplImage* src = NULL;
	src = cvLoadImage(InputImgPath);
	if (src == NULL)
		return FALSE;
	int picwidth = src->width;
	int picHeight = src->height;
	//循环更新ROI并保存
	int stepWidth = picwidth/XNum;
	int stepHeight = picHeight/YNum;
	CString tmpstr;
	for (int ix = 0; ix < XNum;ix++)
	{
		for (int iy = 0; iy < YNum;iy++)
		{
			cvResetImageROI(src);
			cvSetImageROI(src,cvRect(ix*stepWidth,iy*stepHeight,stepWidth,stepHeight));
			tmpstr.Format("%s\\newPic%d_%d.bmp",OutPutDir,ix,iy);
			cvSaveImage(tmpstr,src);
		}
	}
	cvReleaseImage(&src);
	return TRUE;
}
struct paraseThreadData{
	int startindex;
	int endindex;
	vector<CString> *path;
	CvMat *datapath;
	BOOL DealFinish;
	void parase(){
		for(int i = startindex ; i < endindex;i++)
		{
			vector<CString> aline;
			splitStr((*path)[i].GetBuffer(0),",",aline);
			for (int j = 0; j < aline.size();j++)
			{
				cvmSet( datapath, j, i, atof(aline[j]) );
			}
		}
		DealFinish = TRUE;
	}
	void Init(){
		startindex = 0;
		endindex = 0;
		path = NULL;
		datapath = NULL;
		DealFinish = FALSE;
	}
};
UINT ParseThread(LPVOID pParam)
{
	///CLHomePara *pPara = (CLHomePara *)pParam;
	paraseThreadData *pFrame = (paraseThreadData *)pParam;
	pFrame->parase();
	return 1;
}
BOOL CWPowerCV::SplitScvByXYNum(CString InputscvPath,CString OutPutDir,int XNum,int YNum)
{
	CvMat *matHeight = NULL;
	//读取到数组
	vector<CString> alinepts;
	vector<float>	falinepts;
	vector<CString> ptsdata;
	vector<vector<float>> fpts;
	CString tmpSavePath("");
	GetDataFromFile(InputscvPath,ptsdata);
	if(ptsdata.size() < 1)
		return FALSE;
	splitStr(ptsdata[0].GetBuffer(0),",",alinepts);


	matHeight = cvCreateMat( alinepts.size(), ptsdata.size(), CV_32FC1);
	cvZero(matHeight);
	int row = alinepts.size();
	int col = ptsdata.size();

	BOOL usemuliteThread = TRUE;
	if(!usemuliteThread)
	{	
		for (int i = 0; i < ptsdata.size();i++)
		{
			vector<CString> alineptsnew;
			splitStr(ptsdata[i].GetBuffer(0),",",alineptsnew);
			for (int j =0; j < alineptsnew.size();j++){
				cvmSet( matHeight, j, i, atof(alineptsnew[j]) );
			}
		}

	}
	else{
		int threadnum = 15;
		paraseThreadData threaddata[20];
		int linesize = ptsdata.size();
		int linewidth = ptsdata.size()/threadnum;
		if(ptsdata.size()%threadnum != 0)
			linewidth++;
		for (int i = 0; i < linesize;i++)
		{
			vector<float> tmpfpts;
			fpts.push_back(tmpfpts);
		}
		for (int i = 0; i < threadnum ;i ++)
		{ 
			threaddata[i].Init();
			threaddata[i].startindex = i*linewidth;
			int endindex = (i*linewidth +linewidth);
			threaddata[i].endindex = endindex>linesize?linesize:endindex;
			threaddata[i].path = &ptsdata;
			threaddata[i].datapath = matHeight;
			AfxBeginThread(ParseThread,&threaddata[i],THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		while(1){
			BOOL finish = TRUE;
			for (int i = 0; i < threadnum; i++)
			{
				finish = finish && threaddata[i].DealFinish;
			}
			if (finish)
				break;
			MSG msg;
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))  
			{  
				DispatchMessage(&msg);  
				TranslateMessage(&msg);  
			} 
		}
	}

	//加载图像
	int picwidth = matHeight->rows;
	int picHeight = matHeight->cols;
	//循环更新ROI并保存
	int stepWidth = picwidth/XNum;
	int stepHeight = picHeight/YNum;
	CString tmpstr;
	for (int ix = 0; ix < XNum;ix++)
	{
		for (int iy = 0; iy < YNum;iy++)
		{
			vector<CString> newfile;
			for (int stepy = iy*stepHeight ;stepy < (iy+1)*stepHeight;stepy++)
			{
				CString aline;
				for (int stepx = ix*stepWidth ;stepx < (ix+1)*stepWidth;stepx++)
				{
					CString strdata;
					strdata.Format("%.4f",cvmGet(matHeight,stepx,stepy));
					if(stepy == (iy+1)*stepHeight - 1)
					{
						aline =  aline + strdata;
					}
					else
					{
						aline =  aline + strdata + ",";
					}	
				}
				newfile.push_back(aline);
			}
			tmpSavePath.Format("%s\\newscv_x%d_y%d.csv",OutPutDir,ix,iy);
			WriteDataToFile(tmpSavePath,newfile);
			
		}
	}
	if(m_ssznpic.m_matHeightdata != NULL)
	{
		cvReleaseMat(&m_ssznpic.m_matHeightdata);
		m_ssznpic.m_matHeightdata = NULL;
	}
	m_ssznpic.m_matHeightdata = matHeight;
	return TRUE;
}

//两张图像进行计算
//0表示相加1表示相减其它的等需要了再写
BOOL CWPowerCV::ImgCal(CString srcfirst,CString srcsecond,CString savePath,int XMove,int YMove,int CalType /*= 0*/,BOOL bDir)
{


	vector<CString> inputpath;
	if (!FileExist(srcsecond)||XMove <= 0|| YMove <= 0)
		return FALSE;
	if (bDir)
	{
		GetFileNameFromDir(srcfirst,inputpath,1);
		if (inputpath.size() == 0)
		{
			return FALSE;
		}
	}
	else
		inputpath.push_back(srcfirst);
	for (int i = 0; i < inputpath.size();i++)
	{
		srcfirst = inputpath[i];
		if(!FileExist(srcfirst))
			continue;
		IplImage* src1 = NULL;
		IplImage* src2 = NULL;
		src1 = cvLoadImage(srcfirst);
		src2 = cvLoadImage(srcsecond);
		CvRect srcROI;
		srcROI.x = XMove;
		srcROI.y = YMove;
		srcROI.width = (XMove+src1->width)>src2->width? src2->width - XMove : src1->width;
		srcROI.height = (YMove+src1->height)>src2->height? src2->height - YMove : src1->height;
		cvSetImageROI(src2,srcROI);
		int pos = 0;CString newname;
		switch(CalType){
		case 0:
			cvAdd(src2,src1,src2);
			pos = srcfirst.ReverseFind('.');
			newname = srcfirst.Left(pos);
			pos = srcfirst.ReverseFind('\\');
			newname = newname.Right(newname.GetLength() - pos);
			cvResetImageROI(src2);
			cvSaveImage(savePath + newname+"_type0.bmp",src2);
			break;
		default:
			break;
		}
		cvReleaseImage(&src1);
		cvReleaseImage(&src2);
	}
	return TRUE;

}

BOOL CWPowerCV::ParsePtSet(CString inputPath,CString OutPutPath,int type,double *co)
{
	CvMat* matHeight = NULL;
	//读取到数组
	vector<CString> alinepts;
	vector<float>	falinepts;
	vector<CString> ptsdata;
	vector<vector<float>> fpts;
	GetDataFromFile(inputPath,ptsdata);
	if(ptsdata.size() < 1)
		return FALSE;
	CVTRYSTART
	splitStr(ptsdata[0].GetBuffer(0),",",alinepts);


	matHeight = cvCreateMat( alinepts.size(), ptsdata.size(), CV_32FC1);
	cvZero(matHeight);
	int row = alinepts.size();
	int col = ptsdata.size();

	BOOL usemuliteThread = TRUE;
	if(!usemuliteThread)
	{	
		for (int i = 0; i < ptsdata.size();i++)
		{
			vector<CString> alineptsnew;
			splitStr(ptsdata[i].GetBuffer(0),",",alineptsnew);
			for (int j =0; j < alineptsnew.size();j++){
				cvmSet( matHeight, j, i, atof(alineptsnew[j]) );
			}
		}

	}
	else{
		int threadnum = 15;
		paraseThreadData threaddata[20];
		int linesize = ptsdata.size();
		int linewidth = ptsdata.size()/threadnum;
		if(ptsdata.size()%threadnum != 0)
			linewidth++;
		for (int i = 0; i < linesize;i++)
		{
			vector<float> tmpfpts;
			fpts.push_back(tmpfpts);
		}
		for (int i = 0; i < threadnum ;i ++)
		{ 
			threaddata[i].Init();
			threaddata[i].startindex = i*linewidth;
			int endindex = (i*linewidth +linewidth);
			threaddata[i].endindex = endindex>linesize?linesize:endindex;
			threaddata[i].path = &ptsdata;
			threaddata[i].datapath = matHeight;
			AfxBeginThread(ParseThread,&threaddata[i],THREAD_PRIORITY_NORMAL,0,0,NULL);
		}
		while(1){
			BOOL finish = TRUE;
			for (int i = 0; i < threadnum; i++)
			{
				finish = finish && threaddata[i].DealFinish;
			}
			if (finish)
				break;
			MSG msg;
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))  
			{  
				DispatchMessage(&msg);  
				TranslateMessage(&msg);  
			} 
		}
	}


	if(m_ssznpic.m_matHeightdata != NULL)
	{
		cvReleaseMat(&m_ssznpic.m_matHeightdata);
		m_ssznpic.m_matHeightdata = NULL;
	}
	m_ssznpic.m_matHeightdata = matHeight;
	RenewHeightPic(co, OutPutPath);	
	CVTRYEND
	return TRUE;
}
BOOL CWPowerCV::PRenewHeightPic(double * co, IplImage* pImg,int nInputType)
{
	CvMat matHeight;
	CvMat* pmatHeight = NULL;
	switch(nInputType)
	{
	case 1:
		pmatHeight = m_ssznpic.m_matHeightdata;
		break;
	case 2:
		matHeight = m_ssznpic.m_matDis;
		pmatHeight = &matHeight;
		break;
	default:
		break;
	}
	if(pmatHeight == NULL)
	{
		return FALSE;
	}
	double MinValue = 10000;
	double MaxValue = -10000;
	//无效数据点的位置
	vector<int> veci;
	vector<int> vecj;
	for (int i = 0; i < pmatHeight->rows;i++)
	{
		for (int j = 0; j < pmatHeight->cols; j++)
		{
			float val = cvmGet(pmatHeight,i,j);
			if(val == -100){
				veci.push_back(i);
				vecj.push_back(j);
			}
			else{
				if(MinValue > val)
				{
					MinValue = val;
				}
				if(MaxValue < val)
				{
					MaxValue = val;
				}
			}
		}
	}
	if(MinValue == 10000)
	{
		AfxMessageBox("3D数据无效,请重新采集");
		return FALSE;
	}
	// 	for (int num = 0; num < veci.size();num++)
	// 	{
	// 		cvmSet( matHeight,veci[num] , vecj[num],MinValue);
	// 	}
	double co0 = - MinValue*255.0/(MaxValue - MinValue);
	double co1 = 255.0/(MaxValue - MinValue);
	m_ssznpic.co0 = co0;
	m_ssznpic.co1 = co1;
	if(co != NULL){
		co[0] = co0;
		co[1] = co1;
	}
	//将所有数据归一化到0-255
	vector<CString> out;
	CString tmp;
	CTimeCount timer1;
	timer1.Start();
	double *fImgPointer = new double[pImg->height];
	memset(fImgPointer,0,pImg->height*sizeof(double));
	for (int n=1;n<pImg->height;n++)
	{
		fImgPointer[n] = fImgPointer[n-1]+pImg->widthStep;
	}
	for (int i = 0; i < pmatHeight->rows;i++)
	{
		for (int j = 0; j < pmatHeight->cols; j++)
		{
			float val = cvmGet(pmatHeight,i,j)*co1 + co0;
			//cvSet2D(pImg,j,i,cvScalarAll(val));
			//YX_BYTE(pImg,i,j) = val;
			int nPos = fImgPointer[j] + i;
			val = val<0?0:val;
			YX_BYTE_1ARRAY(pImg,nPos) = val;
		}
	}
	delete []fImgPointer;
	fImgPointer = NULL;
	timer1.End();
	TRACE("耗时%.4f",timer1.GetUseTime());
	return TRUE;
}
BOOL CWPowerCV::RenewHeightPic(double * co, CString OutPutPath,int nInputType)
{
	if(m_ssznpic.m_matHeightdata == NULL||m_ssznpic.m_matHeightdata->cols == 0)
		return FALSE;
	IplImage* pImg = cvCreateImage(cvSize(m_ssznpic.m_matHeightdata->rows,m_ssznpic.m_matHeightdata->cols),8,1);
	//将图像加载到pImg中
	BOOL res = PRenewHeightPic(co,pImg,nInputType);
	if(!res)
	{
		cvReleaseImage(&pImg);
		return FALSE;
	}
	m_ssznpic.graypath = OutPutPath;
	cvSaveImage(OutPutPath,pImg);
	cvReleaseImage(&pImg);
	return TRUE;
}
BOOL CWPowerCV::SplitByThreshold(CString InputImgPath,CString OutPutPath,int threshold)
{
	IplImage *pImg = cvLoadImage(InputImgPath,0);
	cvThreshold(pImg,pImg,threshold,255,CV_THRESH_BINARY);
	cvSaveImage(OutPutPath,pImg);
	cvReleaseImage(&pImg);
	return TRUE;
}
void CWPowerCV::FindContour(CString InputImgPath,CString OutPutPath,int method){
	IplImage *frame,*gray,*sobel;
	InputImgPath.Replace("\\","/");
	frame=cvLoadImage(InputImgPath);//加载图像
	gray=cvCreateImage(cvGetSize(frame),frame->depth,1);//分配图像空间
	sobel=cvCreateImage(cvGetSize(frame),IPL_DEPTH_16S,1);
	cvNamedWindow("frame");
	cvNamedWindow("gray");
	cvNamedWindow("sobel");
	cvCvtColor(frame,gray,CV_BGR2GRAY);//转为灰度
	cvSobel(gray,sobel,1,0,3);

	IplImage *sobel8u=cvCreateImage(cvGetSize(sobel),IPL_DEPTH_8U,1);
	cvConvertScaleAbs(sobel,sobel8u,1,0);
	cvSaveImage(OutPutPath,sobel8u);
	cvReleaseImage(&frame);//释放空间（对视频处理很重要，不释放会造成内存泄露）
	cvReleaseImage(&gray);
	cvReleaseImage(&sobel);
	cvReleaseImage(&sobel8u);
}
IplImage *src;  //源图像  
IplImage *mask;     //掩码图像  
IplImage *src1;       
int lodiff = 20, updiff = 20;  
int  connectivity = 4;  
int new_mask_val = 255;  
void onmouse(int event, int x, int y, int flags, void *param)  
{  
	switch (event)  
	{  
	case CV_EVENT_LBUTTONDOWN:  
		{     
			CvPoint seed = cvPoint(x, y);  
			int b = rand() & 255, g = rand() & 255, r = rand() & 255;//随机找到一个颜色  
			CvConnectedComp comp;  
			CvScalar color = CV_RGB(r, g, b);  
			int flags = connectivity | CV_FLOODFILL_FIXED_RANGE | (new_mask_val<< 8);  
			cvFloodFill(src, seed, color, CV_RGB(lodiff, lodiff, lodiff), CV_RGB(updiff, updiff, updiff), &comp, flags, mask);  
			cvShowImage("image", src);  
			printf("%g pixels were repainted\n", comp.area);  
		}  
		break;  
	case  CV_EVENT_RBUTTONDOWN:  
		cvShowImage("image", src1);  
		break;  

	}  
}  
void CWPowerCV::FillFloodFillTest(CString InpuPath)
{
	InpuPath.Replace("\\","/");
	src = cvLoadImage(InpuPath,1);  
	src1 = cvCloneImage(src);  
	/*********************************创建掩码图像****************************/  
	IplImage *mask = cvCreateImage(cvSize(src->width + 2, src->height + 2), 8, 1);  
	cvNamedWindow("image", CV_WINDOW_AUTOSIZE);  
	/******************创建滑动条**********************************/  
	cvCreateTrackbar("loDiff", "image", &lodiff, 255, NULL);  
	cvCreateTrackbar("upDiff", "image", &updiff, 255, NULL);  
	/********************定义鼠标响应函数****************************/  
	cvSetMouseCallback("image", onmouse, NULL);  
	cvShowImage("image", src);  
	cvWaitKey(0);  
	cvReleaseImage(&mask);  
	cvReleaseImage(&src);  
	cvDestroyWindow("image");  
}

BOOL CWPowerCV::SplitByROI(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height)
{
	CVTRYSTART
	InputImgPath.Replace("\\","/");
	IplImage *pImg = cvLoadImage(InputImgPath,0);
	cvSetImageROI(pImg,cvRect(x,y,width,height));
	cvSaveImage(OutPutPath,pImg);
	cvReleaseImage(&pImg);
	m_ssznpic.roipath = OutPutPath;
	return TRUE;
	CVTRYEND
}

BOOL CWPowerCV::SplitByDirection(CString InputImgPath,CString OutPutPath,int diedaiwidth,int boardwidth,int direction1,int seedThreshold)
{
	CVTRYSTART
	InputImgPath.Replace("\\","/");
	IplImage *pImg = cvLoadImage(InputImgPath,0);
	IplImage *pImgnew = cvCloneImage(pImg);
	IplImage *pImgtmp = cvCloneImage(pImg);
	int seedthreshold = seedThreshold;
	int widthboard = boardwidth;
	int direction = direction1;
	int times = 0;
	CvRect rect = cvRect(0,0,diedaiwidth,pImg->height);
	if(direction == 1){
		times = pImg->width / rect.width;
		if(pImg->width % rect.width != 0)
			times++;
	}
	else{
		times = pImg->height / rect.height;
		if(pImg->height % rect.height != 0)
			times++;
	}
	for (int i = 0 ; i < times ; i++)
	{
		if(pImg->width < (rect.x + rect.width))
			rect.width = pImg->width - rect.x;
		if(pImg->height < (rect.y + rect.height))
			rect.height = pImg->height - rect.y;
		cvZero(pImgtmp);
		cvSetImageROI(pImg,rect);
		cvSetImageROI(pImgnew,rect);
		cvSetImageROI(pImgtmp,rect);
		cvThreshold(pImg,pImgnew,seedthreshold - widthboard,255,CV_THRESH_TOZERO);
		cvThreshold(pImgnew,pImgnew,seedthreshold + widthboard,255,CV_THRESH_TOZERO_INV);
		cvCopy(pImgnew,pImgtmp);
		cvResetImageROI(pImg);
		cvResetImageROI(pImgnew);
		cvResetImageROI(pImgtmp);
		CvScalar pixel_v;
		int nozeroNum = 0;
		int nozeroValue = 0;
		for (int k = rect.y; k < rect.y + rect.height; k++)
        {
            for (int j = rect.x; j < rect.x + rect.width; j++)
            {
                pixel_v = cvGet2D(pImgtmp, k, j);
                if(pixel_v.val[0] != 0){
					nozeroNum++;
					nozeroValue += pixel_v.val[0];
				}
            }
        }
		if(nozeroNum != 0){
			seedthreshold = nozeroValue / nozeroNum;
		}
		if(direction == 1)
			rect.x += rect.width;
		else
			rect.y += rect.height;

	}		
	CvScalar pixel_v;
	for (int k = 0; k < pImgnew->height; k++)
	{
		for (int j = 0; j < pImgnew->width; j++)
		{
			pixel_v = cvGet2D(pImgnew, k, j);
			pixel_v.val[0] = 255 - pixel_v.val[0];
			cvSet2D(pImgnew,k,j,pixel_v);
		}
	}
	cvErode(pImgnew,pImgnew,NULL,3);
	cvDilate(pImgnew,pImgnew,NULL,3);
	cvThreshold(pImgnew,pImgnew,254,255,CV_THRESH_BINARY);
	cvSaveImage(OutPutPath,pImgnew);
	cvReleaseImage(&pImg);
	cvReleaseImage(&pImgnew);
	return TRUE;
	}catch(cv::Exception err){
	CString opencverr;
	opencverr.Format("%s",err.msg);
	TRACE(opencverr);
	return FALSE;}
}

void CWPowerCV::testsszn(CString pathColorTemper,SSZNDATA &sszndata)
{
	if(m_ssznpic.m_matDis.data == NULL||m_ssznpic.m_matHeightdata == NULL||!FileExist(m_ssznpic.holepath))
	{
		AfxMessageBox("数据处理流程输入数据无效");
		return;
	}
	//对数据进行处理
	m_ssznpic.roipath.Replace("\\","/");
	IplImage* pImg = cvLoadImage(m_ssznpic.roipath,0);
	IplImage* pImghole = cvLoadImage(m_ssznpic.holepath,0);
	sortByXY(sszndata.m_rownum);

	//提取孔距离数据和非孔距离数据开始
	vector<vector<float>> data;
	vector<double> boarddata;
	for (int i = 0; i < m_ssznpic.hole.size();i++)
	{
		vector<float> aholedata;
		for (int cwx = m_ssznpic.hole[i].x;cwx< m_ssznpic.hole[i].x + m_ssznpic.hole[i].width;cwx++)
		{
			const float* pData = m_ssznpic.m_matDis.ptr<float>(cwx);   //第i+1行的所有元素  
			for (int cwy = m_ssznpic.hole[i].y;cwy< m_ssznpic.hole[i].y + m_ssznpic.hole[i].height;cwy++)
			{
				//获取深度
				double val = pData[cwy];
				//获取是不是孔
				int valhole = YX_BYTE(pImghole,cwy,cwx);
				if(valhole == 0){
					if(val != 0)
						boarddata.push_back(val);
					continue;
				}
				if(val == -100)

					continue;
				aholedata.push_back(val);
			}
		}
		data.push_back(aholedata);
	}
	//提取孔距离数据和非孔距离数据结束
	



	//色温图绘制
	double nonval = sszndata.m_dThresholdDeep;
	double minval = nonval;
	double maxval = 0.050;
	Mat dstImage = Mat::zeros( m_ssznpic.m_matDis.cols,m_ssznpic.m_matDis.rows, CV_8UC3);
	for (int i = 0; i < dstImage.cols;i++)
	{
		for (int j = 0; j < dstImage.rows;j++)
		{
				double val = m_ssznpic.m_matDis.ptr<float>(i)[j];
				if(val == -100)
					continue;
				if(val > minval){
					//转到 0 - 255
					int newval = abs(val/maxval*255) - 255 > 0 ? 255:(abs(val/maxval)*255.0); 
					dstImage.at<Vec3b>(j,i)[0] = 0;
					dstImage.at<Vec3b>(j,i)[1] = 255 - newval;
					dstImage.at<Vec3b>(j,i)[2] = newval;
				}
		}
	}
	imwrite(pathColorTemper.GetBuffer(),dstImage);
	//色温图绘制结束

	//板数据提取开始
	
	if(1024 < m_ssznpic.hole.size())
	{
		//计算孔的外框
		CRect holerect[1024];
		for (int k = 0; k < m_ssznpic.hole.size();k++)
		{
			holerect[k] = CRect(m_ssznpic.hole[k].x + m_ssznpic.m_ROIGray.x,m_ssznpic.hole[k].y 
				+ m_ssznpic.m_ROIGray.y,m_ssznpic.hole[k].x+m_ssznpic.hole[k].width 
				+ m_ssznpic.m_ROIGray.x,m_ssznpic.hole[k].y+m_ssznpic.hole[k].height  
				+ m_ssznpic.m_ROIGray.y);

		}
		for (int i = 0; i < m_ssznpic.m_matDisSide.rows;i++)
		{
			const float* pData = m_ssznpic.m_matDisSide.ptr<float>(i);   //第i+1行的所有元素  
			for (int j = 0; j < m_ssznpic.m_matDisSide.cols;j++)
			{
				BOOL inRect = FALSE;
				for (int k = 0; k < m_ssznpic.hole.size();k++)
				{
					if(holerect[k].PtInRect(CPoint(i,j))){
						inRect = TRUE;
						break;
					}

				}
				if(!inRect){
					double val = pData[j];
					if(val == -100)
						continue;
					if(val != 0)
						boarddata.push_back(val);
				}
			}
		}
		double total = 0;
		double totalg[2] = {0,0};
		int num[2] = {0,0};
		double ave[2] = {0,0};
		for(int i = 0; i < boarddata.size();i++)
		{
			total += abs(boarddata[i]);
			if(boarddata[i] > 0){
				totalg[0] += boarddata[i];
				num[0] += 1;
			}
			else{
				totalg[1] += boarddata[i];
				num[1] += 1;
			}
		}
		ave[0] = totalg[0]/num[0];
		ave[1] = totalg[1]/num[1];
		TRACE("铜面点数\t距离和\t平均距离\t板面上点数\t板面上平均距离\t板面下点数\t板面下点数平均距离\n%d\t%.4f\t%.4f\t%d\t%.4f\t%d\t%.4f\n",
			boarddata.size(),total,total/(double)boarddata.size(),num[0],ave[0],num[1],ave[1]);
		sszndata.m_dis = total/(double)boarddata.size();
	}

	//板数据提取结束

	//孔数据提取开始
	CString index;
	for (int i = 0; i < m_ssznpic.hole.size();i++)
	{
		CvRect pt = m_ssznpic.hole[i];
		index.Format("%d",i+1);
		putText(dstImage, index.GetBuffer(), cv::Point(pt.x,pt.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255));
		rectangle(dstImage,cv::Point(pt.x,pt.y),cv::Point(pt.x+ pt.width,pt.y + pt.height),CV_RGB(255,255, 255));
	}
	TRACE("平均深度\t最大深度\t均方差\t铜面上点个数\t铜面上的平均距离\t铜面下点个数\t铜面下的平均距离\tROI图上坐标\n");

	double totalg[2] = {0,0};//板上点总数
	int num[2] = {0,0};//板下点总数
	for (int i = 0; i < data.size();i++)
	{
		totalg[0] = 0;totalg[1] = 0;num[0] = 0; num[1] = 0;
		double total = 0; 
		double maxhole = -100;
		for(int j = 0; j < data[i].size();j++){
			total += abs(data[i][j]);
			if(data[i][j]>0)
			{
				totalg[0] += data[i][j];
				num[0]++;
			}
			else
			{
				totalg[1] += data[i][j];
				num[1]++;
			}
			maxhole = maxhole<abs(data[i][j])?abs(data[i][j]):maxhole;
		}
		//计算平均值
		double average = total/data[i].size();
		double avetop = 0;
		double avebottom = 0;
		if(num[0] != 0)
			avetop = totalg[0]/num[0];
		if(num[1] != 0)
			avebottom = totalg[1]/num[1];
		//计算均方差
		double s=0;
		for(int j=0; j<data[i].size(); j++)
			s +=abs(abs(data[i][j]) - average);
		double ss = s/data[i].size(); //求均方差
		TRACE("%.4f\t%.4f\t%.4f\t%d\t%.4f\t%d\t%.4f\t(%d,%d)\n",
			average,maxhole,ss,num[0],avetop,num[1],avebottom,m_ssznpic.hole[i].x,m_ssznpic.hole[i].y);
		sszndata.m_diff.push_back(ss);
		sszndata.m_max.push_back(maxhole);
		sszndata.m_veca.push_back(average);
		sszndata.m_vecindex.push_back(i);
	}
	//孔数据提取结束
}



void CWPowerCV::GetContour(CString InputImgPath,CString OutPutPath,int minval)
{
	InputImgPath.Replace("\\","/");
	//载入原始图，且必须以二值图模式载入
	m_ssznpic.holepath = InputImgPath;
	Mat srcImage = imread(InputImgPath.GetBuffer(), 0);
	Mat element = getStructuringElement(MORPH_RECT,Size(2*1+1,2*1+1),Point(1,1));
	erode(srcImage,srcImage,element);
	dilate(srcImage,srcImage,element);
	//初始化结果图
	Mat dstImage = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC3);
	//查找轮廓
	findContours(srcImage, contours, hierarchy,
		CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//遍历所有顶层的轮廓， 以随机颜色绘制出每个连接组件颜色
	int index = 0;
	m_ssznpic.hole.clear();
	for (; index >= 0; index = hierarchy[index][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dstImage, contours, index, color, CV_FILLED, 8, hierarchy);
		CvRect rect = boundingRect(contours[index]);
		if(contourArea(contours[index]) < minval)
			continue;
		m_ssznpic.hole.push_back(rect);
		rectangle(dstImage,cv::Point(rect.x,rect.y),cv::Point(rect.x + rect.width,rect.y + rect.height),CV_RGB(255,255, 255));
		//cvRectangle(dstImage, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);
	}
	imwrite(OutPutPath.GetBuffer(),dstImage);
}

void CWPowerCV::FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,double boarddis,CString disImgPath)
{
	if (width == 0||height == 0)
	{
		return;
	}
	CvMat* matHeight = m_ssznpic.m_matHeightdata;
	vector<float> X_vector;
	vector<float> Y_vector;
	vector<float> Z_vector;
	BOOL bTestMaxDiff = FALSE;
	const int planeType = 1;

	double maxdiff = 0;
	CvRect rect = cvRect(x,y,width,height);
	InputImgPath.Replace("\\","/");
	IplImage* pImg = cvLoadImage(InputImgPath,0);
	IplImage* pImgNew = cvCloneImage(pImg);
	IplImage* pImgDis = cvCloneImage(pImg);

	m_ssznpic.m_matDis=Mat::zeros(pImg->width,pImg->height,CV_32FC1);
	m_ssznpic.m_matDisSide=Mat::zeros(pImg->width,pImg->height,CV_32FC1);
	vector<roiPointDecimal3D> t3dpt;
	roiPointDecimal3D t3dptsingal;
	RATIO_Plane plane3D;
	for(int i = rect.x; i < rect.x + rect.width;i++)
	{
		for (int j = rect.y; j < rect.y +rect.height;j++)
		{
			X_vector.push_back(i);
			Y_vector.push_back(j);
			double val1 = cvmGet( matHeight, i+m_ssznpic.m_ROIGray.x, j+m_ssznpic.m_ROIGray.y);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
			Z_vector.push_back(val1);
			t3dptsingal.xxx = i;
			t3dptsingal.yyy = j;
			t3dptsingal.zzz = val1;

			t3dpt.push_back(t3dptsingal);
//			检测最大误差,结果最大误差为0.005
			if(bTestMaxDiff){
				double real1 = (val1 - m_ssznpic.co0)/m_ssznpic.co1;
				double real2 = cvmGet( matHeight, i+m_ssznpic.m_ROIGray.x, j+m_ssznpic.m_ROIGray.y);
				double diff = real1 - real2;
				TRACE("差值为%.4f\n",diff);
				maxdiff = abs(diff)>maxdiff?abs(diff):maxdiff;
			}
		}
	}
	if(bTestMaxDiff)
		TRACE("最大差值为%.4f\n",maxdiff);
	int totaltimes = 3;
	for(int diedaitimes = 0;diedaitimes < totaltimes;diedaitimes++){
		CvMat*points_mat = cvCreateMat(X_vector.size(), 3, CV_32FC1);//定义用来存储需要拟合点的矩阵   
		for (int i=0;i < X_vector.size(); ++i)  
		{  
			points_mat->data.fl[i*3+0] = X_vector[i];//矩阵的值进行初始化   X的坐标值  
			points_mat->data.fl[i * 3 + 1] = Y_vector[i];//  Y的坐标值  
			points_mat->data.fl[i * 3 + 2] = Z_vector[i];//  Z的坐标值</span>  

		}  
		float plane12[4] = { 0 };//定义用来储存平面参数的数组 
		if(planeType == 0&&X_vector.size() > 2)
			cvFitPlane(points_mat, plane12);//调用方程
		else if(planeType == 1&&X_vector.size() > 2)
			fitPlane3D(&t3dpt[0],t3dpt.size(),&plane3D);
		int cwx = 0;
		int cwy = 0;
		double cwz = 0;
		double co0 = sqrt(plane12[0]*plane12[0]+plane12[1]*plane12[1]+plane12[2]*plane12[2]);
		X_vector.clear();Y_vector.clear();Z_vector.clear();

			for (int i =0 ; i < pImg->width;i++)
			{
				for (int j = 0; j < pImg->height;j++)
				{
					cwx = i;cwy = j;cwz = cvmGet( matHeight, i+m_ssznpic.m_ROIGray.x, j+m_ssznpic.m_ROIGray.y);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
					t3dptsingal.xxx = i;t3dptsingal.yyy = j;t3dptsingal.zzz = cwz;
					double sd = pointToPlaneDis3D(t3dptsingal, &plane3D) ;
					double d = (plane12[0]*(float)cwx + plane12[1]*(float)cwy  + plane12[2]*(float)cwz - plane12[3]) /co0;
					double diff = sd - d;
					if(planeType == 1)
						d = sd;
					//TRACE("diff = %.5f\n",diff);
					if (abs(d) < boarddis)
					{
						X_vector.push_back(i);Y_vector.push_back(j);Z_vector.push_back(cwz);
						if(diedaitimes == totaltimes - 1){
							cvSet2D(pImgNew,j,i,cvScalarAll(0));
						}
					}
					if(diedaitimes == totaltimes - 1){
						cvSet2D(pImgDis,j,i,cvScalarAll(abs(d)));
						m_ssznpic.m_matDis.at<float>(i,j) = (float)d;
					}
 					if (abs(d) < boarddis&&(diedaitimes == totaltimes - 2))
 						m_ssznpic.m_matDisSide.at<float>(i,j) = (float)d;

				}
			}
	}
	cvThreshold(pImgNew,pImgNew,1,255,CV_THRESH_BINARY);
	cvSaveImage(OutPutPath,pImgNew);
	cvSaveImage(disImgPath,pImgDis);
	cvReleaseImage(&pImgNew);
	cvReleaseImage(&pImg);
	cvReleaseImage(&pImgDis);

}

//灰度图弃用,改用matheight
//根据背景图和点集来刷新m_matDis和铜面数据

void CWPowerCV::FitPlane(CString backmaskPath)
{
	CvMat *matHeight = m_ssznpic.m_matHeightdata;
	BOOL buseMask = TRUE;
	if(_T(" ")== backmaskPath)
	{
		buseMask = FALSE;
	}
	//假如用户进入这里的时候,对相机的设置与界面设置不一致
	//会导致两图片大小不一,导致崩溃
	if(matHeight == NULL || matHeight->height == 0)
	{
		return;
	}

	double cwx = 0,cwy = 0.0,cwz = 0.0;
	
	//加载背景图
	IplImage* pBackMaskImg = NULL;
	if(_T(" ") != backmaskPath)
	{
		backmaskPath.Replace("\\","/");
		pBackMaskImg = cvLoadImage(backmaskPath,0);
		if(NULL == pBackMaskImg)
		{
			buseMask = FALSE;
		}
	}
	//初始化距离矩阵和铜面距离矩阵
	m_ssznpic.m_matDis=Mat::zeros(matHeight->height,matHeight->width,CV_32FC1);
	m_ssznpic.m_matDisSide=Mat::zeros(matHeight->height,matHeight->width,CV_32FC1);

	vector<roiPointDecimal3D> t3dpt;
	roiPointDecimal3D t3dptsingal;
	RATIO_Plane plane3D;


	int nMminWidth = matHeight->height; 
	int nMminHeight = matHeight->width; 
	for(int i = 0; i< nMminWidth;i++){
		for (int j =0 ;j < nMminHeight;j++)
		{
			if(!buseMask||cvGet2D(pBackMaskImg,j,i).val[0] == 255){
// 				if(GetRand(-3,3)>0)
// 					continue;
			double val1 = cvmGet( matHeight, i+m_ssznpic.m_ROIGray.x, j+m_ssznpic.m_ROIGray.y);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
			t3dptsingal.xxx = i;
			t3dptsingal.yyy = j;
			t3dptsingal.zzz = val1;
			if(val1 != -100)
			{
				t3dpt.push_back(t3dptsingal);
			}
				
			}

			  
		}
	}
	if(t3dpt.size() > 2)
		fitPlane3D(&t3dpt[0],t3dpt.size(),&plane3D);

	//假如使用的是全部点集,那么我们将点到面的距离进行排序后,将前50的点再拟合一次平面并求出新的距离
	if(!buseMask)
	{
		t3dpt.clear();
		vector<double> vecSortBy;

		for (int i =0 ; i < nMminWidth;i++)
		{
			for (int j = 0; j < nMminHeight;j++)
			{
				cwx = i;
				cwy = j;
				cwz = cvmGet( matHeight, i+m_ssznpic.m_ROIGray.x, j+m_ssznpic.m_ROIGray.y);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
				if(cwz == -100)
				{
					continue;
				}
				t3dptsingal.xxx = i;t3dptsingal.yyy = j;t3dptsingal.zzz = cwz;
				double sd = pointToPlaneDis3D(t3dptsingal, &plane3D) ;
				m_ssznpic.m_matDis.at<float>(i,j) = (float)sd;
				t3dpt.push_back(t3dptsingal);
				vecSortBy.push_back(abs(sd));
			}
		}
		double avgHeight = GetAvg(vecSortBy);
		vector<roiPointDecimal3D> newt3dpt;

		for (int i = 0;i < t3dpt.size();i++)
		{
			if(vecSortBy[i]<avgHeight)
				newt3dpt.push_back(t3dpt[i]);
		}
		if(newt3dpt.size() > 3)
			fitPlane3D(&newt3dpt[0],newt3dpt.size(),&plane3D);

	}
	for (int i =0 ; i < nMminWidth;i++)
	{
		for (int j = 0; j < nMminHeight;j++)
		{
			cwx = i;
			cwy = j;
			cwz = cvmGet( matHeight, i+m_ssznpic.m_ROIGray.x, j+m_ssznpic.m_ROIGray.y);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
			if(cwz == -100)
			{
				//无效数据点,在平面上用-100表示
				m_ssznpic.m_matDis.at<float>(i,j) = (float)-100;
				if(!buseMask||YX_BYTE(pBackMaskImg,j,i) == 255)
				{
					m_ssznpic.m_matDisSide.at<float>(i,j) = (float)-100;
				}
				continue;
			}
			t3dptsingal.xxx = i;t3dptsingal.yyy = j;t3dptsingal.zzz = cwz;
			double sd = pointToPlaneDis3D(t3dptsingal, &plane3D) ;
			m_ssznpic.m_matDis.at<float>(i,j) = (float)sd;
			if(!buseMask||YX_BYTE(pBackMaskImg,j,i) == 255)
			{
 				m_ssznpic.m_matDisSide.at<float>(i,j) = (float)sd;
			}
		}
	}

	cvReleaseImage(&pBackMaskImg);
}


void CWPowerCV::sortByXY(int numrows)
{
	vector<double> sotrby;
	for (int i = 0; i < m_ssznpic.hole.size();i++)
		sotrby.push_back(m_ssznpic.hole[i].y);
	cwsort(sotrby,m_ssznpic.hole,0,m_ssznpic.hole.size());
	sotrby.clear();
	for (int j = 0; j < m_ssznpic.hole.size();j++)
		sotrby.push_back(m_ssznpic.hole[j].x);
	int times = m_ssznpic.hole.size()/numrows;
	if(m_ssznpic.hole.size()%numrows != 0)
		times++;
	for (int k = 0; k < times;k++)
	{
		cwsort(sotrby,m_ssznpic.hole,k*numrows,(k*numrows+numrows)>m_ssznpic.hole.size()?m_ssznpic.hole.size():(k*numrows+numrows));
	}
}

BOOL CWPowerCV::ImgNot(CString InputImgPath,CString OutPutPath)
{
	IplImage* psrcimg = cvLoadImage(InputImgPath,0);
	IplImage* pdstimg = cvCloneImage(psrcimg);
	cvNot(psrcimg,pdstimg);
	cvSaveImage(OutPutPath,pdstimg);
	cvReleaseImage(&psrcimg);
	cvReleaseImage(&pdstimg);
	return TRUE;
}

