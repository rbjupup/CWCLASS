#if !defined(AFX_XFUNCOM_H_INCLUDED_)
#define AFX_XFUNCOM_H_INCLUDED_



#define USE_OPENCV 1
#define HAD_XFUN 1
//#define USE_EMAIL
#define USE_RTREE 0
//使用正则之前要先将extern的正则文件覆盖到mfc目录下
#define USE_STL
#include <vector>
#include <algorithm>
#include <afxtempl.h>
#include <afxcmn.h>
#include <stdlib.h>
#include <afxmt.h>
#include "Def.h"
#ifdef USE_OPENCV
#include "cv.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "minmax.h"
#endif
#ifdef USE_EMAIL
#include "Smtp.h"
#endif
#ifdef USE_STL
#include <atlrx.h>
#endif
#if USE_RTREE == 1
#include "RTree.h"
#endif
using namespace std;



/************************************************************************/
/*                          目录开始                                    */
/************************************************************************/
/*

文件和文件夹
	判断文件夹是否存在
	判断文件是否存在
	获取文件夹中的全部文件夹
	获取文件夹中全部文件
	获取文件中全部数据
	将全部数据写入文件中
	多选文件框
	创建文件夹
	创建完整路径
	获取文件夹路径0表示文件夹，1表示文件
	查找文件夹中所有的文件夹
	查找文件夹中相同后缀的文件
	删除路径和路径中的文件
	拷贝路径
	获取程序运行当前路径
	删除非空文件夹
	获取exe文件夹路径
	文件解压缩
	分割路径

字符串

图像
	填充位图头文件
	在DC上绘制图像
	在图像上绘制十字线

计时

其它函数
	使用CString的格式来写日志
	写日志到文件中
	设置日志保存的地址
	获取一个在最大值到最小值之间的随机数
	调用cmd.exe实现对bat文件的调用,或者直接对其它软件进行调用
	使用正则表达式进行匹配
	内存整理
	等待某个变量为真或者为否再继续执行接下去的代码
	坐标转换,转换成控件坐标0表示传入的点以界面左上角为单位,1表示以屏幕为单位


网络
	判断网络是否连通且输出联通类型
	判断一个IP是否联通
	发送一封Email

数据处理
	求容器均值
	求容器最小值
	拟合平面

高端操作
	仿射变换

低端操作
	获取cvrect的中心
	获取cvrect的左上角
	获取cvrect的右下角

	通信
	与其它软件进行一次通信
	进程之间进行通信

*/

/************************************************************************/
/*                          目录结束                                    */
/************************************************************************/

#ifndef YX_BYTE//8 - bit 1 - channel 
#define YX_BYTE(img,y,x) ((BYTE*)(img->imageData + (y)*img->widthStep))[x]
#endif

#ifndef YX_32F_IPL//8 - bit 1 - channel 
#define YX_32F_IPL(img,y,x) ((float*)(img->imageData + (y)*img->widthStep))[x]
#endif

#ifndef YX_32F_CVMAT//8 - bit 1 - channel 
#define YX_32F_CVMAT(img,y,x) ((float*)(img->data.fl +(y)*(img->step/sizeof(float))))[x]
#endif

#ifndef YX_32F_MAT//8 - bit 1 - channel 
#define YX_32F_MAT(img,y,x) ((float*)((float*)img.data +(y)*(img.cols)))[x]
#endif

#ifndef YX_BYTE_CVMAT//8 - bit 1 - channel 
#define YX_BYTE_CVMAT(img,y,x) ((BYTE*)(img->data.fl +(y)*(img->step/sizeof(BYTE))))[x]
#endif

#ifndef YX_BYTE_MAT//8 - bit 1 - channel 
#define YX_BYTE_MAT(img,y,x) ((BYTE*)((BYTE*)img.data +(y)*(img.cols)))[x]
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

