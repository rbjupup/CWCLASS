#include "StdAfx.h"
#include "WmonUrl.h"
#include "MyBrower.h"
//////////////////////////////////////////////////////////////////////////
//���õ�����
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



	hINet = InternetOpen("IE6.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );    //InternetOpen��ʼ��WININET.DLL



//	string url = "http://www.sina.com";          //ץ������

	if ( !hINet )

	{

		cout << "InternetOpen fail" << endl;

		return 1;

	}

	hHttpFile = InternetOpenUrl(hINet, url.c_str(), NULL, 0, 0, 0);  //����������ӵ�һ������������ϲ�����ӷ������϶�ȡ����

	if(!hHttpFile)

	{

		cout << "error open url" << endl;

		return 1;

	}



	BOOL bQuery = HttpQueryInfo(hHttpFile,

		HTTP_QUERY_CONTENT_LENGTH,

		szSizeBuffer,

		&dwLengthSizeBuffer, NULL); //�õ������ļ�����Ϣ

	if(bQuery ==false)

	{

		InternetCloseHandle(hINet); 

		cout << "error query info" << endl;

		return 3;

	}

	int FileSize=atol(szSizeBuffer);    //atol�������ַ���ת���ɳ�������

	string revData;

	revData.resize(FileSize);



	DWORD dwBytesRead;
#define USE_1
#ifdef USE_1
	BOOL bRead = InternetReadFile(hHttpFile, &revData[0], FileSize, &dwBytesRead);     //web���������InternetReadFile��ѭ�� ����ͣ�ش�Internet�϶������ݿ顣
	if(!bRead)

	{

		cout << "error to read file" << endl;

		return 4;

	}
	ofstream   out_file("duhui.txt");   

	out_file << revData;              //������ļ�
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

	out_file << urldata;              //������ļ�
#endif

	






	InternetCloseHandle(hHttpFile);   //�رվ��

	InternetCloseHandle(hINet);



	cout << "ץȡ�ɹ�!/n" << endl;

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
	return CString("��д��");
}

CString CWmonUrl::OpenOnMyDialog(CString urlpath)
{
	CMyBrower dlg;
	dlg.path = urlpath;
	dlg.m_filterFrontData.push_back("\"�ڳ�");
	dlg.m_filterFrontData.push_back("Room=");
	dlg.m_filterBackData.push_back("����");
	dlg.m_filterBackData.push_back("&amp;");
	dlg.DoModal();
	CString strres;
	strres.Format("������ɣ����ҽ���ֱ�Ϊ\r\n");
	if (dlg.m_res.size() < 2)
	{
		return "δ�ҵ��㹻������";
	}
	for (int i = 0; i < dlg.m_res[1].size();i++)
	{
		CString tmp;
		CString fangxing;
		CString strid;
		for (int j = 0; j < dlg.m_res[0].size();j++)
		{
			if (dlg.m_resNum[1][i] < dlg.m_resNum[0][j])//���ڷ����ڼ۸���棬���Լ۸����ķ��;��ǵ�ǰ���ݵķ���
			{
				fangxing = dlg.m_res[0][j];
				break;
			}
		}
		for (int j = 0; j < dlg.m_res[3].size();j++)
		{
			if (dlg.m_resNum[1][i] < dlg.m_resNum[3][j])//���ڷ����ڼ۸���棬���Լ۸����ķ��;��ǵ�ǰ���ݵķ���
			{
				strid = dlg.m_res[3][j];
				break;
			}
		}
		tmp.Format("����%d    �۸�Ϊ��%s,����Ϊ��%s,idΪ%s\r\n",i,dlg.m_res[1][i],fangxing,strid);
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
	//�������֮���ж���û�г�����Χ
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
			tmp.Format("����%d:%s,��������Ϊ%d\r\n",i,dlg.m_resTotal[i],dlg.m_resTotalType[i]);
			senddata += tmp;
		}
		CSmtp smtp(  
			25,                             /*smtp�˿�*/  
			"smtp.163.com",                 /*smtp��������ַ*/  
			LPCSTR(sendcount),    /*��������ַ*/  
			LPCSTR(sendPwd),                  /*��������*/  
			LPCSTR(receiver),    /*Ŀ�������ַ*/  
			string("����������!"),                          /*����*/  
			LPCSTR(senddata)       /*�ʼ�����*/  
			);  
		smtp.SendEmail_Ex();
	}


	return dlg.m_resTotal;
}
