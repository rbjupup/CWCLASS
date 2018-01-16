#include "StdAfx.h"
#include "CLASSTEST.h"
#include "CLASS\BCG\BCGdlg.h"
#include "afxdialogex.h"


// ABCGDlg 对话框

IMPLEMENT_DYNAMIC(ABCGDlg, CDialog)

	ABCGDlg::ABCGDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ABCGDlg::IDD, pParent)
{
}

ABCGDlg::~ABCGDlg()
{
}

void ABCGDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
#ifdef BCG
	DDX_Control(pDX, IDC_CHART, m_wndChart);
#endif
}


BEGIN_MESSAGE_MAP(ABCGDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_START, &ABCGDlg::OnBnClickedButtonStart)
END_MESSAGE_MAP()


// ABCGDlg 消息处理程序

BOOL ABCGDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
#ifdef BCG
	CBCGPVisualManager2010::SetStyle (CBCGPVisualManager2010::VS2010_Black);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2010));
	EnableVisualManagerStyle();
	// 异常: OCX 属性页应返回 FALSE

	m_arSeries[0] = NULL;

	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	CBCGPColor tmpcolor;
	CBCGPChartTheme theme;
	theme.InitChartColors(theme,CBCGPColor::Black,CBCGPColor::White,CBCGPColor::Green,CBCGPColor::Green,CBCGPColor::Black);
	theme.m_brAxisInterlaceColor = CBCGPBrush(CBCGPColor::Black);
	tmpcolor.SetRGB(RGB(50,100,50));
	theme.m_brAxisMajorGridLineColor = CBCGPBrush(tmpcolor);
	theme.m_brAxisMinorGridLineColor = CBCGPBrush(tmpcolor);
	theme.m_brAxisLineColor = CBCGPBrush(CBCGPColor::Green);
	pChart->SetColors(theme);


	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_X_PRIMARY_AXIS, FALSE);

	pChart->SetChartType (BCGPChartHistoricalLine, BCGP_CT_SIMPLE, TRUE);
	pChart->SetLegendPosition(BCGPChartLayout::LP_RIGHT);

	CBCGPChartAxis* pXAxis = pChart->GetChartAxis(BCGP_CHART_X_PRIMARY_AXIS);
	ASSERT_VALID(pXAxis);

	pXAxis->m_axisLabelType = CBCGPChartAxis::ALT_NO_LABELS;
	pXAxis->m_majorTickMarkType = CBCGPChartAxis::TMT_NO_TICKS;
	pXAxis->SetFixedIntervalWidth(20, 1);
	pXAxis->SetAxisOffsets(0,0);

	CBCGPChartAxis* pYAxis = pChart->GetChartAxis(BCGP_CHART_Y_PRIMARY_AXIS);
	ASSERT_VALID(pYAxis);
	pYAxis->SetFixedIntervalWidth(20, 1);
	pYAxis->SetAxisOffsets(0,0);
	pYAxis->Reset();
	pYAxis->SetFixedDisplayRange(0, 100,10);
	pYAxis->m_bDisplayAxisName = FALSE;

	pChart->ShowAxisIntervalInterlacing(BCGP_CHART_Y_PRIMARY_AXIS);
	CString strLabel;
	strLabel.Format(_T("CycleTime"));
	tmpcolor.SetRGB(RGB(0,255,0));
	if(m_arSeries[0] == NULL)
		m_arSeries[0] = pChart->CreateSeries(strLabel,tmpcolor);
	m_arSeries[0]->SetSeriesLineColor(tmpcolor);
	pChart->SetDirty();
	pChart->Redraw();


#endif

//波形图绘制End


	return TRUE;  // return TRUE unless you set the focus to a control
}


void ABCGDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	CBCGPChartExampleView::OnTimer(nIDEvent);

	
#ifdef BCG
	CBCGPChartVisualObject* pChart = m_wndChart.GetChart();
	ASSERT_VALID(pChart);

	for (int i = 0; i < 1; i++)
	{
		const double d = 100 ;
		const double dblDelta = rand()%(int)d;

		m_arSeries[i]->AddDataPoint(dblDelta);
	}

	pChart->SetDirty();
	pChart->Redraw();
#endif

	CDialog::OnTimer(nIDEvent);
}


void ABCGDlg::OnBnClickedButtonStart()
{
	SetTimer(0, 100, NULL);
#ifdef BCG
	m_wndChart.SetFocus();
#endif
}