#ifndef YX_BYTE_1ARRAY//8 - bit 1 - channel 
#define YX_BYTE_1ARRAY(img, uNo) ((BYTE*)(img->imageData))[uNo]
#endif

#ifndef COUNT_DIS
#define COUNT_DIS(x1,y1,x2,y2) sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2))
#endif

#define Mil2Pix(fx,fDPI)				float(fx*((float)25.4/fDPI))												//mil转像素
#define Pix2Mil(nx,fDPI)				float(nx*(fDPI/(float)25.4))												//像素转mil
#define PixArea2Dia(fArea,fDPI)			Pix2Mil(2*sqrt(fArea/PI),fDPI)												//像素面积转mil直径
#define Dia2PixArea(fDia,fDPI)			PI*(Mil2Pix(fDia/2,fDPI))*(Mil2Pix(fDia/2,fDPI))							//mil直径转像素面积
#define random(x) (rand()%x)								//随机数生成器
		
#ifndef PI
#define					PI								3.141592653			
#endif
#define					ANG2RAD(x)						float((x)*PI/180.0)				//角度转弧度
#define					RAD2ANG(x)						float((x)*180/PI)				//弧度转角度
#define					TRANS2COX(X,Y,fangle)				float((X)*cos(ANG2RAD(fangle))-(Y)*sin(ANG2RAD(fangle)))//旋转坐标轴
#define					TRANS2COY(X,Y,fangle)				float((Y)*cos(ANG2RAD(fangle))+(X)*sin(ANG2RAD(fangle)))

#define					Rad2Ang(x)							float(x*180.0/PI)
#define					Ang2Rad(x)							float(x*PI/180.0)
// #define EUCLIDEAN	-1
// #define SIMILARITY	0
// #define AFFINE		1
// #define PERSPECTIVE 2
// #define PROJECTIVE	3
#define EUCLIDEAN	-1
#define SIMILARITY	0
#define AFFINE		1
#define PERSPECTIVE 2
#define PROJECTIVE	3

#define CWTS(TIMECOUNTNAME) CWTimeCount TIMECOUNTNAME;TIMECOUNTNAME.Start();
#define CWTE(TIMECOUNTNAME,CWSTRDATA) TIMECOUNTNAME.End();TRACE(CWSTRDATA,TIMECOUNTNAME.GetUseTime());

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

typedef struct
{
	double xxx;
	double yyy;
	double zzz;
}roiPointDecimal3D;

/*平面方程式：z=r0*x+r1*y+r2*/
typedef struct
{
	double r0;
	double r1;
	double r2;
	double distB;    //used in distance caculating
}RATIO_Plane;


