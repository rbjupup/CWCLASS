#pragma once
class CWListen
{
public:
	CWListen(void);
	~CWListen(void);

public:
	CString Listen();
	HANDLE InitListen();
	void Start();
	void End();
};

