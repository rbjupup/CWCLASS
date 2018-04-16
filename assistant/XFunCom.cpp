
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
//							ʱ�������ʼ
//////////////////////////////////////////////////////////////////////////
void CWTimeCount::Start() // ��ʱ��ʼ
{
	QueryPerformanceFrequency( &Frequency );
	QueryPerformanceCounter  ( &old );
	UseTime = 0.0;
}
void CWTimeCount::End() // ��ʱ����
{
	QueryPerformanceCounter( &Time );
	UseTime = (double) ( Time.QuadPart - old.QuadPart) / (double)Frequency.QuadPart;
	// 	TRACE("Use time = %20.10f(s)\n", UseTime );
}

double CWTimeCount::GetUseTime() // ����㷨����ʱ��(��λ:��)
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
//�ж�ʱ���Ƿ���ĳ��ʱ�����
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
		m_StFile.WriteString(vecData[i]);				//���ж�ȡ
		m_StFile.WriteString("\n");
	}
	m_StFile.WriteString("\n");
	m_StFile.Close();
	tt.End();
	TRACE("StdioFile����%d�к�ʱ��%.3f\n",m_nRows,tt.GetUseTime());
	return;
}
void GetDataFromFile(CString strFilePath,std::vector<CString>& vecData)
{
	BOOL bShowTmp = FALSE;
	if (!FileExist(strFilePath))
	{
		AfxMessageBox("��ǰָ�����ϲ�����!");
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
	while (m_StFile.ReadString(str))					//���ж�ȡ
	{
		//TRACE("%s\n",str);
		vecData.push_back(str);	
		m_nRows ++;
	}
	m_StFile.Close();
	if(bShowTmp)
	{
		tt.End();
		TRACE("StdioFile����%d�к�ʱ��%.3f\n",m_nRows,tt.GetUseTime());
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
	BOOL bSuccess = FALSE;//�ɹ���־
	// 	CreateDirectory(strDir,NULL); 
	bSuccess = CreateDirectory(strDir, NULL) ? TRUE : FALSE;
	return bSuccess;
}
//�����ļ���
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
			AfxMessageBox("�Ҳ�����ѹ���");
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
/**************************************************************************** 
��������: CString_to_char 
��������: CStringתchar
�������: str �����ַ��� ptr����ַ����� 
�������: �� 
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
/*						  �ַ������ò�������						    */
/************************************************************************/


/************************************************************************/
/*								 ͼ�������ʼ                           */
/************************************************************************/
//���λͼͷ�ļ�
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

	FillBitmapInfo1( bmi, bmp_w, bmp_h, bpp,Img->origin );

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


//enlargeFactor��ʾ���������С����λ��,����cvPoint�Ļ����Կ���ʹ��������������Ǹ�����
static void Curve2Points(double Rad, double centerx, double centery, bool ClockWise, float sa, float ea, vector<CFloatPt> &m_contourPt,double enlargeFactor)
{

	//�жϻ��ߵ�����
	int tmp_case=0;
	ClockWise = !ClockWise;
	//�Ƕȱ仯��Χ�Ƿ���һ��֮��,�ǵĻ���ֱ�����������Ϳ�����
	BOOL bdiffAngInOne = FALSE;
	if (((0<(ea - sa )&&(ea - sa )< 1) &&!ClockWise)||(0<(sa - ea )&&(sa - ea )< 1)&&ClockWise)
	{
		bdiffAngInOne = TRUE;
	}

	if(sa==360)	sa=0;
	if(ea==360) ea=0;
	//��һ������ӿ�ʼ���������ڶ�������ӿ�ʼ��360�ȣ���0������
	if (!ClockWise)
	{
		if (ea>sa)
			tmp_case = 1;
	}else
	{	
		if (ea<sa)
			tmp_case = 1;
	}

	//����opencv�Ļ�Բ�����Ὣ�Ƕ���������,����ȱ��
	//����Ҫ�ȱ�����ĩ����

	CFloatPt firstPt,lastPt;
	//firstPt.x = Rad * cos(ANG2RAD(sa)) + centerx;
	firstPt.x = centerx + TRANS2COX(Rad, 0, sa);
	firstPt.y = centery - TRANS2COX(Rad, 0, 450 -sa);

	lastPt.x = centerx + TRANS2COX(Rad, 0, ea);
	lastPt.y = centery - TRANS2COX(Rad, 0, 450 - ea);
	//����ǻ���,�ڶ�������������
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

	//����ڽǶȱ仯��1������
	if (bdiffAngInOne)
	{
		m_contourPt.push_back(firstPt);	
		m_contourPt.push_back(lastPt);	
		return;
	}

	int nRadius = Rad*enlargeFactor;
	//��ʼ�����������ת���������
	CvPoint *tmppointer = new CvPoint[361];
	CvPoint *tmppointer1 = new CvPoint[361];
	CFloatPt m_tmpPt;					//��Բ��ת����Ϊֱ�߲���adshape����
	CvPoint m_CentePoint;
	m_CentePoint.x = centerx*enlargeFactor;//̫С��ʱ�����ɴ����,Բ���ľ�������
	m_CentePoint.y = centery*enlargeFactor;
	CvSize CriSize;
	CriSize.width = CriSize.height = nRadius;
	int tmp_num;

	//���һ�Ļ��Ͱ��յ������tmmpointer���������ȣ����˾�˵��������
	//���2�Ļ��ͷ�������
	//360 - angle is trans the coordinate System
	m_contourPt.push_back(firstPt);
	if (tmp_case == 1)
	{				
		cvEllipse2Poly(m_CentePoint,CriSize,0,360-sa,360-ea,tmppointer,1);//��ʼ������һ����תһ�ȣ��������
		//��ȡ�����
		tmp_num = abs(int(sa)-int(ea));

		if(ClockWise)
		{		
			for (int i = 0;i<tmp_num;i++)
			{
				m_tmpPt.x = tmppointer[i].x/enlargeFactor;//�Ŵ�100000������С����
				m_tmpPt.y = tmppointer[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);

			}

		}
		else
		{
			for (int i=tmp_num - 1;i >= 0;i--)
			{
				m_tmpPt.x = tmppointer[i].x/enlargeFactor;//�Ŵ�100000������С����
				m_tmpPt.y = tmppointer[i].y/enlargeFactor;
				m_contourPt.push_back(m_tmpPt);
			}
		}
	}
	else if(tmp_case == 0)
	{								
		cvEllipse2Poly(m_CentePoint,CriSize,0,0,360-(ea>sa?ea:sa),tmppointer,1);//��ʼ������һ����תһ�ȣ��������
		cvEllipse2Poly(m_CentePoint,CriSize,0,360-(ea<sa?ea:sa),360,tmppointer1,1);//��ʼ������һ����תһ�ȣ��������

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
	ShellInfo.lpFile	= FileName;     /////����cmd.exe·��
	ShellInfo.lpParameters	= Param;    //////����   /c  copy ....
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
bool CheckInRect(CPoint point,CRect rect)
{
	if(point.x > rect.left && point.x < rect.right
		&& point.y > rect.top && point.y < rect.bottom)
	{
		return true;
	}
	return false;
}
//����
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
	CFileDialog dlg(FALSE, _T("*.*"), strFile,OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,_T("image files (*.bmp; *.jpg;*.png) |*.bmp; *.jpg;*.png| All Files (*.*)|*.*||"), NULL);                        // ѡ��ͼƬ��Լ��
	dlg.m_ofn.lpstrTitle = _T("Save Image");    // ���ļ��Ի���ı�����
	if( dlg.DoModal() != IDOK )                    // �ж��Ƿ���ͼƬ
		return;
	XWaitTime(0.5);
	CString strImgPath = dlg.GetPathName();
	int BitPerPixel = pDstDc->GetDeviceCaps(BITSPIXEL);//�����ɫģʽ
	int Width = SRect.Width();
	int Height = SRect.Height();

	CDC memDC;//�ڴ�DC
	memDC.CreateCompatibleDC(pDstDc);

	CBitmap memBitmap, *oldmemBitmap;//��������Ļ���ݵ�bitmap
	memBitmap.CreateCompatibleBitmap(pDstDc, Width, Height);

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC
	memDC.BitBlt(0, 0, Width, Height, pDstDc, 0, 0, SRCCOPY);//������Ļͼ���ڴ�DC

	//���´��뱣��memDC�е�λͼ���ļ�
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);//���λͼ��Ϣ

	FILE *fp = fopen(strImgPath, "w+b");

	BITMAPINFOHEADER bih = {0};//λͼ��Ϣͷ
	bih.biBitCount = bmp.bmBitsPixel;//ÿ�������ֽڴ�С
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;//�߶�
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//ͼ�����ݴ�С
	bih.biWidth = bmp.bmWidth;//���

	BITMAPFILEHEADER bfh = {0};//λͼ�ļ�ͷ
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//��λͼ���ݵ�ƫ����
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//�ļ��ܵĴ�С
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//д��λͼ�ļ�ͷ

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//д��λͼ��Ϣͷ

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//�����ڴ汣��λͼ����

	GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, Height, p, 
		(LPBITMAPINFO) &bih, DIB_RGB_COLORS);//��ȡλͼ����

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//д��λͼ����

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
	//�ҳ���С�������
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
	//�ҳ���С�������
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
//���ڶ������
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
	//��ʣ�µ�Ԫ�ؼӵ���β
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
//��ȡ��Сֵ
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
//��ȡƽ��ֵ
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
/*                        �Ͷ˲�����ʼ                                  */
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
/*                        �Ͷ˲�������                                  */
/************************************************************************/
/*                        ͨ�Ų�������                                  */
/************************************************************************/

void SendAMessage2Soft(CString windowName,int nmessageType,WPARAM wParam, LPARAM lParam)
{
	//ON_MESSAGE(nmessageType,Ondealfun)	//������Ϣ��Ӧ
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
	TRACE("��ʼ�ڵ�%d�е�\n",nLine);
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
	TRACE("��%d��waitfor�˳��ȴ�\n",nLine);
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
/*                        �������Կ�ʼ                                  */
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
	//����С�ǶȶԲ���
	Curve2Points(2.6,0,0,false,0.6,0.4,m_cur,10000);
	assert(m_cur.size() == 2);

	m_cur.clear();
	Curve2Points(2.6,0,0,true,0.4,0.6,m_cur,10000);
	assert(m_cur.size() == 2);
	//����ƫ�Ʒ���Բ���

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
	CWTE(TIM1,"���Լ����ļ���ʱΪ%.4f\n")
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
		CWTE(TIM1,"������ָ��λ������ĵ��ʱΪ%.4f\n");
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
 		double avg = dsum/pimg->width/pimg->height;TRACE("ƽ��ֵΪ%.4f",avg);
 		CWTE(TIM1,"��һ��iplimg��ʱΪ%.4f\n")
		
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
		CWTE(TIM2,"iplimage��ֵ��ʱ%.4f\n")
		cvSaveImage("D:\\2.bmp",pimg);
		


		CWTS(TIM3)
		for (int j = 0; j < pimg->height;j++)
		{
			for (int i = 0; i < pimg->width;i++)
			{
				YX_32F_MAT(mat,j,i) = int(YX_BYTE(pimg,j,i))+0.1;
			}
		}	
		CWTE(TIM3,"mat��ֵ��ʱ%.4f\n")
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
		CWTE(TIM4,"cvmat��ֵ��ʱ%.4f\n")
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
		TRACE("ƽ��ֵΪ%.4f",avg);
		CWTE(TIM5,"iplimageȡֵ��ʱ%.4f\n")

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
		TRACE("ƽ��ֵΪ%.4f",avg);
		CWTE(TIM5_2,"iplimageȡֵ2��ʱ%.4f\n")

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
		TRACE("ƽ��ֵΪ%.4f",avg);
		CWTE(TIM6,"matȡֵ��ʱ%.4f\n")

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
		TRACE("ƽ��ֵΪ%.4f",avg);
		CWTE(TIM6_2,"matȡֵ2��ʱ%.4f\n")

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
		TRACE("ƽ��ֵΪ%.4f",avg);
		CWTE(TIM7,"cvmatȡֵ��ʱ%.4f\n")

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
		TRACE("ƽ��ֵΪ%.4f",avg);
		CWTE(TIM7_2,"cvmatȡֵ2��ʱ%.4f\n")

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
	cwwnd->GetWindowRect(&rect);//��ȡ�ؼ��������Ļ��λ��
	cwwnd->ScreenToClient(rect);
	cwwnd->GetClientRect(&rect);
	return rect;
}







/************************************************************************/
/*                        �������Խ���                                  */
/************************************************************************/
