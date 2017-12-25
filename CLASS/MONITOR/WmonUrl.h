#pragma once
#include <vector>
using namespace std;
class CWmonUrl
{
public:
	CWmonUrl(void);
	~CWmonUrl(void);
public:
	CString GetPackage(CString urlpath);
	CString OpenOnMyDialog(CString urlpath);
	//这个函数通过设置头和尾来对html进行查找，查找结果会返回
	vector<CString> GetDataByUrl(CString urlpath,vector<CString> filterFrontData,vector<CString> filterBackData,BOOL showWeb,
		int nNumOfLessRes,int dMaxMillTime,vector<int> watchData,vector<double> valueOfWatch,CString msgSendToAdd);
};

