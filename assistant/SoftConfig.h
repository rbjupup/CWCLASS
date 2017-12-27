#pragma once
//////////////////////////////////////////////////////////////////////////
//这个类是用来保存一些软件上面的配置信息
//////////////////////////////////////////////////////////////////////////
#define NUM_DDATA 1
#define DDATA_DATA 0
#define NUM_NDATA 1
#define NUM_BDATA 1
#define B_SHOWPIC 0
struct Cddata{
	double data;
	CString m_dataName;
};
struct Cndata{
	int data;
	CString m_dataName;
};
struct Cbdata{
	BOOL data;
	CString m_dataName;
};
class CSoftConfig
{
public:
#ifdef NUM_DDATA
	Cddata m_ddata[NUM_DDATA];
#endif
#ifdef NUM_NDATA
	Cndata m_ndata[NUM_NDATA];
#endif
#ifdef NUM_BDATA
	Cbdata m_bdata[NUM_BDATA];
#endif
	BOOL saveConfig();
	BOOL readConfig();
	CString m_strSvePath;
	CSoftConfig(void);
	~CSoftConfig(void);
};

