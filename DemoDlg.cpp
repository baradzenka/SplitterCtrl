/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DemoDlg.h"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ChildWnd.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(ChildWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
bool ChildWnd::Create(TCHAR const *name, CWnd *pParentWnd, UINT uID, COLORREF clrBackColor)
{	m_clrBackColor = clrBackColor;
	return CWnd::Create(NULL,name,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),pParentWnd,uID,NULL)!=0;
}
/////////////////////////////////////////////////////////////////////////////
// 
void ChildWnd::OnPaint()
{	CPaintDC dc(this);
		// 
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc,m_clrBackColor);
		// 
	CString text, str;
	GetWindowText(text);
	str.Format(_T("\nWindow\n%s\n%d x %d"), text.GetString(),rc.Width(),rc.Height());
		// 
	HFONT hFontOld = static_cast<HFONT>( ::SelectObject(dc.m_hDC,::GetStockObject(DEFAULT_GUI_FONT)) );
	dc.DrawText(str,rc,DT_CENTER);
	::SelectObject(dc.m_hDC,hFontOld);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// DemoDlg.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(DemoDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUT1, OnBnClickedBut1)
	ON_BN_CLICKED(IDC_BUT2, OnBnClickedBut2)
	ON_BN_CLICKED(IDC_BUT3, OnBnClickedBut3)
	ON_BN_CLICKED(IDC_BUT4, OnBnClickedBut4)
	ON_BN_CLICKED(IDC_BUT5, OnBnClickedBut5)
	ON_BN_CLICKED(IDC_BUT6, OnBnClickedBut6)
	ON_BN_CLICKED(IDC_BUT7, OnBnClickedBut7)
	ON_BN_CLICKED(IDC_BUT8, OnBnClickedBut8)
	ON_BN_CLICKED(IDC_BUT11, OnBnClickedBut11)
	ON_BN_CLICKED(IDC_BUT12, OnBnClickedBut12)
	ON_BN_CLICKED(IDC_BUT13, OnBnClickedBut13)
	ON_BN_CLICKED(IDC_BUT21, OnBnClickedBut21)
	ON_BN_CLICKED(IDC_BUT22, OnBnClickedBut22)
	ON_BN_CLICKED(IDC_BUT31, OnBnClickedBut31)
	ON_BN_CLICKED(IDC_BUT32, OnBnClickedBut32)
	ON_BN_CLICKED(IDC_BUT33, OnBnClickedBut33)
	ON_BN_CLICKED(IDC_BUT34, OnBnClickedBut34)
	ON_BN_CLICKED(IDC_BUT41, OnBnClickedBut41)
	ON_BN_CLICKED(IDC_BUT42, OnBnClickedBut42)
	ON_BN_CLICKED(IDC_BUT43, OnBnClickedBut43)
	ON_BN_CLICKED(IDC_BUT51, OnBnClickedBut51)
	ON_BN_CLICKED(IDC_BUT52, OnBnClickedBut52)
	ON_BN_CLICKED(IDC_BUT61, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUT62, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUT63, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUT64, OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUT65, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUT66, OnBnClickedButton6)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
int DemoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	if(CDialog::OnCreate(lpCreateStruct)==-1)
		return -1;
	ModifyStyle(0,WS_CLIPCHILDREN);
		// 
	if(!m_Splitter.Create(this,WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),150))
		return -1;
		// 
	if(!m_ChildWnd[0].Create(_T("[0,0]"),&m_Splitter,200,RGB(138,168,228)) ||
		!m_ChildWnd[1].Create(_T("[0,1]"),&m_Splitter,201,RGB(138,168,228)) ||
		!m_ChildWnd[2].Create(_T("[0,2]"),&m_Splitter,202,RGB(138,168,228)) ||
		!m_ChildWnd[3].Create(_T("[1,0]"),&m_Splitter,203,RGB(138,168,228)) ||
		!m_ChildWnd[4].Create(_T("[1,1]"),&m_Splitter,204,RGB(193,201,151)) ||
		!m_ChildWnd[5].Create(_T("[1,2]"),&m_Splitter,205,RGB(193,201,151)) ||
		!m_ChildWnd[6].Create(_T("[2,0]"),&m_Splitter,206,RGB(138,168,228)) ||
		!m_ChildWnd[7].Create(_T("[2,1]"),&m_Splitter,207,RGB(193,201,151)) ||
		!m_ChildWnd[8].Create(_T("[2,2]"),&m_Splitter,208,RGB(246,176,120)))
		return -1;
		// 
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 
BOOL DemoDlg::OnInitDialog()
{	CDialog::OnInitDialog();
		// 
	SetSplitterCtrlPos();
		// 
	m_Splitter.AddRow();
	m_Splitter.AddRow();
	m_Splitter.AddRow();
	m_Splitter.AddColumn();
	m_Splitter.AddColumn();
	m_Splitter.AddColumn();
		// 
	m_Splitter.SetWindow(0,0,m_ChildWnd[0].m_hWnd);
	m_Splitter.SetWindow(0,1,m_ChildWnd[1].m_hWnd);
	m_Splitter.SetWindow(0,2,m_ChildWnd[2].m_hWnd);
	m_Splitter.SetWindow(1,0,m_ChildWnd[3].m_hWnd);
	m_Splitter.SetWindow(1,1,m_ChildWnd[4].m_hWnd);
	m_Splitter.SetWindow(1,2,m_ChildWnd[5].m_hWnd);
	m_Splitter.SetWindow(2,0,m_ChildWnd[6].m_hWnd);
	m_Splitter.SetWindow(2,1,m_ChildWnd[7].m_hWnd);
	m_Splitter.SetWindow(2,2,m_ChildWnd[8].m_hWnd);
		// 
	if( !m_Splitter.LoadState(AfxGetApp(),_T("SplitterCtrl_DemoDlg"),_T("SplitterState")) )
	{	m_Splitter.Update();
		m_Splitter.SetColumnsEqualWidth();
		m_Splitter.SetRowsEqualHeight();
	}
		// 
	m_SplitterStyle2.Install(&m_Splitter);
	m_Splitter.Update();
		// 
		// 
		// 
	SetButtonCheck(IDC_BUT2,true);
		// 
	SetButtonCheck(IDC_BUT11,true);
	SetButtonCheck(IDC_BUT12,true);
	SetButtonCheck(IDC_BUT13,true);
		// 
	SetButtonCheck(IDC_BUT21,true);
		// 
	SetButtonCheck(IDC_BUT31,true);
	SetButtonCheck(IDC_BUT41,true);
	SetButtonCheck(IDC_BUT51,true);
		// 
	EnableControl(IDC_BUT65,false);
	EnableControl(IDC_BUT66,false);
		// 
		// 
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::OnDestroy()
{	m_Splitter.SaveState( AfxGetApp(), _T("SplitterCtrl_DemoDlg"),_T("SplitterState"));
		// 
	CDialog::OnDestroy();
}
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::OnGetMinMaxInfo(MINMAXINFO *lpMMI)
{	CDialog::OnGetMinMaxInfo(lpMMI);
		// 
	CWnd *baseWnd = GetDlgItem(IDC_SPLITTERCTRL_BASE);
	if(baseWnd)
	{	CRect rcBase;
		baseWnd->GetWindowRect(&rcBase/*out*/);
		ScreenToClient(&rcBase);
			// 
		CRect rc(CPoint(0,0),CSize(rcBase.left+rcBase.top+2,rcBase.top+rcBase.top+2));
		CalcWindowRect(&rc);
		lpMMI->ptMinTrackSize.x = rc.Width();
		lpMMI->ptMinTrackSize.y = rc.Height();
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::OnSize(UINT nType, int cx, int cy)
{	SetSplitterCtrlPos();
		// 
	CDialog::OnSize(nType, cx, cy);
}
// 
void DemoDlg::SetSplitterCtrlPos()
{	CWnd *baseWnd = GetDlgItem(IDC_SPLITTERCTRL_BASE);
		// 
	if(baseWnd)
	{	CRect rcBase;
		baseWnd->GetWindowRect(&rcBase/*out*/);
		ScreenToClient(&rcBase);
			// 
		CRect rc;
		GetClientRect(&rc/*out*/);
		rc.DeflateRect(rcBase.left,rcBase.top,rcBase.top,rcBase.top);
		m_Splitter.MoveWindow(&rc);
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void DemoDlg::SetButtonCheck(int id, bool check) const 
{	reinterpret_cast<CButton*>( GetDlgItem(id) )->SetCheck(check ? BST_CHECKED : BST_UNCHECKED);
}
bool DemoDlg::GetButtonCheck(int id) const
{	return reinterpret_cast<CButton*>( GetDlgItem(id) )->GetCheck() == BST_CHECKED;
}
// 
void DemoDlg::EnableControl(int id, bool enable) const
{	CWnd *wnd = GetDlgItem(id);
	enable ? wnd->ModifyStyle(WS_DISABLED,0) : wnd->ModifyStyle(0,WS_DISABLED);
	wnd->Invalidate();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Style: Custom1.
// 
void DemoDlg::OnBnClickedBut1()
{	m_SplitterStyle1.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom2.
// 
void DemoDlg::OnBnClickedBut2()
{	m_SplitterStyle2.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom3.
// 
void DemoDlg::OnBnClickedBut3()
{	m_SplitterStyle3.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom4.
// 
void DemoDlg::OnBnClickedBut4()
{	m_SplitterStyle4.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom5.
// 
void DemoDlg::OnBnClickedBut5()
{	m_SplitterStyle5.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom6.
// 
void DemoDlg::OnBnClickedBut6()
{	m_SplitterStyle6.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom7.
// 
void DemoDlg::OnBnClickedBut7()
{	m_SplitterStyle7.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
// Style: Custom8.
// 
void DemoDlg::OnBnClickedBut8()
{	m_SplitterStyle8.Install(&m_Splitter);
	m_Splitter.Update();
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);   // border update.
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Show border.
// 
void DemoDlg::OnBnClickedBut11()
{	m_Splitter.ShowBorder( GetButtonCheck(IDC_BUT11) );
	m_Splitter.SetWindowPos(NULL, 0,0,0,0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);	// border update.
}
/////////////////////////////////////////////////////////////////////////////
// Active horizontal splitters.
// 
void DemoDlg::OnBnClickedBut12()
{	for(int r=0; r<m_Splitter.GetNumberRow()-1; ++r)
		m_Splitter.ActivateHorzSplitter(r, GetButtonCheck(IDC_BUT12) );
}
/////////////////////////////////////////////////////////////////////////////
// Active vertical splitters.
// 
void DemoDlg::OnBnClickedBut13()
{	for(int c=0; c<m_Splitter.GetNumberColumn()-1; ++c)
		m_Splitter.ActivateVertSplitter(c, GetButtonCheck(IDC_BUT13) );
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Cursors: System.
// 
void DemoDlg::OnBnClickedBut21()
{	m_Splitter.SetCursors(::LoadCursor(NULL,IDC_SIZENS),::LoadCursor(NULL,IDC_SIZEWE),::LoadCursor(NULL,IDC_SIZEALL));
}
/////////////////////////////////////////////////////////////////////////////
// Cursors: User.
// 
void DemoDlg::OnBnClickedBut22()
{	m_Splitter.SetCursors( ::AfxGetResourceHandle(), IDC_CURSOR1,IDC_CURSOR2,IDC_CURSOR3);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Snap: Left-Top.
// 
void DemoDlg::OnBnClickedBut31()
{	m_Splitter.SetSnapMode(SplitterCtrl::SnapLeftTop);
	m_Splitter.Update();
	m_Splitter.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}
/////////////////////////////////////////////////////////////////////////////
// Snap: Left-Bottom.
// 
void DemoDlg::OnBnClickedBut32()
{	m_Splitter.SetSnapMode(SplitterCtrl::SnapLeftBottom);
	m_Splitter.Update();
	m_Splitter.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}
/////////////////////////////////////////////////////////////////////////////
// Snap: Right-Top.
// 
void DemoDlg::OnBnClickedBut33()
{	m_Splitter.SetSnapMode(SplitterCtrl::SnapRightTop);
	m_Splitter.Update();
	m_Splitter.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}
/////////////////////////////////////////////////////////////////////////////
// Snap: Right-Bottom.
// 
void DemoDlg::OnBnClickedBut34()
{	m_Splitter.SetSnapMode(SplitterCtrl::SnapRightBottom);
	m_Splitter.Update();
	m_Splitter.RedrawWindow(NULL,NULL,RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Resize: Static.
// 
void DemoDlg::OnBnClickedBut41()
{	m_Splitter.SetResizeMode(SplitterCtrl::ResizeStatic);
	m_Splitter.Update();
		// 
	EnableControl(IDC_BUT65,false);
	EnableControl(IDC_BUT66,false);
}
/////////////////////////////////////////////////////////////////////////////
// Resize: Static full.
// 
void DemoDlg::OnBnClickedBut42()
{	m_Splitter.SetResizeMode(SplitterCtrl::ResizeStaticFull);
	m_Splitter.Update();
		// 
	EnableControl(IDC_BUT65,false);
	EnableControl(IDC_BUT66,false);
}
/////////////////////////////////////////////////////////////////////////////
// Resize: Dynamic.
// 
void DemoDlg::OnBnClickedBut43()
{	m_Splitter.SetResizeMode(SplitterCtrl::ResizeDynamic);
	m_Splitter.Update();
		// 
	EnableControl(IDC_BUT65,true);
	EnableControl(IDC_BUT66,true);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Dragging: Static.
// 
void DemoDlg::OnBnClickedBut51()
{	m_Splitter.SetDraggingMode(SplitterCtrl::DraggingStatic);
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Dragging: Dynamic.
// 
void DemoDlg::OnBnClickedBut52()
{	m_Splitter.SetDraggingMode(SplitterCtrl::DraggingDynamic);
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Set min sizes: 0x0 px.
// 
void DemoDlg::OnBnClickedButton1()
{	m_Splitter.SetWindowMinWidth(0);
	m_Splitter.SetWindowMinHeight(0);
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Set min sizes: 20x40 px.
// 
void DemoDlg::OnBnClickedButton2()
{	m_Splitter.SetWindowMinWidth(20);
	m_Splitter.SetWindowMinHeight(40);
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Set equal widths.
// 
void DemoDlg::OnBnClickedButton3()
{	m_Splitter.SetColumnsEqualWidth();
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Set equal heights.
// 
void DemoDlg::OnBnClickedButton4()
{	m_Splitter.SetRowsEqualHeight();
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Set widths: 20%|60%|20%.
// 
void DemoDlg::OnBnClickedButton5()
{	const int parts[] = {20,60,20};
	m_Splitter.SetColumnWidthsForDynamic(parts);
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
// Set heights: 10%|30%|60%.
// 
void DemoDlg::OnBnClickedButton6()
{	const int parts[] = {10,30,60};
	m_Splitter.SetRowHeightsForDynamic(parts);
	m_Splitter.Update();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////




