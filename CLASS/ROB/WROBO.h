#pragma once
#include "WTalk.h"
#include "WListen.h"
#include "TuLing.h"
class CWROBO
{
public:
	CWROBO(void);
	~CWROBO(void);

public:
	enum sex{WOMAN,MAN}m_sex;
	BOOL TranslateTextToWav(CString text,CString savePath,int soundType);
	BOOL Talk(CString text);
	CString StartListen();
	void PIKAFreeStyle();
	void ChatTuLing();
private:
	CWTalk m_mouse;
	CWListen m_ear;
	CTuLing m_brain;
};
