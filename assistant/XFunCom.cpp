
#include "stdafx.h"
#include "XFunCom.h"
#include <WinInet.h>
#include <Iphlpapi.h>
#pragma comment(lib,"Wininet.lib")
#pragma comment(lib,"Iphlpapi.lib")
#include <ctime>
#include <tlhelp32.h>
#include <psapi.h>
#pragma comment (lib,"psapi.lib") 

CString LogSavePath = CString("LOG\\log.txt");
//���ͷ�CPU,��Ϣѭ�����еĵȴ�
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
	CloseHandle(eWaitTimeEvent);
	eWaitTimeEvent = NULL;

}
//////////////////////////////////////////////////////////////////////////
//							ʱ�������ʼ
//////////////////////////////////////////////////////////////////////////
void LTimeCount::Start() // ��ʱ��ʼ
{
	QueryPerformanceFrequency( &Frequency );
	QueryPerformanceCounter  ( &old );
	UseTime = 0.0;
}
void LTimeCount::End() // ��ʱ����
{
	QueryPerformanceCounter( &Time );
	UseTime = (double) ( Time.QuadPart - old.QuadPart) / (double)Frequency.QuadPart;
	// 	TRACE("Use time = %20.10f(s)\n", UseTime );
}

double LTimeCount::GetUseTime() // ����㷨����ʱ��(��λ:��)
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

//////////////////////////////////////////////////////////////////////////
//							ʱ���������
//////////////////////////////////////////////////////////////////////////



