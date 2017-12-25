#include "StdAfx.h"
#include "WmonUrl.h"
#include "MyBrower.h"
//////////////////////////////////////////////////////////////////////////
//引用的内容
#include<windows.h>

#include<Wininet.h>

#include<iostream>

#include<fstream>

#include<string>

#pragma comment(lib,"WinInet.lib")

#include "Smtp.h"

using namespace std;

int getUrlContent(string url)

{

	HINTERNET hINet, hHttpFile;               

	char szSizeBuffer[32];



	DWORD dwLengthSizeBuffer = sizeof(szSizeBuffer);



	hINet = InternetOpen("IE6.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );    //InternetOpen初始化WININET.DLL



//	string url = "http://www.sina.com";          //抓新浪网

	if ( !hINet )

	{

		cout << "InternetOpen fail" << endl;

		return 1;

	}

	hHttpFile = InternetOpenUrl(hINet, url.c_str(), NULL, 0, 0, 0);  //这个函数连接到一个网络服务器上并且最被从服务器上读取数据

	if(!hHttpFile)

	{

		cout << "error open url" << endl;

		return 1;

	}



	BOOL bQuery = HttpQueryInfo(hHttpFile,

		HTTP_QUERY_CONTENT_LENGTH,

		szSizeBuffer,

		&dwLengthSizeBuffer, NULL); //得到关于文件的信息

	if(bQuery ==false)

	{

		InternetCloseHandle(hINet); 

		cout << "error query info" << endl;

		return 3;

	}

	int FileSize=atol(szSizeBuffer);    //atol函数把字符串转换成长整型数

	string revData;

	revData.resize(FileSize);



	DWORD dwBytesRead;
#define USE_1
#ifdef USE_1
	BOOL bRead = InternetReadFile(hHttpFile, &revData[0], FileSize, &dwBytesRead);     //web浏览器将在InternetReadFile上循环 ，不停地从Internet上读入数据块。
	if(!bRead)

	{

		cout << "error to read file" << endl;

		return 4;

	}
	ofstream   out_file("duhui.txt");   

	out_file << revData;              //输出到文件
#else
	string urldata;
	DWORD readed = 0;
	char buffer[1025];
	do {
		ZeroMemory(buffer, 1025);
		BOOL bRead = InternetReadFile(hHttpFile, buffer, 1024, &readed);
		if(!bRead)

		{

			cout << "error to read file" << endl;

			return 4;

		}
		buffer[readed] = '\0';
		urldata += buffer;
	} while(readed);
	ofstream   out_file("duhui.txt");   

	out_file << urldata;              //输出到文件
#endif

	






	InternetCloseHandle(hHttpFile);   //关闭句柄

	InternetCloseHandle(hINet);



	cout << "抓取成功!/n" << endl;

	return 0;



}

//////////////////////////////////////////////////////////////////////////

CWmonUrl::CWmonUrl(void)
{
}


CWmonUrl::~CWmonUrl(void)
{
}

CString CWmonUrl::GetPackage(CString urlpath)
{
	getUrlContent(urlpath.GetBuffer());
	urlpath.ReleaseBuffer();
	return CString("编写中");
}

CString CWmonUrl::OpenOnMyDialog(CString urlpath)
{
	CMyBrower dlg;
	dlg.path = urlpath;
	dlg.m_filterFrontData.push_back("\"亿程");
	dlg.m_filterFrontData.push_back("Room=");
	dlg.m_filterBackData.push_back("行社");
	dlg.m_filterBackData.push_back("&amp;");
	dlg.DoModal();
	CString strres;
	strres.Format("查找完成，查找结果分别为\r\n");
	if (dlg.m_res.size() < 2)
	{
		return "未找到足够的数据";
	}
	for (int i = 0; i < dlg.m_res[1].size();i++)
	{
		CString tmp;
		CString fangxing;
		CString strid;
		for (int j = 0; j < dlg.m_res[0].size();j++)
		{
			if (dlg.m_resNum[1][i] < dlg.m_resNum[0][j])//由于房型在价格后面，所以价格后面的房型就是当前数据的房型
			{
				fangxing = dlg.m_res[0][j];
				break;
			}
		}
		for (int j = 0; j < dlg.m_res[3].size();j++)
		{
			if (dlg.m_resNum[1][i] < dlg.m_resNum[3][j])//由于房型在价格后面，所以价格后面的房型就是当前数据的房型
			{
				strid = dlg.m_res[3][j];
				break;
			}
		}
		tmp.Format("数据%d    价格为：%s,房型为：%s,id为%s\r\n",i,dlg.m_res[1][i],fangxing,strid);
		strres+=tmp;
	}
	return strres;
}

vector<CString> CWmonUrl::GetDataByUrl(CString urlpath,vector<CString> filterFrontData,vector<CString> filterBackData,BOOL showWeb,
	int nNumOfLessRes,int dMaxMillTime,vector<int> watchData,vector<double> valueOfWatch,CString receiver)
{
	CMyBrower dlg;
	dlg.path = urlpath;
	dlg.m_bshowWeb = showWeb;
	dlg.m_nNumOfLessRes = nNumOfLessRes;
	dlg.m_dMaxMillTime = dMaxMillTime;
	dlg.m_filterFrontData.clear();
	dlg.m_filterBackData.clear();
	for (int i = 0; i < filterFrontData.size();i++)
	{
		dlg.m_filterFrontData.push_back(filterFrontData[i]);
		dlg.m_filterBackData.push_back(filterBackData[i]);
	}
	dlg.DoModal();
	//获得数据之后判断有没有超出范围
	BOOL isOk = TRUE;
	for (int  i = 0 ; i < watchData.size(); i++)
	{
		if(valueOfWatch[i] > atof(dlg.m_resTotal[watchData[i]-1])){
			isOk = FALSE;
			break;
		}
	}
	if(!isOk){
		char t_receiver[50];char t_sendcount[50],t_sendPwd[50];
		CString sendcount = CString((_T("wuzenan1234@163.com")));
		CString sendPwd = CString(_T("a123456a"));
		CString senddata;
		for (int i = 0 ; i < dlg.m_resTotal.size();i++)
		{
			CString tmp;
			tmp.Format("数据%d:%s,数据类型为%d\r\n",i,dlg.m_resTotal[i],dlg.m_resTotalType[i]);
			senddata += tmp;
		}
		CSmtp smtp(  
			25,                             /*smtp端口*/  
			"smtp.163.com",                 /*smtp服务器地址*/  
			LPCSTR(sendcount),    /*你的邮箱地址*/  
			LPCSTR(sendPwd),                  /*邮箱密码*/  
			LPCSTR(receiver),    /*目的邮箱地址*/  
			string("宝宝，宝宝!"),                          /*主题*/  
			LPCSTR(senddata)       /*邮件正文*/  
			);  
		smtp.SendEmail_Ex();
	}


	return dlg.m_resTotal;
}
