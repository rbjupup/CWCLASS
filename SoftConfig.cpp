#include "StdAfx.h"
#include "SoftConfig.h"
#include "YCIni.h"

CSoftConfig::CSoftConfig(void)
{
	m_strSvePath = "SoftFile\\软件配置.ini";
#ifdef NUM_NDATA
	for (int i = 0;i < NUM_NDATA;i++)
	{
		m_ndata[i].data = 0;
		m_ndata[i].m_dataName = "未定义";
	}
#endif
#ifdef NUM_DDATA
	for (int i = 0;i < NUM_DDATA;i++)
	{
		m_ddata[i].data = 0;
		m_ddata[i].m_dataName = "未定义";
	}
#endif
#ifdef NUM_BDATA
	for (int i = 0;i < NUM_BDATA;i++)
	{
		m_bdata[i].data = 0;
		m_bdata[i].m_dataName = "未定义";
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
	//将各个参数保存下来
	CString paraIndex;
	CString paraValue;
	CString ParaIntroduce;
#ifdef NUM_NDATA
	for (int i = 0;i < NUM_NDATA;i++)
	{
		paraIndex.Format("[参数%d]",i);
		m_ini.SetValue(paraIndex,"参数值设置 = ",m_ndata[i].data);
		m_ini.SetValue(paraIndex,"参数值介绍 = ",m_ndata[i].m_dataName);
	}
#endif
#ifdef NUM_DDATA
	for (int i = 0;i < NUM_DDATA;i++)
	{
		paraIndex.Format("[参数%d]",i + NUM_NDATA);
		m_ini.SetValue(paraIndex,"参数值设置 = ",m_ddata[i].data);
		m_ini.SetValue(paraIndex,"参数值介绍 = ",m_ddata[i].m_dataName);
	}
#endif
#ifdef NUM_BDATA
	for (int i = 0;i < NUM_BDATA;i++)
	{
		paraIndex.Format("[参数%d]",i + NUM_NDATA + NUM_DDATA);
		m_ini.SetValue(paraIndex,"参数值设置 = ",m_bdata[i].data);
		m_ini.SetValue(paraIndex,"参数值介绍 = ",m_bdata[i].m_dataName);
	}
#endif
	m_ini.Write(m_strSvePath);
	return TRUE;
}

BOOL CSoftConfig::readConfig()
{
	YCIni m_ini;
	m_ini.Read(m_strSvePath);
	//将各个参数保存下来
	CString paraIndex;
	CString paraValue;
	CString ParaIntroduce;
#ifdef NUM_NDATA
	for (int i = 0;i < NUM_NDATA;i++)
	{
		paraIndex.Format("[参数%d]",i);
		m_ini.GetValue(paraIndex,"参数值设置 = ",m_ndata[i].data);
		m_ini.GetValue(paraIndex,"参数值介绍 = ",m_ndata[i].m_dataName);
	}
#endif
#ifdef NUM_DDATA
	for (int i = 0;i < NUM_DDATA;i++)
	{
		paraIndex.Format("[参数%d]",i + NUM_NDATA);
		m_ini.GetValue(paraIndex,"参数值设置 = ",m_ddata[i].data);
		m_ini.GetValue(paraIndex,"参数值介绍 = ",m_ddata[i].m_dataName);
	}
#endif
#ifdef NUM_BDATA
	for (int i = 0;i < NUM_BDATA;i++)
	{
		paraIndex.Format("[参数%d]",i + NUM_NDATA + NUM_DDATA);
		m_ini.GetValue(paraIndex,"参数值设置 = ",m_bdata[i].data);
		m_ini.GetValue(paraIndex,"参数值介绍 = ",m_bdata[i].m_dataName);
	}
#endif
	m_ini.Write(m_strSvePath);
	return TRUE;
}