/////////////////Ŀ¼��顢�ļ���顢Ŀ¼����
BOOL FolderExist(CString strPath)
{
	WIN32_FIND_DATA   wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath+"*.*", &wfd);
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
void GetDirNameFromDir(CString csDirPath,std::vector<CString> &m_FileList )  
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
//strDirΪ������ļ���·����vecFilesΪɨ�赽���ļ��� 
void GetFileNameFromDir(CString strDir, std::vector<CString>& vecFiles,int type) 
{  
	vecFiles.clear();
	CFileFind ff;  
	//��·���������\*.*��׺  
	if (strDir.Right(1) != "\\")  
		strDir += "\\";  
	strDir += "*.*";  
	BOOL ret = ff.FindFile(strDir);  
	while (ret)  
	{  
		ret = ff.FindNextFile();  
		if (ff.IsDirectory() && !ff.IsDots())  
		{  
			CString path = ff.GetFilePath();  
			GetFileNameFromDir(path, vecFiles);  
		}  
		else if (!ff.IsDirectory() && !ff.IsDots())  
		{  

			CString name = ff.GetFileName();//��ȡ����׺���ļ���  
			switch(type){
			case 0:
				name = ff.GetFileName();//��ȡ����׺���ļ���  
				break;
			case 1:
				name = ff.GetFilePath();//��ȡ�ļ�·��  
				break;
			case 2:
				name = ff.GetFileTitle();//��ȡ������׺���ļ���  
				break;
			}
			vecFiles.push_back(name);  
		}  

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
	BOOL bSuccess = FALSE;//�ɹ���־
	// 	CreateDirectory(strDir,NULL); 
	bSuccess = CreateDirectory(strDir, NULL) ? TRUE : FALSE;
	return bSuccess;
}
//�����ļ�����������ͬ��׺���ļ�
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
		FilePath = finder.GetFilePath();								//�ļ���ȫ·�� 
		CString Exten;
		Exten = PathFindExtension(FilePath);
		if (strcmp(Exten,strExtension) == 0)		//��ȡ�ļ��ĺ�׺���ж��Ƿ���Ҫ�ҵĺ�׺��ͬ
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
	//���涨������Կ���ʡ�ԡ� ֱ��return ::CreateDirectory( path, NULL); ����
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
//ɾ���ļ���Ŀ¼(�ǿ�) �����ᵽ��ɾ��Ŀ¼�ķ���ֻ��ɾ����Ŀ¼�����ļ��У������Ŀ¼�����ļ�������Ŀ¼���Ͳ���ɾ���ˣ�VC�����û��ֱ�ӵĺ�����ֻ���ֶ�д��������ɾ���ˣ������ṩһ��ɾ���ǿ�Ŀ¼�ķ����� 

bool DeleteDirectory(CString strDir)
{
	if(strDir.IsEmpty())   

	{ 

		RemoveDirectory(strDir); 

		return false; 

	} 

	//����ɾ���ļ������ļ��� 

	CFileFind   ff; 

	BOOL bFound = ff.FindFile(strDir+ _T("\\*"),0); 

	while(bFound) 

	{ 

		bFound = ff.FindNextFile(); 

		if(ff.GetFileName()== _T(".")||ff.GetFileName()== _T("..")) 

			continue; 

		//ȥ���ļ�(��)ֻ�������� 

		SetFileAttributes(ff.GetFilePath(),FILE_ATTRIBUTE_NORMAL); 

		if(ff.IsDirectory())  

		{   

			//�ݹ�ɾ�����ļ��� 

			DeleteDirectory(ff.GetFilePath()); 

			RemoveDirectory(ff.GetFilePath()); 

		} 

		else   

		{ 

			DeleteFile(ff.GetFilePath());   //ɾ���ļ� 

		} 

	} 

	ff.Close(); 

	//Ȼ��ɾ�����ļ��� 

	RemoveDirectory(strDir); 
	return true;
}
//����·��
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

//�жϵ��Ƿ���Rect��
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

CString GetDirPathByDialog(CString rootpath,CWnd* papa)
{
	try{
		CFolderPickerDialog fd(NULL, 0, papa, 0);
		//fd.GetOFN().lpstrInitialDir = rootpath;// Ĭ��Ŀ¼
		if (fd.DoModal()== IDOK)
		{
			CString des;
			des = fd.GetPathName();
			return des;
		}
	}
	catch(...){
		int a = 0;
		AfxMessageBox("����");
	}
	return "";


} 
//��ȡ�������е�ǰ·��
CString GetCurRunDir()
{
	CString str = "";
	TCHAR szFilePath[MAX_PATH + 1];
	GetModuleFileName(NULL,szFilePath,MAX_PATH);
	(_tcsrchr(szFilePath,_T('\\')))[1] = 0;
	return str = szFilePath;
}


void Decompresss(CString Input,CString OutPut,int useTool)
{
	CString pstrPath = GetAppPath();
	CString cmd;
	CreateAllDirectories(OutPut);
	switch(useTool){
	case 0://ʹ�õ���7z
		pstrPath += "7za.exe";
		if(strstr(Input.GetBuffer(),".tgz") != NULL || strstr(Input.GetBuffer(),".tar.gz") != NULL){
			cmd.Format("/c %s x \"%s\" -so | \"%s\" x -si -ttar -y -o\"%s""",pstrPath,Input,pstrPath,OutPut);
		}
		else{
			cmd.Format("/c %s x \"%s\" -y -o\"%s\"",pstrPath,Input,OutPut);
		}
		break;
	case 1:
		pstrPath += "decom\\HaoZipC.exe";
		if(!FileExist(pstrPath))
		{
			myAfxMessageBox("�Ҳ�����ѹ���");
			return;
		}
		cmd.Format("/c %s x \"%s\" -y -o\"%s\"",pstrPath,Input,OutPut);
		break;
	}
// 	USES_CONVERSION;
// 	LPTSTR p = W2A(cmd);
	WaitProcess("cmd.exe",cmd.GetBuffer());
}
void SplitFilePath(vector<CString> &res)
{
	res.clear();

	TCHAR pstrPath[MAX_PATH];

	TCHAR sFilename[MAX_PATH];

	TCHAR sDrive[_MAX_DRIVE];

	TCHAR sDir[_MAX_DIR];

	TCHAR sFname[_MAX_FNAME] = _T("");

	TCHAR sExt[_MAX_EXT];

	TCHAR curWorkDir[_MAX_EXT] = _T("");

	GetModuleFileName(NULL, sFilename, _MAX_PATH);

	_tsplitpath_s(sFilename, sDrive, sDir, sFname, sExt);
	res.push_back(sDrive);
	res.push_back(sDir);
	res.push_back(sFname);
	res.push_back(sExt);
	return;
}
/************************************************************************/
/*							�ַ������ò���						        */
/************************************************************************/
/************************************************************************
��������: str_to_hex 
��������: �ַ���ת��Ϊʮ������ 
�������: string �ַ��� cbuf ʮ������ len �ַ����ĳ��ȡ� 
�������: �� 
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
��������: hex_to_str 
��������: ʮ������ת�ַ��� 
�������: ptr �ַ��� buf ʮ������ len ʮ�������ַ����ĳ��ȡ� 
�������: �� 
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
��������: char_to_wchar 
��������: charתwchar
�������: ptr �����ַ��� buf����ַ����� 
�������: �� 
*****************************************************************************/   
void char_to_wchar(const char *ptr, wchar_t *buf)
{
	size_t len = strlen(ptr);
	size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)ptr, int(len), NULL, 0);
	/*buf = new wchar_t[wlen + 1];*/
	MultiByteToWideChar(CP_ACP, 0, (const char*)ptr, int(len), buf, int(wlen));
}
/************************************************************************
��������: CStringExistSymble 
��������: �ж��ַ������Ƿ��г����֡���ĸ����������ķǷ��ַ� 
�������: str �ַ��� 
�������: �� 
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
��������: GetCStringBetweenAB
��������: ��ȡ�ַ�����A��B֮����ַ� 
�������: strSrc ԭʼ�ַ��� cA cBΪ����ַ� 
����ֵ: ��� �ַ���Ϊ�����ʾʧ�� 
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
��������: GetNumberAfterA
��������: ��ȡ�ַ�����A�ַ��Ժ�����֣�������������ֹͣ 
�������: strSrc ԭʼ�ַ��� cA�ַ� 
����ֵ: ��� �ַ���Ϊ�����ʾʧ�� 
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
//��nPOsλ�ÿ�ʼ��ȡһ������ֱ�����Ϸ������ַ�������ԭʼ�ַ���ȥ����ȡ���Ĳ���
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
//����ַ����Ƿ��ĳ���Ӵ���ʼ
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
//Ѱ���ַ������滻
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
/*						  �ַ������ò�������						    */
/************************************************************************/


