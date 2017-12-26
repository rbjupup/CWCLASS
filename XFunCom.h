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
using namespace std;
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

#define random(x) (rand()%x)								//�����������

// #define EUCLIDEAN	-1
// #define SIMILARITY	0
// #define AFFINE		1
// #define PERSPECTIVE 2
// #define PROJECTIVE	3
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




/************************************************************************/
/*                      �ļ�/�ļ��в���                                 */
/************************************************************************/
//�ж��ļ����Ƿ����
BOOL FolderExist(CString strPath);
//�ж��ļ��Ƿ����
BOOL FileExist(CString strFileName);
//��ȡ�Ի����е�ȫ���ļ�
void GetFileFromDir(CString csDirPath,std::vector<CString> &m_FileList )  ;
void OpenFilePath(vector<CString> &FilePathVector,CString strInitPath = _T(""));
//�����ļ���
BOOL CreateFolder(CString strPath);
//��������·��
BOOL CreateAllDirectories(CString strDir);
//��ȡ�ļ���·��
CString GetDirPathByDialog();
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
/************************************************************************/
/*						  �ַ������ò�������						    */
/************************************************************************/

/************************************************************************/
/*								 ͼ�������ʼ                           */
/************************************************************************/
//���λͼͷ�ļ�
void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
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
class LTimeCount
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
/************************************************************************/
/*								��ʱ��������                             */
/************************************************************************/

/************************************************************************/
/*								����������ʼ                             */
/************************************************************************/
void CWlog(CString mlog);
int GetRand(int min,int max);
//����cmd.exeʵ�ֶ�bat�ļ��ĵ���,����ֱ�Ӷ�����������е���
void WaitProcess(LPTSTR FileName,LPTSTR Param);

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

/************************************************************************/
/*						  ������ز�������						        */
/************************************************************************/

#endif
