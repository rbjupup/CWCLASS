#if !defined(AFX_XFUNCOM_H_INCLUDED_)
#define AFX_XFUNCOM_H_INCLUDED_



#define USE_OPENCV 1
#define HAD_XFUN 1
//#define USE_EMAIL
#define USE_RTREE 0
//ʹ������֮ǰҪ�Ƚ�extern�������ļ����ǵ�mfcĿ¼��
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
/*                          Ŀ¼��ʼ                                    */
/************************************************************************/
/*

�ļ����ļ���
	�ж��ļ����Ƿ����
	�ж��ļ��Ƿ����
	��ȡ�ļ����е�ȫ���ļ���
	��ȡ�ļ�����ȫ���ļ�
	��ȡ�ļ���ȫ������
	��ȫ������д���ļ���
	��ѡ�ļ���
	�����ļ���
	��������·��
	��ȡ�ļ���·��0��ʾ�ļ��У�1��ʾ�ļ�
	�����ļ��������е��ļ���
	�����ļ�������ͬ��׺���ļ�
	ɾ��·����·���е��ļ�
	����·��
	��ȡ�������е�ǰ·��
	ɾ���ǿ��ļ���
	��ȡexe�ļ���·��
	�ļ���ѹ��
	�ָ�·��

�ַ���

ͼ��
	���λͼͷ�ļ�
	��DC�ϻ���ͼ��
	��ͼ���ϻ���ʮ����

��ʱ

��������
	ʹ��CString�ĸ�ʽ��д��־
	д��־���ļ���
	������־����ĵ�ַ
	��ȡһ�������ֵ����Сֵ֮��������
	����cmd.exeʵ�ֶ�bat�ļ��ĵ���,����ֱ�Ӷ�����������е���
	ʹ��������ʽ����ƥ��
	�ڴ�����
	�ȴ�ĳ������Ϊ�����Ϊ���ټ���ִ�н���ȥ�Ĵ���
	����ת��,ת���ɿؼ�����0��ʾ����ĵ��Խ������Ͻ�Ϊ��λ,1��ʾ����ĻΪ��λ


����
	�ж������Ƿ���ͨ�������ͨ����
	�ж�һ��IP�Ƿ���ͨ
	����һ��Email

���ݴ���
	��������ֵ
	��������Сֵ
	���ƽ��

�߶˲���
	����任

�Ͷ˲���
	��ȡcvrect������
	��ȡcvrect�����Ͻ�
	��ȡcvrect�����½�

	ͨ��
	�������������һ��ͨ��
	����֮�����ͨ��

*/

/************************************************************************/
/*                          Ŀ¼����                                    */
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

#define Mil2Pix(fx,fDPI)				float(fx*((float)25.4/fDPI))												//milת����
#define Pix2Mil(nx,fDPI)				float(nx*(fDPI/(float)25.4))												//����תmil
#define PixArea2Dia(fArea,fDPI)			Pix2Mil(2*sqrt(fArea/PI),fDPI)												//�������תmilֱ��
#define Dia2PixArea(fDia,fDPI)			PI*(Mil2Pix(fDia/2,fDPI))*(Mil2Pix(fDia/2,fDPI))							//milֱ��ת�������
#define random(x) (rand()%x)								//�����������
		
#ifndef PI
#define					PI								3.141592653			
#endif
#define					ANG2RAD(x)						float((x)*PI/180.0)				//�Ƕ�ת����
#define					RAD2ANG(x)						float((x)*180/PI)				//����ת�Ƕ�
#define					TRANS2COX(X,Y,fangle)				float((X)*cos(ANG2RAD(fangle))-(Y)*sin(ANG2RAD(fangle)))//��ת������
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
	SHAPE_RECTANGLE_R,				//����һ��RECT�ķ�ʽ���ƾ���
	SHAPE_RECTANGLE_P,				//����һ��Point�ķ�ʽ���ƾ���
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
//�����͵�����
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
//�����͵�����
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

