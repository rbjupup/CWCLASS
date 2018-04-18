#include "StdAfx.h"
#include "WPowerCV.h"
#include "XFunCom.h"
#include "opencv2\video\video.hpp"
#include "opencv2\objdetect\objdetect.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"


MFace::MFace(void)
{
	m_cascadeface = Loadhaar("C:\\Program Files\\opencv2.3.1\\data\\haarcascades\\haarcascade_frontalface_alt.xml");
}

MFace::~MFace(void)
{
}
CvHaarClassifierCascade* MFace::Loadhaar(CString path){
	return NULL;//return (CvHaarClassifierCascade*)cvLoad(path);
}

int MFace::FindAndDraw(CString image,CvHaarClassifierCascade* cascade,int pyrdown){
	//加载检测图片
	IplImage* mainImage = cvLoadImage(image,0);
	Facedata face;
	FindAndDraw(mainImage,pyrdown );

	//保存图像
	cvSaveImage("D:\\result.bmp",mainImage);

	return 0;
}

int MFace::FindAndDraw(IplImage* mainImage,int pyrdown)
{
	if(mainImage == NULL)
	{
		return 0;
	}
	facedata.Faceplace.clear();
	FindFace(mainImage,pyrdown,facedata);
	for (int i =0 ; i < facedata.Faceplace.size();i++)
	{
		cvDrawRect(mainImage,CwcvRectLP(facedata.Faceplace[i]),
		CwcvRectRB(facedata.Faceplace[i]),cvScalar(255,255,0));
	}
	return 1;
}

void MFace::FindFace(IplImage* mainImage,int pyrdown , Facedata &data)
{
	CvHaarClassifierCascade* cascade = m_cascadeface;
	CvMemStorage* mem= cvCreateMemStorage(0);
	CvSeq* seq = NULL;

	//如果金子塔变换加载小图片
	IplImage* smallImage;
	if(pyrdown > 0){
		smallImage = cvCreateImage(cvSize(mainImage->width/2,mainImage->height/2),mainImage->depth,mainImage->nChannels);
		cvPyrDown(mainImage,smallImage,CV_GAUSSIAN_5x5);
	}
	else{
		smallImage = cvCreateImage(cvGetSize(mainImage),mainImage->depth,mainImage->nChannels);
		cvCopy(mainImage,smallImage);
	}
	//绘制找到的地方
	//seq = cvHaarDetectObjects(smallImage,cascade,mem,1.2,2,CV_HAAR_DO_CANNY_PRUNING);
	for (int i = 0; i < seq->total;i++){
		CvRect center = *(CvRect*)cvGetSeqElem(seq,i);
		if(!pyrdown)
		{
			data.Faceplace.push_back(center);
		}
		else
		{
			center.x *=2;
			center.y *=2;
			center.width *= 2;
			center.height *= 2;
			data.Faceplace.push_back(center);
		}
		
		//cvDrawRect(mainImage,cvPoint(center.x,center.y),cvPoint(center.x+center.width,center.y + center.height),cvScalarAll(255),2);
	}
}

CWPowerCV::CWPowerCV(void)
{
	m_bShowTmp =  TRUE;
	capture =NULL;
}


