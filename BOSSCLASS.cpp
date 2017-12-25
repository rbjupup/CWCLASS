#include "StdAfx.h"
#include "BOSSCLASS.h"
#include "mmsystem.h"
#include "CLASSTEST.h"
#include "YCIni.h"
BOSSCLASS::BOSSCLASS(void)
{
//	m_vecSupportClass.push_back(CLASSNAME1);
//	m_vecSupportClass.push_back(CLASSNAME2);
//	m_vecSupportClass.push_back(CLASSNAME3);
	m_vecSupportClass.push_back(CLASSNAME4);
//	m_vecSupportClass.push_back(CLASSNAME5);
	m_vecSupportClass.push_back(CLASSNAME6);
	m_vecSupportClass.push_back(CLASSNAME7);
	m_vecSupportClass.push_back(CLASSNAME8);

	vector<CString> WZNCalibreateSupportFunction;
	WZNCalibreateSupportFunction.push_back(CALFUN1);
	WZNCalibreateSupportFunction.push_back(CALFUN2);
	WZNCalibreateSupportFunction.push_back(CALFUN3);
	WZNCalibreateSupportFunction.push_back(CALFUN4);
	WZNCalibreateSupportFunction.push_back(CALFUN5);
	WZNCalibreateSupportFunction.push_back(CALFUN6);
//	m_vecSupportFunction.push_back(WZNCalibreateSupportFunction);

	vector<CString> SocketClassSupportFunction;
	SocketClassSupportFunction.push_back(SOCKETFUN1);
	SocketClassSupportFunction.push_back(SOCKETFUN2);
	SocketClassSupportFunction.push_back(SOCKETFUN3);
	SocketClassSupportFunction.push_back(SOCKETFUN4);
//	m_vecSupportFunction.push_back(SocketClassSupportFunction);	
	
	
	vector<CString> ComClassSupportFunction;
	ComClassSupportFunction.push_back(COMFUN1);
	ComClassSupportFunction.push_back(COMFUN2);
//	m_vecSupportFunction.push_back(ComClassSupportFunction);

	vector<CString> BCGClassSupportFunction;
	BCGClassSupportFunction.push_back(BCGFUN1);
	BCGClassSupportFunction.push_back(BCGFUN2);
	m_vecSupportFunction.push_back(BCGClassSupportFunction);

	vector<CString> CAMClassSupportFunction;
	CAMClassSupportFunction.push_back(CAMFUN1);
	CAMClassSupportFunction.push_back(CAMFUN2);
//	m_vecSupportFunction.push_back(CAMClassSupportFunction);

	vector<CString> ROBOClassSupportFunction;
	ROBOClassSupportFunction.push_back(ROBOFUN1);
	ROBOClassSupportFunction.push_back(ROBOFUN2);
	ROBOClassSupportFunction.push_back(ROBOFUN3);
	ROBOClassSupportFunction.push_back(ROBOFUN4);
	ROBOClassSupportFunction.push_back(ROBOFUN5);
	m_vecSupportFunction.push_back(ROBOClassSupportFunction);


	vector<CString> MFCBTNClassSupportFunction;
	MFCBTNClassSupportFunction.push_back(MFCBTNFUN1);
	m_vecSupportFunction.push_back(MFCBTNClassSupportFunction);

	vector<CString> MFCMONClassSupportFunction;
	MFCMONClassSupportFunction.push_back(MONFUN1);
	MFCMONClassSupportFunction.push_back(MONFUN2);
	MFCMONClassSupportFunction.push_back(MONFUN3);
	MFCMONClassSupportFunction.push_back(MONFUN4);
	m_vecSupportFunction.push_back(MFCMONClassSupportFunction);
}


BOSSCLASS::~BOSSCLASS(void)
{
}

vector<CString> BOSSCLASS::GetSupportClass()
{
	return m_vecSupportClass;
}