/*ƽ�淽��ʽ��z=r0*x+r1*y+r2*/
typedef struct
{
	double r0;
	double r1;
	double r2;
	double distB;    //used in distance caculating
}RATIO_Plane;


/************************************************************************/
/*                      �ļ�/�ļ��в���                                 */
/************************************************************************/
//�ж��ļ����Ƿ����
BOOL FolderExist(CString strPath);
//�ж��ļ��Ƿ����
BOOL FileExist(CString strFileName);
//��ȡ�ļ����е�ȫ���ļ���
void GetDirNameFromDir(CString csDirPath,std::vector<CString> &m_FileList )  ;
//��ȡ�ļ�����ȫ���ļ�
void GetFileNameFromDir(CString strDir, std::vector<CString>& vecFiles,int type = 0);
//��ȡ�ļ���ȫ������
void GetDataFromFile(CString filePath,std::vector<CString>& vecData);
//��ȫ������д���ļ���
void WriteDataToFile(CString strFilePath,std::vector<CString>& vecData);
//��ѡ�ļ���
void OpenFilePath(vector<CString> &FilePathVector,CString strInitPath = _T(""));
//�����ļ���
BOOL CreateFolder(CString strPath);
//��������·��
BOOL CreateAllDirectories(CString strDir);
//�����ļ���
void CopyDirectory(CString Src,CString Dst);
//��ȡ�ļ���·��0��ʾ�ļ��У�1��ʾ�ļ�
CString GetDirPathByDialog(CString rootpath,CWnd* papa,int type = 0);
//�����ļ��������е��ļ���
void AddDirDir(CString strText,vector<CString> &vecDir,vector<CTime> &vecDirTime) ;
//�����ļ�������ͬ��׺���ļ�
CString FindFileSameExtension(CString strPath,CString strExtension);
//ɾ��·����·���е��ļ�
BOOL DelFilesOfDir(CString strDir);
//����·��
BOOL CopyFolder(CString strFromDir,CString strToDir);
//��ȡ�������е�ǰ·��
CString GetCurRunDir();
//ɾ���ǿ��ļ���
bool DeleteDirectory(CString strDir);
//��ȡexe�ļ���·��
CString GetAppPath();
//�ļ���ѹ��
void Decompresss(CString Input,CString OutPut,int useTool);
//�ָ�·��
void SplitFilePath(vector<CString> &res);
/************************************************************************/
/*                    �ļ�/�ļ��в�������                               */
/************************************************************************/

