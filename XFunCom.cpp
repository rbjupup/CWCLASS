
#include "stdafx.h"
#include "XFunCom.h"
#include <WinInet.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib,"Iphlpapi.lib")
#include <ctime>


//////////////////////////////////////////////////////////////////////////
//							时间操作开始
//////////////////////////////////////////////////////////////////////////
void LTimeCount::Start() // 计时开始
{
	QueryPerformanceFrequency( &Frequency );
	QueryPerformanceCounter  ( &old );
	UseTime = 0.0;
}
void LTimeCount::End() // 计时结束
{
	QueryPerformanceCounter( &Time );
	UseTime = (double) ( Time.QuadPart - old.QuadPart) / (double)Frequency.QuadPart;
// 	TRACE("Use time = %20.10f(s)\n", UseTime );
}

double LTimeCount::GetUseTime() // 获得算法处理时间(单位:秒)
{		
	return UseTime;
}
void LTimeCount::WaitTime(double waitTime)
{
	LTimeCount tt;
	tt.Start();
	tt.End();
	while(tt.GetUseTime()*1000 < waitTime)
	{
		tt.End();
	}
}
//可释放CPU,消息循环队列的等待
void XWaitTime(float fTime)
{
	LTimeCount tc;
	tc.Start();	
	MSG msg;
	bool bTimeOut = false;
	HANDLE eWaitTimeEvent = NULL;
	eWaitTimeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while(!bTimeOut)
	{
		if (PeekMessage(&msg, NULL, 0, 0,0))
		{
			GetMessage(&msg, NULL, 0, 0);
			DispatchMessage(&msg);
		}
		tc.End();
		if (tc.GetUseTime()>fTime)
		{
			bTimeOut =  true;
		}
		WaitForSingleObject(eWaitTimeEvent,1);
	}

}
//////////////////////////////////////////////////////////////////////////
//							时间操作结束
//////////////////////////////////////////////////////////////////////////



/////////////////目录检查、文件检查、目录创建
BOOL FolderExist(CString strPath)
{
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;   
	}
	FindClose(hFind);
	return rValue;
}

BOOL FileExist(CString strFileName)
{
	CFileFind fFind;
	return fFind.FindFile(strFileName); 
}
void GetFileFromDir(CString csDirPath,std::vector<CString> &m_FileList )  
{  
	csDirPath+="\\*";  
	HANDLE file;  
	WIN32_FIND_DATA fileData;  
	char line[1024];  
	char fn[1000];  
	//mbstowcs(fn,csDirPath.GetBuffer(),999);  
	file = FindFirstFile(csDirPath.GetBuffer(), &fileData);  
	if (strstr(fileData.cFileName,".") == NULL)
	{
		m_FileList.push_back(fileData.cFileName);  
	}
	bool bState = false;  
	bState = FindNextFile(file, &fileData);  
	while(bState){  
		//wcstombs(line,(const char*)fileData.cFileName,259);  
		if (strstr(fileData.cFileName,".") == NULL)
		{
			m_FileList.push_back(fileData.cFileName);  
		} 
		bState = FindNextFile(file, &fileData);  
	}  
}  

void OpenFilePath(vector<CString> &FilePathVector,CString strInitPath)
{
	CString t_odbFilePath;
	CFileDialog OpenDialog(TRUE,NULL,strInitPath,OFN_ALLOWMULTISELECT|OFN_ENABLESIZING|OFN_HIDEREADONLY);
	if (OpenDialog.DoModal() == IDOK)
	{
		CString path;
		IShellItemArray *pResult=OpenDialog.GetResults();
		OpenDialog.m_ofn.nMaxFile = 500 * MAX_PATH;
		DWORD dwCount=0;
		IShellItem *pItem;
		WCHAR *pFilePath;
		pResult->GetCount(&dwCount);
		for (DWORD i=0;i<dwCount;i++)
		{
			pResult->GetItemAt(i,&pItem);
			pItem->GetDisplayName(SIGDN_FILESYSPATH,&pFilePath);
			t_odbFilePath+=pFilePath;
			FilePathVector.push_back(t_odbFilePath);
			t_odbFilePath=_T("");
		}
	}
}

