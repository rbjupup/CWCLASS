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