/************************************************************************/
/*								 ͼ�������ʼ                           */
/************************************************************************/
//���λͼͷ�ļ�
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
//����ʮ����
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
/* �������ܣ���DC�ϻ���			ͼ��                                    */
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
		//����ֱ��
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
/* �������ܣ���DC�ϻ���ͼ��		                                        */
/*	   ������Img ͼ�� MemDc  Ŀ��DC								        */
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
	//����cvvimage��Bpp()����

	FillBitmapInfo( bmi, bmp_w, bmp_h, bpp,Img->origin );

	HBITMAP hOldBitmap;
	CBitmap *pOldBit = NULL;

	pOldBit = pMemDC->SelectObject(bmp);									//��λͼѡ����ڴ�DC
	pMemDC->FillSolidRect(CRect(0,0,bmp_w,bmp_h),RGB(255,255,255)); //��ԭ���������ͻ�������Ȼ���Ǻ�ɫ
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
/*								ͼ���������                             */
/************************************************************************/



/************************************************************************/
/*								����������ʼ                             */
/************************************************************************/

CCriticalSection g_filelock;
void CwlogFormat(LPCTSTR log , ...)
{
	CString sFormat = log;
	// Do ordinary printf replacements
	// NOTE: Documented max-length of wvsprintf() is 1024
	TCHAR szBuffer[1025] = { 0 };
	va_list argList;
	va_start( argList, log );

	// wvsprintf��֧�ָ����ʽ�����Ի���_vstprintf
	int iRet = ::_vstprintf( szBuffer, sFormat, argList );
	CWlog(szBuffer);
	va_end( argList );

}
void CWlog(CString mlog)	{
	g_filelock.Lock();
	CFile test;
	CString ttt;
	SYSTEMTIME time;
	GetLocalTime(&time);
	ttt.Format("%d/%d/%d/ %d:%d:%d:    %s\r\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond,mlog);
	TRACE(ttt);
	test.Open(_T(LogSavePath),CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite);
	test.SeekToEnd();
	test.Write(ttt,ttt.GetLength());
	test.Close();
	g_filelock.Unlock();
}
void SetLogPath(CString savePath)
{
	if (savePath == "")
		return;
	if (!FileExist(savePath))
	{
		int pos = savePath.ReverseFind('\\');
		CString path = savePath.Left(pos);
		CreateAllDirectories(path);

	}
	LogSavePath = savePath;
}
int GetRand(int min,int max)
{
	int m_nMax;
	m_nMax=RAND_MAX;
	srand((unsigned)time(0));
	return (int)(rand()*(max-min)/m_nMax+min);
}
void WaitProcess(LPTSTR FileName,LPTSTR Param)
{
	SHELLEXECUTEINFO ShellInfo = {0};
	ShellInfo.cbSize	= sizeof(SHELLEXECUTEINFO);
	ShellInfo.fMask	= SEE_MASK_NOCLOSEPROCESS;
	ShellInfo.hwnd	= 0;
	ShellInfo.lpVerb	= 0;
	ShellInfo.lpFile	= FileName;     /////����cmd.exe·��
	ShellInfo.lpParameters	= Param;    //////����   /c  copy ....
	ShellInfo.lpDirectory	= 0;
	ShellInfo.nShow	= SW_HIDE;
	ShellInfo.hInstApp	= 0;

	ShellExecuteEx(&ShellInfo);
	//WaitForSingleObject(ShellInfo.hProcess,-1);
	DWORD dwExitCode;
	GetExitCodeProcess(ShellInfo.hProcess,&dwExitCode);
	while (dwExitCode == STILL_ACTIVE)
	{ 
		XWaitTime(0.1);
		GetExitCodeProcess(ShellInfo.hProcess, &dwExitCode);
	}
	CloseHandle(ShellInfo.hProcess); 
}
BOOL memsort()
{
	HANDLE SnapShot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(SnapShot==NULL)
	{
		return FALSE;
	}
	PROCESSENTRY32 ProcessInfo;//����������Ϣ����
	ProcessInfo.dwSize=sizeof(ProcessInfo);//����ProcessInfo�Ĵ�С
	//����ϵͳ�е�һ�����̵���Ϣ
	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	while(Status)
	{
		HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,ProcessInfo.th32ProcessID);
		if(hProcess)
		{
			SetProcessWorkingSetSize(hProcess,0,0);
			//�ڴ�����
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		printf("%s/n",ProcessInfo.szExeFile);
		//��ȡ��һ�����̵���Ϣ
		Status=Process32Next(SnapShot,&ProcessInfo);
	}
	return TRUE;
}
#ifdef USE_STL
vector<CString> GetresByStlRx(CString word,CString rule)
{
	//http://blog.csdn.net/flyfish1986/article/details/5877838�﷨�ο�
	// 	CString strRegex = "({[0-9_]+}@{[a-zA-Z0-9]+[.][a-zA-Z0-9]+[.]?[a-zA-Z0-9]+})" ;
	// 	CString strInput = "1234@domain.com" ;
	CString strRegex = rule ;
	CString strInput = word ;
	vector<CString> result;
	CAtlRegExp < CAtlRECharTraits > reRule ;
	char * wt = strRegex.GetBuffer() ;
	REParseError status = reRule.Parse((const ATL :: CAtlRegExp < CAtlRECharTraits > :: RECHAR *)wt);
	if (REPARSE_ERROR_OK != status)
	{
		return result;
	}
	CAtlREMatchContext<CAtlRECharTraits> mcRule;
	wt = strInput.GetBuffer();
	if (!reRule.Match((const ATL::CAtlRegExp<CAtlRECharTraits>::RECHAR *)wt,&mcRule))
	{
		return result;
	}
	else
	{
		for (UINT nGroupIndex = 0; nGroupIndex < mcRule.m_uNumGroups; ++nGroupIndex)
		{
			const CAtlREMatchContext<>::RECHAR* szStart = 0;
			const CAtlREMatchContext<>::RECHAR* szEnd = 0;
			mcRule.GetMatch(nGroupIndex, &szStart, &szEnd);
			ptrdiff_t nLength = szEnd - szStart;
			CString singalpart(szStart,  static_cast<int>(nLength));
			result.push_back(singalpart);
		}
	}
	return result;
}
#endif
void WaitForBOOL(volatile BOOL &waitObject , BOOL isWaitForTrue)
{
	if(isWaitForTrue){
		while(!waitObject)
		{
			Sleep(10);
			MSG msg;
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))  
			{  
				DispatchMessage(&msg);  
				TranslateMessage(&msg);  
			} 
		}
	}
	else
	{
		while(waitObject)
		{
			Sleep(10);
			MSG msg;
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))  
			{  
				DispatchMessage(&msg);  
				TranslateMessage(&msg);  
			} 

		}
	}

}
/************************************************************************/
/*								������������                             */
/************************************************************************/
/************************************************************************/
/*							������ز���						        */
/************************************************************************/
int GetInternetConnectState()
{
	DWORD flags;//������ʽ 
	BOOL m_bOnline=TRUE;//�Ƿ����� 

	m_bOnline=InternetGetConnectedState(&flags,0); 
	if(m_bOnline)//���� 
	{ 
		if ((flags & INTERNET_CONNECTION_MODEM) ==INTERNET_CONNECTION_MODEM) 
		{ 
			//���߲�������
			return 0;
		} 
		if ((flags & INTERNET_CONNECTION_LAN) ==INTERNET_CONNECTION_LAN) 
		{ 
			return 1;
			//cout<<"���ߣ�ͨ��������\n"; 
		} 
		if ((flags & INTERNET_CONNECTION_PROXY) ==INTERNET_CONNECTION_PROXY) 
		{ 
			return 2;
			//cout<<"���ߣ�����\n"; 
		} 
		if ((flags & INTERNET_CONNECTION_MODEM_BUSY) ==INTERNET_CONNECTION_MODEM_BUSY) 
		{ 
			//cout<<"MODEM��������INTERNET����ռ��\n"; 
			return 3;
		} 
	} 
	else 
	{
		return -1;
	}
}
//�ж�һ��IP�Ƿ���ͨ
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


