#if !defined(AFX_XFUNCOM_H_INCLUDED_)
#define AFX_XFUNCOM_H_INCLUDED_
//#define USE_OPENCV 1
#include "resource.h"
#include <vector>
#include <algorithm>
#include <afxtempl.h>
#include <afxcmn.h>
#include <stdlib.h>
#include <afxmt.h>
#ifdef USE_OPENCV
#include "cv.h"
#include "minmax.h"
#endif
#ifndef YX_BYTE//8 - bit 1 - channel 
#define YX_BYTE(img,y,x) ((BYTE*)(img->imageData + (y)*img->widthStep))[x]
#endif

#ifndef YX_BYTE_B//8 - bit 3 - channel
#define YX_BYTE_B(img,y,x) ((BYTE*)(img->imageData + (y)*img->widthStep))[x*3]
#endif

#ifndef YX_BYTE_G//8 - bit 3 - channel
#define YX_BYTE_G(img,y,x) ((BYTE*)(img->imageData + (y)*img->widthStep))[x*3+1]
#endif

#ifndef YX_BYTE_R//8 - bit 3 - channel
#define YX_BYTE_R(img,y,x) ((BYTE*)(img->imageData + (y)*img->widthStep))[x*3+2]
#endif

#define random(x) (rand()%x)								//随机数生成器