void GetWEBDataFromVec(CString &urlpath1, vector<CString> param, vector<CString> &filterFrontData, vector<CString> &filterBackData, vector<int> &watchData, vector<double> &valueOfWatch, CString &msgSendToAdd, BOOL &showWeb, int &nNumOfLessRes, int &dMaxMillTime)
{
	urlpath1 = param[0];
	for (int i = 0 ; i < atoi(param[1]);i++)
	{
		filterFrontData.push_back(param[2+i]);
		filterBackData.push_back(param[2+atoi(param[1])+i]);
	}
	for (int i = 0 ; i < atoi(param[2+2*atoi(param[1])]);i++)
	{
		watchData.push_back(atoi(param[3+2*atoi(param[1]) + i]));
		valueOfWatch.push_back(atoi(param[3+2*atoi(param[1]) + atoi(param[2+2*atoi(param[1])]) + i]));
	}
	msgSendToAdd = param[3+2*atoi(param[1]) + 2*atoi(param[2+2*atoi(param[1])])];
	showWeb = atoi(param[4+2*atoi(param[1]) + 2*atoi(param[2+2*atoi(param[1])])]);
	nNumOfLessRes = atoi(param[5+2*atoi(param[1]) + 2*atoi(param[2+2*atoi(param[1])])]);
	dMaxMillTime = atoi(param[6+2*atoi(param[1]) + 2*atoi(param[2+2*atoi(param[1])])]);
}

bool BOSSCLASS::CallFunction( CString classname,CString funname ,vector<CString> param)
{
	if (classname == CLASSNAME4)
	{
		if (funname == BCGFUN1){
			m_cwBcg.openABCGdlg();
		}
		if (funname == BCGFUN2){
			AfxMessageBox(HELPBCGFUN2);
			m_cwBcg.openABCGdlg();
		}
	}

	if (classname == CLASSNAME6)
	{
		if (funname == ROBOFUN1){
			if (param.size()<3){
				AfxMessageBox("输入参数太少，请重新输入");
				return false;
			}
			m_robo.TranslateTextToWav(param[0],param[1],atoi(param[2]));
			PlaySound(param[1],NULL, SND_ASYNC|SND_NODEFAULT );
		}
		if (funname == ROBOFUN2){
			if (param.size()<1){
				AfxMessageBox("输入参数太少，请重新输入");
				return false;
			}
			m_robo.Talk(param[0]);
		}
		if (funname == ROBOFUN3){
			AfxMessageBox(m_robo.StartListen());
		}
		if (funname == ROBOFUN4){
			for (int i = 0 ;i < 10; i++)
			{
				m_robo.PIKAFreeStyle();
			}
		}
		if (funname == ROBOFUN5){
				m_robo.ChatTuLing();
		}
	}
	if (classname == CLASSNAME7)
	{
		if (funname == MFCBTNFUN1){
			m_cwmfcBtn.DoModal();
		}
	}
	if (classname == CLASSNAME8)
	{
		if (funname == MONFUN1){
			CString res = m_monitor.GetPackage(CString("https://m.ctrip.com/webapp/hotel/hoteldetail/669353.html?atime=20171217&days=1&shareId=NjY5MzUzfGhvdGVsX2lubGFuZF9kZXRhaWx8MXwyMDE3MTIxNzExMTc0MQ%3D%3D%0A&uid=EE8B27719B38CD16943E3605C06EA0CE&from=singlemessage"));
			theApp.m_resdlg->AddStatus(res);
		}
		if (funname == MONFUN2){
			CString res = m_monitor.OpenOnMyDialog(CString("http://hotels.ctrip.com/hotel/669353.html"));
			theApp.m_resdlg->m_strRes = "";
			theApp.m_resdlg->AddStatus(res);
		}
		if (funname == MONFUN3){
			if (param.size()<1)
				return FALSE;
			CString urlpath1;
			vector<CString> filterFrontData;
			vector<CString> filterBackData;
			vector<int> watchData;
			vector<double> valueOfWatch;
			CString msgSendToAdd;
			BOOL showWeb;
			int nNumOfLessRes;
			int dMaxMillTime;

			GetWEBDataFromVec(urlpath1, param, filterFrontData, filterBackData, watchData, valueOfWatch, msgSendToAdd, showWeb, nNumOfLessRes, dMaxMillTime);

			vector<CString> res = m_monitor.GetDataByUrl(urlpath1,filterFrontData,filterBackData,showWeb,nNumOfLessRes,dMaxMillTime,watchData,valueOfWatch,msgSendToAdd);
			theApp.m_resdlg->m_strRes = "";
			for (int i = 0; i < res.size();i++)
			{
				theApp.m_resdlg->AddStatus(res[i]);
			}
		}
		if (funname == MONFUN4){
			int Webnum = atoi(param[0]);
			vector<CString> vecIniPath;
			for (int i = 0 ; i < Webnum ; i++)
				vecIniPath.push_back(param[1+i]);
			double delayTime = atof(param[1+Webnum]);
			CString urlpath1;
			vector<CString> filterFrontData;
			vector<CString> filterBackData;
			vector<int> watchData;
			vector<double> valueOfWatch;
			CString msgSendToAdd;
			BOOL showWeb;
			int nNumOfLessRes;
			int dMaxMillTime;

			while(1){

				for (int  i = 0; i < Webnum;i++)
				{
					CString iniPath = vecIniPath[i];
					vector<CString> vecparam;
					YCIni ini;
					ini.Read(iniPath);
					int numOfInpt = 0;
					ini.GetValue("[输入数据]","输入总数",numOfInpt);
					for (int i = 0; i < numOfInpt; i++)
					{
						CString numIndex;
						numIndex.Format("%d",i+1);
						CString strdata;
						ini.GetValue("[输入数据]",numIndex,strdata);
						vecparam.push_back(strdata);
					}
					ini.Write(iniPath);
					GetWEBDataFromVec(urlpath1, vecparam, filterFrontData, filterBackData, watchData, valueOfWatch, msgSendToAdd, showWeb, nNumOfLessRes, dMaxMillTime);

					vector<CString> res = m_monitor.GetDataByUrl(urlpath1,filterFrontData,filterBackData,showWeb,nNumOfLessRes,dMaxMillTime,watchData,valueOfWatch,msgSendToAdd);
					theApp.m_resdlg->m_strRes = "";
					for (int i = 0; i < res.size();i++)
					{
						theApp.m_resdlg->AddStatus(res[i]);
					}
				}
				Sleep(delayTime);
			}
		}

	}
	return true;
}