#ifdef USE_EMAIL

BOOL SendAEmail(CString sendcount, CString sendPwd, CString receiver, CString senddata,CString topic,CString filePath)
{
	char t_receiver[50];char t_sendcount[50],t_sendPwd[50];
	CSmtp smtp(  
		25,                    /*smtp�˿�*/  
		"smtp.163.com",        /*smtp��������ַ*/  
		LPCSTR(sendcount),    /*��������ַ*/  
		LPCSTR(sendPwd),      /*��������*/  
		LPCSTR(receiver),    /*Ŀ�������ַ*/  
		LPCSTR(topic),       /*����*/  
		LPCSTR(senddata)     /*�ʼ�����*/  
		);  
	if(filePath != ""){/*������ַ*/  
		string tmp = LPCSTR(filePath);
		smtp.AddAttachment(tmp); 
	}
	smtp.SendEmail_Ex();
	return TRUE;
}
#endif




/************************************************************************/
/*						  ������ز�������						        */
/************************************************************************/
//�����ļ��������е��ļ���
void AddDirDir(CString strText,vector<CString> &vecDir,vector<CTime> &vecDirTime) {

	//    CString strText = GetFullPath(hTree,hTree->GetRootItem());  //�����б�����Ŀ����  
	if(strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�  
		strText += _T("\\\\");  
	strText += "*";  
	//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ  
	CFileFind file;                                       //���屾���ļ�����  
	BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�  
	while(bContinue)  
	{  
		bContinue = file.FindNextFile();                  //������һ���ļ�  
		if((/*file.IsDirectory() && */!file.IsDots()))          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."  
		{
			CString filenameWithoutDot;
			CTime lasttime;
			filenameWithoutDot = file.GetFileName();
			file.GetLastWriteTime(lasttime);
			vecDirTime.push_back(lasttime);
			vecDir.push_back(filenameWithoutDot);
		}
	}
}

void SplitFilePath(CString inPutPath,vector<CString> &res)
{

}
