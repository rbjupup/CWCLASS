#include "BCGdlg.h"
class CWBCG
{
public:
	CWBCG(void);
	~CWBCG(void);



//////////////////////////////////////////////////////////////////////////
//创建一个BCG对话框：流程为添加afxstd。h里面那个头文件，重定义CDialog
	//app的inis函数前面加
//#ifdef BCG
//	CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Blue);
//	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
//#endif
	//后面加,记得加blue的dll
//#ifdef BCG
//	BCGCBProCleanUp ();
//#endif
//初始化里面加上一句初始化语句就可以了
//////////////////////////////////////////////////////////////////////////
	void openABCGdlg();
};


