#include "StdAfx.h"
#include "WPowerCV.h"
#include "XFunCom.h"
CWPowerCV::CWPowerCV(void)
{
}
struct SSZNPRPJECT{
	double co0,co1;
	vector<CvRect> hole;
	vector<double> block;
	CString graypath;
	CString holepath;
} ssznpic;

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
	ssznpic.co0 = co0;
	ssznpic.co1 = co1;
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
	ssznpic.graypath = OutPutPath;
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
	ssznpic.block.clear();
	ssznpic.block.push_back(seedthreshold);
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
			ssznpic.block.push_back(seedthreshold);
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

void CWPowerCV::testsszn()
{
	//对数据进行处理
	ssznpic.graypath.Replace("\\","/");
	IplImage* pImg = cvLoadImage("D:/Dis.bmp",0);
	IplImage* pImghole = cvLoadImage(ssznpic.holepath,0);
	vector<vector<float>> data;
	for (int i = 0; i < ssznpic.hole.size(); i++) {
		for (int j = 0; j < ssznpic.hole.size() - i - 1; j++) {
			if (ssznpic.hole[j].x > ssznpic.hole[j + 1].x) {
				CvRect temp = ssznpic.hole[j];
				ssznpic.hole[j] = ssznpic.hole[j + 1];
				ssznpic.hole[j + 1] = temp;
			}
		}
	}
	for (int i = 0; i < ssznpic.hole.size();i++)
	{
		vector<float> aholedata;
		for (int cwx = ssznpic.hole[i].x;cwx< ssznpic.hole[i].x + ssznpic.hole[i].width;cwx++)
		{
			for (int cwy = ssznpic.hole[i].y;cwy< ssznpic.hole[i].y + ssznpic.hole[i].height;cwy++)
			{
				double val = cvGet2D(pImg,cwy,cwx).val[0];
				int valhole = cvGet2D(pImghole,cwy,cwx).val[0];
				if(valhole == 0)
					continue;
				val = (val - ssznpic.co0)/ssznpic.co1;
				aholedata.push_back(val);
			}
		}
		data.push_back(aholedata);
	}
	for (int i = 0; i < data.size();i++)
	{
		double total = 0; 
		double minhole = 100;
		for(int j = 0; j < data[i].size();j++){
			total += data[i][j];
			minhole = minhole>data[i][j]?data[i][j]:minhole;
		}
		//计算平均值
		double average = total/data[i].size();
		//计算均方差
		double s=0;
		for(int j=0; j<data[i].size(); j++)
			s +=(data[i][j] - average)*(data[i][j]-average);
		double ss = sqrt(s/(data[i].size())); //求均方差
		TRACE("第%d个孔的平均深度为:%.4f\n",i,average);
		TRACE("第%d个孔的最大深度为:%.4f\n",i,minhole);
		TRACE("第%d个孔的均方差为:%.4f\n",i,ss);
		TRACE("第%d个孔的中心坐标为:(%d,%d)\n",i,ssznpic.hole[i].x + ssznpic.hole[i].width/2,ssznpic.hole[i].y + ssznpic.hole[i].height/2);
	}
	for (int i = 0; i < ssznpic.block.size();i++)
	{
		double val = (ssznpic.block[i] - ssznpic.co0)/ssznpic.co1;
		TRACE("从左往右第%d块的基准面深度为:%.4f\n",i,val);
	}
}