/************************************************************************/
/*                      文件/文件夹操作                                 */
/************************************************************************/
//判断文件夹是否存在
BOOL FolderExist(CString strPath);
//判断文件是否存在
BOOL FileExist(CString strFileName);
//获取文件夹中的全部文件夹
void GetDirNameFromDir(CString csDirPath,std::vector<CString> &m_FileList )  ;
//获取文件夹中全部文件
void GetFileNameFromDir(CString strDir, std::vector<CString>& vecFiles,int type = 0);
//获取文件中全部数据
void GetDataFromFile(CString filePath,std::vector<CString>& vecData);
//将全部数据写入文件中
void WriteDataToFile(CString strFilePath,std::vector<CString>& vecData);
//多选文件框
void OpenFilePath(vector<CString> &FilePathVector,CString strInitPath = _T(""));
//创建文件夹
BOOL CreateFolder(CString strPath);
//创建完整路径
BOOL CreateAllDirectories(CString strDir);
//复制文件夹
void CopyDirectory(CString Src,CString Dst);
//获取文件夹路径0表示文件夹，1表示文件
CString GetDirPathByDialog(CString rootpath,CWnd* papa,int type = 0);
//查找文件夹中所有的文件夹
void AddDirDir(CString strText,vector<CString> &vecDir,vector<CTime> &vecDirTime) ;
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
//文件解压缩
void Decompresss(CString Input,CString OutPut,int useTool);
//分割路径
void SplitFilePath(vector<CString> &res);
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
int String2Hex(CString str, char *SendOut);
char ConvertHexData(char ch);
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
/************************************************************************
函数名称: GetCStringBetweenAB
函数功能: 获取字符串中A和B之间的字符 
输入参数: strSrc 原始字符串 cA cB为差分字符 
返回值: 结果 字符串为空则标示失败 
************************************************************************/
CString GetCStringBetweenAB(CString strSrc,char cA,char cB);
CString GetCStringBetweenAB(CString strSrc,CString strA,CString strB);
/************************************************************************
函数名称: GetNumberAfterA
函数功能: 获取字符串中A字符以后的数字，遇到非数字则停止 
输入参数: strSrc 原始字符串 cA字符 
返回值: 结果 字符串为空则标示失败 
************************************************************************/
CString GetNumberAfterA(CString &strSrc,char cA);
//从nPOs位置开始获取一串数字直至碰上非数字字符，并将原始字符串去掉以取出的部分
CString GetNumberStartN(CString &strSrc,int nPos);
//去掉前面的空格,将字符串按照分割符分开
void splitStr(TCHAR* srcStr,TCHAR* findedStr,std::vector<CString> &param);
//检测字符串是否从某个子串开始
bool IsStartWith(TCHAR *srcStr ,TCHAR *FindedSrt);
//替换部分字符
void ReplaceStr(CString &dynText, CString strFinded, CString strMid);
//CString转char*
#ifndef _CString2Char
#define _CString2Char(str)			(LPSTR)(LPCSTR)str
#endif
/**************************************************************************** 
函数名称: CString_to_char 
函数功能: CString转char
输入参数: str 输入字符串 ptr输出字符串。 
输出参数: 无 
*****************************************************************************/   
BOOL CString_to_char(CString str,char *ptr,int charlength);
/************************************************************************/
/*						  字符串常用操作结束						    */
/************************************************************************/