BOOL CreateAllDirectories(CString strDir)
{
	//remove ending / if exists
	if(strDir.Right(1)=="\\")
		strDir=strDir.Left(strDir.GetLength()-1); 

	// base case . . .if directory exists
	if(GetFileAttributes(strDir)!=-1) 
		return FALSE;

	// recursive call, one less directory
	int nFound = strDir.ReverseFind('\\');
	if (nFound != -1)
		CreateAllDirectories(strDir.Left(nFound)); 

	// actual work
	BOOL bSuccess = FALSE;//成功标志
	// 	CreateDirectory(strDir,NULL); 
	bSuccess = CreateDirectory(strDir, NULL) ? TRUE : FALSE;
	return bSuccess;
}
//查找文件夹中所有相同后缀的文件
CString FindFileSameExtension(CString strPath,CString strExtension)
{
	CFileFind finder; 
	CString FileName;
	CString FilePath;
	bool bExist = finder.FindFile(strPath); 
	if(bExist) 
	{ 
		finder.FindNextFile();
		FileName = finder.GetFileName();
		FilePath = finder.GetFilePath();								//文件的全路径 
		CString Exten;
		Exten = PathFindExtension(FilePath);
		if (strcmp(Exten,strExtension) == 0)		//获取文件的后缀并判断是否与要找的后缀相同
		{
			finder.Close();
			return finder.GetFileTitle();
		}
	}
	finder.Close();
	return "";
}

BOOL CreateFolder(CString strPath)
{
	// 	SECURITY_ATTRIBUTES attrib;
	// 	attrib.bInheritHandle = FALSE;
	// 	attrib.lpSecurityDescriptor = NULL;
	// 	attrib.nLength =sizeof(SECURITY_ATTRIBUTES);
	//上面定义的属性可以省略。 直接return ::CreateDirectory( path, NULL); 即可
	return ::CreateDirectory( strPath, NULL/*&attrib*/);
} 

BOOL DelFilesOfDir(CString strDir)
{
	SHFILEOPSTRUCT    shFileOp;
	memset(&shFileOp,0,sizeof(shFileOp));

	shFileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION ; 
	shFileOp.hNameMappings = NULL; 
	shFileOp.hwnd = NULL; 
	shFileOp.lpszProgressTitle = NULL; 
	shFileOp.pFrom = strDir; 
	shFileOp.pTo = NULL; 
	shFileOp.wFunc = FO_DELETE; 

	return SHFileOperation(&shFileOp)==0;
}
//删除文件夹目录(非空) 上面提到的删除目录的方法只能删除空目录（即文件夹），如果目录下有文件或者子目录，就不能删除了，VC里好像没有直接的函数，只能手动写个函数来删除了，下面提供一个删除非空目录的方法： 

	bool DeleteDirectory(CString strDir)
{
	if(strDir.IsEmpty())   

	{ 

		RemoveDirectory(strDir); 

		return false; 

	} 

	//首先删除文件及子文件夹 

	CFileFind   ff; 

	BOOL bFound = ff.FindFile(strDir+ _T("\\*"),0); 

	while(bFound) 

	{ 

		bFound = ff.FindNextFile(); 

		if(ff.GetFileName()== _T(".")||ff.GetFileName()== _T("..")) 

			continue; 

		//去掉文件(夹)只读等属性 

		SetFileAttributes(ff.GetFilePath(),FILE_ATTRIBUTE_NORMAL); 

		if(ff.IsDirectory())  

		{   

			//递归删除子文件夹 

			DeleteDirectory(ff.GetFilePath()); 

			RemoveDirectory(ff.GetFilePath()); 

		} 

		else   

		{ 

			DeleteFile(ff.GetFilePath());   //删除文件 

		} 

	} 

	ff.Close(); 

	//然后删除该文件夹 

	RemoveDirectory(strDir); 
	return true;
}
//拷贝路径
BOOL CopyFolder(CString strFromDir,CString strToDir)
{
	SHFILEOPSTRUCT FileOp; 
	ZeroMemory((void*)&FileOp,sizeof(SHFILEOPSTRUCT));

	FileOp.fFlags = FOF_SILENT | FOF_NOCONFIRMATION ; 
	FileOp.hNameMappings = NULL; 
	FileOp.hwnd = NULL; 
	FileOp.lpszProgressTitle = NULL; 
	FileOp.pFrom = strFromDir; 
	FileOp.pTo = strToDir; 
	FileOp.wFunc = FO_COPY; 

	return SHFileOperation(&FileOp) == 0;
}