void CWPowerCV::GetContour(CString InputImgPath,CString OutPutPath,int minval)
{
	InputImgPath.Replace("\\","/");
	//载入原始图，且必须以二值图模式载入
	ssznpic.holepath = InputImgPath;
	Mat srcImage = imread(InputImgPath.GetBuffer(), 0);
	//初始化结果图
	Mat dstImage = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC3);
	//定义轮廓和层次结构
	vector<vector<cv::Point> > contours;
	vector<Vec4i> hierarchy;
	//查找轮廓
	findContours(srcImage, contours, hierarchy,
		CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//遍历所有顶层的轮廓， 以随机颜色绘制出每个连接组件颜色
	int index = 0;
	ssznpic.hole.clear();
	for (; index >= 0; index = hierarchy[index][0])
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(dstImage, contours, index, color, CV_FILLED, 8, hierarchy);
		CvRect rect = boundingRect(contours[index]);
		if(contourArea(contours[index]) < minval)
			continue;
		ssznpic.hole.push_back(rect);
		rectangle(dstImage,cv::Point(rect.x,rect.y),cv::Point(rect.x + rect.width,rect.y + rect.height),CV_RGB(255,255, 255));
		//cvRectangle(dstImage, cvPoint(rect.x, rect.y), cvPoint(rect.x + rect.width, rect.y + rect.height),CV_RGB(255,255, 255), 1, 8, 0);
	}
	imwrite(OutPutPath.GetBuffer(),dstImage);
}

void CWPowerCV::FitPlane(CString InputImgPath,CString OutPutPath,int x,int y, int width,int height,int boarddis)
{
	InputImgPath.Replace("\\","/");
	IplImage* pImg = cvLoadImage(InputImgPath,0);
	IplImage* pImgNew = cvCloneImage(pImg);
	IplImage* pImgDis = cvCloneImage(pImg);
	CvRect rect = cvRect(x,y,width,height);
	vector<float> X_vector;
	vector<float> Y_vector;
	vector<float> Z_vector;
	for(int i = rect.x; i < rect.x + rect.width;i++)
	{
		for (int j = rect.y; j < rect.y +rect.height;j++)
		{
			X_vector.push_back(i);
			Y_vector.push_back(j);
			Z_vector.push_back(cvGet2D(pImg,j,i).val[0]);
		}
	}
	for(int diedaitimes = 0;diedaitimes < 3;diedaitimes++){
		CvMat*points_mat = cvCreateMat(X_vector.size(), 3, CV_32FC1);//定义用来存储需要拟合点的矩阵   
		for (int i=0;i < X_vector.size(); ++i)  
		{  
			points_mat->data.fl[i*3+0] = X_vector[i];//矩阵的值进行初始化   X的坐标值  
			points_mat->data.fl[i * 3 + 1] = Y_vector[i];//  Y的坐标值  
			points_mat->data.fl[i * 3 + 2] = Z_vector[i];//  Z的坐标值</span>  

		}  
		float plane12[4] = { 0 };//定义用来储存平面参数的数组   
		cvFitPlane(points_mat, plane12);//调用方程
		int cwx = 0;
		int cwy = 0;
		int cwz = 0;
		double co0 = sqrt(plane12[0]*plane12[0]+plane12[1]*plane12[1]+plane12[2]*plane12[2]);
		if(diedaitimes == 0){
			X_vector.clear();Y_vector.clear();Z_vector.clear();
		}

			for (int i =0 ; i < pImg->width;i++)
			{
				for (int j = 0; j < pImg->height;j++)
				{
					cwx = i;cwy = j;cwz = cvGet2D(pImg,j,i).val[0];
					double d = abs(plane12[0]*(float)cwx + plane12[1]*(float)cwy  + plane12[2]*(float)cwz - plane12[3] )/co0;
					if (d < boarddis)
					{
						X_vector.push_back(i);Y_vector.push_back(j);Z_vector.push_back(cwz);
						if(diedaitimes >1){
							cvSet2D(pImgNew,j,i,cvScalarAll(0));
							
						}
					}
					else{
						if(diedaitimes >1)
							cvSet2D(pImgDis,j,i,cvGet2D(pImg,j,i));
					}
				}
			}
	}
	cvThreshold(pImgNew,pImgNew,1,255,CV_THRESH_BINARY);
	cvSaveImage(OutPutPath,pImgNew);
	cvSaveImage("D:\\Dis.bmp",pImgDis);
	cvReleaseImage(&pImgNew);
	cvReleaseImage(&pImg);
	cvReleaseImage(&pImgDis);

}