/************************************************************************/
/*								 图像操作开始                           */
/************************************************************************/
//填充位图头文件
void FillBitmapInfo1(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
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
//将圆弧转换成圆上面的点集
static void Curve2Points(double Rad, double centerx, double centery, bool ClockWise, float sa, float ea, vector<CFloatPt> &m_contourPt,double enlargeFactor);
//opencv图像格式转换
enum {IPL_MAT,MAT_IPL,CVMAT_IPL,IPL_CVMAT,CVMAT_MAT,MAT_CVMAT};
BOOL  CvTranPointerType(IplImage &src_ipl , cv::Mat &src_mat , CvMat &src_cvmat,BOOL bcopy,int type);

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
class CWTimeCount
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
void XWaitTime(float fTime);
//判断时间是否在某个时间段内--精确到小时
BOOL JudgeInTime(SYSTEMTIME CurrentTime,SYSTEMTIME STime,SYSTEMTIME ETime);
/************************************************************************/
/*								计时操作结束                             */
/************************************************************************/

/************************************************************************/
/*								其它操作开始                             */
/************************************************************************/
//使用CString的格式来写日志
//写日志到文件中
void CwlogFormat(LPCTSTR log , ... );
void CWlog(CString mlog);
//设置日志保存的地址
void SetLogPath(CString savePath);
//获取一个在最大值到最小值之间的随机数
int GetRand(int min,int max);
//调用cmd.exe实现对bat文件的调用,或者直接对其它软件进行调用
void WaitProcess(LPTSTR FileName,LPTSTR Param,BOOL waitExit = TRUE);
//使用正则表达式进行匹配
vector<CString> GetresByStlRx(CString word,CString rule);
//内存整理
BOOL memsort();
//检查是否在矩形中
bool CheckInRect(CPoint point,CRect rect);
//截屏
void CutScreen(CDC *pDstDc);
//透明位图显示
BOOL DrawTransparentBitmap(HDC hdcDest,int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent);
//获取外包框
CRect GetRelRoundRect(CPoint *pt,int nPointNum);
CRect GetRoundRect(CPoint *pt,int nPointNum);
//点在多边形内
BOOL PtInPolygon(CPoint p, CPoint pt[], int nCount);
//坐标转换,转换成控件坐标0表示传入的点以界面左上角为单位,1表示以屏幕为单位
BOOL TranParentToChild(CWnd &cwwnd, CWnd* papa,CPoint &point,int ptType = 0);
//获取控件的位置
CRect CWGetWndRect(CWnd *cwwnd,int ptType = 0);
//CRect转CvRect
CvRect ChangeRect(CRect rect);
/************************************************************************/
/*								其它操作结束                             */
/************************************************************************/
/************************************************************************/
/*							网络相关操作						        */
/************************************************************************/
//判断网络是否连通且输出联通类型
int GetInternetConnectState();
//判断一个IP是否联通
BOOL IfIPConnect(const char *strIPAddr);
//发送一封Email
#ifdef USE_EMAIL
BOOL SendAEmail(CString sendcount, CString sendPwd, CString receiver, CString senddata,CString topic,CString filePath = "");
#endif
/************************************************************************/
/*						  网络相关操作结束						        */
/************************************************************************/

/************************************************************************/
/*                        数据处理                                      */
/************************************************************************/

#ifdef USE_OPENCV
// Ax + by + cz = D   拟合一个平面
void cvFitPlane(const CvMat* points, float* plane);
#endif
//求最小值
double GetMin(vector<double> &srcdata);
//求平均值
double GetAvg(vector<double> &srcdata);

//冒泡排序
template<typename T>
void cwsort(vector<double> &sortBy,vector<T> &data,int startindex,int endindex)
{
	for (int i = startindex; i < endindex; i++) {
		for (int j = startindex; j < endindex - i - 1+startindex; j++) {
			if (sortBy[j] > sortBy[j + 1]) {
				T temp = data[j];
				double tmp = sortBy[j];
				data[j] = data[j + 1];
				sortBy[j] = sortBy[j + 1];
				data[j + 1] = temp;
				sortBy[j + 1] = tmp;
			}
		}
	}
}
//合并排序
struct  mergedata
{
	double m_data;
	int m_index;
	mergedata()
	{

	}
	mergedata(double data,int index)
	{
		m_data = data;
		m_index  = index;
	}
};
    //合并两个数组
vector<mergedata> mergetwo(vector<mergedata> &L1,vector<mergedata> &L2);
	//合并排序
template<typename T>
vector<T> cwsortMerge(vector<double> &sortBy,const vector<T> &data)
{
	vector<vector<mergedata>> vecResult;
	vecResult.reserve(data.size());
	vector<vector<mergedata>> vecTmp;
	vecTmp.reserve(data.size());
	vector<T> vecRes;
	vecRes.reserve(data.size());
	for (int i = 0;i < sortBy.size();i++)
	{
		mergedata tmpmergeData = mergedata(sortBy[i],i);
		vector<mergedata> vectmpmergeData;
		vectmpmergeData.push_back(tmpmergeData);
		vecResult.push_back(vectmpmergeData);
	}
	while(vecResult.size() != 1)
	{
		for (int i = 0; i < (int)(vecResult.size()/2);i++)
		{
			vecTmp.push_back(mergetwo(vecResult[i],vecResult[vecResult.size() - 1 - i]));
		}
		if(vecResult.size() % 2 != 0)
		{
			vecTmp.push_back(vecResult[int(vecResult.size()/2)]);
		}
		vecResult.swap(vecTmp);
		vecTmp.clear();
	}
	for (int i = 0; i <vecResult[0].size();i++)
	{
		vecRes.push_back(data[vecResult[0][i].m_index]);
	}
	return vecRes;
}


//深视智能的拟合平面
int fitPlane3D(const roiPointDecimal3D *point, int pNum, RATIO_Plane *plane3D);
double pointToPlaneDis3D(roiPointDecimal3D calPt, const RATIO_Plane *plane3D);
/************************************************************************/
/*                        数据处理结束                                  */
/************************************************************************/
/************************************************************************/
/*                        高端操作开始                                  */
/************************************************************************/
//文件映射
BOOL fileMappingW(CString mapname,IplImage *pimg);
BOOL fileMappingR(CString mapname,IplImage *pimg);
//仿射变换
void CwTransform(double x0,double y0,double &x1,double &y1,double parms[9],int method);
//RTREE
#if USE_RTREE == 1
typedef RTree<int, int, 2> SomeThingTree;
void AddDataToTREE(SomeThingTree &rtree,vector<CvRect> &rect);
BOOL GetHitIndex(SomeThingTree &rtree,CvRect &rect);
void ReleaseTree(SomeThingTree &rtree);
#endif


/************************************************************************/
/*                        高端操作结束                                  */
/************************************************************************/
/************************************************************************/
/*                        低端操作开始                                  */
/************************************************************************/
//获取cvrect的中心
CvPoint CwcvRectCenter(CvRect &rect);
//获取cvrect的左上角
CvPoint CwcvRectLP(CvRect &rect);
//获取cvrect的右下角
CvPoint CwcvRectRB(CvRect &rect);
//判断cvpoint有没有在矩形中
BOOL IsPtInRect(CvRect &rect , CvPoint &pt);
//排序矩阵的距离
vector<CPoint> SortMatrixDis(int rows,int cols,int findrow,int findcol);
/************************************************************************/
/*                        低端操作结束                                  */
/************************************************************************/
/************************************************************************/
/*                        通信操作开始                                  */
/************************************************************************/
//与其它软件进行一次通信
	//进程间的通信
void SendAMessage2Soft(CString windowName,int nmessageType,WPARAM wParam, LPARAM lParam);

//进程之间进行通信
	//线程间的通信之一,事件通信
	//创建事件CreateEvent(NULL, TRUE, FALSE, NULL);
struct cwcommunity{
	vector<int> vecLine;				//在等待的位置
	vector<int> vecTimes;				//进该等待位置的次数
	void waitAddOne(int nLine);		//等待中的事件加一//这两个函数主要是防止程序卡死的时候难以找到卡在哪里
	void waitdiffOne(int nLine);		//等待中的事件减一//通过
	void waitForSelf(HANDLE hwaitevent,int waittime,int npos);							//等待本软件的事件让它走
	void LetEventGo(HANDLE hwaitevent);											//让本软件的继续向下走
	BOOL WaitForIT(volatile BOOL &waitObject , BOOL isWaitForTrue,int npos,double waitTimes = 100000);
};
void WaitForBOOL(volatile BOOL &waitObject , BOOL isWaitForTrue);			//等待某个变量为真或者为否再继续执行接下去的代码
BOOL WaitForBOOL(volatile BOOL &waitObject , BOOL isWaitForTrue,double waitTimes);

/************************************************************************/
/*                        通信操作结束                                  */
/************************************************************************/


/************************************************************************/
/*                        函数测试开始                                  */
/************************************************************************/
//测试函数,在运行软件前加上assert(test_XFunCom());即可
BOOL test_XFunCom();
BOOL test_fileMapping();
BOOL test_mergeSort();
BOOL test_Curve2Points();
BOOL test_LoadFile();
BOOL test_Transform();
BOOL test_SortMatrixDis();
BOOL test_TimOfPICREAD();
BOOL test_CvTranPointerType();
BOOL test_rtree();
/************************************************************************/
/*                        函数测试结束                                  */
/************************************************************************/
#endif

