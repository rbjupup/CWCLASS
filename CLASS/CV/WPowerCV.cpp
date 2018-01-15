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

BOOL CWPowerCV::ParsePtSet(CString inputPath,CString OutPutPath,int type)
{
	//读取到数组
	vector<CString> alinepts;
	vector<float>	falinepts;
	vector<CString> ptsdata;
	GetDataFromFile(inputPath,ptsdata);
	if(ptsdata.size() < 1)
		return FALSE;
	CVTRYSTART
	splitStr(ptsdata[0].GetBuffer(0),",",alinepts);
	vector<vector<float>> pts;
	CvMat *mat;
	mat = cvCreateMat( alinepts.size(), ptsdata.size(), CV_32FC1);
	int row = alinepts.size();
	int col = ptsdata.size();
	int maxvalue = 0;
	cvZero(mat);
	for (int i = 0; i < ptsdata.size();i++)
	{
		alinepts.clear();
		splitStr(ptsdata[i].GetBuffer(0),",",alinepts);
		for (int j =0; j < alinepts.size();j++){
			cvmSet( mat, j, i, atof(alinepts[j]) );
		}
	}
	double MinValue;
	double MaxValue;

	CvPoint MinLocation;
	CvPoint MaxLocation;
	cvMinMaxLoc(mat,& MinValue,& MaxValue,& MinLocation,& MaxLocation);
	maxvalue = MaxValue +1;
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col; j++)
		{
			float val = cvmGet(mat,i,j);
			if(val == MinValue)
				cvmSet( mat,i , j,maxvalue);
			else
				cvmSet( mat,i , j,val);
		}
	}
	cvMinMaxLoc(mat,& MinValue,& MaxValue,& MinLocation,& MaxLocation);
	for (int i = 0; i < row;i++)
	{
		for (int j = 0; j < col; j++)
		{
			float val = cvmGet(mat,i,j);
			if(val == maxvalue)
				cvmSet( mat,i , j,MinValue);
		}
	}
	cvMinMaxLoc(mat,& MinValue,& MaxValue,& MinLocation,& MaxLocation);
	double co0 = - MinValue/(MaxValue - MinValue)*255.0;
	double co1 = 255.0/(MaxValue - MinValue);
	//将所有数据归一化到0-255
	IplImage* pImg = cvCreateImage(cvSize(row,col),8,1);
	vector<CString> out;
	CString tmp;
	for (int i = 0; i < row;i++)
	{
		CString alinestr;
		for (int j = 0; j < col; j++)
		{
			float val = cvmGet(mat,i,j);
			tmp.Format("%.3f",val);
			val = val*co1 + co0;
			//cvmSet( mat,i , j,val);
			if(j == 0)
				alinestr += tmp;
			else 
				alinestr = alinestr + ","+ tmp;
			cvSet2D(pImg,j,i,cvScalarAll(val));
		}
		if(i>=1000&&i<2000)
			out.push_back(alinestr);
	}
	//WriteDataToFile("D:\\2.csv",out);
	cvSaveImage(OutPutPath,pImg);
	cvReleaseImage(&pImg);
	cvReleaseMat(&mat);
	CVTRYEND
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
	return TRUE;
	CVTRYEND
}
