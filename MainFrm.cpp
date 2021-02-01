/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CellDialog, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
void CellDialog::OnBnClickedButton1()
{	::MessageBox(m_hWnd,_T(":-)"),_T("CellDialog"),MB_OK);
}
// 
void CellDialog::OnCancel()
{	//CDialog::OnCancel();
}
// 
void CellDialog::OnOK()
{	//CDialog::OnOK();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOWDEMODIALOG, OnUpdateShowDemoDlg)
	ON_COMMAND(ID_VIEW_SHOWDEMODIALOG, OnShowDemoDlg)
	ON_UPDATE_COMMAND_UI(ID_RESIZE_STATIC, OnUpdateResizeStatic)
	ON_COMMAND(ID_RESIZE_STATIC, OnResizeStatic)
	ON_UPDATE_COMMAND_UI(ID_RESIZE_DYNAMIC, OnUpdateResizeDynamic)
	ON_COMMAND(ID_RESIZE_DYNAMIC, OnResizeDynamic)
	ON_UPDATE_COMMAND_UI(ID_RESIZE_STATICFULL, OnUpdateResizeStaticfull)
	ON_COMMAND(ID_RESIZE_STATICFULL, OnResizeStaticfull)
	ON_UPDATE_COMMAND_UI(ID_DRAGGING_STATIC, OnUpdateDraggingStatic)
	ON_COMMAND(ID_DRAGGING_STATIC, OnDraggingStatic)
	ON_UPDATE_COMMAND_UI(ID_DRAGGING_DYNAMIC, OnUpdateDraggingDynamic)
	ON_COMMAND(ID_DRAGGING_DYNAMIC, OnDraggingDynamic)
	ON_UPDATE_COMMAND_UI(ID_SET_EQUAL_WIDTHS, OnUpdateSetEqualWidths)
	ON_COMMAND(ID_SET_EQUAL_WIDTHS, OnSetEqualWidths)
	ON_UPDATE_COMMAND_UI(ID_SET_EQUAL_HEIGHTS, OnUpdateSetEqualHeights)
	ON_COMMAND(ID_SET_EQUAL_HEIGHTS, OnSetEqualHeights)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
/////////////////////////////////////////////////////////////////////////////
// 
CMainFrame::CMainFrame()
{
}
CMainFrame::~CMainFrame()
{
}
/////////////////////////////////////////////////////////////////////////////
// 
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	if(CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1;
		// 
		// 
		// Create splitters and child controls.
	if(m_Splitter.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),AFX_IDW_PANE_FIRST)==false ||
		m_Splitter00.Create(&m_Splitter,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),200)==false ||
		m_Splitter00_01.Create(&m_Splitter00,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),201)==false ||
		m_Splitter10.Create(&m_Splitter,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),202)==false ||
		m_Splitter10_01.Create(&m_Splitter10,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),203)==false)
		return -1;
		// 
	m_Splitter.SetCursors( ::AfxGetResourceHandle(), IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3);
	m_Splitter00.SetCursors( ::AfxGetResourceHandle(), IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3);
	m_Splitter00_01.SetCursors( ::AfxGetResourceHandle(), IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3);
	m_Splitter10.SetCursors( ::AfxGetResourceHandle(), IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3);
	m_Splitter10_01.SetCursors( ::AfxGetResourceHandle(), IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3);
		// 
	if(m_Tree00_00.Create(WS_CHILD | WS_VISIBLE | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,CRect(0,0,0,0),&m_Splitter00,300)==0 ||
		m_Tree10_00.Create(WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,CRect(0,0,0,0),&m_Splitter10,301)==0 ||
		m_Edit00_01_00.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),&m_Splitter00_01,302)==0 ||
		m_Edit00_01_10.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),&m_Splitter00_01,303)==0 ||
		m_List10_01_00.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_REPORT,CRect(0,0,0,0),&m_Splitter10_01,304)==0 ||
		m_List10_01_10.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_REPORT,CRect(0,0,0,0),&m_Splitter10_01,305)==0 ||
		m_List10_01_20.Create(WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | LVS_REPORT,CRect(0,0,0,0),&m_Splitter10_01,306)==0 ||
		m_Dlg10_02.Create(IDD_DIALOG,&m_Splitter10)==0)
		return -1;
		// 
	m_Splitter.AddRow();
	m_Splitter.AddRow();
	m_Splitter.AddColumn();
	m_Splitter.SetWindow(0,0,m_Splitter00.m_hWnd);
	m_Splitter.SetWindow(1,0,m_Splitter10.m_hWnd);
	m_Splitter.SetWindowMinHeight(20);
		// 
	m_Splitter00.AddRow();
	m_Splitter00.AddColumn();
	m_Splitter00.AddColumn();
	m_Splitter00.SetWindow(0,0,m_Tree00_00.m_hWnd);
	m_Splitter00.SetWindow(0,1,m_Splitter00_01.m_hWnd);
	m_Splitter00.ShowBorder(false);
	m_Splitter00.SetWindowMinWidth(40);
		// 
	m_Splitter00_01.AddRow();
	m_Splitter00_01.AddRow();
	m_Splitter00_01.AddColumn();
	m_Splitter00_01.SetWindow(0,0,m_Edit00_01_00.m_hWnd);
	m_Splitter00_01.SetWindow(1,0,m_Edit00_01_10.m_hWnd);
	m_Splitter00_01.ShowBorder(false);
		// 
	m_Splitter10.AddRow();
	m_Splitter10.AddColumn();
	m_Splitter10.AddColumn();
	m_Splitter10.AddColumn();
	m_Splitter10.SetWindow(0,0,m_Tree10_00.m_hWnd);
	m_Splitter10.SetWindow(0,1,m_Splitter10_01.m_hWnd);
	m_Splitter10.SetWindow(0,2,m_Dlg10_02.m_hWnd);
	m_Splitter10.ShowBorder(false);
		// 
	m_Splitter10_01.AddRow();
	m_Splitter10_01.AddRow();
	m_Splitter10_01.AddRow();
	m_Splitter10_01.AddColumn();
	m_Splitter10_01.SetWindow(0,0,m_List10_01_00.m_hWnd);
	m_Splitter10_01.SetWindow(1,0,m_List10_01_10.m_hWnd);
	m_Splitter10_01.SetWindow(2,0,m_List10_01_20.m_hWnd);
	m_Splitter10_01.ShowBorder(false);
		// 
		// 
		// Load state from registry.
	if( !m_Splitter.LoadState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState")) )
	{	m_Splitter.SetRowHeightForStatic(0,200);
		m_Splitter00.SetColumnWidthForStatic(0,400);
		m_Splitter10.SetColumnWidthForStatic(0,300);
		m_Splitter10.SetColumnWidthForStatic(1,200);
	}
	m_Splitter.Update();
		// 
	m_Splitter00.LoadState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState00"));
	m_Splitter10.LoadState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState10"));
	m_Splitter00.Update();
	m_Splitter10.Update();
		// 
	m_Splitter00_01.LoadState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState00_01"));
	m_Splitter10_01.LoadState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState10_01"));
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
		// 
		// 
		// Initialization of child controls (cells).
	HTREEITEM hParent = m_Tree00_00.InsertItem( _T("CTreeCtrl 00-00") );
	m_Tree00_00.InsertItem(_T("Item 1"),hParent);
	m_Tree00_00.InsertItem(_T("Item 2"),hParent);
	m_Tree00_00.InsertItem(_T("Item 3"),hParent);
	m_Tree00_00.Expand(hParent,TVE_EXPAND);
		// 
	m_Edit00_01_00.SetWindowText(_T("CEdit 00-01-00"));
	m_Edit00_01_00.SetFont(CFont::FromHandle(static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT))));
	m_Edit00_01_10.SetWindowText(_T("CEdit 00-01-10"));
	m_Edit00_01_10.SetFont(CFont::FromHandle(static_cast<HFONT>(::GetStockObject(DEFAULT_GUI_FONT))));
		// 
	hParent = m_Tree10_00.InsertItem(_T("CTreeCtrl 10-00"));
	m_Tree10_00.InsertItem(_T("Item 1"),hParent);
	m_Tree10_00.InsertItem(_T("Item 2"),hParent);
	m_Tree10_00.InsertItem(_T("Item 3"),hParent);
	m_Tree10_00.Expand(hParent,TVE_EXPAND);
		// 
	m_List10_01_00.InsertColumn(0,_T("List 10-01-00"),LVCFMT_LEFT,100);
	m_List10_01_00.InsertItem(0,_T("Item 1"));
	m_List10_01_00.InsertItem(1,_T("Item 2"));
	m_List10_01_10.InsertColumn(0,_T("List 10-01-10"),LVCFMT_LEFT,100);
	m_List10_01_10.InsertItem(0,_T("Item 1"));
	m_List10_01_10.InsertItem(1,_T("Item 2"));
	m_List10_01_20.InsertColumn(0,_T("List 10-01-20"),LVCFMT_LEFT,100);
	m_List10_01_20.InsertItem(0,_T("Item 1"));
	m_List10_01_20.InsertItem(1,_T("Item 2"));
		// 
	m_Dlg10_02.ShowWindow(SW_SHOW);
		// 
		// 
		// 
		// Create Demo Dialog.
	m_DemoDlg.Create(IDD_DEMODLG,this);
	m_DemoDlg.CenterWindow();
	m_DemoDlg.ShowWindow(SW_SHOW);
		// 
		// 
		// 
		// 
	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		return -1;      // fail to create
	}
		// 
	if(!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT)))
	{
		return -1;      // fail to create
	}
		// 
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
		// 
		// 
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 
void CMainFrame::OnDestroy()
{	m_Splitter.SaveState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState"));
	m_Splitter00.SaveState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState00"));
	m_Splitter10.SaveState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState10"));
	m_Splitter00_01.SaveState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState00_01"));
	m_Splitter10_01.SaveState( AfxGetApp(), _T("SplitterCtrl"),_T("SplitterState10_01"));
		// 
	CFrameWnd::OnDestroy();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Show Demo Dialog ...
// 
void CMainFrame::OnUpdateShowDemoDlg(CCmdUI *pCmdUI)
{	pCmdUI->Enable( !m_DemoDlg.IsWindowVisible() );
}
void CMainFrame::OnShowDemoDlg()
{	m_DemoDlg.ShowWindow(SW_SHOW);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Resize: Static.
// 
void CMainFrame::OnUpdateResizeStatic(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_Splitter.GetResizeMode()==SplitterCtrl::ResizeStatic);
}
void CMainFrame::OnResizeStatic()
{	m_Splitter.SetResizeMode(SplitterCtrl::ResizeStatic);
	m_Splitter.Update();
	m_Splitter00.SetResizeMode(SplitterCtrl::ResizeStatic);
	m_Splitter10.SetResizeMode(SplitterCtrl::ResizeStatic);
	m_Splitter00.Update();
	m_Splitter10.Update();
	m_Splitter00_01.SetResizeMode(SplitterCtrl::ResizeStatic);
	m_Splitter10_01.SetResizeMode(SplitterCtrl::ResizeStatic);
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Resize: Static full.
// 
void CMainFrame::OnUpdateResizeStaticfull(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_Splitter.GetResizeMode()==SplitterCtrl::ResizeStaticFull);
}
void CMainFrame::OnResizeStaticfull()
{	m_Splitter.SetResizeMode(SplitterCtrl::ResizeStaticFull);
	m_Splitter.Update();
	m_Splitter00.SetResizeMode(SplitterCtrl::ResizeStaticFull);
	m_Splitter10.SetResizeMode(SplitterCtrl::ResizeStaticFull);
	m_Splitter00.Update();
	m_Splitter10.Update();
	m_Splitter00_01.SetResizeMode(SplitterCtrl::ResizeStaticFull);
	m_Splitter10_01.SetResizeMode(SplitterCtrl::ResizeStaticFull);
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Resize: Dynamic.
// 
void CMainFrame::OnUpdateResizeDynamic(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_Splitter.GetResizeMode()==SplitterCtrl::ResizeDynamic);
}
void CMainFrame::OnResizeDynamic()
{	m_Splitter.SetResizeMode(SplitterCtrl::ResizeDynamic);
	m_Splitter.Update();
	m_Splitter00.SetResizeMode(SplitterCtrl::ResizeDynamic);
	m_Splitter10.SetResizeMode(SplitterCtrl::ResizeDynamic);
	m_Splitter00.Update();
	m_Splitter10.Update();
	m_Splitter00_01.SetResizeMode(SplitterCtrl::ResizeDynamic);
	m_Splitter10_01.SetResizeMode(SplitterCtrl::ResizeDynamic);
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Dragging: Static.
// 
void CMainFrame::OnUpdateDraggingStatic(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_Splitter.GetDraggingMode()==SplitterCtrl::DraggingStatic);
}
void CMainFrame::OnDraggingStatic()
{	m_Splitter.SetDraggingMode(SplitterCtrl::DraggingStatic);
	m_Splitter.Update();
	m_Splitter00.SetDraggingMode(SplitterCtrl::DraggingStatic);
	m_Splitter10.SetDraggingMode(SplitterCtrl::DraggingStatic);
	m_Splitter00.Update();
	m_Splitter10.Update();
	m_Splitter00_01.SetDraggingMode(SplitterCtrl::DraggingStatic);
	m_Splitter10_01.SetDraggingMode(SplitterCtrl::DraggingStatic);
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Dragging: Dynamic.
// 
void CMainFrame::OnUpdateDraggingDynamic(CCmdUI *pCmdUI)
{	pCmdUI->SetCheck(m_Splitter.GetDraggingMode()==SplitterCtrl::DraggingDynamic);
}
void CMainFrame::OnDraggingDynamic()
{	m_Splitter.SetDraggingMode(SplitterCtrl::DraggingDynamic);
	m_Splitter.Update();
	m_Splitter00.SetDraggingMode(SplitterCtrl::DraggingDynamic);
	m_Splitter10.SetDraggingMode(SplitterCtrl::DraggingDynamic);
	m_Splitter00.Update();
	m_Splitter10.Update();
	m_Splitter00_01.SetDraggingMode(SplitterCtrl::DraggingDynamic);
	m_Splitter10_01.SetDraggingMode(SplitterCtrl::DraggingDynamic);
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Set equal widths.
// 
void CMainFrame::OnUpdateSetEqualWidths(CCmdUI * /*pCmdUI*/)
{
}
void CMainFrame::OnSetEqualWidths()
{	m_Splitter00.SetColumnsEqualWidth();
	m_Splitter10.SetColumnsEqualWidth();
	m_Splitter00.Update();
	m_Splitter10.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Set equal heights.
// 
void CMainFrame::OnUpdateSetEqualHeights(CCmdUI * /*pCmdUI*/)
{
}
void CMainFrame::OnSetEqualHeights()
{	m_Splitter.SetRowsEqualHeight();
	m_Splitter.Update();
	m_Splitter00_01.SetRowsEqualHeight();
	m_Splitter10_01.SetRowsEqualHeight();
	m_Splitter00_01.Update();
	m_Splitter10_01.Update();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////







BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_Splitter.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_Splitter.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}







