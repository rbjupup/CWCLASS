
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
//可释放CPU,消息循环队列的等待
void XWaitTime(float fTime)
{
	CWTimeCount tc;
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
//							时间操作开始
//////////////////////////////////////////////////////////////////////////
void CWTimeCount::Start() // 计时开始
{
	QueryPerformanceFrequency( &Frequency );
	QueryPerformanceCounter  ( &old );
	UseTime = 0.0;
}
void CWTimeCount::End() // 计时结束
{
	QueryPerformanceCounter( &Time );
	UseTime = (double) ( Time.QuadPart - old.QuadPart) / (double)Frequency.QuadPart;
	// 	TRACE("Use time = %20.10f(s)\n", UseTime );
}

double CWTimeCount::GetUseTime() // 获得算法处理时间(单位:秒)
{		
	return UseTime;
}
void CWTimeCount::WaitTime(double waitTime)
{
	CWTimeCount tt;
	tt.Start();
	tt.End();
	while(tt.GetUseTime()*1000 < waitTime)
	{
		tt.End();
	}
}
//判断时间是否在某个时间段内
BOOL JudgeInTime(SYSTEMTIME CurrentTime,SYSTEMTIME STime,SYSTEMTIME ETime)
{
	BOOL bInTime = FALSE;
	if (CurrentTime.wYear>STime.wYear&&CurrentTime.wYear<ETime.wYear)
	{
		return TRUE;
	}
	else if (CurrentTime.wYear<STime.wYear||CurrentTime.wYear>ETime.wYear)
	{
		return FALSE;
	}
	else
	{
		if (CurrentTime.wMonth>STime.wMonth&&CurrentTime.wMonth<ETime.wMonth)
		{
			return TRUE;
		}
		else if (CurrentTime.wMonth<STime.wMonth||CurrentTime.wMonth>ETime.wMonth)
		{
			return FALSE;
		}
		else
		{
			if (CurrentTime.wDay>STime.wDay&&CurrentTime.wDay<ETime.wDay)
			{
				return TRUE;
			}
			else if (CurrentTime.wDay<STime.wDay||CurrentTime.wDay>ETime.wDay)
			{
				return FALSE;
			}
			else
			{
				if (CurrentTime.wHour>=STime.wHour&&CurrentTime.wHour<=ETime.wHour)
				{
					return TRUE;
				}
				else if (CurrentTime.wHour<STime.wHour||CurrentTime.wHour>ETime.wHour)
				{
					return FALSE;
				}
			}
		}
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
//strDir为传入的文件夹路径，vecFiles为扫描到的文件集 
void GetFileNameFromDir(CString strDir, std::vector<CString>& vecFiles,int type) 
{  
	vecFiles.clear();
	CFileFind ff;  
	//在路径后面添加\*.*后缀  
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

			CString name = ff.GetFileName();//获取带后缀的文件名  
			switch(type){
			case 0:
				name = ff.GetFileName();//获取带后缀的文件名  
				break;
			case 1:
				name = ff.GetFilePath();//获取文件路径  
				break;
			case 2:
				name = ff.GetFileTitle();//获取不带后缀的文件名  
				break;
			}
			vecFiles.push_back(name);  
		}  

	}  
}  
void WriteDataToFile(CString strFilePath,std::vector<CString>& vecData)
{
	if (!FileExist(strFilePath))
	{	
		CreateAllDirectories(strFilePath.Left(strFilePath.ReverseFind('\\')));
	}
	CWTimeCount tt;
	tt.Start();
	CStdioFile m_StFile;
	int m_nRows = 0;
	m_StFile.Open(strFilePath,CFile::modeReadWrite|CFile::modeCreate);
	if (m_StFile == NULL)
	{
		return;
	}
	CString str;
	for (int i = 0; i < vecData.size();i++)
	{
		m_StFile.WriteString(vecData[i]);				//整行读取
		m_StFile.WriteString("\n");
	}
	m_StFile.WriteString("\n");
	m_StFile.Close();
	tt.End();
	TRACE("StdioFile读入%d行耗时：%.3f\n",m_nRows,tt.GetUseTime());
	return;
}
void GetDataFromFile(CString strFilePath,std::vector<CString>& vecData)
{
	BOOL bShowTmp = FALSE;
	if (!FileExist(strFilePath))
	{
		AfxMessageBox("当前指定资料不存在!");
		return;
	}
	CStdioFile m_StFile;
	int m_nRows = 0;
	m_StFile.Open(strFilePath,CFile::modeRead);
	int lengthFile = m_StFile.GetLength();
	if (m_StFile == NULL)
	{
		return;
	}
	CString str;
	CWTimeCount tt;
	tt.Start();
	while (m_StFile.ReadString(str))					//整行读取
	{
		//TRACE("%s\n",str);
		vecData.push_back(str);	
		m_nRows ++;
	}
	m_StFile.Close();
	if(bShowTmp)
	{
		tt.End();
		TRACE("StdioFile读入%d行耗时：%.3f\n",m_nRows,tt.GetUseTime());
	}

	return;
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
//复制文件夹
void CopyDirectory(CString Src,CString Dst)
{
	Src += "\0\0";
	Dst += "\0\0";
	SHFILEOPSTRUCT FileOp;
	ZeroMemory(&FileOp,sizeof(SHFILEOPSTRUCT));
	FileOp.hwnd=NULL;
	FileOp.wFunc=FO_COPY;
	FileOp.pFrom=Src;
	FileOp.pTo=Dst;
	FileOp.fFlags=FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_NOERRORUI | FOF_SILENT;
	FileOp.hNameMappings=NULL;
	FileOp.lpszProgressTitle=NULL;
	SHFileOperation(&FileOp);
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

CString GetDirPathByDialog(CString rootpath,CWnd* papa,int type)
{
	if(type == 1){
		CString path;
		CFileDialog OpenDialog(TRUE,NULL,"",OFN_ALLOWMULTISELECT|OFN_ENABLESIZING|OFN_HIDEREADONLY);
		if (IDOK == OpenDialog.DoModal())
		{

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
				path+=pFilePath;
				path+="\n";
			}
		}
		path.TrimLeft();
		path.TrimRight();
		return path;
	}
	try{
		CFolderPickerDialog fd(NULL, 0, papa, 0);
		//fd.GetOFN().lpstrInitialDir = rootpath;// 默认目录
		if (fd.DoModal()== IDOK)
		{
			CString des;
			des = fd.GetPathName();
			return des;
		}
	}
	catch(...){
		int a = 0;
		AfxMessageBox("进了");
	}
	return "";


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


void Decompresss(CString Input,CString OutPut,int useTool)
{
	CString pstrPath = GetAppPath();
	CString cmd;
	CreateAllDirectories(OutPut);
	switch(useTool){
	case 0://使用的是7z
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
			AfxMessageBox("找不到解压软件");
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
int String2Hex(CString str, char *SendOut)
{
	int hexdata,lowhexdata; 
	int hexdatalen=0;
	int len=str.GetLength();
	//SendOut.SetSize(len/2);
	for(int i=0;i<len;)
	{
		char lstr,hstr=str[i];
		if(hstr==' '||hstr=='\r'||hstr=='\n')
		{
			i++;
			continue;
		}
		i++;
		if (i>=len)
			break;
		lstr=str[i];
		hexdata=ConvertHexData(hstr);
		lowhexdata=ConvertHexData(lstr);
		if((hexdata==16)||(lowhexdata==16))
			break;
		else
			hexdata=hexdata*16+lowhexdata;
		i++;
		SendOut[hexdatalen]=(char)hexdata;
		hexdatalen++;
	}
	//senddata.SetSize(hexdatalen);
	return hexdatalen;
}
char ConvertHexData(char ch)
{
	if((ch>='0')&&(ch<='9'))
		return ch-0x30;
	if((ch>='A')&&(ch<='F'))
		return ch-'A'+10;
	if((ch>='a')&&(ch<='f'))
		return ch-'a'+10;
	else return(-1);
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
	char* hadstr = 0;
	hadstr = strstr(TmpStr,findedStr);
	while(hadstr != NULL)
	{
		pos =len - strlen(hadstr);
		TmpStr[pos] ='\0';
		if (pos != 0)
			param.push_back(CString(srcStr));
		TmpStr += (pos += strlen(findedStr)); 
		srcStr = TmpStr;
		len = strlen(TmpStr);
		hadstr = strstr(TmpStr,findedStr);
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
/**************************************************************************** 
函数名称: CString_to_char 
函数功能: CString转char
输入参数: str 输入字符串 ptr输出字符串。 
输出参数: 无 
*****************************************************************************/   
BOOL CString_to_char(CString str,char *ptr,int charlength)
{
	if (NULL == ptr || charlength<str.GetLength())
	{
		return FALSE;
	}
	memset(ptr,0,charlength*sizeof(char));
	memcpy(ptr,str.GetBuffer(0),charlength*sizeof(char));
	return TRUE;
}
/************************************************************************/
/*						  字符串常用操作结束						    */
/************************************************************************/


/************************************************************************/
/*								 图像操作开始                           */
/************************************************************************/
//填充位图头文件
void FillBitmapInfo1(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
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

	FillBitmapInfo1( bmi, bmp_w, bmp_h, bpp,Img->origin );

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


//enlargeFactor表示保留计算的小数点位数,不是cvPoint的话可以考虑使用这个代替上面那个函数
static void Curve2Points(double Rad, double centerx, double centery, bool ClockWise, float sa, float ea, vector<CFloatPt> &m_contourPt,double enlargeFactor)
{

	//判断弧线的数量
	int tmp_case=0;
	ClockWise = !ClockWise;
	//角度变化范围是否在一度之内,是的话就直接输出两个点就可以了
	BOOL bdiffAngInOne = FALSE;
	if (((0<(ea - sa )&&(ea - sa )< 1) &&!ClockWise)||(0<(sa - ea )&&(sa - ea )< 1)&&ClockWise)
	{
		bdiffAngInOne = TRUE;
	}

	if(sa==360)	sa=0;
	if(ea==360) ea=0;
	//第一种情况从开始都结束，第二种情况从开始到360度，从0到结束
	if (!ClockWise)
	{
		if (ea>sa)
			tmp_case = 1;
	}else
	{	
		if (ea<sa)
			tmp_case = 1;
	}

	//由于opencv的画圆函数会将角度四舍五入,导致缺点
	//所以要先保存首末两点

	CFloatPt firstPt,lastPt;
	//firstPt.x = Rad * cos(ANG2RAD(sa)) + centerx;
	firstPt.x = centerx + TRANS2COX(Rad, 0, sa);
	firstPt.y = centery - TRANS2COX(Rad, 0, 450 -sa);

	lastPt.x = centerx + TRANS2COX(Rad, 0, ea);
	lastPt.y = centery - TRANS2COX(Rad, 0, 450 - ea);
	//如果是弧线,第二个点向内收缩
	if(sa < ea)
	{
		if(tmp_case)
			sa = int(sa+1.0);
		else
			sa = int(sa);
		if(tmp_case)
			ea = int(ea);
		else
			ea = int(ea + 1.0);
	}
	else
	{
		if(tmp_case)
			sa = int(sa);
		else
			sa = int(sa+1.0);
		if(tmp_case)
			ea = int(ea + 1.0);
		else
			ea = int(ea);
	}

	//如果在角度变化在1度以内
	if (bdiffAngInOne)
	{
		m_contourPt.push_back(firstPt);	
		m_contourPt.push_back(lastPt);	
		return;
	}

	int nRadius = Rad*enlargeFactor;
	//初始化数组来存放转化后的数据
	CvPoint *tmppointer = new CvPoint[361];
	CvPoint *tmppointer1 = new CvPoint[361];
	CFloatPt m_tmpPt;					//将圆弧转化成为直线插入adshape里面
	CvPoint m_CentePoint;
	m_CentePoint.x = centerx*enlargeFactor;//太小的时候会造成大误差,圆弧的精度问题
	m_CentePoint.y = centery*enlargeFactor;
	CvSize CriSize;
	CriSize.width = CriSize.height = nRadius;
	int tmp_num;

	//情况一的话就把终点坐标和tmmpointer里面的坐标比，到了就说明结束、
	//情况2的画就分两部分
	//360 - angle is trans the coordinate System
	m_contourPt.push_back(firstPt);
	if (tmp_case == 1)
	{				
		cvEllipse2Poly(m_CentePoint,CriSize,0,360-sa,360-ea,tmppointer,1);//开始到结束一次旋转一度，算出坐标
		//获取点个数
		tmp_num = abs(int(sa)-int(ea));

		if(ClockWise)
		{		
			for (int i = 0;i<tmp_num;i++)
			{
				m_tmpPt.x = tmppointer[i].x/enlargeFactor;//放大100000倍，缩小回来
				m_tmpPt.y = tmppointer[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);

			}

		}
		else
		{
			for (int i=tmp_num - 1;i >= 0;i--)
			{
				m_tmpPt.x = tmppointer[i].x/enlargeFactor;//放大100000倍，缩小回来
				m_tmpPt.y = tmppointer[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);
			}
		}
	}
	else if(tmp_case == 0)
	{								
		cvEllipse2Poly(m_CentePoint,CriSize,0,0,360-(ea>sa?ea:sa),tmppointer,1);//开始到结束一次旋转一度，算出坐标
		cvEllipse2Poly(m_CentePoint,CriSize,0,360-(ea<sa?ea:sa),360,tmppointer1,1);//开始到结束一次旋转一度，算出坐标

		if(ClockWise)
		{
			tmp_num = ea<sa?ea:sa;
			for (int i=0;i<tmp_num;i++)
			{
				m_tmpPt.x = tmppointer1[i].x/enlargeFactor;
				m_tmpPt.y = tmppointer1[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);	
			}
			tmp_num = 360 - (ea>sa?ea:sa);
			for (int i=0;i<tmp_num;i++)
			{
				m_tmpPt.x = tmppointer[i].x/enlargeFactor;
				m_tmpPt.y = tmppointer[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);	
			}	
		}
		else
		{
			tmp_num = 360-(ea>sa?ea:sa);
			for (int i=tmp_num;i>=0;i--)
			{
				m_tmpPt.x = tmppointer[i].x/enlargeFactor;
				m_tmpPt.y = tmppointer[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);	
			}
			tmp_num = ea<sa?ea:sa;
			for (int i=tmp_num;i>=0;i--)
			{
				m_tmpPt.x = tmppointer1[i].x/enlargeFactor;
				m_tmpPt.y = tmppointer1[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);	
			}
		}

	}	
	m_contourPt.push_back(lastPt);	
	delete []tmppointer;
	tmppointer = NULL;
	delete []tmppointer1;
	tmppointer1 = NULL;
}

#endif
/************************************************************************/
/*								图像操作结束                             */
/************************************************************************/



/************************************************************************/
/*								其它操作开始                             */
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

	// wvsprintf不支持浮点格式，所以换成_vstprintf
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
	double m_nMax;
	m_nMax=RAND_MAX;
	srand((unsigned)time(0));
	return (int)((double)rand()*(double)(max-min)/m_nMax+(double)min);
}
void WaitProcess(LPTSTR FileName,LPTSTR Param,BOOL waitExit)
{
	SHELLEXECUTEINFO ShellInfo = {0};
	ShellInfo.cbSize	= sizeof(SHELLEXECUTEINFO);
	ShellInfo.fMask	= SEE_MASK_NOCLOSEPROCESS;
	ShellInfo.hwnd	= 0;
	ShellInfo.lpVerb	= 0;
	ShellInfo.lpFile	= FileName;     /////传入cmd.exe路径
	ShellInfo.lpParameters	= Param;    //////传入   /c  copy ....
	ShellInfo.lpDirectory	= 0;
	ShellInfo.nShow	= SW_HIDE;
	ShellInfo.hInstApp	= 0;

	ShellExecuteEx(&ShellInfo);
	//WaitForSingleObject(ShellInfo.hProcess,-1);
	DWORD dwExitCode;
	GetExitCodeProcess(ShellInfo.hProcess,&dwExitCode);
	if(waitExit){
		while (dwExitCode == STILL_ACTIVE)
		{ 
			XWaitTime(0.1);
			GetExitCodeProcess(ShellInfo.hProcess, &dwExitCode);
		}
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
	PROCESSENTRY32 ProcessInfo;//声明进程信息变量
	ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小
	//返回系统中第一个进程的信息
	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	while(Status)
	{
		HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,TRUE,ProcessInfo.th32ProcessID);
		if(hProcess)
		{
			SetProcessWorkingSetSize(hProcess,0,0);
			//内存整理
			EmptyWorkingSet(hProcess);
			CloseHandle(hProcess);
		}
		printf("%s/n",ProcessInfo.szExeFile);
		//获取下一个进程的信息
		Status=Process32Next(SnapShot,&ProcessInfo);
	}
	return TRUE;
}
#ifdef USE_STL
vector<CString> GetresByStlRx(CString word,CString rule)
{
	//http://blog.csdn.net/flyfish1986/article/details/5877838语法参考
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
bool CheckInRect(CPoint point,CRect rect)
{
	if(point.x > rect.left && point.x < rect.right
		&& point.y > rect.top && point.y < rect.bottom)
	{
		return true;
	}
	return false;
}
//截屏
void CutScreen(CDC *pDstDc)
{
	if (pDstDc == NULL)
	{
		return;
	}
	CRect SRect;
	GetClientRect(pDstDc->GetWindow()->m_hWnd,&SRect);
	CString strFile;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	strFile.Format("%04d%02d%02d%02d%02d%02d.bmp",sysTime.wYear,sysTime.wMonth,sysTime.wDay,sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	CFileDialog dlg(FALSE, _T("*.*"), strFile,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,_T("image files (*.bmp; *.jpg;*.png) |*.bmp; *.jpg;*.png| All Files (*.*)|*.*||"), NULL);                        // 选项图片的约定
	dlg.m_ofn.lpstrTitle = _T("Save Image");    // 打开文件对话框的标题名
	if( dlg.DoModal() != IDOK )                    // 判断是否获得图片
		return;
	XWaitTime(0.5);
	CString strImgPath = dlg.GetPathName();
	int BitPerPixel = pDstDc->GetDeviceCaps(BITSPIXEL);//获得颜色模式
	int Width = SRect.Width();
	int Height = SRect.Height();

	CDC memDC;//内存DC
	memDC.CreateCompatibleDC(pDstDc);

	CBitmap memBitmap, *oldmemBitmap;//建立和屏幕兼容的bitmap
	memBitmap.CreateCompatibleBitmap(pDstDc, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC
	memDC.BitBlt(0, 0, Width, Height, pDstDc, 0, 0, SRCCOPY);//复制屏幕图像到内存DC

	//以下代码保存memDC中的位图到文件
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//获得位图信息

	FILE *fp = fopen(strImgPath, "w+b");

	BITMAPINFOHEADER bih = {0};//位图信息头
	bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//高度
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
	bih.biWidth = bmp.bmWidth;//宽度

	BITMAPFILEHEADER bfh = {0};//位图文件头
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据

	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
		(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据

	delete [] p;

	fclose(fp);

	memDC.SelectObject(oldmemBitmap);
}

BOOL DrawTransparentBitmap(HDC hdcDest,int nXOriginDest, int nYOriginDest, int nWidthDest, int nHeightDest, HDC hdcSrc,
	int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent)
{
	if (nWidthDest < 1) 
		return false; 
	if (nWidthSrc < 1) 
		return false; 
	if (nHeightDest < 1) 
		return false; 
	if (nHeightSrc < 1) 
		return false; 
	HDC dc = CreateCompatibleDC(NULL); 
	HBITMAP bitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, GetDeviceCaps(dc, BITSPIXEL), NULL); 
	if (bitmap == NULL) 
	{ 
		DeleteDC(dc); 
		return false; 
	} 
	HBITMAP oldBitmap = (HBITMAP)SelectObject(dc, bitmap); 
	if (!BitBlt(dc, 0, 0, nWidthSrc, nHeightSrc, hdcSrc, nXOriginSrc, nYOriginSrc, SRCCOPY)) 
	{ 
		SelectObject(dc, oldBitmap); 
		DeleteObject(bitmap); 
		DeleteDC(dc); 
		return false; 
	} 
	HDC maskDC = CreateCompatibleDC(NULL); 
	HBITMAP maskBitmap = CreateBitmap(nWidthSrc, nHeightSrc, 1, 1, NULL); 
	if (maskBitmap == NULL) 
	{ 
		SelectObject(dc, oldBitmap); 
		DeleteObject(bitmap); 
		DeleteDC(dc); 
		DeleteDC(maskDC); 
		return false; 
	} 
	HBITMAP oldMask = (HBITMAP)SelectObject(maskDC, maskBitmap); 
	SetBkColor(maskDC, RGB(0,0,0)); 
	SetTextColor(maskDC, RGB(255,255,255)); 
	if (!BitBlt(maskDC, 0,0,nWidthSrc,nHeightSrc,NULL,0,0,BLACKNESS)) 
	{ 
		SelectObject(maskDC, oldMask);
		DeleteObject(maskBitmap); 
		DeleteDC(maskDC); 
		SelectObject(dc, oldBitmap); 
		DeleteObject(bitmap); 
		DeleteDC(dc); 
		return false;
	} 
	SetBkColor(dc, crTransparent); 
	BitBlt(maskDC, 0,0,nWidthSrc,nHeightSrc,dc,0,0,SRCINVERT); 
	SetBkColor(dc, RGB(0,0,0)); 
	SetTextColor(dc, RGB(255,255,255)); 
	BitBlt(dc, 0,0,nWidthSrc,nHeightSrc,maskDC,0,0,SRCAND); 
	HDC newMaskDC = CreateCompatibleDC(NULL); 
	HBITMAP newMask; 
	newMask = CreateBitmap(nWidthDest, nHeightDest, 1, GetDeviceCaps(newMaskDC, BITSPIXEL), NULL); 
	if (newMask == NULL)
	{
		SelectObject(dc, oldBitmap); 
		DeleteDC(dc); 
		SelectObject(maskDC, oldMask); 
		DeleteDC(maskDC); 
		DeleteDC(newMaskDC); 
		DeleteObject(bitmap);
		DeleteObject(maskBitmap); 
		return false; 
	} 
	SetStretchBltMode(newMaskDC, COLORONCOLOR); 
	HBITMAP oldNewMask = (HBITMAP) SelectObject(newMaskDC, newMask); 
	StretchBlt(newMaskDC, 0, 0, nWidthDest, nHeightDest, maskDC, 0, 0, nWidthSrc, nHeightSrc, SRCCOPY);
	SelectObject(maskDC, oldMask);
	DeleteDC(maskDC); 
	DeleteObject(maskBitmap); 
	HDC newImageDC = CreateCompatibleDC(NULL); 
	HBITMAP newImage = CreateBitmap(nWidthDest, nHeightDest, 1, GetDeviceCaps(newMaskDC, BITSPIXEL), NULL); 
	if (newImage == NULL)
	{ 
		SelectObject(dc, oldBitmap); 
		DeleteDC(dc); 
		DeleteDC(newMaskDC);
		DeleteObject(bitmap); 
		return false; 
	} 
	HBITMAP oldNewImage = (HBITMAP)SelectObject(newImageDC, newImage); 
	StretchBlt(newImageDC, 0, 0, nWidthDest, nHeightDest, dc, 0, 0, nWidthSrc, nHeightSrc, SRCCOPY); 
	SelectObject(dc, oldBitmap); DeleteDC(dc); DeleteObject(bitmap); 
	BitBlt( hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, newMaskDC, 0, 0, SRCAND); 
	BitBlt( hdcDest, nXOriginDest, nYOriginDest, nWidthDest, nHeightDest, newImageDC, 0, 0, SRCPAINT); 
	SelectObject(newImageDC, oldNewImage); 
	DeleteDC(newImageDC); 
	SelectObject(newMaskDC, oldNewMask);
	DeleteDC(newMaskDC); 
	DeleteObject(newImage); 
	DeleteObject(newMask); 
	return true;
}

CRect GetRelRoundRect(CPoint *pt,int nPointNum)
{
	//		ASSERT(nPointNum>2);
	//找出最小外包矩形
	int	nMaxX = 0;	
	int nMinX = INT_MAX;
	int nMaxY = 0;
	int nMinY = INT_MAX;
	for (int i=0;i<nPointNum;i++)
	{
		if (nMaxX<pt[i].x)
		{
			nMaxX = pt[i].x;
		}
		if (nMinX>pt[i].x)
		{
			nMinX = pt[i].x;
		}
		if (nMaxY<pt[i].y)
		{
			nMaxY = pt[i].y;
		}
		if (nMinY>pt[i].y)
		{
			nMinY = pt[i].y;
		}
	}
	CRect BoundRect;
	BoundRect.left = nMinX;
	BoundRect.top = nMinY;
	BoundRect.right = BoundRect.left + nMaxX-nMinX;
	BoundRect.bottom = BoundRect.top + nMaxY-nMinY;
	return BoundRect;
}
CRect GetRoundRect(CPoint *pt,int nPointNum)
{
	//		ASSERT(nPointNum>2);
	//找出最小外包矩形
	int	nMaxX = 0;	
	int nMinX = INT_MAX;
	int nMaxY = 0;
	int nMinY = INT_MAX;
	for (int i=0;i<nPointNum;i++)
	{
		if (nMaxX<pt[i].x)
		{
			nMaxX = pt[i].x;
		}
		if (nMinX>pt[i].x)
		{
			nMinX = pt[i].x;
		}
		if (nMaxY<pt[i].y)
		{
			nMaxY = pt[i].y;
		}
		if (nMinY>pt[i].y)
		{
			nMinY = pt[i].y;
		}
	}
	CRect BoundRect;
	BoundRect.left = (nMinX/8)*8;
	BoundRect.top = (nMinY/8)*8;
	BoundRect.right = BoundRect.left + ((nMaxX-nMinX)/8)*8;
	BoundRect.bottom = BoundRect.top + ((nMaxY-nMinY)/8)*8;
	return BoundRect;
}
//点在多边形中
BOOL PtInPolygon(CPoint p, CPoint pt[], int nCount)
{  
	int x= p.x;
	int y = p.y;
	int   i,j=nCount-1 ;
	bool  oddNodes = FALSE;

	for (i=0;i<nCount; i++) {
		if((pt[i].y< y && pt[j].y>=y
			||   pt[j].y<y && pt[i].y>=y)
			&& (pt[i].x<=x || pt[j].x<=x)) {
				oddNodes^=(pt[i].x+(y-pt[i].y)/(pt[j].y-pt[i].y)*(pt[j].x-pt[i].x)<x);}
		j=i;}

	return oddNodes;		
} 

BOOL TranParentToChild(CWnd &cwwnd, CWnd* papa,CPoint &point,int PtType)
{
	CRect rect;
	cwwnd.GetWindowRect(&rect);
	if(PtType == 0)
		papa->ScreenToClient(rect);
	if(PtInRect(rect,point)){
		point -= rect.TopLeft();
		return TRUE;
	}	
	return FALSE;
}

CvRect ChangeRect(CRect rect)
{
	return cvRect(rect.left,rect.top,rect.Width(),rect.Height());
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


#ifdef USE_EMAIL

BOOL SendAEmail(CString sendcount, CString sendPwd, CString receiver, CString senddata,CString topic,CString filePath)
{
	char t_receiver[50];char t_sendcount[50],t_sendPwd[50];
	CSmtp smtp(  
		25,                    /*smtp端口*/  
		"smtp.163.com",        /*smtp服务器地址*/  
		LPCSTR(sendcount),    /*你的邮箱地址*/  
		LPCSTR(sendPwd),      /*邮箱密码*/  
		LPCSTR(receiver),    /*目的邮箱地址*/  
		LPCSTR(topic),       /*主题*/  
		LPCSTR(senddata)     /*邮件正文*/  
		);  
	if(filePath != ""){/*附件地址*/  
		string tmp = LPCSTR(filePath);
		smtp.AddAttachment(tmp); 
	}
	smtp.SendEmail_Ex();
	return TRUE;
}
#endif




/************************************************************************/
/*						  网络相关操作结束						        */
/************************************************************************/
//查找文件夹中所有的文件夹
void AddDirDir(CString strText,vector<CString> &vecDir,vector<CTime> &vecDirTime) {

	//    CString strText = GetFullPath(hTree,hTree->GetRootItem());  //检索列表中项目文字  
	if(strText.Right(1) != "\\")                   //从右边1开始获取从右向左nCount个字符  
		strText += _T("\\\\");  
	strText += "*";  
	//将当前目录下文件枚举并InsertItem树状显示  
	CFileFind file;                                       //定义本地文件查找  
	BOOL bContinue = file.FindFile(strText);              //查找包含字符串的文件  
	while(bContinue)  
	{  
		bContinue = file.FindNextFile();                  //查找下一个文件  
		if((/*file.IsDirectory() && */!file.IsDots()))          //找到文件为内容且不为点"."  
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
#ifdef USE_OPENCV
void cvFitPlane(const CvMat* points, float* plane)
{  
	// Estimate geometric centroid.  
	int nrows = points->rows;  
	int ncols = points->cols;  
	int type = points->type;  
	CvMat* centroid = cvCreateMat(1, ncols, type);  
	cvSet(centroid, cvScalar(0));  
	for (int c = 0; c<ncols; c++){  
		for (int r = 0; r < nrows; r++)  
		{  
			centroid->data.fl[c] += points->data.fl[ncols*r + c];  
		}  
		centroid->data.fl[c] /= nrows;  
	}  
	// Subtract geometric centroid from each point.  
	CvMat* points2 = cvCreateMat(nrows, ncols, type);  
	for (int r = 0; r<nrows; r++)  
		for (int c = 0; c<ncols; c++)  
			points2->data.fl[ncols*r + c] = points->data.fl[ncols*r + c] - centroid->data.fl[c];  
	// Evaluate SVD of covariance matrix.  
	CvMat* A = cvCreateMat(ncols, ncols, type);  
	CvMat* W = cvCreateMat(ncols, ncols, type);  
	CvMat* V = cvCreateMat(ncols, ncols, type);  
	cvGEMM(points2, points, 1, NULL, 0, A, CV_GEMM_A_T);  
	cvSVD(A, W, NULL, V, CV_SVD_V_T);  
	// Assign plane coefficients by singular vector corresponding to smallest singular value.  
	plane[ncols] = 0;  
	for (int c = 0; c<ncols; c++){  
		plane[c] = V->data.fl[ncols*(ncols - 1) + c];  
		plane[ncols] += plane[c] * centroid->data.fl[c];  
	}  
	// Release allocated resources.  
	cvReleaseMat(&centroid);  
	cvReleaseMat(&points2);  
	cvReleaseMat(&A);  
	cvReleaseMat(&W);  
	cvReleaseMat(&V);  
}  
#endif

int fitPlane3D(const roiPointDecimal3D *point, int pNum, RATIO_Plane *plane3D)
{
	double sum_xx = 0;
	double sum_xy = 0;
	double sum_yy = 0;
	double sum_xz = 0;
	double sum_yz = 0;
	double sum_x = 0;
	double sum_y = 0;
	double sum_z = 0;
	double mean_xx, mean_yy, mean_xy, mean_yz, mean_xz, mean_x, mean_y, mean_z;
	double a[4];
	double b[4];
	double c[4];
	double d[4];

	double D1, D2, D3, DD;
	int i;

	double effSize = 0;
	for (i = 0; i < pNum; i++)
	{
		sum_xx += (double)((point[i].xxx)*(point[i].xxx));
		sum_xy += (double)((point[i].xxx)*(point[i].yyy));
		sum_yy += (double)((point[i].yyy)*(point[i].yyy));
		sum_yz += (double)((point[i].yyy)*(point[i].zzz));
		sum_xz += (double)((point[i].xxx)*(point[i].zzz));
		sum_x += (double)((point[i].xxx));
		sum_y += (double)((point[i].yyy));
		sum_z += (double)((point[i].zzz));
		effSize += 1.0;
	}
	mean_xx = sum_xx / effSize;
	mean_xy = sum_xy / effSize;
	mean_yy = sum_yy / effSize;
	mean_yz = sum_yz / effSize;
	mean_xz = sum_xz / effSize;
	mean_x = sum_x / effSize;
	mean_y = sum_y / effSize;
	mean_z = sum_z / effSize;
	a[1] = sum_xx;
	a[2] = sum_xy;
	a[3] = sum_x;
	b[1] = sum_xy;
	b[2] = sum_yy;
	b[3] = sum_y;
	c[1] = sum_x;
	c[2] = sum_y;
	c[3] = effSize;
	d[1] = sum_xz;
	d[2] = sum_yz;
	d[3] = sum_z;


	D1 = (b[2] * ((d[1] * c[3]) - (c[1] * d[3]))) + (b[1] * ((c[2] * d[3]) - (d[2] * c[3]))) + (b[3] * ((d[2] * c[1]) - (c[2] * d[1])));
	D2 = (d[2] * ((a[1] * c[3]) - (c[1] * a[3]))) + (d[1] * ((c[2] * a[3]) - (a[2] * c[3]))) + (d[3] * ((a[2] * c[1]) - (c[2] * a[1])));
	D3 = (b[2] * ((a[1] * d[3]) - (d[1] * a[3]))) + (b[1] * ((d[2] * a[3]) - (a[2] * d[3]))) + (b[3] * ((a[2] * d[1]) - (d[2] * a[1])));

	DD = (b[2] * ((a[1] * c[3]) - (c[1] * a[3]))) + (b[1] * ((c[2] * a[3]) - (a[2] * c[3]))) + (b[3] * ((a[2] * c[1]) - (c[2] * a[1])));

	plane3D->r0 = D1 / DD;
	plane3D->r1 = D2 / DD;
	plane3D->r2 = D3 / DD;

	plane3D->distB = sqrt(plane3D->r0*plane3D->r0 + plane3D->r1*plane3D->r1 + 1.0);
	return 0;
}

double pointToPlaneDis3D(roiPointDecimal3D calPt, const RATIO_Plane *plane3D)
{
	return (calPt.zzz - calPt.xxx * plane3D->r0 - calPt.yyy * plane3D->r1 - plane3D->r2) / plane3D->distB;
}

BOOL fileMappingW(CString mapname,IplImage *pimg)
{
	__int64 qwFileOffset = 0;
	HANDLE hFileMapping = NULL;
	PBYTE pbFile = NULL;
	//filename.Format("Datamap%02d",i);
	int bufsize = pimg->height*pimg->widthStep * sizeof(BYTE);
	hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, 
		NULL,
		PAGE_READWRITE,
		0,bufsize,mapname);

	pbFile = (PBYTE)MapViewOfFile(hFileMapping,
		FILE_MAP_WRITE,
		(DWORD)(qwFileOffset>>32),
		(DWORD)(qwFileOffset&0xFFFFFFFF),(DWORD)bufsize);
	memcpy(pbFile,pimg->imageData,bufsize);
	return TRUE;
}
BOOL fileMappingR(CString mapname,IplImage *pimg)
{
	HANDLE m_hReceiveMap2= OpenFileMapping(FILE_MAP_READ, FALSE, mapname);

	if (m_hReceiveMap2 == NULL)
		return FALSE;//FALSE
	PBYTE m_lpbReceiveBuf2 = (LPBYTE)MapViewOfFile(m_hReceiveMap2,FILE_MAP_READ,0,0,0);
	if (m_lpbReceiveBuf2 == NULL)
	{
		CloseHandle(m_hReceiveMap2);
		m_hReceiveMap2 = NULL;
	}
	memcpy(pimg->imageData,(BYTE*)m_lpbReceiveBuf2,sizeof(BYTE)*pimg->width*pimg->height);
	if (m_lpbReceiveBuf2 != NULL)
	{
		UnmapViewOfFile(m_lpbReceiveBuf2);
		m_lpbReceiveBuf2 = NULL;
	}
	if (m_hReceiveMap2 != NULL)
	{
		CloseHandle(m_hReceiveMap2);
		m_hReceiveMap2=NULL;
	}
	return TRUE;
}


void CwTransform(double x0,double y0,double &x1,double &y1,double parms[9],int method)
{
	double dx = x0;
	double dy = y0;
	double denominator = 1;
	switch(method)
	{
	case -1:
	case 0:
		x1 =   parms[0] * dx + parms[1] * dy + parms[2];
		y1 = - parms[1] * dx + parms[0] * dy + parms[3];
		break;
	case 1:
		x1 = parms[0] * dx + parms[1] * dy + parms[2];
		y1 = parms[3] * dx + parms[4] * dy + parms[5];		
		break;
	case 2:
	case 3:
		denominator = parms[6] * dx + parms[7] * dy + parms[8];
		x1 = (parms[0] * dx + parms[1] * dy + parms[2]) / denominator;
		y1 = (parms[3] * dx + parms[4] * dy + parms[5]) / denominator;
		break;
	default:
		break;
	}
}


vector<mergedata> mergetwo(vector<mergedata> &L1,vector<mergedata> &L2)
{
	int cwi = 0; 
	int cwj = 0;
	int isize = L1.size();
	int jsize = L2.size();
	vector<mergedata> result;
	while (cwi != isize && cwj != jsize)
	{
		if(L1[cwi].m_data <= L2[cwj].m_data)
		{
			result.push_back(L1[cwi]);
			cwi++;
		}
		else
		{
			result.push_back(L2[cwj]);
			cwj++;
		}
	}
	//将剩下的元素加到结尾
	for (int i = cwi; i < L1.size();i++)
	{
		result.push_back(L1[i]);
	}
	for (int i = cwj; i < L2.size();i++)
	{
		result.push_back(L2[i]);
	}
	return result;
}
//获取最小值
double GetMin(vector<double> &srcdata)
{
	double min = INT_MAX;
	for (int i = 0; i < srcdata.size();i++)
	{
		if(srcdata[i]<min)
			min = srcdata[i];
	}
	return min;
}
//获取平均值
double GetAvg(vector<double> &srcdata)
{
	double sum = 0;
	for (int i = 0; i < srcdata.size();i++)
	{
		sum += srcdata[i];
	}
	return sum / (double)srcdata.size();
}


#if USE_RTREE == 1
void AddDataToTREE(SomeThingTree &rtree,vector<CvRect> &rect)
{
	int rectsmin[2] = {0,0};
	int rectsmax[2] = {0,0};
	for(int i=0; i<rect.size(); i++)
	{
		rectsmin[0] = rect[i].x;
		rectsmin[1] = rect[i].y;
		rectsmax[0] = rect[i].x + rect[i].width;
		rectsmax[1] = rect[i].y + rect[i].height;
		rtree.Insert(rectsmin, rectsmax, i); // Note, all values including zero are fine in this version
	}
}
vector<int> hitIndex;
bool MySearchCallback(int id, void* arg) 
{
	return false; // keep going
}
BOOL GetHitIndex(SomeThingTree &rtree,CvRect &rect)
{
	int rectsmin[2] = {0,0};
	int rectsmax[2] = {0,0};
	rectsmin[0] = rect.x;
	rectsmin[1] = rect.y;
	rectsmax[0] = rect.x + rect.width;
	rectsmax[1] = rect.y + rect.height;
	int nhits = rtree.Search(rectsmin, rectsmax, MySearchCallback, NULL);
	return nhits > 0;
}

void ReleaseTree(SomeThingTree &rtree)
{
	rtree.RemoveAll();
}
#endif


/************************************************************************/
/*                        低端操作开始                                  */
/************************************************************************/


CvPoint CwcvRectCenter(CvRect &rect)
{
	return cvPoint(rect.x + rect.width / 2,rect.y  + rect.height / 2);
}

CvPoint CwcvRectLP(CvRect &rect)
{
	return cvPoint(rect.x,rect.y);
}

CvPoint CwcvRectRB(CvRect &rect)
{
	return cvPoint(rect.x + rect.width,rect.y + rect.height);
}
BOOL IsPtInRect(CvRect &rect , CvPoint &pt)
{
	if (pt.x<rect.x || pt.x>(rect.x + rect.width)||pt.y<rect.y||pt.y>(rect.y + rect.height))
	{
		return false;
	}
	else
	{
		return true;
	}
}
/************************************************************************/
/*                        低端操作结束                                  */
/************************************************************************/
/*                        通信操作结束                                  */
/************************************************************************/

void SendAMessage2Soft(CString windowName,int nmessageType,WPARAM wParam, LPARAM lParam)
{
	//ON_MESSAGE(nmessageType,Ondealfun)	//接收消息响应
	CString str;
	str.Format(windowName);
	HWND hRecv;
	hRecv = ::FindWindow(NULL,str);
	if (hRecv != NULL)
	{
		::PostMessage(hRecv, nmessageType, wParam, lParam);
	}
}

void cwcommunity::waitAddOne(int nLine)
{
	BOOL hadPush = FALSE;
	int hadindex = -1;
	for (int i = 0; i < vecLine.size();i++)
	{
		if(nLine == vecLine[i])
		{
			hadPush = TRUE;
			hadindex = i;
			break;
		}
	}
	if(hadPush)
	{
		vecTimes[hadindex]++;
	}
	else
	{
		vecLine.push_back(nLine);
		vecTimes.push_back(1);
	}
	TRACE("开始在第%d行等\n",nLine);
}
void cwcommunity::waitdiffOne(int nLine)
{
	BOOL hadPush = FALSE;
	int hadindex = -1;
	for (int i = 0; i < vecLine.size();i++)
	{
		if(nLine == vecLine[i])
		{
			hadPush = TRUE;
			hadindex = i;
			break;
		}
	}
	if(hadPush)
	{
		vecTimes[hadindex]--;
	}
	else
	{
		vecLine.push_back(nLine);
	}
	TRACE("第%d行waitfor退出等待\n",nLine);
}

void cwcommunity::waitForSelf(HANDLE hwaitevent,int waittime,int nPos)
{
	waitAddOne(nPos);
	WaitForSingleObject(hwaitevent,waittime);
	waitdiffOne(nPos);
}
void cwcommunity::LetEventGo(HANDLE hwaitevent)
{
	ResetEvent(hwaitevent);
}

BOOL cwcommunity::WaitForIT(volatile BOOL &waitObject , BOOL isWaitForTrue,int npos,double waitTimes)
{
	waitAddOne(npos);
	BOOL res = WaitForBOOL(waitObject ,isWaitForTrue,waitTimes);
	waitdiffOne(npos);
	return res;
}

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

BOOL WaitForBOOL(volatile BOOL &waitObject , BOOL isWaitForTrue,double waitTimes)
{
	CWTimeCount tc;
	tc.Start();	
	MSG msg;
	if(isWaitForTrue){
		while(!waitObject)
		{
			Sleep(1);
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))  
			{  
				DispatchMessage(&msg);  
				TranslateMessage(&msg);  
			} 
			if (tc.GetUseTime()>waitTimes)
			{
				//return ERROR_TIME_OUT;
				return FALSE;
			}
		}
	}
	else
	{
		while(waitObject)
		{
			Sleep(1);
			MSG msg;
			if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))  
			{  
				DispatchMessage(&msg);  
				TranslateMessage(&msg);  
			} 
			if (tc.GetUseTime()>waitTimes)
			{
				//return ERROR_TIME_OUT;
				return FALSE;
			}

		}
	}
	return TRUE;
}
/************************************************************************/


/************************************************************************/
/*                        函数测试开始                                  */
/************************************************************************/

BOOL test_XFunCom()
{
	//assert(test_fileMapping());
	//assert(test_mergeSort());
	//assert(test_LoadFile());
	//assert(test_Transform());
	//assert(test_SortMatrixDis());
	//assert(test_CvTranPointerType());
	//assert(test_TimOfPICREAD());
	assert(test_rtree());
	return TRUE;
}

BOOL test_fileMapping()
{
	vector<CString> data;
	CString pathfile = GetDirPathByDialog("",NULL,1);
	IplImage* img = cvLoadImage(pathfile,0);
	BOOL createmap = TRUE;
	if(createmap == FALSE)
	{
		cvZero(img);
		fileMappingR("PICTEST",img);
		cvSaveImage("D:\\test.bmp",img);
	}
	else
	{
		fileMappingW("Datamap01",img);
	}
	
	return TRUE;
}

BOOL test_mergeSort()
{
	vector<double> sortdata;
	sortdata.push_back(0.08);
	sortdata.push_back(0.34);
	sortdata.push_back(0.52);
	sortdata.push_back(0.13);
	sortdata.push_back(0.68);
	sortdata.push_back(0.78);
	vector<double> res = cwsortMerge(sortdata,sortdata);
	assert(res[0] < res[res.size() - 1]);
	sortdata.push_back(0.88);
	vector<double> res1 = cwsortMerge(sortdata,sortdata);
	assert(res1[0] < res1[res1.size() - 1]);
	return TRUE;
}

BOOL test_Curve2Points()
{
	vector<CFloatPt> m_cur;
	//测试小角度对不对
	Curve2Points(2.6,0,0,false,0.6,0.4,m_cur,10000);
	assert(m_cur.size() == 2);

	m_cur.clear();
	Curve2Points(2.6,0,0,true,0.4,0.6,m_cur,10000);
	assert(m_cur.size() == 2);
	//测试偏移方向对不对

	m_cur.clear();
	Curve2Points(100,0,0,true,90.1,181.1,m_cur,10000);
	assert(m_cur[0].y < 100);
	assert(m_cur[m_cur.size() - 1].x < 100);

	m_cur.clear();
	Curve2Points(100,0,0,false,90.1,180.1,m_cur,10000);
	assert(m_cur[0].y < 100);
	assert(m_cur[m_cur.size() - 1].x < 100);

	m_cur.clear();
	Curve2Points(100,0,0,true,90.1,179.9,m_cur,10000);
	assert(m_cur[0].y < 100);
	assert(m_cur[m_cur.size() - 1].y < 0);

	m_cur.clear();
	Curve2Points(100,0,0,false,179.9,90.1,m_cur,10000);
	assert(m_cur[0].x < m_cur[1].x);
	return TRUE;
}

BOOL test_LoadFile()
{

	vector<CString> data;
	CString pathfile = GetDirPathByDialog("",NULL,1);
	CWTS(TIM1)
	data.reserve(10000);
	GetDataFromFile(pathfile,data);
	CWTE(TIM1,"测试加载文件耗时为%.4f\n")
	return TRUE;
}

BOOL test_Transform()
{
	double para[9] = {1,0,-50,0,1,-100,0,0,1};
	double resx = 0.0 ,resy = 0.0;
	CwTransform(100,100,resx,resy,para,1);
	assert(resx == 50);
	assert(resy == 0);
	return TRUE;
}

vector<CPoint> SortMatrixDis(int rows,int cols,int findrow,int findcol)
{
	BOOL bshowTmp = FALSE;
	CWTS(TIM1)
	vector<double> vecdis;
	vector<CPoint> resPoss;
	for (int i = 0; i < rows;i++)
	{
		for (int j = 0; j < cols;j++)
		{
			double dx = abs(i - findrow);
			double dy = abs(j - findcol);
			double dis = sqrt(dx*dx + dy*dy);
			resPoss.push_back(CPoint(i,j));
			vecdis.push_back(dis);
		}
	}
	resPoss = cwsortMerge(vecdis,resPoss);
	//cwsort(vecdis,resPoss,0,vecdis.size());
	if(bshowTmp)
	{
		CWTE(TIM1,"查找离指定位置最近的点耗时为%.4f\n");
	}
	return resPoss;
}

BOOL test_SortMatrixDis()
{
	vector<CPoint> pts = SortMatrixDis(100,100,50,50);
	assert(pts[0] == CPoint(50,50));
	return TRUE;
}

BOOL test_TimOfPICREAD()
{
	try{
 		CWTS(TIM1)
		IplImage *pimgHe = cvLoadImage("D:\\12.bmp",-1);
		if(pimgHe == NULL)
		{
			return TRUE;
		}
		double dsum = 0;
		for (int i = 0; i < pimgHe->width;i++)
		{
			for (int j = 0; j < pimgHe->height;j++)
			{
				dsum += YX_32F_IPL(pimgHe,j,i);
			}
		}		
 		IplImage *pimg = cvLoadImage("D:\\1.bmp",0);
 		for (int i = 0; i < pimg->width;i++)
 		{
 			for (int j = 0; j < pimg->height;j++)
 			{
 				dsum += int(YX_BYTE(pimg,j,i));
 			}
 		}
 		double avg = dsum/pimg->width/pimg->height;TRACE("平均值为%.4f",avg);
 		CWTE(TIM1,"第一种iplimg耗时为%.4f\n")
		
		CvMat *pmat = cvCreateMat(pimg->height,pimg->width,CV_32F);
		cv::Mat mat = cv::Mat(pimg->height,pimg->width ,CV_32F);
		cv::Mat mat1 = cv::Mat::zeros(pimg->height,pimg->width ,CV_32F);
		IplImage *pFimg = cvCreateImage(cvSize(pimg->width,pimg->height),IPL_DEPTH_32F,1);
		CWTS(TIM2)
		for (int j = 0; j < pimg->height;j++)
		{
			for (int i = 0; i < pimg->width;i++)
			{
				YX_32F_IPL(pFimg,j,i) = int(YX_BYTE(pimg,j,i))+0.1;
			}
		}
		CWTE(TIM2,"iplimage赋值耗时%.4f\n")
		cvSaveImage("D:\\2.bmp",pimg);
		


		CWTS(TIM3)
		for (int j = 0; j < pimg->height;j++)
		{
			for (int i = 0; i < pimg->width;i++)
			{
				YX_32F_MAT(mat,j,i) = int(YX_BYTE(pimg,j,i))+0.1;
			}
		}	
		CWTE(TIM3,"mat赋值耗时%.4f\n")
		imwrite("D:\\3.bmp",mat);
		IplImage img_ipl;
		CvTranPointerType(img_ipl,mat,CvMat(),FALSE,MAT_IPL);
		cvSaveImage("D:\\4.bmp",&img_ipl);
		
		CWTS(TIM4)
		for (int j = 0; j < pimg->height;j++)
		{
			for (int i = 0; i < pimg->width;i++)
			{
				//cvmSet(pmat,j,i,int(YX_BYTE(pimg,j,i))+0.1);
				YX_32F_CVMAT(pmat,j,i) = int(YX_BYTE(pimg,j,i))+0.1;
			}
		}
		CWTE(TIM4,"cvmat赋值耗时%.4f\n")
		CvTranPointerType(img_ipl,cv::Mat(),*pmat,FALSE,CVMAT_IPL);
		cvSaveImage("D:\\5.bmp",&img_ipl);	
		
		
		CWTS(TIM5)
		dsum = 0;
		for (int i = 0; i < pimg->width;i++)
		{
			for (int j = 0; j < pimg->height;j++)
			{
				dsum += YX_32F_IPL(pFimg,j,i);
			}
		}
		avg = dsum/pimg->width/pimg->height;
		TRACE("平均值为%.4f",avg);
		CWTE(TIM5,"iplimage取值耗时%.4f\n")

		CWTS(TIM5_2)
		dsum = 0;
		for (int j = 0; j < pimg->height;j++)
		{
			for (int i = 0; i < pimg->width;i++)
			{
				dsum += YX_32F_IPL(pFimg,j,i);
			}
		}
		avg = dsum/pimg->width/pimg->height;
		TRACE("平均值为%.4f",avg);
		CWTE(TIM5_2,"iplimage取值2耗时%.4f\n")

		CWTS(TIM6)
		dsum = 0;
		for (int i = 0; i < pimg->width;i++)
		{
			for (int j = 0; j < pimg->height;j++)
			{
				dsum += YX_32F_MAT(mat,j,i);//mat.at<float>(i,j);
			}
		}	
		avg = dsum/pimg->width/pimg->height;
		TRACE("平均值为%.4f",avg);
		CWTE(TIM6,"mat取值耗时%.4f\n")

		CWTS(TIM6_2)
		dsum = 0;
		
		for (int j = 0; j < mat.rows;j++)
		{
			for (int i = 0; i < mat.cols;i++)
			{
				dsum += YX_32F_MAT(mat,j,i);//mat.at<float>(i,j);
			}
		}	
		avg = dsum/pimg->width/pimg->height;
		TRACE("平均值为%.4f",avg);
		CWTE(TIM6_2,"mat取值2耗时%.4f\n")

		CWTS(TIM7)
		dsum = 0;
		for (int i = 0; i < pimg->width;i++)
		{
			for (int j = 0; j < pimg->height;j++)
			{
				dsum += YX_32F_CVMAT(pmat,j,i);
			}
		}	
		avg = dsum/pimg->width/pimg->height;
		TRACE("平均值为%.4f",avg);
		CWTE(TIM7,"cvmat取值耗时%.4f\n")

		CWTS(TIM7_2)
		dsum = 0;
		vector<roiPointDecimal3D> t3dpt;
		roiPointDecimal3D t3dptsingal;
		//t3dpt.reserve(pmat->height*pmat->width);
		for (int j = 0; j < pimg->height;j++)
		{
			for (int i = 0; i < pimg->width;i++)
			{
				//dsum += YX_32F_CVMAT(pmat,j,i);
				t3dptsingal.xxx = i;
				t3dptsingal.yyy = j;
				t3dptsingal.zzz = YX_32F_CVMAT(pmat,j,i);
				if(t3dptsingal.zzz != -100)
				{
					t3dpt.push_back(t3dptsingal);
				}
			}
		}	
		avg = dsum/pimg->width/pimg->height;
		TRACE("平均值为%.4f",avg);
		CWTE(TIM7_2,"cvmat取值2耗时%.4f\n")

		cvReleaseImage(&pimg);
		cvReleaseImage(&pFimg);
		cvReleaseMat(&pmat);

	}
	catch (cv::Exception e)
	{
		AfxMessageBox(e.err.c_str());
	}
	return TRUE;
}


BOOL CvTranPointerType(IplImage &src_ipl , cv::Mat &src_mat , CvMat &src_cvmat,BOOL bcopy,int type)
{
	if(type == IPL_MAT)
	{
		src_mat = cv::Mat(&src_ipl,bcopy);
	}
	if(type == MAT_IPL)
	{
		src_ipl = src_mat;
	}
	else if(type == IPL_CVMAT)
	{
		CvMat* mat = cvGetMat(&src_ipl,&src_cvmat);
		src_cvmat = *mat;

	}
	else if(type == CVMAT_MAT)
	{
		src_mat = cv::Mat(&src_cvmat,bcopy);
	}
	else if(type == MAT_CVMAT)
	{
		src_cvmat = src_mat;
	}

	return TRUE;
}

BOOL test_CvTranPointerType()
{
 	IplImage *pimg = cvLoadImage("D:\\1.bmp",0);
	CvMat img_cvmat;
	CvTranPointerType(*pimg,cv::Mat(),img_cvmat,FALSE,IPL_CVMAT);
	cvSaveImage("D:\\2.bmp",&img_cvmat);

	cv::Mat img_mat;
	CvTranPointerType(*pimg,img_mat,img_cvmat,FALSE,CVMAT_MAT);
	imwrite("D:\\2.bmp",img_mat);

	cvReleaseImage(&pimg);
	return TRUE;
}

BOOL test_rtree()
{
#if USE_RTREE == 1
	vector<CvRect> rects;
	rects.push_back(cvRect(0,0,60,60));
	rects.push_back(cvRect(75,0,100,25));
	rects.push_back(cvRect(25,25,75,75));
	rects.push_back(cvRect(40,40,100,100));
	rects.push_back(cvRect(0,25,25,100));
	SomeThingTree cwrtree;
	AddDataToTREE(cwrtree,rects);

	BOOL res = GetHitIndex(cwrtree,cvRect(49,49,1,1));
	ReleaseTree(cwrtree);
#endif

	return TRUE;

}

CRect CWGetWndRect(CWnd *cwwnd,int ptType /*= 0*/)
{
	CRect rect;
	cwwnd->GetWindowRect(&rect);//获取控件相对于屏幕的位置
	cwwnd->ScreenToClient(rect);
	cwwnd->GetClientRect(&rect);
	return rect;
}







/************************************************************************/
/*                        函数测试结束                                  */
/************************************************************************/