CWPowerCV::~CWPowerCV(void)
{
	releasecwcam();
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
	CString strTmpSplite;
	void parase(){
		for(int i = startindex ; i < endindex;i++)
		{
			vector<CString> aline;
			strTmpSplite.Format("%s",(*path)[i]);
			splitStr(strTmpSplite.GetBuffer(0),",",aline);
			for (int j = 0; j < aline.size();j++)
			{
				YX_32F_CVMAT( datapath, i, j) =  atof(aline[j]) ;
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


	matHeight = cvCreateMat( ptsdata.size(),alinepts.size(), CV_32F);
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
				YX_32F_CVMAT( matHeight, i,j) =  atof(alineptsnew[j]) ;
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
	int picwidth = matHeight->cols;
	int picHeight = matHeight->rows;
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
					strdata.Format("%.4f",YX_32F_CVMAT(matHeight,stepy,stepx));
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
	CString strTmpSplite;
	strTmpSplite.Format("%s",ptsdata[0]);
	splitStr(strTmpSplite.GetBuffer(0),",",alinepts);


	matHeight = cvCreateMat( ptsdata.size(),alinepts.size(),  CV_32F);
	cvZero(matHeight);
	BOOL usemuliteThread = TRUE;
	if(!usemuliteThread)
	{	
		for (int i = 0; i < ptsdata.size();i++)
		{
			vector<CString> alineptsnew;
			strTmpSplite.Format("%s",ptsdata[i]);
			splitStr(strTmpSplite.GetBuffer(0),",",alineptsnew);
			for (int j =0; j < alineptsnew.size();j++){
				YX_32F_CVMAT( matHeight, i, j ) = atof(alineptsnew[j]) ;
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
			float val = YX_32F_CVMAT(pmatHeight,i,j);
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
			float val = YX_32F_CVMAT(pmatHeight,i,j)*co1 + co0;
 			val = val<0?0:val;
// 			YX_BYTE(pImg,i,j) = val;
 			int nPos = fImgPointer[i] + j;
 			YX_BYTE_1ARRAY(pImg,nPos) = val;
		}
	}
	delete []fImgPointer;
	fImgPointer = NULL;
	timer1.End();
	return TRUE;
}
BOOL CWPowerCV::RenewHeightPic(double * co, CString OutPutPath,int nInputType)
{
	if(m_ssznpic.m_matHeightdata == NULL||m_ssznpic.m_matHeightdata->cols == 0)
		return FALSE;
	IplImage* pImg = cvCreateImage(cvSize(m_ssznpic.m_matHeightdata->cols,m_ssznpic.m_matHeightdata->rows),8,1);
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
void CWPowerCV::FillFloodFill(CString InpuPath)
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
BOOL CWPowerCV::GetHoleDeep(IplImage* pImghole,CString pathColorTemper,SSZNDATA &sszndata)
{
	if(m_ssznpic.m_matDis.data == NULL||m_ssznpic.m_matHeightdata == NULL)
	{
		AfxMessageBox("数据处理流程输入数据无效");
		return FALSE;
	}

	BOOL bShowTmp = m_bShowTmp;
	BOOL bNeedSort = TRUE;
	BOOL bNeedBoard = FALSE;
	BOOL bNeedTemper = TRUE;
	//孔位置排序
	if(bNeedSort)
	{
		CWTS(TIMSORTHOLE)
		sortByXY(sszndata.m_rownum);
		if (bShowTmp)
		{
			CWTE(TIMSORTHOLE,"获取孔深_孔排序耗时%.4f\n")
		}
	}

	//提取孔距离数据和非孔距离数据开始
	CWTS(TIMGETDATA)
	vector<vector<float>> data;
	vector<double> boarddata;
	for (int i = 0; i < m_ssznpic.hole.size();i++)
	{
		vector<float> aholedata;
		for (int cwx = m_ssznpic.hole[i].x;cwx< m_ssznpic.hole[i].x + m_ssznpic.hole[i].width;cwx++)
		{
			for (int cwy = m_ssznpic.hole[i].y;cwy< m_ssznpic.hole[i].y + m_ssznpic.hole[i].height;cwy++)
			{
				//获取深度
				double val = YX_32F_MAT(m_ssznpic.m_matDis,cwy,cwx);
				//插入到数组
				int valhole = YX_BYTE(pImghole,cwy,cwx);
				if(bNeedBoard)//不是孔的话就插入板数据中
				{
					if(valhole == 0||valhole < sszndata.m_dthresholdeff)
					{
						if(val != -100)
							boarddata.push_back(val);
						continue;
					}
				}
				if(val == -100)
					continue;
				aholedata.push_back(val);
			}
		}
		data.push_back(aholedata);
	}
	if (bShowTmp)
	{
		CWTE(TIMGETDATA,"获取孔深_获取孔数据耗时%.4f\n")	//孔数据提取开始
	}
	//提取孔距离数据和非孔距离数据结束




	//色温图绘制
	Mat dstImage = Mat::zeros( m_ssznpic.m_matDis.rows,m_ssznpic.m_matDis.cols, CV_8UC3);
	if(bNeedTemper&&pathColorTemper != _T(" "))
	{
		CWTS(TIMTEMPER)
		double nonval = sszndata.m_dThresholdDeep;
		double minval = nonval;
		double maxval = 0.050;
		for (int i = 0; i < dstImage.cols;i++)
		{
			for (int j = 0; j < dstImage.rows;j++)
			{
				double val = YX_32F_MAT(m_ssznpic.m_matDis,j,i);//.ptr<float>(i)[j];
				if(val == -100)
					continue;
				//if(val > minval){
				//转到 0 - 255
				int newval = abs(val/maxval*255) - 255 > 0 ? 255:(abs(val/maxval)*255.0); 
				((BYTE*)(dstImage.data +3*(j)*(dstImage.cols)))[3*i] = 0;
				((BYTE*)(dstImage.data +3*(j)*(dstImage.cols)))[3*i+1] = 255 - newval;
				((BYTE*)(dstImage.data +3*(j)*(dstImage.cols)))[3*i+2] = newval;
				//}
			}
		}
		if (bShowTmp)
		{
			CWTE(TIMTEMPER,"获取孔深_绘制色温图耗时%.4f\n")		
			CString index;
			for (int i = 0; i < m_ssznpic.hole.size();i++)
			{
				CvRect pt = m_ssznpic.hole[i];
				index.Format("%d",i+1);
				putText(dstImage, index.GetBuffer(), cv::Point(pt.x,pt.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255));
				rectangle(dstImage,cv::Point(pt.x,pt.y),cv::Point(pt.x+ pt.width,pt.y + pt.height),CV_RGB(255,255, 255));
			}
		}
		imwrite(pathColorTemper.GetBuffer(),dstImage);
	}
	//色温图绘制结束


	//板数据提取开始

	if(bNeedBoard&&m_ssznpic.hole.size()> 0)
	{
		CWTS(TIMGETBOARD)
		//计算孔的外框
		CRect *grouprect = new CRect[m_ssznpic.hole.size()];
		CRect *holerect = &grouprect[0];
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
		sszndata.m_dis = total/(double)boarddata.size();
		delete [] grouprect;
		if (bShowTmp)
		{
			TRACE("铜面点数\t距离和\t平均距离\t板面上点数\t板面上平均距离\t板面下点数\t板面下点数平均距离\n%d\t%.4f\t%.4f\t%d\t%.4f\t%d\t%.4f\n",
				boarddata.size(),total,total/(double)boarddata.size(),num[0],ave[0],num[1],ave[1]);
			CWTE(TIMGETBOARD,"获取孔深_提取计算板数据耗时%.4f\n")
			TRACE("平均深度\t最大深度\t均方差\t铜面上点个数\t铜面上的平均距离\t铜面下点个数\t铜面下的平均距离\tROI图上坐标\n");

		}
	}

	//板数据提取结束


	//孔数据计算开始
	CWTS(TIMHOLECAL)
	double totalg[2] = {0,0};//板上点总数
	int num[2] = {0,0};//板下点总数
	for (int i = 0; i < data.size();i++)
	{
		if(data[i].size() == 0)
		{
			continue;
		}
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
		sszndata.m_diff.push_back(ss);
		sszndata.m_max.push_back(maxhole);
		sszndata.m_veca.push_back(average);
		sszndata.m_vecindex.push_back(i);
		sszndata.m_center.push_back(m_ssznpic.hole[i]);
		if(bShowTmp)
		{
			TRACE("%.4f\t%.4f\t%.4f\t%d\t%.4f\t%d\t%.4f\t(%d,%d)\n",
				average,maxhole,ss,num[0],avetop,num[1],avebottom,m_ssznpic.hole[i].x,m_ssznpic.hole[i].y);
		}
		if (bShowTmp)
		{
			if (bShowTmp)
			{
				CString index;
				CvRect pt = m_ssznpic.hole[i];
				index.Format("%.4f",average);
				putText(dstImage, index.GetBuffer(), cv::Point(pt.x+90,pt.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(255));
			}
			imwrite(pathColorTemper.GetBuffer(),dstImage);
		}
	}
	if (bShowTmp)
	{
		CWTE(TIMHOLECAL,"获取孔深_计算孔数据耗时%.4f\n")
	}
	//孔数据提取结束

	return TRUE;
}
void CWPowerCV::testsszn(CString pathColorTemper,SSZNDATA &data)
{
	//对数据进行处理
	if(!FileExist(m_ssznpic.holepath))
	{
		return;
	}
	IplImage* pImghole = cvLoadImage(m_ssznpic.holepath,0);
	GetHoleDeep(pImghole,pathColorTemper,data);
	cvReleaseImage(&pImghole);
	pImghole = NULL;
}



BOOL CWPowerCV::GetContour(CString InputImgPath,CString OutPutPath,int minval)
{
	double dConArea;     
	CvSeq *pContour = NULL;     
	CvSeq *pConInner = NULL;     
	CvMemStorage *pStorage = NULL;  
	BOOL showTestPic = TRUE;
	IplImage* pBinary = cvLoadImage(InputImgPath,0);
	IplImage* pdst = cvCloneImage(pBinary);
	cvZero(pdst);
	// 执行条件     
	if (pBinary)     
	{     
		// 查找所有轮廓     
		pStorage = cvCreateMemStorage(0);     
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);     
		// 填充所有轮廓     
		cvDrawContours(pdst, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 2, CV_FILLED, 8, cvPoint(0, 0));    
		// 外轮廓循环     
		int wai = 0;    
		int nei = 0;    
		for (; pContour != NULL; pContour = pContour->h_next)     
		{     
			wai++;    
// 			// 内轮廓循环     
// 			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)     
// 			{     
// 				nei++;    
// 				// 内轮廓面积     
// 				dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ)); 
// 			}    
			dConArea = fabs(cvContourArea(pContour, CV_WHOLE_SEQ)); 
			if(dConArea < minval)
				continue;
			CvRect rect = cvBoundingRect(pContour,0);
			m_ssznpic.hole.push_back(rect);
			cvRectangle(pdst, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);  
		}       
		cvReleaseMemStorage(&pStorage);     
		pStorage = NULL;     
	}
	else
	{
		return FALSE;
	}
	cvSaveImage(OutPutPath,pdst);
	cvReleaseImage(&pBinary);
	cvReleaseImage(&pdst);
	return TRUE;
}


BOOL CWPowerCV::CvToFile(Mat&mat,CString savePath,int type)
{
	BOOL bshowTmp = TRUE;
	CWTS(TIM1)
		if(type == 1)
		{
			CvMat cvmat;
			CvTranPointerType(IplImage(),mat,cvmat,FALSE,MAT_CVMAT);
			cvSave(savePath,&cvmat);
		}
		if(type == 2)
		{
			CFile file(savePath,CFile::modeCreate|CFile::modeWrite);
			file.Write(mat.data,mat.cols*mat.rows*sizeof(float));
			file.Close();

		}
		if(type == 3)
		{
			vector<float> fdata;
			fdata.reserve(mat.cols*mat.rows);
			for (int j = 0; j < mat.rows;j++)
			{
				for (int i = 0; i < mat.cols;i++)
				{
					fdata.push_back(YX_32F_MAT(mat,j,i));
				}
			}
			FileStorage fw(savePath.GetBuffer(0), FileStorage::WRITE);//做输出
			fw << "vector" << fdata;//通过输出流写文件，给定MAT标签
			fw.release();//释放文件流对象 	Mat mat;

		}
		if(bshowTmp)
		{
			CWTE(TIM1,"保存到文件 耗时为%.4f\n");
		}
		return TRUE;
}

BOOL CWPowerCV::FileToCv(Mat&mat,CString LoadPath,int type)
{
	BOOL bshowTmp = TRUE;
	CWTS(TIM1)
		if(type == 1)
		{
			CvMat* cvmat;
			cvmat = (CvMat*)cvLoad(LoadPath);
			CvTranPointerType(IplImage(),mat,*cvmat,FALSE,CVMAT_MAT);
		}
		if(type == 2)
		{
			CFile file(LoadPath,CFile::modeCreate|CFile::modeRead);
			file.Read(mat.data,mat.cols*mat.rows*sizeof(float));
			file.Close();

		}
		if(type == 3)
		{
			vector<float> fdata;
			fdata.reserve(mat.cols*mat.rows);
			FileStorage fw(LoadPath.GetBuffer(0), FileStorage::READ);//做输出
			fw [ "vector"] >> fdata;//通过输出流写文件，给定MAT标签
			fw.release();//释放文件流对象 	Mat mat;
			for (int j = 0; j < mat.rows;j++)
			{
				for (int i = 0; i < mat.cols;i++)
				{
					YX_32F_MAT(mat,j,i) = fdata[i*mat.rows+j];
				}
			}

		}
		if(bshowTmp)
		{
			CWTE(TIM1,"保存到文件 耗时为%.4f\n");
		}
		return TRUE;
}
BOOL CWPowerCV::GetContour(IplImage* pBinary,double dAreaThre,vector<CvRect> &boundrect)
{
	double dConArea;     
	CvSeq *pContour = NULL;     
	CvSeq *pConInner = NULL;     
	CvMemStorage *pStorage = NULL;  
	BOOL showTestPic = FALSE;
	IplImage* ptest = NULL;
	if (showTestPic)
	{
		ptest = cvCloneImage(pBinary);
		cvZero(ptest);
	}
	// 执行条件     
	if (pBinary)     
	{     
		// 查找所有轮廓     
		pStorage = cvCreateMemStorage(0);     
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_CODE);     
		// 填充所有轮廓         
		// 外轮廓循环     
		int wai = 0;    
		int nei = 0;    
		for (; pContour != NULL; pContour = pContour->h_next)     
		{     
			wai++;    
			if (showTestPic)
			{
				cvSeqInsert(pContour,pContour->total,cvGetSeqElem(pContour, 0));
				cvDrawContours(ptest, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));
			}
				
			// 			// 内轮廓循环     
			// 			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)     
			// 			{     
			// 				nei++;    
			// 				// 内轮廓面积     
			// 				dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ)); 
			// 			}    

			dConArea = fabs(cvContourArea(pContour, CV_WHOLE_SEQ)); 
			if(dConArea < dAreaThre)
				continue;

			CvRect rect = cvBoundingRect(pContour,0);
			boundrect.push_back(rect);
			//if (showTestPic)
			//	cvRectangle(ptest, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);  
		}       
		cvReleaseMemStorage(&pStorage);     
		pStorage = NULL;     
	}
	else
	{
		return FALSE;
	}
	if (showTestPic)
	{
		cvSaveImage("D:\\test.bmp",ptest);
		cvReleaseImage(&ptest);
		ptest = NULL;
	}


	return TRUE;
}

BOOL CWPowerCV::GetContour(IplImage* pBinary,double dAreaThre,vector<CRect> &boundrect)
{
	double dConArea;     
	CvSeq *pContour = NULL;     
	CvSeq *pConInner = NULL;     
	CvMemStorage *pStorage = NULL;  
	// 执行条件     
	if (pBinary)     
	{     
		// 查找所有轮廓     
		pStorage = cvCreateMemStorage(0);     
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);      
		// 外轮廓循环     
		int wai = 0;    
		int nei = 0;    
		for (; pContour != NULL; pContour = pContour->h_next)     
		{     
			wai++;    
			dConArea = fabs(cvContourArea(pContour, CV_WHOLE_SEQ)); 
			if(dConArea < dAreaThre)
				continue;

			CvRect rect = cvBoundingRect(pContour,0);
			boundrect.push_back(CRect(CPoint(rect.x,rect.y),CPoint(rect.x+rect.width,rect.y+rect.height)));
		}       
		cvReleaseMemStorage(&pStorage);     
		pStorage = NULL;     
	}
	else
	{
		return FALSE;
	}


	return TRUE;
}
BOOL CWPowerCV::GetContour(IplImage* pBinary, double dAreaThre)
{    
	double dConArea;     
	CvSeq *pContour = NULL;     
	CvSeq *pConInner = NULL;     
	CvMemStorage *pStorage = NULL;  
	BOOL showTestPic = TRUE;
	IplImage* ptest = NULL;
	if (showTestPic)
	{
		ptest = cvCloneImage(pBinary);
		cvZero(ptest);
	}
	// 执行条件     
	if (pBinary)     
	{     
		// 查找所有轮廓     
		pStorage = cvCreateMemStorage(0);     
		cvFindContours(pBinary, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_CODE);        
		// 外轮廓循环     
		int wai = 0;    
		int nei = 0;    
		for (; pContour != NULL; pContour = pContour->h_next)     
		{     
			wai++;    
// 			// 内轮廓循环     
// 			for (pConInner = pContour->v_next; pConInner != NULL; pConInner = pConInner->h_next)     
// 			{     
// 				nei++;    
// 				// 内轮廓面积     
// 				dConArea = fabs(cvContourArea(pConInner, CV_WHOLE_SEQ)); 
// 			}    

			if (showTestPic)
			{
				cvSeqInsert(pContour,pContour->total,cvGetSeqElem(pContour, 0));
				cvDrawContours(ptest, pContour, CV_RGB(255, 255, 255), CV_RGB(255, 255, 255), 0, CV_FILLED, 8, cvPoint(0, 0));
			}
			dConArea = fabs(cvContourArea(pContour, CV_WHOLE_SEQ)); 
			if(dConArea < dAreaThre)
				continue;

			CvRect rect = cvBoundingRect(pContour,0);
			m_ssznpic.hole.push_back(rect);
			if (showTestPic)
				cvRectangle(ptest, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);  
		}       
		cvReleaseMemStorage(&pStorage);     
		pStorage = NULL;     
	}
	else
	{
		return FALSE;
	}
	if (showTestPic)
	{
		cvSaveImage("D:\\test.bmp",ptest);
		cvReleaseImage(&ptest);
		ptest = NULL;
	}


	return TRUE;
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
	if (NULL == pImg)
	{
		return;
	}
	IplImage* pImgNew = cvCloneImage(pImg);
	IplImage* pImgDis = cvCloneImage(pImg);

	m_ssznpic.m_matDis=Mat::zeros(pImg->height,pImg->width,CV_32F);
	m_ssznpic.m_matDisSide=Mat::zeros(pImg->height,pImg->width,CV_32F);
	vector<roiPointDecimal3D> t3dpt;
	roiPointDecimal3D t3dptsingal;
	RATIO_Plane plane3D;
	for(int i = rect.x; i < rect.x + rect.width;i++)
	{
		for (int j = rect.y; j < rect.y +rect.height;j++)
		{
			X_vector.push_back(i);
			Y_vector.push_back(j);
			double val1 = YX_32F_CVMAT( matHeight, j+m_ssznpic.m_ROIGray.y , i+m_ssznpic.m_ROIGray.x);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
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
		CvMat*points_mat = cvCreateMat(X_vector.size(), 3, CV_32F);//定义用来存储需要拟合点的矩阵   
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
		for (int j = 0; j < pImg->height;j++)
		{
			for (int i =0 ; i < pImg->width;i++)
			{

				cwx = i;cwy = j;cwz = YX_32F_CVMAT( matHeight, j+m_ssznpic.m_ROIGray.y, i+m_ssznpic.m_ROIGray.x);//*ssznpic.co1+ssznpic.co0;//cvGet2D(pImg,j,i).val[0];
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
						YX_BYTE(pImgNew,j,i) = 0;
					}
				}
				if(diedaitimes == totaltimes - 1){
					YX_BYTE(pImgDis,j,i) = abs(d);
					YX_32F_MAT(m_ssznpic.m_matDis,j,i)  = (float)d;
				}
				if (abs(d) < boarddis&&(diedaitimes == totaltimes - 2))
					YX_32F_MAT(m_ssznpic.m_matDisSide,j,i)  = (float)d;

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
	BOOL buseMask = TRUE;
	if(_T(" ")== backmaskPath||!FileExist(backmaskPath))
	{
		buseMask = FALSE;
	}	
	//加载背景图
	IplImage* pBackMaskImg = NULL;
	if(_T(" ") != backmaskPath&&FileExist(backmaskPath))
	{
		backmaskPath.Replace("\\","/");
		pBackMaskImg = cvLoadImage(backmaskPath,0);
		if(NULL == pBackMaskImg)
		{
			buseMask = FALSE;
		}
		buseMask = TRUE;
	}
	FitPlane(buseMask?pBackMaskImg:NULL);
	cvReleaseImage(&pBackMaskImg);
	pBackMaskImg = NULL;
}

void CWPowerCV::FitPlane(IplImage* pBackMaskImg)
{
	CvMat *matHeight = m_ssznpic.m_matHeightdata;
	BOOL buseMask = TRUE;
	BOOL bSaveBack = TRUE;

	BOOL bShowTmp = m_bShowTmp;

	if(pBackMaskImg == NULL)
		buseMask = FALSE;
	//假如用户进入这里的时候,对相机的设置与界面设置不一致
	//会导致两图片大小不一,导致崩溃
	if(matHeight == NULL || matHeight->height == 0)
	{
		return;
	}

	double cwx = 0,cwy = 0.0,cwz = 0.0;


	//初始化距离矩阵和铜面距离矩阵
	m_ssznpic.m_matDis=Mat::zeros(matHeight->height,matHeight->width,CV_32F);
	m_ssznpic.m_matDisSide=Mat::zeros(matHeight->height,matHeight->width,CV_32F);

	vector<roiPointDecimal3D> t3dpt;
	t3dpt.reserve(matHeight->height*matHeight->width);
	roiPointDecimal3D t3dptsingal;
	RATIO_Plane plane3D;

	//提取点集拟合平面
	CWTS(TIMGETPT)
	int nMminWidth = matHeight->width-m_ssznpic.m_ROIGray.width; 
	int nMminHeight = matHeight->height-m_ssznpic.m_ROIGray.height; 
	for (int j =0 ;j < nMminHeight;j++)
	{
		for(int i = 0; i< nMminWidth;i++)
		{

			int valgray = 255;
			if(buseMask == 1)
				valgray = (int)YX_BYTE(pBackMaskImg,j,i);
			if(!buseMask||valgray == 255){
				t3dptsingal.xxx = i;
				t3dptsingal.yyy = j;
				t3dptsingal.zzz = YX_32F_CVMAT(matHeight,j,i);
				if(t3dptsingal.zzz != -100)
				{
					t3dpt.push_back(t3dptsingal);
				}

			}
		}
	}
	if(bShowTmp)
	{
		CWTE(TIMGETPT,"平面拟合_提取用来拟合平面的点耗时为%.4f\n")
	}
	CWTS(TIMFitP)
	if(t3dpt.size() > 2)
		fitPlane3D(&t3dpt[0],t3dpt.size(),&plane3D);
	if(bShowTmp)
	{
		CWTE(TIMFitP,"平面拟合_初步拟合拟合平面耗时为%.4f\n")
	}
	if(!buseMask)//如果不使用背景图,那么使用全部点初次拟合的平面进行二次拟合
	{
		CWTS(TIMTWICE)
		//t3dpt.clear();
		vector<double> vecSortBy;
		vecSortBy.reserve(t3dpt.size());
		for (int i = 0; i < t3dpt.size();i++)
		{
			double sd = pointToPlaneDis3D(t3dpt[i], &plane3D) ;
			vecSortBy.push_back(abs(sd));
		}

		double avgHeight = GetAvg(vecSortBy);
		vector<roiPointDecimal3D> newt3dpt;

		newt3dpt.reserve(vecSortBy.size());
		//使用低于平均距离的点重新拟合平面
		for (int i = 0;i < t3dpt.size();i++)
		{
			if(vecSortBy[i]<avgHeight)
				newt3dpt.push_back(t3dpt[i]);
		}
		if(newt3dpt.size() > 3)
			fitPlane3D(&newt3dpt[0],newt3dpt.size(),&plane3D);
		if(bShowTmp)
		{
			CWTE(TIMTWICE,"平面拟合_二次拟合拟合平面耗时为%.4f\n")
		}

	}

	//计算背景距离矩阵和距离矩阵
	CWTS(TIMDIS)
	for (int i =0 ; i < nMminWidth;i++)
	{
		for (int j = 0; j < nMminHeight;j++)
		{
			cwx = i;
			cwy = j;
			cwz = YX_32F_CVMAT( matHeight, j+m_ssznpic.m_ROIGray.y, i+m_ssznpic.m_ROIGray.x);
			if(cwz == -100)
			{
				//无效数据点,在平面上用-100表示
				YX_32F_MAT(m_ssznpic.m_matDis,j,i)= (float)-100;			
				int valgray = 255;
				if(buseMask == 1)
					valgray = (int)YX_BYTE(pBackMaskImg,j,i);
				if((!buseMask||valgray == 255)&&bSaveBack)
				{
					YX_32F_MAT(m_ssznpic.m_matDisSide,j,i) = (float)-100;
				}
				continue;
			}
			t3dptsingal.xxx = i;t3dptsingal.yyy = j;t3dptsingal.zzz = cwz;
			double sd = pointToPlaneDis3D(t3dptsingal, &plane3D) ;
			YX_32F_MAT(m_ssznpic.m_matDis,j,i) = (float)sd;
			int valgray = 255;
			if(buseMask == 1)
				valgray = (int)YX_BYTE(pBackMaskImg,j,i);
			if((!buseMask||valgray == 255)&&bSaveBack)
			{
				YX_32F_MAT(m_ssznpic.m_matDisSide,j,i) = (float)sd;
			}
		}
	}
	if(bShowTmp)
	{
		CWTE(TIMDIS,"平面拟合_计算距离矩阵耗时为%.4f\n")
	}
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

BOOL CWPowerCV::MatchImage(CString inputPath,CString TemplatePath,CString OutPutPath)
{
	IplImage *src = NULL, *templ = NULL, *ftmp[6] = {NULL};
	int i = 0 ;
	src = cvLoadImage(inputPath,0);
	if(src == NULL)
	{
		return FALSE;
	}
	templ = cvLoadImage(TemplatePath,0);
	if(templ == NULL)
	{
		return FALSE;
	}
	int iwidth = src->width - templ->width + 1;
	int iheight = src->height - templ->height + 1;
	for (i = 0; i < 6 ; i++)
	{
		ftmp[i] = cvCreateImage(cvSize(iwidth,iheight),32,1);
	}
	for (i = 0 ; i < 6 ; i++)
	{
		CWTS(mtime)
		cvMatchTemplate(src,templ,ftmp[i],i);
		cvNormalize(ftmp[i],ftmp[i],1,0,CV_MINMAX);
		mtime.End();
		TRACE("第%d种方法,耗时为%.4f\n",i,mtime.GetUseTime());
		double minValue, maxValue;  
		CvPoint minLoc, maxLoc;  
		cvMinMaxLoc(ftmp[i],&minValue,&maxValue,&minLoc,&maxLoc);
		cvCircle(src,minLoc,3,cvScalarAll(255));
		//cvCircle(src,maxLoc,3,cvScalarAll(255));
		cvRectangle(src,minLoc,cvPoint(minLoc.x+templ->width,minLoc.y+templ->height),cvScalarAll(255),1);
		//cvRectangle(src,maxLoc,cvPoint(maxLoc.x+templ->width,maxLoc.y+templ->height),cvScalarAll(255),1);
		CString saveName; 
		saveName.Format("D:\\test%d.bmp",i);
		cvSaveImage(saveName,src);
		src = cvLoadImage(inputPath,0);
	}
	return TRUE;
}

BOOL CWPowerCV::Normalize(Mat *srcdata, double lownum,double highnum)
{

	return TRUE;
}

BOOL CWPowerCV::test_cwcv()
{
	try
	{
		//assert(test_cwcvFront());
		//assert(test_cwcvEdge());
		//assert(test_cwcvBack());
		//assert(test_match());
		//assert(test_Contour());
		//assert(test_cvline());
		//assert(test_matchContour());
	}
	catch (CMemoryException* e)
	{

	}
	catch (CFileException* e)
	{
	}
	catch (CException* e)
	{
	}
	catch (cv::Exception e)
	{
		AfxMessageBox(e.err.c_str());
	}
	return TRUE;
}

BOOL CWPowerCV::test_cwcvEdge()
{
	return TRUE;
}

BOOL CWPowerCV::test_cwcvFront()
{
	return TRUE;
}

BOOL CWPowerCV::test_cwcvBack()
{
	return TRUE;
}

BOOL CWPowerCV::test_Contour()
{
	BOOL res = TRUE;
	try
	{
		IplImage* psrc = cvLoadImage("D:\\gko1.bmp",0);
		res = GetContour(psrc,2.0);

	}
	catch (cv::Exception e)
	{
		AfxMessageBox(e.err.c_str());
	}
	return res;
}

BOOL CWPowerCV::test_match()
{
	BOOL res = TRUE;
	try
	{
		res = MatchImage("F:\\TMP\\a01.bmp","F:\\TMP\\Height0.bmp","D:\\tmp\\match.bmp");

	}
	catch (cv::Exception e)
	{
		AfxMessageBox(e.err.c_str());
	}
	return res;
}
//缺点:只能处理3个点
BOOL CWPowerCV::CwWarp(vector<CvPoint2D32f> &RealCenterPt,vector<CvPoint2D32f> &StdCenterPt,CvMat* warp_mat)
{
	cvGetAffineTransform(&RealCenterPt[0],&StdCenterPt[0],warp_mat);
	return TRUE;
}
BOOL CWPowerCV::CwWarpMat(vector<CvPoint2D32f> &RealCenterPt,vector<CvPoint2D32f> &StdCenterPt
	,double *m_AffinePrjPara,double *m_AffinePrjParaInv)
{
	int nTotalNum = StdCenterPt.size();
	//通过对应中心计算变换系数
	CvMat PrjPara_mat,PrjParaInv_mat;
	cvInitMatHeader(&PrjPara_mat,3,3,CV_64FC1,m_AffinePrjPara);
	cvInitMatHeader(&PrjParaInv_mat,3,3,CV_64FC1,m_AffinePrjParaInv);
	CvMat *A,*b;
	CvMat X;
	cvInitMatHeader(&X,6,1,CV_64FC1,m_AffinePrjPara);
	if (nTotalNum<4)
	{
		m_AffinePrjPara[0] = 1;
		m_AffinePrjPara[1] = 0;
		m_AffinePrjPara[2] = 0;
		m_AffinePrjPara[3] = 0;
		m_AffinePrjPara[4] = 1;
		m_AffinePrjPara[5] = 0;
		m_AffinePrjPara[6]=m_AffinePrjPara[7]=0;
		m_AffinePrjPara[8]=1;
		cvInvert(&PrjPara_mat,&PrjParaInv_mat);
		// 		//轮廓转换
		// 		AffineTransformRealToStd();
		return false;
	}
	A=cvCreateMat(nTotalNum*2,6,CV_64FC1);
	b=cvCreateMat(nTotalNum*2,1,CV_64FC1);
	cvZero(A);

	for(int i=0;i<nTotalNum;i++){
		A->data.db[6*i+0]=RealCenterPt[i].x;
		A->data.db[6*i+1]=RealCenterPt[i].y;
		A->data.db[6*i+2]=1;
		b->data.db[i]=StdCenterPt[i].x;

		A->data.db[6*(nTotalNum+i)+3]=RealCenterPt[i].x;
		A->data.db[6*(nTotalNum+i)+4]=RealCenterPt[i].y;
		A->data.db[6*(nTotalNum+i)+5]=1;
		b->data.db[nTotalNum+i]=StdCenterPt[i].y;
	}
	cvSolve(A,b,&X,CV_SVD);
	m_AffinePrjPara[6]=m_AffinePrjPara[7]=0;
	m_AffinePrjPara[8]=1;
	cvReleaseMat(&A);
	cvReleaseMat(&b);
	RealCenterPt.swap(vector<CvPoint2D32f>(0));
	StdCenterPt.swap(vector<CvPoint2D32f>(0));
	//RealCenterPt.clear();
	//StdCenterPt.clear();
	cvInvert(&PrjPara_mat,&PrjParaInv_mat);

	return TRUE;
}

BOOL CWPowerCV::test_CwWarp()
{
	vector<CvPoint2D32f> RealCenterPt;
	vector<CvPoint2D32f> StdCenterPt;
	vector<CvPoint2D32f> AfterPt;
	RealCenterPt.push_back(cvPoint2D32f(5.0,15.0));
	RealCenterPt.push_back(cvPoint2D32f(10.0,10.0));
	RealCenterPt.push_back(cvPoint2D32f(19.0,8.0));
	RealCenterPt.push_back(cvPoint2D32f(16.0,15.0));
	StdCenterPt.push_back(cvPoint2D32f(5.0,15.0));
	StdCenterPt.push_back(cvPoint2D32f(10.0,10.0));
	StdCenterPt.push_back(cvPoint2D32f(20.0,10.0));
	StdCenterPt.push_back(cvPoint2D32f(15.0,15.0));
	IplImage *prealImg = cvCreateImage(cvSize(100,100),8,3);
	IplImage *pstdImg = cvCreateImage(cvSize(100,100),8,3);
	for (int i = 0; i < RealCenterPt.size();i++)
	{
		cvLine(prealImg,cvPoint(RealCenterPt[i].x,RealCenterPt[i].y),
			cvPoint(RealCenterPt[(i+1)%RealCenterPt.size()].x,RealCenterPt[(i+1)%RealCenterPt.size()].y),
			cvScalar(255,0,0));
	}
	for (int i = 0; i < StdCenterPt.size();i++)
	{
		cvLine(pstdImg,cvPoint(StdCenterPt[i].x,StdCenterPt[i].y),
			cvPoint(StdCenterPt[(i+1)%StdCenterPt.size()].x,StdCenterPt[(i+1)%StdCenterPt.size()].y),
			cvScalar(255,0,0));
	}
	cvSaveImage("D:\\tmp\\real.bmp",prealImg);
	cvSaveImage("D:\\tmp\\std.bmp",pstdImg);

 	CvMat* warp_mat=cvCreateMat(2,3,CV_32F);
 	CwWarp(RealCenterPt,StdCenterPt,warp_mat);
	cvWarpAffine(prealImg,pstdImg,warp_mat);
	cvSaveImage("D:\\tmp\\stdrot.bmp",pstdImg);
	cvReleaseMat(&warp_mat);

	double m_AffinePrjPara[9] = {0};
	double m_AffinePrjParaInv[9] = {0};
	CwWarpMat(RealCenterPt,StdCenterPt,m_AffinePrjPara,m_AffinePrjParaInv);
	double diffx = 0;
	double diffy = 0;
	
	for (int i = 0; i < RealCenterPt.size();i++)
	{
		diffx = m_AffinePrjPara[0]*RealCenterPt[i].x+m_AffinePrjPara[1]*RealCenterPt[i].y+
			m_AffinePrjPara[2];
		diffy = m_AffinePrjPara[3]*RealCenterPt[i].x+m_AffinePrjPara[4]*RealCenterPt[i].y+
			m_AffinePrjPara[5];
		TRACE("第%d个点为(%.4f,%.4f)\n",i,diffx,diffy);
		AfterPt.push_back(cvPoint2D32f(diffx,diffy));
	}
	cvZero(pstdImg);
	for (int i = 0; i < AfterPt.size();i++)
	{
		cvLine(pstdImg,cvPoint(AfterPt[i].x,AfterPt[i].y),
			cvPoint(AfterPt[(i+1)%AfterPt.size()].x,AfterPt[(i+1)%AfterPt.size()].y),
			cvScalar(255,0,0));
	}
	cvSaveImage("D:\\tmp\\stdrotmat.bmp",pstdImg);
	
	cvReleaseImage(&pstdImg);
	cvReleaseImage(&prealImg);
	pstdImg = NULL;
	prealImg = NULL;
	return TRUE;
}

BOOL CWPowerCV::test_cvline()
{
	IplImage *pimg = cvCreateImage(cvSize(150,150),8,1);
	cvLine(pimg,cvPoint(1969,504),cvPoint(2138,504),cvScalarAll(255),7,4);
	cvSaveImage("D:\\tmp\\testline.bmp",pimg);

	double testwidth[3] = {1,17,18};
	CvPoint testPt[8] = {
		cvPoint(30,0),
		cvPoint(30,30),
		cvPoint(0,30),
		cvPoint(-30,30),
		cvPoint(-30,0),
		cvPoint(-30,-30),
		cvPoint(0,-30),
		cvPoint(30,-30)
	};
	CvPoint StartPt[2] = {
		cvPoint(41,41),
		cvPoint(42,42)
	};

	for (int linewidth = 0; linewidth < 3; linewidth++)
	{
		for (int nstartpt = 0; nstartpt < 2;nstartpt++)
		{
			for (int ntestpt = 0; ntestpt < 8; ntestpt++)
			{
				cvZero(pimg);
				cwline(pimg,StartPt[nstartpt],
					cvPoint(StartPt[nstartpt].x + testPt[ntestpt].x,StartPt[nstartpt].y + testPt[ntestpt].y),
					cvScalarAll(255),testwidth[linewidth],8);
				CString savepath;
				savepath.Format("D:\\tmp\\testline_%d_%d_%d.bmp",linewidth,nstartpt,ntestpt);
				cvSaveImage(savepath,pimg);
			}
		}
	}

	cvReleaseImage(&pimg);
	pimg = NULL;
	
	return TRUE;
}

BOOL CWPowerCV::MatchByContour(IplImage * src,IplImage* templ,IplImage* resImg)
{

	if(src == NULL|| templ == NULL|| resImg == NULL)
		return FALSE;
	if(resImg->depth != 32)
	{
		AfxMessageBox("please input a depth 32 resImg pic ");
		return FALSE;
	}
	BOOL bShowTmp = TRUE;
	BOOL buseRTree = FALSE;
	//提取标准图轮廓矩形
	CWTS(TIMTOTAL)
	CWTS(TIMGETCON)
	vector<CvRect> srcrect;
	GetContour(src,2,srcrect);
	vector<CvRect> templrect;
	GetContour(templ,2,templrect);
	if(bShowTmp)
	{
		CWTE(TIMGETCON,"get contour total use time is %.4f\n")
	}
	//提取中心
	CWTS(TIMADD2TREE)
	vector<CvPoint> center;
	center.reserve(templrect.size());
	for (int i = 0 ; i < templrect.size();i++)
	{
		center.push_back(CwcvRectCenter(templrect[i]));
	}
#if USE_RTREE == 1
	SomeThingTree rtree;
	if (buseRTree)
	{
		AddDataToTREE(rtree,srcrect);
	}
#endif


	if(bShowTmp)
	{
		CWTE(TIMADD2TREE,"插入到RTREE耗时 %.4f\n")
	}
	//移动图形并匹配点
	double totalnum = 1.0/center.size();
	double maxval = 0;
	for (int disj = 0 ; disj < resImg->height;disj++)
	{
		for (int disi = 0; disi < resImg->width;disi++)
		{
			double resscale = 0;
			int ntotal = 0;
			//得到矩形内有孔的比例,从中间向两边
			int i = disi / 2;
			int j = disj / 2;
			if(disi%2 == 1)
			{
				i *= -1;
			}
			if(disj%2 == 1)
			{
				j *= -1;
			}
			i += resImg->width/2;
			j += resImg->height/2;
			if(buseRTree)
			{
#if USE_RTREE == 1
				for (int k = 0; k < center.size();k++)
				{
					BOOL res = GetHitIndex(rtree,cvRect(center[k].x + i,center[k].y + j,1,1));
					if(res)
						ntotal++;
				}
#endif
			}
			else
			{
				for (int k = 0; k < center.size();k++)
				{
					BOOL res = FALSE;
					int xpos = center[k].x + i;
					int ypos = center[k].y + j;
					if(YX_BYTE(src,ypos,xpos) > 0)
					{
						ntotal++;
					}						
				}
			}
			double res = ntotal*totalnum;
			YX_32F_IPL(resImg,j,i) = ntotal*totalnum;
			maxval = max(res,maxval);
			if(res > 0.7)
				return TRUE;
		}
	}

#if USE_RTREE == 1
	if (buseRTree)
	{
		ReleaseTree(rtree);
	}
#endif
	
	if(bShowTmp)
	{
		CWTE(TIMTOTAL,"total use time is %.4f\n")
	}
	return TRUE;
}

BOOL CWPowerCV::test_matchContour()
{
	int widthTmep = 3000;//标准图宽
	int heightTemp = 5000;//标准图高
	int widthFinded = 2100;//模板宽
	int heightFinded = 4500;//模板高
	int testCirNum  = 2100;//测试用的圆的个数
	int testNoise  = 0;//噪声
	int halfNoise  = 0;//测试用的圆的个数
	CvPoint templPos = cvPoint(400,200);//测试用，标准图跟木板的位置关系

	//创建标准图
	IplImage *src = cvCreateImage(cvSize(widthTmep,heightTemp),8,1);
	IplImage *templ = cvCreateImage(cvSize(widthFinded,heightFinded),8,1);
	cvZero(src);
	cvZero(templ);

	//应该匹配出来的位置为
	CvRect bestPos = cvRect(templPos.x,templPos.y,widthFinded,heightFinded);
	CvRect candrawPos = cvRect(0,0,widthFinded,heightFinded);

	//画圆函数
	CWTS(TIMCREATEPIC)
	for (int i = 0;i < testCirNum;i++)
	{
		int drawatx = random(widthTmep);
		int drawaty = random(heightTemp);
		int cird = random(10)+5;
		int noise = 0;//random(testNoise)-halfNoise;
		cvCircle(src,cvPoint(drawatx,drawaty),cird,cvScalarAll(255),-1);
		CvPoint newpos;
		newpos.x = drawatx - templPos.x + noise;
		newpos.y = drawaty - templPos.y + noise;
		if(IsPtInRect(candrawPos,newpos))
			cvCircle(templ,newpos,cird,cvScalarAll(255),-1);

	}
	CWTE(TIMCREATEPIC,"创建两个图像耗时为%.4f\n")
	cvSaveImage("D:\\tmp\\src.bmp",src);
	cvSaveImage("D:\\tmp\\templ.bmp",templ);


	//创建图像保存最终结果
	int iwidth = src->width - templ->width + 1;
	int iheight = src->height - templ->height + 1;
	IplImage* ftmp = cvCreateImage(cvSize(iwidth,iheight),32,1);//浮点型数据32位
	CWTS(mtime)
	MatchByContour(src,templ,ftmp);
	double minval = 0;
	double maxval = 1;
	CvPoint MinLoc,MaxLoc;

	cvMinMaxLoc(ftmp,&minval,&maxval,&MinLoc,&MaxLoc);
	CWTE(mtime,"match Time is %.4f\n");
	TRACE("pic size is(%d*%d)/(%d*%d) bestmatch is (%d,%d),resmatchis(%d,%d)",widthTmep,heightTemp,widthFinded,heightFinded,
		templPos.x,templPos.y,MaxLoc.x,MaxLoc.y);


	cvReleaseImage(&src);
	src = NULL;
	cvReleaseImage(&templ);
	templ = NULL;
	cvReleaseImage(&ftmp);
	ftmp = NULL;
	return TRUE;
}

void CWPowerCV::cwline(CvArr* m_pImg, CvPoint SPoint, CvPoint EPoint, CvScalar color, int thickness , int line_type , int shift )
{
	if(thickness == 1)
	{
		cvLine(m_pImg,SPoint,EPoint,color,thickness,line_type,shift);
		return;
	}
	BOOL odd = thickness&1;
	double sx = SPoint.x, sy = SPoint.y;
	double ex = EPoint.x, ey = EPoint.y;
	double dx = ex - sx, dy = ey - sy;
	double rsina,rcosa;
	double rsinas,rcosas;
	double a = atan2(dy, dx);
	if(odd)
	{
		thickness -= 1;
		rsina = thickness/2.0 * sin(a), rcosa = thickness/2.0 * cos(a);
		rsinas = thickness/2.0 * sin(a), rcosas = thickness/2.0 * cos(a);
	}
	else
	{
		rsina = thickness/2.0 * sin(a), rcosa = thickness/2.0 * cos(a);
		rsinas = (thickness/2.0 - 1) * sin(a), rcosas = (thickness/2.0 - 1) * cos(a);
	}
	//发现有的时候,竖直会算成一个很小很小的数导致计算错误
	if(rsina>-0.000001&&rsina<0.000001)
		rsina = 0;
	if(rcosa>-0.000001&&rcosa<0.000001)
		rcosa = 0;
	if(rsinas>-0.000001&&rsinas<0.000001)
		rsinas = 0;
	if(rcosas>-0.000001&&rcosas<0.000001)
		rcosas = 0;
	CvPoint pt[4];
	pt[0].x = SPoint.x - rsina ;
	pt[0].y = SPoint.y + rcosa ;
	pt[1].x = SPoint.x + rsinas ;
	pt[1].y = SPoint.y - rcosas ;
	pt[2].x = EPoint.x + rsinas ;
	pt[2].y = EPoint.y - rcosas ;
	pt[3].x = EPoint.x - rsina ;
	pt[3].y = EPoint.y + rcosa ;
// 	//如果是偶数的线宽对多出来的一个像素进行修正
// 	if(!(odd) && rsina >= 1.0)
// 	{
// 		pt[1].x += 1;
// 		pt[2].x += 1;
// 	}
// 	if(!(odd) && rcosa >= 1.0)
// 	{
// 		pt[1].y += 1;
// 		pt[2].y += 1;
// 	}
// 	if(!(odd) && rsina <= -1.0)
// 	{
// 		pt[1].x -= 1;
// 		pt[2].x -= 1;
// 	}
// 	if(!(odd) && rcosa <= -1.0)
// 	{
// 		pt[1].y -= 1;
// 		pt[2].y -= 1;
// 	}
	if(pt[0].x >=0 && pt[0].y >= 0 &&
		pt[1].x >=0 && pt[1].y >= 0 &&
		pt[2].x >=0 && pt[2].y >= 0 &&
		pt[3].x >=0 && pt[3].y >= 0 )
		TRACE("绘制方形直线,传入cvline的参数是pt(%.4f,%.4f,%.4f,%.4f)\n",pt[0],pt[1],pt[2],pt[3]);
	// 	int pts[1] = {1};
	// 	vector<vector<CvPoint>> vecpts;
	// 	vector<CvPoint> sinpts;
	// 	sinpts.push_back(pt[0]);
	// 	sinpts.push_back(pt[1]);
	// 	sinpts.push_back(pt[2]);
	// 	sinpts.push_back(pt[3]);
	// 	vecpts.push_back(sinpts);
	cvFillConvexPoly (m_pImg,pt,4,color,line_type,shift);
	int rad = 0;
	if(odd)
	{
		rad = thickness/2;
	}
	else
	{
		rad = thickness/2 - 1;
	}
	cvCircle(m_pImg,SPoint,rad,color,-1);
	cvCircle(m_pImg,EPoint,rad,color,-1);

}

void CWPowerCV::opencwcam()
{
	if(capture  != NULL)
	{
		cvReleaseCapture(&capture); 
		capture = NULL;
	}
	capture=cvCreateCameraCapture(-1);  
}

void CWPowerCV::releasecwcam()
{
	if(capture  != NULL)
	{
		cvReleaseCapture(&capture); 
		capture = NULL;
	}
		 
}

void CWPowerCV::GetOneFrame(IplImage *&frame)
{
	frame=cvQueryFrame(capture);  

}



void CWPowerCV::GrayEqualization(IplImage *pSrc,IplImage *pDst)
{
	if(1)
	{
		ASSERT(pSrc != NULL);
		ASSERT(pSrc->nChannels == 1);
		ASSERT(pDst != NULL);
		ASSERT(pDst->nChannels == 1);
		BYTE* pixel = (BYTE*)pSrc->imageData;
		BYTE* tempPixel = (BYTE*)pDst->imageData;
		UINT width = pSrc->widthStep;
		UINT height = pSrc->height;
		int channel = pSrc->nChannels;
		// 灰度映射表
		BYTE map[256];
		long lCounts[256];
		double p[256] = {0.0};//概率
		long valcount = 0;	//有效数据总数
		memset(lCounts, 0, sizeof(long) * 256);

		// 计算各灰度值个数
		for (UINT i = 0; i < width * height; i++)
		{
			int x = pixel[i];
			if(x == 0){
				continue;
			}
			lCounts[x]++;
			valcount++;
		}


		// 计算概率
		for (int i = 0; i < 256; i++)
		{
			p[i] = (lCounts[i] / (double)valcount);
		}

		//计算分布
		double lTemp[256];

		for (int i = 0; i < 256; i++)
		{
			if(i == 0)
				lTemp[i] = p[i];
			else
				lTemp[i]  = lTemp[i - 1] + p[i];

			map[i] = lTemp[i] *255.f + 0.5;
		}

		// 变换后的值直接在映射表中查找
		for (int j=0;j<pSrc->height;j++)
		{
			for (int i=0;i<pSrc->width;i++)
			{
				int nSrcValue = YX_BYTE(pSrc,j,i);
				int nValue = map[nSrcValue];
				YX_BYTE(pDst,j,i) = nValue;
			}
		}
	}
	else
	{
		ASSERT(pSrc != NULL);
		ASSERT(pSrc->nChannels == 1);
		ASSERT(pDst != NULL);
		ASSERT(pDst->nChannels == 1);
		int nHist[256] = {0};
		double p[256] = {0.0};
		double c[256] = {0.0};
		double nTotalNum = pSrc->width*pSrc->height;
		int nMaxGrey = 0;
		int nMinGrey = INT_MAX;
		int nCount = 0;
		//统计灰度直方图
		for (int j=0;j<pSrc->height;j++)
		{
			for (int i=0;i<pSrc->width;i++)
			{
				int nValue = YX_BYTE(pSrc,j,i);
				if (nValue > 0)
				{
					nHist[nValue] ++;
					nMaxGrey = nMaxGrey>nValue?nMaxGrey:nValue;
					nMinGrey = nMinGrey<nValue?nMinGrey:nValue;
					nCount ++;
				}
			}
		}
		//直方图归一化
		for (int i=0;i<256;i++)
		{
			p[i] = (double)nHist[i]/(double)nCount;
		}
		//计算累计直方图
		for (int i=0;i<256;i++)
		{
			if (0 == i)
			{
				c[i] = p[i];
			}
			else
			{
				c[i] = c[i-1]+p[i];
			}
		}
		//均衡化
		int nGrey = nMaxGrey-nMinGrey;
		cvZero(pDst);
		for (int j=0;j<pSrc->height;j++)
		{
			for (int i=0;i<pSrc->width;i++)
			{
				int nSrcValue = YX_BYTE(pSrc,j,i);
				int nValue = c[nSrcValue]*nGrey;
				YX_BYTE(pDst,j,i) = nValue;
			}
		}
	}
}

void CWPowerCV::GrayEqualization(CString path,CString savepath)
{
	IplImage *srcimg = cvLoadImage(path,0);
	IplImage *dstimg = cvCreateImage(cvGetSize(srcimg),srcimg->depth,srcimg->nChannels);
	cvSaveImage(savepath,srcimg);
	cvZero(dstimg);
	GrayEqualization(srcimg,dstimg);
	IplImage *hist = Gethist(dstimg);
	cvSaveImage("D:\\afhistsrcsmall1.bmp",hist);
	cvSaveImage(savepath,dstimg);
	cvReleaseImage(&srcimg);
	cvReleaseImage(&dstimg);
	cvReleaseImage(&hist);
	srcimg = NULL;
	hist = NULL;
	dstimg = NULL;
}

IplImage* CWPowerCV::Gethist(IplImage* src)
{
	int hist_size = 256;    //直方图尺寸  
	int hist_height = 256;  
	float range[] = {0,255};  //灰度级的范围  
	float* ranges[]={range};  
	//创建一维直方图，统计图像在[0 255]像素的均匀分布  
	CvHistogram* gray_hist = cvCreateHist(1,&hist_size,CV_HIST_ARRAY,ranges,1);  

	//计算灰度图像的一维直方图  
	IplImage* tmpbin = cvCloneImage(src);
	cvThreshold(src,tmpbin,1,255,CV_THRESH_BINARY);
	cvCalcHist(&src,gray_hist,0,tmpbin); 
	cvReleaseImage(&tmpbin);
	tmpbin = NULL;
	//归一化直方图  
	//cvNormalizeHist(gray_hist,1.0);  
	int scale = 2;  
	//创建一张一维直方图的“图”，横坐标为灰度级，纵坐标为像素个数（*scale）  
	IplImage* hist_image = cvCreateImage(cvSize(hist_size*scale,hist_height),8,3);  
	cvZero(hist_image);  
	//统计直方图中的最大直方块  
	float max_value = 0;  
	cvGetMinMaxHistValue(gray_hist, 0,&max_value,0,0);
	//分别将每个直方块的值绘制到图中  
	for(int i=0;i<hist_size;i++)  
	{  
		float bin_val = cvQueryHistValue_1D(gray_hist,i); //像素i的概率  
		int intensity = cvRound(bin_val*hist_height/max_value);  //要绘制的高度  
		cvRectangle(hist_image,  
			cvPoint(i*scale,hist_height-1),  
			cvPoint((i+1)*scale - 1, hist_height - intensity),  
			CV_RGB(255,255,255));    
	}  
	return hist_image;
}

void CWPowerCV::Gethist(CString path,CString savepath)
{
	IplImage *srcimg = cvLoadImage(path,0);
	IplImage *hist = Gethist(srcimg);
	cvSaveImage(savepath,hist);
	cvReleaseImage(&srcimg);
	cvReleaseImage(&hist);
	srcimg = NULL;
	hist = NULL;
}

