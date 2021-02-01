/////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////////
#include "SplitterCtrl/SplitterCtrl.h"
#include "DemoDlg.h"
/////////////////////////////////////////////////////////////////////////////
// 
class CellDialog : public CDialog
{	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedButton1();
};
/////////////////////////////////////////////////////////////////////////////
// 
class CMainFrame : public CFrameWnd
{public:
	CMainFrame();
	virtual ~CMainFrame();

protected: 
	DECLARE_DYNAMIC(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
		// 
	SplitterCtrlEx<SplitterCtrlStyle1> m_Splitter;
	SplitterCtrlEx<SplitterCtrlStyle1> m_Splitter00, m_Splitter10;
	SplitterCtrlEx<SplitterCtrlStyle1> m_Splitter00_01, m_Splitter10_01;
	CTreeCtrl m_Tree00_00, m_Tree10_00;
	CEdit m_Edit00_01_00, m_Edit00_01_10;
	CListCtrl m_List10_01_00, m_List10_01_10, m_List10_01_20;
	CellDialog m_Dlg10_02;
		// 
	DemoDlg m_DemoDlg;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnUpdateShowDemoDlg(CCmdUI *pCmdUI);
	afx_msg void OnShowDemoDlg();
	afx_msg void OnUpdateResizeStatic(CCmdUI *pCmdUI);
	afx_msg void OnResizeStatic();
	afx_msg void OnUpdateResizeStaticfull(CCmdUI *pCmdUI);
	afx_msg void OnResizeStaticfull();
	afx_msg void OnUpdateResizeDynamic(CCmdUI *pCmdUI);
	afx_msg void OnResizeDynamic();
	afx_msg void OnUpdateDraggingStatic(CCmdUI *pCmdUI);
	afx_msg void OnDraggingStatic();
	afx_msg void OnUpdateDraggingDynamic(CCmdUI *pCmdUI);
	afx_msg void OnDraggingDynamic();
	afx_msg void OnUpdateSetEqualWidths(CCmdUI *pCmdUI);
	afx_msg void OnSetEqualWidths();
	afx_msg void OnUpdateSetEqualHeights(CCmdUI *pCmdUI);
	afx_msg void OnSetEqualHeights();
};