//�жϵ��Ƿ���Rect��
bool JudgePointInRect(CPoint pt,CRect JRect);
/************************************************************************/
/*							�ַ������ò���						        */
/************************************************************************/
/************************************************************************
��������: str_to_hex 
��������: �ַ���ת��Ϊʮ������ 
�������: string �ַ��� cbuf ʮ������ len �ַ����ĳ��ȡ� 
�������: �� 
************************************************************************/   
int str_to_hex(char *string, unsigned char *cbuf, int len);
int String2Hex(CString str, char *SendOut);
char ConvertHexData(char ch);
/**************************************************************************** 
��������: hex_to_str 
��������: ʮ������ת�ַ��� 
�������: ptr �ַ��� buf ʮ������ len ʮ�������ַ����ĳ��ȡ� 
�������: �� 
*****************************************************************************/   
void hex_to_str(char *ptr,unsigned char *buf,int len);
/**************************************************************************** 
��������: char_to_wchar 
��������: charתwchar
�������: ptr �����ַ��� buf����ַ����� 
�������: �� 
*****************************************************************************/   
void char_to_wchar(const char *ptr, wchar_t *buf);
/************************************************************************
��������: CStringExistSymble 
��������: �ж��ַ������Ƿ��г����֡���ĸ����������ķǷ��ַ� 
�������: str �ַ��� 
�������: �� 
����ֵ	:TRUE��ʾ���� FALSE��ʾ������
************************************************************************/
int IsCStringExistSymble(CString str);
/************************************************************************
��������: GetCStringBetweenAB
��������: ��ȡ�ַ�����A��B֮����ַ� 
�������: strSrc ԭʼ�ַ��� cA cBΪ����ַ� 
����ֵ: ��� �ַ���Ϊ�����ʾʧ�� 
************************************************************************/
CString GetCStringBetweenAB(CString strSrc,char cA,char cB);
CString GetCStringBetweenAB(CString strSrc,CString strA,CString strB);
/************************************************************************
��������: GetNumberAfterA
��������: ��ȡ�ַ�����A�ַ��Ժ�����֣�������������ֹͣ 
�������: strSrc ԭʼ�ַ��� cA�ַ� 
����ֵ: ��� �ַ���Ϊ�����ʾʧ�� 
************************************************************************/
CString GetNumberAfterA(CString &strSrc,char cA);
//��nPOsλ�ÿ�ʼ��ȡһ������ֱ�����Ϸ������ַ�������ԭʼ�ַ���ȥ����ȡ���Ĳ���
CString GetNumberStartN(CString &strSrc,int nPos);
//ȥ��ǰ��Ŀո�,���ַ������շָ���ֿ�
void splitStr(TCHAR* srcStr,TCHAR* findedStr,std::vector<CString> &param);
//����ַ����Ƿ��ĳ���Ӵ���ʼ
bool IsStartWith(TCHAR *srcStr ,TCHAR *FindedSrt);
//�滻�����ַ�
void ReplaceStr(CString &dynText, CString strFinded, CString strMid);
//CStringתchar*
#ifndef _CString2Char
#define _CString2Char(str)			(LPSTR)(LPCSTR)str
#endif
/**************************************************************************** 
��������: CString_to_char 
��������: CStringתchar
�������: str �����ַ��� ptr����ַ����� 
�������: �� 
*****************************************************************************/   
BOOL CString_to_char(CString str,char *ptr,int charlength);
/************************************************************************/
/*						  �ַ������ò�������						    */
/************************************************************************/