//判断点是否在Rect中
bool JudgePointInRect(CPoint pt,CRect JRect)
{
	if (pt.x<JRect.left || pt.x>JRect.right||pt.y<JRect.top||pt.y>JRect.bottom)
	{
		return false;
	}
	else
	{
		return true;
	}
}
CString GetAppPath()
{
	CString Tmp;
	CHAR filepath[MAX_PATH];
	TCHAR driver[MAX_PATH];
	TCHAR fullPath[MAX_PATH];
	TCHAR namePath[MAX_PATH];
	GetModuleFileName(NULL,filepath,MAX_PATH);
	_tsplitpath(filepath,driver,fullPath,namePath,namePath);
	Tmp.Format("%s%s",driver,fullPath);
	return Tmp;
}

CString GetDirPathByDialog()
{  
	TCHAR           szFolderPath[MAX_PATH] = {0};  
	CString         strFolderPath = TEXT("");  

	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot   = 0;  
	sInfo.lpszTitle   = _T("请选择一个文件夹：");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	}  

	return strFolderPath;  

} 
//获取程序运行当前路径
CString GetCurRunDir()
{
	CString str = "";
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL,szFilePath,MAX_PATH);
	(_tcsrchr(szFilePath,_T('\\')))[1] = 0;
	return str = szFilePath;
}
/************************************************************************/
/*							字符串常用操作						        */
/************************************************************************/
/************************************************************************
函数名称: str_to_hex 
函数功能: 字符串转换为十六进制 
输入参数: string 字符串 cbuf 十六进制 len 字符串的长度。 
输出参数: 无 
************************************************************************/   
int str_to_hex(char *string, unsigned char *cbuf, int len)
{
	BYTE high, low;  
	int idx, ii=0;  
	for (idx=0; idx<len; idx+=2)   
	{  
		high = string[idx];  
		low = string[idx+1];  

		if(high>='0' && high<='9')  
			high = high-'0';  
		else if(high>='A' && high<='F')  
			high = high - 'A' + 10;  
		else if(high>='a' && high<='f')  
			high = high - 'a' + 10;  
		else  
			return -1;  

		if(low>='0' && low<='9')  
			low = low-'0';  
		else if(low>='A' && low<='F')  
			low = low - 'A' + 10;  
		else if(low>='a' && low<='f')  
			low = low - 'a' + 10;  
		else  
			return -1;  

		cbuf[ii++] = high<<4 | low;  
	}  
	return 0; 
}
/**************************************************************************** 
函数名称: hex_to_str 
函数功能: 十六进制转字符串 
输入参数: ptr 字符串 buf 十六进制 len 十六进制字符串的长度。 
输出参数: 无 
*****************************************************************************/   
void hex_to_str(char *ptr,unsigned char *buf,int len)  
{  
	for(int i = 0; i < len; i++)  
	{  
		sprintf(ptr, "%02x",buf[i]);  
		ptr += 2;  
	}  
}  
/**************************************************************************** 
函数名称: char_to_wchar 
函数功能: char转wchar
输入参数: ptr 输入字符串 buf输出字符串。 
输出参数: 无 
*****************************************************************************/   
void char_to_wchar(const char *ptr, wchar_t *buf)
{
	size_t len = strlen(ptr);
	size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)ptr, int(len), NULL, 0);
	/*buf = new wchar_t[wlen + 1];*/
	MultiByteToWideChar(CP_ACP, 0, (const char*)ptr, int(len), buf, int(wlen));
}
/************************************************************************
函数名称: CStringExistSymble 
函数功能: 判断字符串中是否含有除数字、字母、汉字以外的非法字符 
输入参数: str 字符串 
输出参数: 无 
************************************************************************/
int IsCStringExistSymble(CString str)
{
	if (str.GetLength() == 0)
	{
		return 1;
	}
	char *FileN = str.GetBuffer(0);
	for (int i=0;i<str.GetLength();i++)
	{
		if ((FileN[i]>='0'&&FileN[i]<='9')||(FileN[i]>='a'&&FileN[i]<='z')||(FileN[i]>='A'&&FileN[i]<='Z')||FileN[i]>127||FileN[i]<0)
		{
			
		}
		else
		{
			return 2;
		}
	}
	return 0;
}
/************************************************************************
函数名称: GetCStringBetweenAB
函数功能: 获取字符串中A和B之间的字符 
输入参数: strSrc 原始字符串 cA cB为差分字符 
返回值: 结果 字符串为空则标示失败 
************************************************************************/
CString GetCStringBetweenAB(CString strSrc,char cA,char cB)
{
	CString tmp;
	int nPos1,nPos2;
	int nLength = strSrc.GetLength();
	tmp = _T("");
	if(nLength<3)
	{
		return tmp;
	}
	nPos1 = strSrc.Find(cA);
	if(nPos1<0)
	{
		return tmp;
	}
	if (cA == cB)
	{
		nPos2 = strSrc.Find(cB,nPos1+1);
	}
	else
	{
		nPos2 = strSrc.Find(cB);
	}
	if(nPos2<0)
	{
		return tmp;
	}
	tmp = strSrc.Mid(nPos1+1,nPos2-nPos1-1);
	return tmp;
}
CString GetCStringBetweenAB(CString strSrc,CString strA,CString strB)
{
	CString tmp = _T("");
	int nPos1,nPos2;
	int nL,nL1,nL2;
	nL = strSrc.GetLength();
	nL1 = strA.GetLength();
	nL2 = strB.GetLength();
	nPos1 = strSrc.Find(strA);
	if (strcmp(strA,strB) == 0)
	{
		nPos2 = strSrc.Find(strB,nPos1+1);
	}
	else
	{
		nPos2 = strSrc.Find(strB);
	}

	if(nPos1<0||nPos2<0)
	{
		return tmp;
	}
	tmp = strSrc.Mid(nPos1+nL1,nPos2-nPos1-nL1);
	return tmp;
}
/************************************************************************
函数名称: GetNumberAfterA
函数功能: 获取字符串中A字符以后的数字，遇到非数字则停止 
输入参数: strSrc 原始字符串 cA字符 
返回值: 结果 字符串为空则标示失败 
************************************************************************/