CString BOSSCLASS::GetFunctionHelp(CString classname,CString funname){
	CString totalhelp = CString("此函数没有声明，无需输入");

	if (classname == CLASSNAME4)
	{
		if (funname == BCGFUN1)
			return HELPBCGFUN1;
	}
	if (classname == CLASSNAME6)
	{
		if (funname == ROBOFUN1)
			return HELPROBOFUN1;
		if (funname == ROBOFUN2)
			return HELPROBOFUN2;
		if (funname == ROBOFUN3)
			return HELPROBOFUN3;
		if (funname == ROBOFUN4)
			return HELPROBOFUN4;
		if (funname == ROBOFUN5)
			return HELPROBOFUN5;
	}
	if (classname == CLASSNAME7)
	{
		if (funname == MFCBTNFUN1)
			return HELPMFCBTNFUN1;
	}
	if (classname == CLASSNAME8)
	{
		if (funname == MONFUN1)
			return HELPMONBTNFUN1;
		if (funname == MONFUN2)
			return HELPMONFUN2;
		if (funname == MONFUN3)
			return HELPMONFUN3;
		if (funname == MONFUN4)
			return HELPMONFUN4;
	}
	return totalhelp;
}
vector<CString> BOSSCLASS::GetSupportFunCtion( CString ClassName )
{
	for (int i = 0 ; i < m_vecSupportClass.size(); i++)
	{
		if (m_vecSupportClass[i] == ClassName)
		{
			return m_vecSupportFunction[i];
		}
	}
	vector<CString> strvecnull;
	return strvecnull;
}
