#include "StdAfx.h"
#include "SoftConfig.h"
#include "YCIni.h"

CSoftConfig::CSoftConfig(void)
{
	m_strSvePath = "SoftFile\\�������.ini";
#ifdef NUM_NDATA
	for (int i = 0;i < NUM_NDATA;i++)
	{
		m_ndata[i].data = 0;
		m_ndata[i].m_dataName = "δ����";
	}
#endif
#ifdef NUM_DDATA
	for (int i = 0;i < NUM_DDATA;i++)
	{
		m_ddata[i].data = 0;
		m_ddata[i].m_dataName = "δ����";
	}
#endif
#ifdef NUM_BDATA
	for (int i = 0;i < NUM_BDATA;i++)
	{
		m_bdata[i].data = 0;
		m_bdata[i].m_dataName = "δ����";
	}
#endif
}


CSoftConfig::~CSoftConfig(void)
{
}

BOOL CSoftConfig::saveConfig()
{
	YCIni m_ini;
	m_ini.Read(m_strSvePath);
	//������������������
	CString paraIndex;
	CString paraValue;
	CString ParaIntroduce;
#ifdef NUM_NDATA
	for (int i = 0;i < NUM_NDATA;i++)
	{
		paraIndex.Format("[����%d]",i);
		m_ini.SetValue(paraIndex,"����ֵ���� = ",m_ndata[i].data);
		m_ini.SetValue(paraIndex,"����ֵ���� = ",m_ndata[i].m_dataName);
	}
#endif
#ifdef NUM_DDATA
	for (int i = 0;i < NUM_DDATA;i++)
	{
		paraIndex.Format("[����%d]",i + NUM_NDATA);
		m_ini.SetValue(paraIndex,"����ֵ���� = ",m_ddata[i].data);
		m_ini.SetValue(paraIndex,"����ֵ���� = ",m_ddata[i].m_dataName);
	}
#endif
#ifdef NUM_BDATA
	for (int i = 0;i < NUM_BDATA;i++)
	{
		paraIndex.Format("[����%d]",i + NUM_NDATA + NUM_DDATA);
		m_ini.SetValue(paraIndex,"����ֵ���� = ",m_bdata[i].data);
		m_ini.SetValue(paraIndex,"����ֵ���� = ",m_bdata[i].m_dataName);
	}
#endif
	m_ini.Write(m_strSvePath);
	return TRUE;
}

BOOL CSoftConfig::readConfig()
{
	YCIni m_ini;
	m_ini.Read(m_strSvePath);
	//������������������
	CString paraIndex;
	CString paraValue;
	CString ParaIntroduce;
#ifdef NUM_NDATA
	for (int i = 0;i < NUM_NDATA;i++)
	{
		paraIndex.Format("[����%d]",i);
		m_ini.GetValue(paraIndex,"����ֵ���� = ",m_ndata[i].data);
		m_ini.GetValue(paraIndex,"����ֵ���� = ",m_ndata[i].m_dataName);
	}
#endif
#ifdef NUM_DDATA
	for (int i = 0;i < NUM_DDATA;i++)
	{
		paraIndex.Format("[����%d]",i + NUM_NDATA);
		m_ini.GetValue(paraIndex,"����ֵ���� = ",m_ddata[i].data);
		m_ini.GetValue(paraIndex,"����ֵ���� = ",m_ddata[i].m_dataName);
	}
#endif
#ifdef NUM_BDATA
	for (int i = 0;i < NUM_BDATA;i++)
	{
		paraIndex.Format("[����%d]",i + NUM_NDATA + NUM_DDATA);
		m_ini.GetValue(paraIndex,"����ֵ���� = ",m_bdata[i].data);
		m_ini.GetValue(paraIndex,"����ֵ���� = ",m_bdata[i].m_dataName);
	}
#endif
	m_ini.Write(m_strSvePath);
	return TRUE;
}