CString GetNumberAfterA(CString &strSrc,char cA)
{
	CString tmp = _T("");
	int nPos1;
	int nL;
	nL = strSrc.GetLength();
	nPos1 = strSrc.Find(cA);
	tmp=strSrc.Right(nL-nPos1-1).SpanIncluding(_T("0123456789.+-"));
	return tmp;
}
//从nPOs位置开始获取一串数字直至碰上非数字字符，并将原始字符串去掉以取出的部分
CString GetNumberStartN(CString &strSrc,int nPos)
{
	CString tmp = _T("");
	int nL;
	nL = strSrc.GetLength();
	int nNum = 0;
	if (nPos>=0)
	{
		for (int i=nPos;i<nL;i++)
		{
			char c = strSrc.GetAt(i);
			if ((c>='0'&&c<='9')||c == '-'||c == '+'||c == '.')
			{
				tmp+=c;
				nNum ++;
			}
			else
			{
				break;
			}
		}
		if (nNum>0)
		{
			strSrc = strSrc.Right(nL-nPos-nNum);
		}
	}
	return tmp;
}
void splitStr(TCHAR* srcStr,TCHAR* findedStr,vector<CString> &param)
{
	TCHAR* TmpStr = srcStr;
	size_t len = strlen(srcStr);
	size_t pos = 0;
	while(strstr(TmpStr,findedStr) != NULL)
	{
		pos =len - strlen(strstr(TmpStr,findedStr));
		TmpStr[pos] ='\0';
		if (pos != 0)
			param.push_back(CString(srcStr));
		TmpStr += (pos += strlen(findedStr)); 
		srcStr = TmpStr;
		len = strlen(TmpStr);
	}
	if(len > 0)
		param.push_back(CString(srcStr));
}
//检测字符串是否从某个子串开始
bool IsStartWith(TCHAR *srcStr ,TCHAR *FindedSrt)
{
	if ( 0 == strlen(srcStr))
		return false;
	size_t lenght = strlen(srcStr);
	for (int i = 0;i < lenght ; i++) {
		if(TCHAR(' ') == *srcStr)srcStr++;
		else
			break;
	}
	TCHAR *clearStr = strstr(srcStr,FindedSrt);
	if (clearStr == NULL)
		return false;
	return strcmp(srcStr,clearStr) == 0;
}
//寻找字符串并替换
void ReplaceStr(CString &dynText, CString strFinded, CString strMid)
{
	int npos = dynText.Find(strFinded);
	if (npos >=0) {
		CString strLeft = dynText.Left(npos);
		CString strRight = dynText.Right(dynText.GetLength() - npos - strFinded.GetLength());
		dynText.Format("%s%s%s",strLeft,strMid,strRight);
		npos = 0;
	}
}
/************************************************************************/
/*						  字符串常用操作结束						    */
/************************************************************************/


