#pragma once
#include <iostream>

#define TULING_URL  "www.tuling123.com/openapi/api?key=cf561b49fb8044dd913329e6e86f217d&info=%s"
static wchar_t String[1024];

class CTuLing
{
public:
	CTuLing(void);
	~CTuLing(void);
public:
	CString ChatTuling(CString myWrod);
private:
	void LinkTuLing();
};

