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
	//�������ͨ������ͷ��β����html���в��ң����ҽ���᷵��
	vector<CString> GetDataByUrl(CString urlpath,vector<CString> filterFrontData,vector<CString> filterBackData,BOOL showWeb,
		int nNumOfLessRes,int dMaxMillTime,vector<int> watchData,vector<double> valueOfWatch,CString msgSendToAdd);
};

