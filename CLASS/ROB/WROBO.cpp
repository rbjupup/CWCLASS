#include "StdAfx.h"
#include "WROBO.h"
#include "pinyin.h"
#ifdef TEST_CLASS
#include "CLASSTEST.h"
#endif
CWROBO::CWROBO(void)
{
	m_sex = sex::MAN;
}


CWROBO::~CWROBO(void)
{
}

BOOL CWROBO::TranslateTextToWav(CString text,CString savePath,int soundType)
{
	return m_mouse.Text2WAV(text,savePath,soundType);
}

BOOL CWROBO::Talk(CString text)
{
	return m_mouse.TalkText(text,m_sex);
}

CString CWROBO::StartListen()
{
	return m_ear.Listen();
}


CString GetAnswer(CString strLis)
{
	if(strLis.Find(change2PinYin("诚心诚意"),0) != -1){
		return CString("我们就大发慈悲的告诉你！");
	}
	else if(strLis.Find(change2PinYin("世界被破坏"),0) != -1){
		return CString("为了守护世界的和平");
	}
	else if(strLis.Find(change2PinYin("邪恶"),0) != -1){
		return CString("可爱又迷人的反派角色");
	}
	else if(strLis.Find(change2PinYin("武藏"),0) != -1){
		return CString("小次郎，我们是穿梭在银河的火箭队！白洞，白色的明天在等着我们！！");
	}
	else if(strLis.Find(change2PinYin("就是这样"),0) != -1){
		return CString("喵");
	}
	return CString("识别失败");
}

void CWROBO::PIKAFreeStyle()
{
	CString strLis;
	strLis = m_ear.Listen();
	CString pinyin = change2PinYin(strLis);
	CString answer;
	answer = GetAnswer(pinyin);
	m_mouse.TalkText(answer);
	return;
}

void CWROBO::ChatTuLing()
{
	while(1){
		CString strLis;
		strLis = m_ear.Listen();

		if (strLis == "退出"||strLis == "打开听写线程失败"||strLis == "识别错误,线程未返回就退出"||strLis == "")
		{
			m_mouse.TalkText(strLis,0);
			m_mouse.TalkText(CString("再见"),0);
			return;
		}
#ifdef TEST_CLASS
			theApp.m_resdlg->AddStatus(strLis);
#endif
		CString strTuling = m_brain.ChatTuling(strLis);
#ifdef TEST_CLASS
		theApp.m_resdlg->AddStatus(strTuling);
#endif
		m_mouse.TalkText(strTuling,0);
	}
}