/************************************************************************/
/*								 ͼ�������ʼ                           */
/************************************************************************/
//���λͼͷ�ļ�
void FillBitmapInfo1(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
/************************************************************************/
/* �������ܣ���DC�ϻ���			ͼ��                                    */
/************************************************************************/
void DrawGraphOnDC(CDC *pDC,DrawShape DShape);
#ifdef USE_OPENCV
/************************************************************************/
/* �������ܣ���DC�ϻ���ͼ��		                                        */
/*	   ������Img ͼ�� MemDc  Ŀ��DC								        */
/************************************************************************/
void DrawImageOnMemDc(IplImage *Img,CDC *pMemDC,CBitmap *bmp,float fImageScale);
/************************************************************************/
/* �������ܣ���ͼ���ϻ���		ʮ����                                  */
/*	   ������Img Ŀ��ͼ�� ptĿ��λ�� nLength ʮ�����߳� rgb��ɫ         */
/************************************************************************/
void DrawCrossOnImage(IplImage *Img,CvPoint pt,int nLength,CvScalar rgb);
//��Բ��ת����Բ����ĵ㼯
static void Curve2Points(double Rad, double centerx, double centery, bool ClockWise, float sa, float ea, vector<CFloatPt> &m_contourPt,double enlargeFactor);
//opencvͼ���ʽת��
enum {IPL_MAT,MAT_IPL,CVMAT_IPL,IPL_CVMAT,CVMAT_MAT,MAT_CVMAT};
BOOL  CvTranPointerType(IplImage &src_ipl , cv::Mat &src_mat , CvMat &src_cvmat,BOOL bcopy,int type);

#endif
/************************************************************************/
/*								ͼ���������                             */
/************************************************************************/

/************************************************************************/
/*								��ʱ������ʼ                             */
/************************************************************************/
// ��ȷ����㷨����ʱ�����(��������)

class CTimeCount
{
private:	
	double			UseTime;				// �㷨����ʱ��(��λ:��)
	LARGE_INTEGER	Time, Frequency, old;	// ����ֵ

public:	
	void Start() // ��ʱ��ʼ
	{
		QueryPerformanceFrequency( &Frequency );
		QueryPerformanceCounter  ( &old );
		UseTime = 0.0;
	}
	void End() // ��ʱ����
	{
		QueryPerformanceCounter( &Time );
		UseTime = (double) ( Time.QuadPart - old.QuadPart) / (double)Frequency.QuadPart;
		//		TRACE("Use time = %20.10f(ms)\n", UseTime );
	}
	double GetUseTime() // ����㷨����ʱ��(��λ:��)
	{		
		return UseTime;
	}
};
class CWTimeCount
{
private:	
	double			UseTime;				// �㷨����ʱ��(��λ:��)
	LARGE_INTEGER	Time, Frequency, old;	// ����ֵ

public:	
	void Start(); // ��ʱ��ʼ
	void End(); // ��ʱ����
	double GetUseTime(); // ����㷨����ʱ��(��λ:��)
	void WaitTime(double waitTime);
};
void XWaitTime(float fTime);
//�ж�ʱ���Ƿ���ĳ��ʱ�����--��ȷ��Сʱ
BOOL JudgeInTime(SYSTEMTIME CurrentTime,SYSTEMTIME STime,SYSTEMTIME ETime);
/************************************************************************/
/*								��ʱ��������                             */
/************************************************************************/

/************************************************************************/
/*								����������ʼ                             */
/************************************************************************/
//ʹ��CString�ĸ�ʽ��д��־
//д��־���ļ���
void CwlogFormat(LPCTSTR log , ... );
void CWlog(CString mlog);
//������־����ĵ�ַ
void SetLogPath(CString savePath);
//��ȡһ�������ֵ����Сֵ֮��������
int GetRand(int min,int max);
//����cmd.exeʵ�ֶ�bat�ļ��ĵ���,����ֱ�Ӷ�����������е���
void WaitProcess(LPTSTR FileName,LPTSTR Param,BOOL waitExit = TRUE);
//ʹ��������ʽ����ƥ��
vector<CString> GetresByStlRx(CString word,CString rule);
//�ڴ�����
BOOL memsort();
//����Ƿ��ھ�����
bool CheckInRect(CPoint point,CRect rect);
//����
void CutScreen(CDC *pDstDc);
//͸��λͼ��ʾ
BOOL DrawTransparentBitmap(HDC hdcDest,int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent);
//��ȡ�����
CRect GetRelRoundRect(CPoint *pt,int nPointNum);
CRect GetRoundRect(CPoint *pt,int nPointNum);
//���ڶ������
BOOL PtInPolygon(CPoint p, CPoint pt[], int nCount);
//����ת��,ת���ɿؼ�����0��ʾ����ĵ��Խ������Ͻ�Ϊ��λ,1��ʾ����ĻΪ��λ
BOOL TranParentToChild(CWnd &cwwnd, CWnd* papa,CPoint &point,int ptType = 0);
//��ȡ�ؼ���λ��
CRect CWGetWndRect(CWnd *cwwnd,int ptType = 0);
//CRectתCvRect
CvRect ChangeRect(CRect rect);
/************************************************************************/
/*								������������                             */
/************************************************************************/
/************************************************************************/
/*							������ز���						        */
/************************************************************************/
//�ж������Ƿ���ͨ�������ͨ����
int GetInternetConnectState();
//�ж�һ��IP�Ƿ���ͨ
BOOL IfIPConnect(const char *strIPAddr);
//����һ��Email
#ifdef USE_EMAIL
BOOL SendAEmail(CString sendcount, CString sendPwd, CString receiver, CString senddata,CString topic,CString filePath = "");
#endif
/************************************************************************/
/*						  ������ز�������						        */
/************************************************************************/

/************************************************************************/
/*                        ���ݴ���                                      */
/************************************************************************/

#ifdef USE_OPENCV
// Ax + by + cz = D   ���һ��ƽ��
void cvFitPlane(const CvMat* points, float* plane);
#endif
//����Сֵ
double GetMin(vector<double> &srcdata);
//��ƽ��ֵ
double GetAvg(vector<double> &srcdata);

//ð������
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
//�ϲ�����
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
    //�ϲ���������
vector<mergedata> mergetwo(vector<mergedata> &L1,vector<mergedata> &L2);
	//�ϲ�����
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


//�������ܵ����ƽ��
int fitPlane3D(const roiPointDecimal3D *point, int pNum, RATIO_Plane *plane3D);
double pointToPlaneDis3D(roiPointDecimal3D calPt, const RATIO_Plane *plane3D);
/************************************************************************/
/*                        ���ݴ������                                  */
/************************************************************************/
/************************************************************************/
/*                        �߶˲�����ʼ                                  */
/************************************************************************/
//�ļ�ӳ��
BOOL fileMappingW(CString mapname,IplImage *pimg);
BOOL fileMappingR(CString mapname,IplImage *pimg);
//����任
void CwTransform(double x0,double y0,double &x1,double &y1,double parms[9],int method);
//RTREE
#if USE_RTREE == 1
typedef RTree<int, int, 2> SomeThingTree;
void AddDataToTREE(SomeThingTree &rtree,vector<CvRect> &rect);
BOOL GetHitIndex(SomeThingTree &rtree,CvRect &rect);
void ReleaseTree(SomeThingTree &rtree);
#endif


/************************************************************************/
/*                        �߶˲�������                                  */
/************************************************************************/
/************************************************************************/
/*                        �Ͷ˲�����ʼ                                  */
/************************************************************************/
//��ȡcvrect������
CvPoint CwcvRectCenter(CvRect &rect);
//��ȡcvrect�����Ͻ�
CvPoint CwcvRectLP(CvRect &rect);
//��ȡcvrect�����½�
CvPoint CwcvRectRB(CvRect &rect);
//�ж�cvpoint��û���ھ�����
BOOL IsPtInRect(CvRect &rect , CvPoint &pt);
//�������ľ���
vector<CPoint> SortMatrixDis(int rows,int cols,int findrow,int findcol);
/************************************************************************/
/*                        �Ͷ˲�������                                  */
/************************************************************************/
/************************************************************************/
/*                        ͨ�Ų�����ʼ                                  */
/************************************************************************/
//�������������һ��ͨ��
	//���̼��ͨ��
void SendAMessage2Soft(CString windowName,int nmessageType,WPARAM wParam, LPARAM lParam);

//����֮�����ͨ��
	//�̼߳��ͨ��֮һ,�¼�ͨ��
	//�����¼�CreateEvent(NULL, TRUE, FALSE, NULL);
struct cwcommunity{
	vector<int> vecLine;				//�ڵȴ���λ��
	vector<int> vecTimes;				//���õȴ�λ�õĴ���
	void waitAddOne(int nLine);		//�ȴ��е��¼���һ//������������Ҫ�Ƿ�ֹ��������ʱ�������ҵ���������
	void waitdiffOne(int nLine);		//�ȴ��е��¼���һ//ͨ��
	void waitForSelf(HANDLE hwaitevent,int waittime,int npos);							//�ȴ���������¼�������
	void LetEventGo(HANDLE hwaitevent);											//�ñ�����ļ���������
	BOOL WaitForIT(volatile BOOL &waitObject , BOOL isWaitForTrue,int npos,double waitTimes = 100000);
};
void WaitForBOOL(volatile BOOL &waitObject , BOOL isWaitForTrue);			//�ȴ�ĳ������Ϊ�����Ϊ���ټ���ִ�н���ȥ�Ĵ���
BOOL WaitForBOOL(volatile BOOL &waitObject , BOOL isWaitForTrue,double waitTimes);

/************************************************************************/
/*                        ͨ�Ų�������                                  */
/************************************************************************/


/************************************************************************/
/*                        �������Կ�ʼ                                  */
/************************************************************************/
//���Ժ���,���������ǰ����assert(test_XFunCom());����
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
/*                        �������Խ���                                  */
/************************************************************************/
#endif