// #define EUCLIDEAN	-1
// #define SIMILARITY	0
// #define AFFINE		1
// #define PERSPECTIVE 2
// #define PROJECTIVE	3
enum  SHAPE
{
	SHAPE_RECTANGLE_R,				//给定一个RECT的方式绘制矩形
	SHAPE_RECTANGLE_P,				//给定一个Point的方式绘制矩形
	SHAPE_CIRCLE,
	SHAPE_CROSS,
	SHAPE_RULER
};
typedef struct DRAW_GRAPH
{
	SHAPE			Shape;
	CRect			RectRegion;
	CPoint			CterPoint;
	int				nLength;
	int				nPenThick;
	int				nPenStyle;
	COLORREF		rgb;
	DRAW_GRAPH()
	{
		Shape				= SHAPE_RECTANGLE_P;
		RectRegion			= CRect(0,0,0,0);
		CterPoint			= CPoint(0,0);
		nLength				= 2;
		nPenThick			= 1;
		nPenStyle			= PS_DOT;
		rgb					= RGB(255,0,0);
	}
}DrawShape;
//浮点型点坐标
typedef struct FloatPoint2D 
{
	float x;
	float y;
	FloatPoint2D()
	{
		x = 0.0;
		y = 0.0;
	}
	FloatPoint2D(float a,float b)
	{
		x = a;
		y = b;
	}
};
//浮点型点坐标
typedef struct FloatPoint3D 
{
	float x;
	float y;
	float z;
	FloatPoint3D()
	{
		x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	FloatPoint3D(float a,float b,float c)
	{
		x = a;
		y = b;
		z = c;
	}
};




/************************************************************************/
/*                      文件/文件夹操作                                 */
/************************************************************************/
//判断文件夹是否存在
BOOL FolderExist(CString strPath);
//判断文件是否存在
BOOL FileExist(CString strFileName);
//获取对话框中的全部文件
void GetFileFromDir(CString csDirPath,std::vector<CString> &m_FileList )  ;
//创建文件夹
BOOL CreateFolder(CString strPath);
//创建完整路径
BOOL CreateAllDirectories(CString strDir);
//获取文件夹路径
CString GetDirPathByDialog();
//查找文件夹中相同后缀的文件
CString FindFileSameExtension(CString strPath,CString strExtension);
//删除路径和路径中的文件
BOOL DelFilesOfDir(CString strDir);
//拷贝路径
BOOL CopyFolder(CString strFromDir,CString strToDir);
//获取程序运行当前路径
CString GetCurRunDir();
//删除非空文件夹
bool DeleteDirectory(CString strDir);
//获取exe文件夹路径
CString GetAppPath();
/************************************************************************/
/*                    文件/文件夹操作结束                               */
/************************************************************************/

//判断点是否在Rect中
bool JudgePointInRect(CPoint pt,CRect JRect);
/************************************************************************/
/*							字符串常用操作						        */
/************************************************************************/
/************************************************************************
函数名称: str_to_hex 
函数功能: 字符串转换为十六进制 
输入参数: string 字符串 cbuf 十六进制 len 字符串的长度。 
输出参数: 无 
************************************************************************/   
int str_to_hex(char *string, unsigned char *cbuf, int len);
/**************************************************************************** 
函数名称: hex_to_str 
函数功能: 十六进制转字符串 
输入参数: ptr 字符串 buf 十六进制 len 十六进制字符串的长度。 
输出参数: 无 
*****************************************************************************/   
void hex_to_str(char *ptr,unsigned char *buf,int len);
/**************************************************************************** 
函数名称: char_to_wchar 
函数功能: char转wchar
输入参数: ptr 输入字符串 buf输出字符串。 
输出参数: 无 
*****************************************************************************/   
void char_to_wchar(const char *ptr, wchar_t *buf);
/************************************************************************
函数名称: CStringExistSymble 
函数功能: 判断字符串中是否含有除数字、字母、汉字以外的非法字符 
输入参数: str 字符串 
输出参数: 无 
返回值	:TRUE标示存在 FALSE标示不存在
************************************************************************/
int IsCStringExistSymble(CString str);
/************************************************************************/
/*						  字符串常用操作结束						    */
/************************************************************************/

/************************************************************************/
/*								 图像操作开始                           */
/************************************************************************/
//填充位图头文件
void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
/************************************************************************/
/* 函数功能：在DC上绘制			图形                                    */
/************************************************************************/
void DrawGraphOnDC(CDC *pDC,DrawShape DShape);
#ifdef USE_OPENCV
/************************************************************************/
/* 函数功能：在DC上绘制图像		                                        */
/*	   参数：Img 图像 MemDc  目标DC								        */
/************************************************************************/
void DrawImageOnMemDc(IplImage *Img,CDC *pMemDC,CBitmap *bmp,float fImageScale);
/************************************************************************/
/* 函数功能：在图像上绘制		十字线                                  */
/*	   参数：Img 目标图像 pt目标位置 nLength 十字线线长 rgb颜色         */
/************************************************************************/
void DrawCrossOnImage(IplImage *Img,CvPoint pt,int nLength,CvScalar rgb);
#endif
/************************************************************************/
/*								图像操作结束                             */
/************************************************************************/

/************************************************************************/
/*								计时操作开始                             */
/************************************************************************/
// 精确获得算法处理时间的类(毫秒量级)

class CTimeCount
{
private:	
	double			UseTime;				// 算法处理时间(单位:秒)
	LARGE_INTEGER	Time, Frequency, old;	// 计数值

public:	
	void Start() // 计时开始
	{
		QueryPerformanceFrequency( &Frequency );
		QueryPerformanceCounter  ( &old );
		UseTime = 0.0;
	}
	void End() // 计时结束
	{
		QueryPerformanceCounter( &Time );
		UseTime = (double) ( Time.QuadPart - old.QuadPart) / (double)Frequency.QuadPart;
		//		TRACE("Use time = %20.10f(ms)\n", UseTime );
	}
	double GetUseTime() // 获得算法处理时间(单位:秒)
	{		
		return UseTime;
	}
};
class LTimeCount
{
private:	
	double			UseTime;				// 算法处理时间(单位:秒)
	LARGE_INTEGER	Time, Frequency, old;	// 计数值

public:	
	void Start(); // 计时开始
	void End(); // 计时结束
	double GetUseTime(); // 获得算法处理时间(单位:秒)
	void WaitTime(double waitTime);
};
/************************************************************************/
/*								计时操作结束                             */
/************************************************************************/

/************************************************************************/
/*								其它操作开始                             */
/************************************************************************/
void CWlog(CString mlog);
int GetRand(int min,int max);
/************************************************************************/
/*								其它操作结束                             */
/************************************************************************/

#endif
