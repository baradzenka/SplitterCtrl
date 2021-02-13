//==========================================================
// Author: Baradzenka Aleh (baradzenka@gmail.com)
//==========================================================
// 
#pragma once
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
class SplitterCtrl : public CWnd
{	DECLARE_DYNCREATE(SplitterCtrl)

///////////////////////////////////////
// PUBLIC
///////////////////////////////////////
public:
	struct Draw
	{	virtual void DrawBegin(SplitterCtrl const * /*ctrl*/, CDC * /*dc*/) const {}
		virtual void DrawSplitter(SplitterCtrl const * /*ctrl*/, CDC * /*dc*/, bool /*horz*/, int /*idx*/, CRect const * /*rect*/) const {}
		virtual void DrawDragRect(SplitterCtrl const * /*ctrl*/, CDC * /*dc*/, bool /*horz*/, bool /*firstTime*/, CRect const * /*rectOld*/, CRect const * /*rectNew*/) const {}
		virtual void DrawBorder(SplitterCtrl const * /*ctrl*/, CDC * /*dc*/, CRect const * /*rect*/) const {}
		virtual void DrawEnd(SplitterCtrl const * /*ctrl*/, CDC * /*dc*/) const {}
	};
	interface IRecalc
	{	virtual int GetBorderWidth(SplitterCtrl const *ctrl, IRecalc const *base) const = 0;
		virtual int GetVertSplitterWidth(SplitterCtrl const *ctrl, IRecalc const *base) const = 0;   // width of vertical splitter.
		virtual int GetHorzSplitterHeight(SplitterCtrl const *ctrl, IRecalc const *base) const = 0;   // height of horizontal splitter.
	};

public:
	SplitterCtrl();
	~SplitterCtrl();

public:
	bool Create(CWnd *parentWnd, DWORD style, RECT const &rect, UINT id);
	bool AddRow();
	bool AddColumn();
	bool InsertRow(int row);   // insert before row.
	bool InsertColumn(int col);   // insert before col.
	void DeleteRow(int row);
	void DeleteColumn(int col);
	void DeleteAll();
	void Update();   // recalculate control.
		// 
	void SetDrawManager(Draw *p/*or null*/);
	Draw *GetDrawManager();
	void SetRecalcManager(IRecalc *p/*or null*/);   // NULL for default manager.
	IRecalc *GetRecalcManager();
		// 
	void SetCursors(HCURSOR horz, HCURSOR vert, HCURSOR cross);
	void SetCursors(HMODULE module, UINT horz, UINT vert, UINT cross);
		// 
	void SetWindow(int row, int col, HWND hWnd);
	HWND GetWindow(int row, int col) const;
		// 
	void SetColumnWidthForStatic(int col, int width);   // only for ResizeStatic and ResizeStaticFull.
	void SetRowHeightForStatic(int row, int height);   // only for ResizeStatic and ResizeStaticFull.
		// 
	void SetColumnWidthForDynamic(int col, float percent);   // for ResizeDynamic (for sure use Update() after each call).
	void SetColumnWidthsForDynamic(int const *percent/*in*/);   // set width for every column (number items in 'percent'==GetNumberColumn()).
	void SetRowHeightForDynamic(int row, float percent);   // for ResizeDynamic (for sure use Update() after each call).
	void SetRowHeightsForDynamic(int const *percent/*in*/);   // set height for every row (number items in 'percent'==GetNumberRow()).
		// 
	void SetRowsEqualHeight();
	void SetColumnsEqualWidth();
		// 
	void SetWindowMinWidth(int width);   // min width of every window.
	int GetWindowMinWidth() const;
	void SetWindowMinHeight(int height);   // min height of every window.
	int GetHeightMinWindow() const;
		// 
	int GetNumberRow() const;
	int GetNumberColumn() const;
	void HitTest(CPoint point, int *horz/*out,or null*/, int *vert/*out,or null*/) const;   // searching splitter in point.
	RECT GetWindowRect(int row, int col) const;
	RECT GetSplitterRect(bool horz, int idx) const;
		// 
	enum Snap { SnapLeftTop, SnapLeftBottom, SnapRightTop, SnapRightBottom };
	void SetSnapMode(Snap mode);
	Snap GetSnapMode() const;
		// 
	enum Resize { ResizeStatic, ResizeStaticFull, ResizeDynamic };
	void SetResizeMode(Resize mode);
	Resize GetResizeMode() const;
		// 
	enum Dragging { DraggingStatic, DraggingDynamic };
	void SetDraggingMode(Dragging mode);
	Dragging GetDraggingMode() const;
		// 
	bool IsDragging() const;
	void GetDraggingState(bool *dragHorz/*out,or null*/, bool *dragVert/*out,or null*/, bool *dragCross/*out,or null*/) const;
	void CancelDragging();
		// 
	void ActivateHorzSplitter(int idx, bool active);
	bool IsHorzSplitterActive(int idx) const;
	void ActivateVertSplitter(int idx, bool active);
	bool IsVertSplitterActive(int idx) const;
		// 
	void ShowBorder(bool show);
	bool IsBorderVisible() const;
		// 
	int GetBorderWidth() const;
	int GetVertSplitterWidth() const;
	int GetHorzSplitterHeight() const;
		// 
	bool LoadState(CWinApp *app, TCHAR const *section, TCHAR const *entry);   // load state from registry.
	bool SaveState(CWinApp *app, TCHAR const *section, TCHAR const *entry) const;   // save state in registry.
	void LoadState(CArchive *ar);
	void SaveState(CArchive *ar) const;

///////////////////////////////////////
// PRIVATE
///////////////////////////////////////
private:
	class Private;
	Private &p;

///////////////////////////////////////
// PROTECTED
///////////////////////////////////////
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL Create(LPCTSTR className, LPCTSTR windowName, DWORD style, const RECT &rect, CWnd *parentWnd, UINT id, CCreateContext *context = NULL);
	afx_msg void OnDestroy();
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS *lpncsp);
	afx_msg BOOL OnSetCursor(CWnd *pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
struct SplitterCtrlStyle1 : 
	SplitterCtrl::Draw,
	SplitterCtrl::IRecalc
{
	void Install(SplitterCtrl *ctrl);

protected:
		// SplitterCtrl::Draw.
	virtual void DrawSplitter(SplitterCtrl const *ctrl, CDC *dc, bool horz, int idx, CRect const *rect) const;
	virtual void DrawDragRect(SplitterCtrl const *ctrl, CDC *dc, bool horz, bool firstTime, CRect const *rectOld, CRect const *rectNew) const;
	virtual void DrawBorder(SplitterCtrl const *ctrl, CDC *dc, CRect const *rect) const;
	virtual void DrawEnd(SplitterCtrl const *ctrl, CDC *dc) const;

		// SplitterCtrl::IRecalc.
	virtual int GetBorderWidth(SplitterCtrl const *ctrl, IRecalc const *base) const;
	virtual int GetVertSplitterWidth(SplitterCtrl const *ctrl, IRecalc const *base) const;
	virtual int GetHorzSplitterHeight(SplitterCtrl const *ctrl, IRecalc const *base) const;

	virtual COLORREF GetBackgroundColor() const { return ::GetSysColor(COLOR_WINDOW); }
	virtual bool IsInnerBorderVisible() const { return true; }
	virtual COLORREF GetInnerBorderColor() const { return ::GetSysColor(COLOR_BTNSHADOW); }
	virtual COLORREF GetOuterBorderColor() const { return ::GetSysColor(COLOR_BTNSHADOW); }
	virtual bool IsDotsVisible() const { return true; }
	virtual COLORREF GetDotsColor() const { return ::GetSysColor(COLOR_BTNSHADOW); }
	virtual CBrush *GetDragBrush() const { return NULL; }

	static void DrawSplitterDots(CDC *dc, CRect const *rect, bool horz, int number, int size, COLORREF color);
};
// 
class SplitterCtrlStyle2 : public SplitterCtrlStyle1
{	virtual bool IsDotsVisible() const { return false; }
};
/////////////////////////////////////////////////////////////////////////////
// 
class SplitterCtrlStyle3 : public SplitterCtrlStyle1
{	virtual COLORREF GetBackgroundColor() const { return ::GetSysColor(COLOR_BTNFACE); }
	virtual bool IsInnerBorderVisible() const { return false; }
};
// 
class SplitterCtrlStyle4 : public SplitterCtrlStyle3
{	virtual bool IsDotsVisible() const { return false; }
};
/////////////////////////////////////////////////////////////////////////////
// 
class SplitterCtrlStyle5 : public SplitterCtrlStyle1
{		// SplitterCtrl::IRecalc.
	virtual int GetVertSplitterWidth(SplitterCtrl const * /*ctrl*/, IRecalc const * /*base*/) const { return 6; }
	virtual int GetHorzSplitterHeight(SplitterCtrl const * /*ctrl*/, IRecalc const * /*base*/) const { return 6; }

	virtual COLORREF GetBackgroundColor() const { return RGB(45,64,94); }
	virtual bool IsInnerBorderVisible() const { return false; }
	virtual COLORREF GetOuterBorderColor() const { return RGB(45,64,94); }
	virtual COLORREF GetDotsColor() const { return RGB(206,212,223); }
	virtual CBrush *GetDragBrush() const { static CBrush br(RGB(128,128,128)); return &br; }
};
// 
class SplitterCtrlStyle6 : public SplitterCtrlStyle5
{	virtual bool IsDotsVisible() const { return false; }
};
/////////////////////////////////////////////////////////////////////////////
// 
class SplitterCtrlStyle7 : public SplitterCtrlStyle1
{		// SplitterCtrl::Draw.
	virtual void DrawSplitter(SplitterCtrl const *ctrl, CDC *dc, bool horz, int idx, CRect const *rect) const;
	virtual void DrawEnd(SplitterCtrl const *ctrl, CDC *dc) const;

	virtual bool IsInnerBorderVisible() const { return false; }
	virtual COLORREF GetOuterBorderColor() const { return RGB(77,115,61); }
	virtual bool IsDotsVisible() const { return false; }

	static void DrawGradient(CDC *dc, CRect const *rc, bool horz, COLORREF clrTop, COLORREF clrBottom);
};
/////////////////////////////////////////////////////////////////////////////
// 
class SplitterCtrlStyle8 : public SplitterCtrlStyle1
{		// SplitterCtrl::IRecalc.
	virtual int GetBorderWidth(SplitterCtrl const * /*ctrl*/, IRecalc const * /*base*/) const { return 2; }

		// SplitterCtrl::Draw.
	virtual void DrawSplitter(SplitterCtrl const *ctrl, CDC *dc, bool horz, int idx, CRect const *rect) const;
	virtual void DrawBorder(SplitterCtrl const *ctrl, CDC *dc, CRect const *rect) const;
	virtual void DrawEnd(SplitterCtrl const *ctrl, CDC *dc) const;

	virtual bool IsInnerBorderVisible() const { return false; }
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
template<typename STYLE>
struct SplitterCtrlEx : SplitterCtrl
{	SplitterCtrlEx()
	{	style.Install(this);
	}
	STYLE style;
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

