/************************************************************************/
/*								 图像操作开始                           */
/************************************************************************/
//填充位图头文件
void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert( bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));

	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);

	memset( bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;

	if( bpp == 8 )
	{
		RGBQUAD* palette = bmi->bmiColors;
		int i;
		for( i = 0; i < 256; i++ )
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}
#ifdef USE_OPENCV
//绘制十字线
void DrawCrossOnImage(IplImage *Img,CvPoint pt,int nLength,CvScalar rgb)
{
	if (Img == NULL ||pt.x<0||pt.y<0||pt.x>Img->width||pt.y>Img->height)
	{
		return;
	}
	CvPoint pt1,pt2,pt3,pt4;
	pt1.x = pt.x-nLength<0?0:pt.x-nLength;
	pt1.y = pt.y;
	pt2.x = pt.x+nLength>Img->width?Img->width:pt.x+nLength;
	pt2.y = pt.y;
	pt3.x = pt.x;
	pt3.y = pt.y-nLength<0?0:pt.y-nLength;
	pt4.x = pt.x;
	pt4.y = pt.y+nLength>Img->height?Img->height:pt.y+nLength;
	cvLine(Img,pt1,pt2,rgb,2);
	cvLine(Img,pt3,pt4,rgb,2);
}
/************************************************************************/
/* 函数功能：在DC上绘制			图形                                    */
/************************************************************************/
void DrawGraphOnDC(CDC *pDC,DrawShape DShape)
{
	ASSERT(pDC);
	CPoint pt1,pt2,pt3,pt4;
	CPen pen,pen1,*oldpen;
	CRect rect;
	switch(DShape.Shape)
	{
	case SHAPE_RECTANGLE_R:
		break;
	case SHAPE_RECTANGLE_P:
		pt1.x = DShape.CterPoint.x - DShape.nLength;
		pt1.y = DShape.CterPoint.y - DShape.nLength;
		pt2.x = DShape.CterPoint.x + DShape.nLength;
		pt2.y = DShape.CterPoint.y - DShape.nLength;
		pt3.x = DShape.CterPoint.x + DShape.nLength;
		pt3.y = DShape.CterPoint.y + DShape.nLength;
		pt4.x = DShape.CterPoint.x - DShape.nLength;
		pt4.y = DShape.CterPoint.y + DShape.nLength;
		
		pen.CreatePen(PS_SOLID,1,DShape.rgb);
		oldpen=pDC->SelectObject(&pen);
		//绘制直线
		pDC->MoveTo(pt1.x,pt1.y);
		pDC->LineTo(pt2.x,pt2.y);
		pDC->LineTo(pt3.x,pt3.y);
		pDC->LineTo(pt4.x,pt4.y);
		pDC->LineTo(pt1.x,pt1.y);
		pDC->SelectObject(oldpen);
		pen.DeleteObject();
		break;
	case SHAPE_CIRCLE:
		pen.CreatePen(PS_SOLID,1,DShape.rgb);
		oldpen=pDC->SelectObject(&pen);
		rect.left = DShape.CterPoint.x - DShape.nLength;
		rect.top = DShape.CterPoint.y - DShape.nLength;
		rect.right = DShape.CterPoint.x + DShape.nLength;
		rect.bottom = DShape.CterPoint.y + DShape.nLength;
		pDC->Arc(rect,CPoint(0,0),CPoint(0,0));
		pDC->SelectObject(oldpen);
		pen.DeleteObject();
		break;
	case SHAPE_CROSS:
		break;
	case SHAPE_RULER:
		break;
	default:
		break;
	}
}
/************************************************************************/
/* 函数功能：在DC上绘制图像		                                        */
/*	   参数：Img 图像 MemDc  目标DC								        */
/************************************************************************/
void DrawImageOnMemDc(IplImage *Img,CDC *pMemDC,CBitmap *bmp,float fImageScale)
{
	ASSERT(Img);
	ASSERT(pMemDC);
  	uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
  	BITMAPINFO* bmi = (BITMAPINFO*)buffer;
  	int bmp_w = Img->width;
  	int bmp_h = Img->height;
  
  	int bpp = Img ? (Img->depth & 255)*Img->nChannels : 0;
  	//代替cvvimage的Bpp()函数
  
  	FillBitmapInfo( bmi, bmp_w, bmp_h, bpp,Img->origin );
  
  	HBITMAP hOldBitmap;
  	CBitmap *pOldBit = NULL;
  
	pOldBit = pMemDC->SelectObject(bmp);									//将位图选择进内存DC
	pMemDC->FillSolidRect(CRect(0,0,bmp_w,bmp_h),RGB(255,255,255)); //按原来背景填充客户区，不然会是黑色
	pMemDC->SetStretchBltMode(COLORONCOLOR);

	int t = StretchDIBits(
		pMemDC->GetSafeHdc(),0,0,int(Img->width * fImageScale+0.5), int(Img->height * fImageScale+0.5),0,0, bmp_w, bmp_h,
		Img->imageData, bmi, DIB_RGB_COLORS,SRCCOPY );
  	pMemDC->SelectObject(&hOldBitmap);
  	if (hOldBitmap)
  	{
  		DeleteObject(hOldBitmap);
  		hOldBitmap=NULL;
  	}
}
#endif
/************************************************************************/
/*								图像操作结束                             */
/************************************************************************/



/************************************************************************/
/*								其它操作开始                             */
/************************************************************************/

CCriticalSection g_filelock;

void CWlog(CString mlog)	{
	g_filelock.Lock();
	CFile test;
	CString ttt;
	SYSTEMTIME time;
	GetLocalTime(&time);
	ttt.Format("%s:%d/%d/%d/ %d:%d:%d\r\n",mlog,time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);\
	TRACE(ttt);
	test.Open(_T("LOG\\log.txt"),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
	test.SeekToEnd();
	test.Write(ttt,ttt.GetLength());
	test.Close();
	g_filelock.Unlock();
}
int GetRand(int min,int max)
{
	int m_nMax;
	m_nMax=RAND_MAX;
	srand((unsigned)time(0));
	return (int)(rand()*(max-min)/m_nMax+min);
}

/************************************************************************/
/*								其它操作结束                             */
/************************************************************************/
/************************************************************************/
/*							网络相关操作						        */
/************************************************************************/
int GetInternetConnectState()
{
	DWORD flags;//上网方式 
	BOOL m_bOnline=TRUE;//是否在线 

	m_bOnline=InternetGetConnectedState(&flags,0); 
	if(m_bOnline)//在线 
	{ 
		if ((flags & INTERNET_CONNECTION_MODEM) ==INTERNET_CONNECTION_MODEM) 
		{ 
			//在线拨号上网
			return 0;
		} 
		if ((flags & INTERNET_CONNECTION_LAN) ==INTERNET_CONNECTION_LAN) 
		{ 
			return 1;
			//cout<<"在线：通过局域网\n"; 
		} 
		if ((flags & INTERNET_CONNECTION_PROXY) ==INTERNET_CONNECTION_PROXY) 
		{ 
			return 2;
			//cout<<"在线：代理\n"; 
		} 
		if ((flags & INTERNET_CONNECTION_MODEM_BUSY) ==INTERNET_CONNECTION_MODEM_BUSY) 
		{ 
			//cout<<"MODEM被其他非INTERNET连接占用\n"; 
			return 3;
		} 
	} 
	else 
	{
		return -1;
	}
}
//判断一个IP是否联通
BOOL IfIPConnect(const char *strIPAddr)
{
	UINT ip = inet_addr(strIPAddr);
	ULONG hopCount = 0;
	ULONG RTT = 0;

	if(GetRTTAndHopCount(ip, &hopCount, 2, &RTT) == TRUE) 
	{
		// 		printf("Hops: %ld\n", hopCount);
		// 		printf("RTT: %ld\n", RTT);
		return TRUE;
	}
	else 
	{
		//printf("Error: %ld\n", GetLastError());
		return FALSE;
	}
}
/************************************************************************/
/*						  网络相关操作结束						        */
/************************************************************************/