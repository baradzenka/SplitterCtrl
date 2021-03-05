/////////////////////////////////////////////////////////////////////////////
#pragma once
/////////////////////////////////////////////////////////////////////////////
#include "SplitterCtrl/SplitterCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// 
class ChildWnd : public CWnd
{public:
	bool Create(TCHAR const *name, CWnd *pParentWnd, UINT uID, COLORREF clrBackColor);

private:
	COLORREF m_clrBackColor;

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
class DemoDlg : public CDialog
{	SplitterCtrl m_Splitter;
	SplitterCtrlStyle1 m_SplitterStyle1;
	SplitterCtrlStyle2 m_SplitterStyle2;
	SplitterCtrlStyle3 m_SplitterStyle3;
	SplitterCtrlStyle4 m_SplitterStyle4;
	SplitterCtrlStyle5 m_SplitterStyle5;
	SplitterCtrlStyle6 m_SplitterStyle6;
	SplitterCtrlStyle7 m_SplitterStyle7;
	SplitterCtrlStyle8 m_SplitterStyle8;
	ChildWnd m_ChildWnd[9];

private:
	void SetSplitterCtrlPos();
	void SetButtonCheck(int id, bool check) const;
	bool GetButtonCheck(int id) const;
	void EnableControl(int id, bool enable) const;

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBut1();
	afx_msg void OnBnClickedBut2();
	afx_msg void OnBnClickedBut3();
	afx_msg void OnBnClickedBut4();
	afx_msg void OnBnClickedBut5();
	afx_msg void OnBnClickedBut6();
	afx_msg void OnBnClickedBut7();
	afx_msg void OnBnClickedBut8();
	afx_msg void OnBnClickedBut11();
	afx_msg void OnBnClickedBut12();
	afx_msg void OnBnClickedBut13();
	afx_msg void OnBnClickedBut21();
	afx_msg void OnBnClickedBut22();
	afx_msg void OnBnClickedBut31();
	afx_msg void OnBnClickedBut32();
	afx_msg void OnBnClickedBut33();
	afx_msg void OnBnClickedBut34();
	afx_msg void OnBnClickedBut41();
	afx_msg void OnBnClickedBut42();
	afx_msg void OnBnClickedBut43();
	afx_msg void OnBnClickedBut51();
	afx_msg void OnBnClickedBut52();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
