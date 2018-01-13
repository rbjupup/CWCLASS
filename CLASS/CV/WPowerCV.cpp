#include "StdAfx.h"
#include "WPowerCV.h"
#include "XFunCom.h"
CWPowerCV::CWPowerCV(void)
{
}


CWPowerCV::~CWPowerCV(void)
{
}

BOOL CWPowerCV::SplitIMG(CString InputImgPath,CString OutPutDir,int XNum,int YNum)
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
