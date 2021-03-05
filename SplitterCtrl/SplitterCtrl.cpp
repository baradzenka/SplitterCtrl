//==========================================================
// Author: Baradzenka Aleh (baradzenka@gmail.com)
//==========================================================
// 
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <vector>
#include <map>
#include <cassert>
#include "SplitterCtrl.h"
/////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4355)   // 'this' : used in base member initializer list.
#undef max
#undef min
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
class SplitterCtrl::Private : public IRecalc
{	template<typename T> struct Matrix
	{	Matrix()
		{	m_iRow = m_iCol = 0;
		}

	public:
		bool AddRow()
		{	try
			{	m_data.push_back( std::vector<T>(m_iCol,T()) );
			}
			catch(std::bad_alloc &)
			{	return false;
			}
			m_iRow++;
			return true;
		}
			// 
		bool AddColumn()
		{	i_data i = m_data.begin();
			try
			{	for(i_data e=m_data.end(); i!=e; ++i)
					i->push_back(T());
			}
			catch(std::bad_alloc &)
			{	for(; i!=m_data.begin(); )
					(--i)->pop_back();
				return false;
			}
			m_iCol++;
			return true;
		}
			// 
		bool InsertRow(int row)	// insert before.
		{	assert(row>=0 && row<m_iRow);
			try
			{	m_data.insert(m_data.begin()+row, std::vector<T>(m_iCol,T()) );
			}
			catch(std::bad_alloc &)
			{	return false;
			}
			m_iRow++;
			return true;
		}
			// 
		bool InsertColumn(int col)	// insert before.
		{	assert(col>=0 && col<m_iCol);
			i_data i = m_data.begin();
			try
			{	for(i_data e=m_data.end(); i!=e; ++i)
					i->insert(i->begin()+col,T());
			}
			catch(std::bad_alloc &)
			{	for(; i!=m_data.begin(); )
				{	--i;
					i->erase(i->begin()+col);
				}
				return false;
			}
			m_iCol++;
			return true;
		}
			// 
		void DeleteRow(int row)
		{	assert(row>=0 && row<m_iRow);
			m_data.erase( m_data.begin()+row );
			m_iRow--;
		}
			// 
		void DeleteColumn(int col)
		{	assert(col>=0 && col<m_iCol);
			for(i_data i=m_data.begin(), e=m_data.end(); i!=e; ++i)
				i->erase( i->begin()+col );
			m_iCol--;
		}
			// 
		void DeleteAll()
		{	m_data.clear();
			m_iRow = m_iCol = 0;
		}
			// 
		T *GetCell(int row, int col)
		{	assert(row>=0 && row<m_iRow);
			assert(col>=0 && col<m_iCol);
			return &m_data[row][col];
		}
		T const *GetCell(int row, int col) const
		{	assert(row>=0 && row<m_iRow);
			assert(col>=0 && col<m_iCol);
			return &m_data[row][col];
		}
			// 
		int GetNumberRow() const
		{	return m_iRow;
		}
		int GetNumberColumn() const
		{	return m_iCol;
		}

	private:
		typedef std::vector< std::vector<T> > dataType;
		dataType m_data;
		typedef typename dataType::iterator i_data;
		int m_iRow, m_iCol;
	};

public:
	struct VirtualWindow : CDC
	{	VirtualWindow(CWnd *wnd)
		{	assert(wnd && ::IsWindow(wnd->m_hWnd));
			pwnd = wnd;
			pdc = pwnd->BeginPaint(&ps/*out*/);
			pwnd->GetClientRect(&rect/*out*/);
			if(CreateCompatibleDC(pdc) && bitmap.CreateCompatibleBitmap(pdc,rect.Width(),rect.Height()))
			{	SelectObject(&bitmap);
				SetBkMode(TRANSPARENT);
			}
		}
		~VirtualWindow()
		{	if(bitmap.m_hObject)
				pdc->BitBlt(0,0,rect.Width(),rect.Height(), this, 0,0, SRCCOPY);
			pwnd->EndPaint(&ps);
		}

	private:
		CWnd *pwnd;
		PAINTSTRUCT ps;
		CDC *pdc;
		CRect rect;
		CBitmap bitmap;
	};

public:
	Private(SplitterCtrl &owner);
	~Private();

private:
	SplitterCtrl &o;

private:   // SplitterCtrl::IRecalc.
	int GetBorderWidth(SplitterCtrl const *ctrl, IRecalc *base) override;
	int GetVertSplitterWidth(SplitterCtrl const *ctrl, IRecalc *base) override;
	int GetHorzSplitterHeight(SplitterCtrl const *ctrl, IRecalc *base) override;

public:
	Draw *m_pDrawMngr;
	IRecalc *m_pRecalcMngr;
		// 
	Snap m_SnapMode;
	Resize m_ResizeMode;
	Dragging m_DraggingMode;
	int m_iMinWinWidth, m_iMinWinHeight;
	bool m_bShowBorder;
	HCURSOR m_hCurHorz, m_hCurVert, m_hCurCross;

public:
	struct Cell
	{	HWND wnd;
		struct { bool horz, vert; } splitterActive;
			// 
		struct State
		{	CRect rc;
			int width, height;
			struct { double width, height; } factor;
		} real, store;
	};
	Matrix<Cell> m_Matrix;
		// 
	int m_iTotalWidth, m_iTotalHeight;
		// 
	bool m_bDragHorz, m_bDragVert, m_bDragCross;
	int m_iDragHorz, m_iDragVert;
	CPoint m_ptStartDrag;
	static HHOOK m_hKeyboardHook;
	static std::pair<SplitterCtrl *,void (SplitterCtrl::*)()> m_pFuncCancelDragging;
		// 
	static const CSize m_szDefWindowSize;
	enum { HTSplitterHorz=0x0000fff0, HTSplitterVert=0x0000fff1, HTSplitterCross=0x0000fff2 };

public:
	void Recalc();
	void RecalcStatic(CRect *rc, int numberRow, int numberColumn);
	void RecalcStaticFull(CRect *rc, int numberRow, int numberColumn);
	void RecalcDynamic(CRect *rc, int numberRow, int numberColumn);
	void StopDragging(bool reset);
	void SetDraggingWidth(int col, int offset, std::map<HWND,CRect> &changedWindows/*out*/, CRect *rcdrag/*out*/);
	void SetDraggingHeight(int row, int offset, std::map<HWND,CRect> &changedWindows/*out*/, CRect *rcdrag/*out*/);
	void DrawDragRectVert(CRect const *rect);
	void DrawDragRectHorz(CRect const *rect);
	void DrawDragRectManage(CRect *rectOld, CRect const *rectNew, bool horz);
	void SetHalfAlignWidth(int col);
	void SetHalfAlignHeight(int row);
	void MoveChangedWindow(HWND wnd, CRect const *rc) const;
	Cell const *GetCell(int row, int col) const;
	Cell *GetCell(int row, int col);
	static LRESULT __stdcall KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam);
};
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
const CSize SplitterCtrl::Private::m_szDefWindowSize(100,100);
HHOOK SplitterCtrl::Private::m_hKeyboardHook = nullptr;
std::pair<SplitterCtrl *,void (SplitterCtrl::*)()> SplitterCtrl::Private::m_pFuncCancelDragging;
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(SplitterCtrl,CWnd)
/////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(SplitterCtrl, CWnd)
	ON_WM_DESTROY()
	ON_WM_NCCALCSIZE()
	ON_WM_NCPAINT()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_CAPTURECHANGED()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCLBUTTONDBLCLK()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// 
SplitterCtrl::SplitterCtrl() : 
	p( *new Private(*this) )
{
}
// 
SplitterCtrl::~SplitterCtrl()
{	delete &p;
}
/////////////////////////////////////////////////////////////////////////////
// 
SplitterCtrl::Private::Private(SplitterCtrl &owner) : o(owner)
{	m_pDrawMngr = nullptr;
	m_pRecalcMngr = this;
		// 
	m_SnapMode = SnapLeftTop;
	m_ResizeMode = ResizeStatic;
	m_DraggingMode = DraggingStatic;
	m_iMinWinWidth = m_iMinWinHeight = 0;
	m_bShowBorder = true;
	m_hCurHorz = m_hCurVert = m_hCurCross = nullptr;
}
//
SplitterCtrl::Private::~Private()
{	o.DestroyWindow();
		// 
	if(m_hCurHorz)
		::DestroyCursor(m_hCurHorz);
	if(m_hCurVert)
		::DestroyCursor(m_hCurVert);
	if(m_hCurCross)
		::DestroyCursor(m_hCurCross);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
BOOL SplitterCtrl::Create(LPCTSTR /*className*/, LPCTSTR /*windowName*/, DWORD style, const RECT &rect, CWnd *parentWnd, UINT id, CCreateContext* /*context*/)
{	return Create(parentWnd,style,rect,id);
}
// 
bool SplitterCtrl::Create(CWnd *parentWnd, DWORD style, RECT const &rect, UINT id)
{	p.m_iTotalWidth = p.m_iTotalHeight = 0;
	p.m_bDragHorz = p.m_bDragVert = p.m_bDragCross = false;
		// 
	const CString classname = AfxRegisterWndClass(CS_DBLCLKS,::LoadCursor(nullptr,IDC_ARROW),nullptr,nullptr);
	if( !CWnd::Create(classname,_T(""),style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,rect,parentWnd,id) )
		return false;
		// 
	if(!p.m_hCurHorz && !p.m_hCurVert && !p.m_hCurCross)
		SetCursors(::LoadCursor(nullptr,IDC_SIZENS),::LoadCursor(nullptr,IDC_SIZEWE),::LoadCursor(nullptr,IDC_SIZEALL));
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::OnDestroy()
{	DeleteAll();
		// 
	CWnd::OnDestroy();
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetCursors(HCURSOR horz, HCURSOR vert, HCURSOR cross)
{	if(p.m_hCurHorz)
		::DestroyCursor(p.m_hCurHorz);
	if(p.m_hCurVert)
		::DestroyCursor(p.m_hCurVert);
	if(p.m_hCurCross)
		::DestroyCursor(p.m_hCurCross);
		// 
	p.m_hCurHorz = horz;
	p.m_hCurVert = vert;
	p.m_hCurCross = cross;
}
// 
void SplitterCtrl::SetCursors(HMODULE module, UINT horz, UINT vert, UINT cross)
{	if(p.m_hCurHorz)
	{	::DestroyCursor(p.m_hCurHorz);
		p.m_hCurHorz = nullptr;
	}
	if(p.m_hCurVert)
	{	::DestroyCursor(p.m_hCurVert);
		p.m_hCurVert = nullptr;
	}
	if(p.m_hCurCross)
	{	::DestroyCursor(p.m_hCurCross);
		p.m_hCurCross = nullptr;
	}
		// 
	if(horz)
		p.m_hCurHorz = ::LoadCursor(module,MAKEINTRESOURCE(horz));
	if(vert)
		p.m_hCurVert = ::LoadCursor(module,MAKEINTRESOURCE(vert));
	if(cross)
		p.m_hCurCross = ::LoadCursor(module,MAKEINTRESOURCE(cross));
}
/////////////////////////////////////////////////////////////////////////////
// 
SplitterCtrl::Private::Cell *SplitterCtrl::Private::GetCell(int row, int col)
{	return m_Matrix.GetCell(row,col);
}
// 
SplitterCtrl::Private::Cell const *SplitterCtrl::Private::GetCell(int row, int col) const
{	return m_Matrix.GetCell(row,col);
}
/////////////////////////////////////////////////////////////////////////////
// 
bool SplitterCtrl::AddRow()
{	if( !p.m_Matrix.AddRow() )
		return false;
		// 
	const int row = GetNumberRow();
	const int col = GetNumberColumn();
		// 
	if(row==1)	// it is first addition of row.
		for(int c=0; c<col; ++c)
		{	Private::Cell *p1 = p.GetCell(0,c);
			p1->real.width = p.m_szDefWindowSize.cx;
				// 
			if(c==0)
				p1->real.factor.width = 1.0;
			else
			{	Private::Cell *p2 = p.GetCell(0,c-1);
				p1->real.factor.width = p2->real.factor.width/2.0;
				p2->real.factor.width -= p1->real.factor.width;
			}
				// 
         p1->real.height = p.m_szDefWindowSize.cy;
			p1->real.factor.height = 1.0;
			p1->wnd = nullptr;
			p1->splitterActive.horz = p1->splitterActive.vert = true;
		}
	else
		for(int c=0; c<col; ++c)
		{	Private::Cell *p1 = p.GetCell(row-1,c);	// just added item.
			Private::Cell *p2 = p.GetCell(row-2,c);	// item before it.
			p1->real.width = p2->real.width;
			p1->real.factor.width = p2->real.factor.width;
			p1->real.height = p.m_szDefWindowSize.cy;
			p1->real.factor.height = p2->real.factor.height/2.0;
			p2->real.factor.height -= p1->real.factor.height;
			p1->wnd = nullptr;
			p1->splitterActive.horz = p1->splitterActive.vert = true;
		}
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool SplitterCtrl::AddColumn()
{	if( !p.m_Matrix.AddColumn() )
		return false;
		// 
	const int row = GetNumberRow();
	const int col = GetNumberColumn();
		// 
	if(col==1)	// it is first addition of column.
		for(int r=0; r<row; ++r)
		{	Private::Cell *p1 = p.GetCell(r,0);
			p1->real.width = p.m_szDefWindowSize.cx;
			p1->real.factor.width = 1.0;
				// 
			if(r==0)
				p1->real.factor.height = 1.0;
			else
			{	Private::Cell *p2 = p.GetCell(r-1,0);
				p1->real.factor.height = p2->real.factor.height/2.0;
				p2->real.factor.height -= p1->real.factor.height;
			}
			p1->real.height = p.m_szDefWindowSize.cy;
			p1->wnd = nullptr;
			p1->splitterActive.horz = p1->splitterActive.vert = true;
		}
	else
		for(int r=0; r<row; ++r)
		{	Private::Cell *p1 = p.GetCell(r,col-1);	// just added item.
			Private::Cell *p2 = p.GetCell(r,col-2);	// item before it.
			p1->real.width = p.m_szDefWindowSize.cx;
			p1->real.factor.width = p2->real.factor.width/2.0;
			p2->real.factor.width -= p1->real.factor.width;
			p1->real.height = p2->real.height;
			p1->real.factor.height = p2->real.factor.height;
			p1->wnd = nullptr;
			p1->splitterActive.horz = p1->splitterActive.vert = true;
		}
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool SplitterCtrl::InsertRow(int r)
{	if( !p.m_Matrix.InsertRow(r) )
		return false;
		// 
	const int col = GetNumberColumn();
		// 
	for(int c=0; c<col; ++c)
	{	Private::Cell *p1 = p.GetCell(r,c);
		Private::Cell *p2 = p.GetCell(r+1,c);
		p1->real.width = p2->real.width;
		p1->real.factor.width = p2->real.factor.width;
			// 
		if(r==0)
		{	p1->real.factor.height = p2->real.factor.height/2.0;
			p2->real.factor.height -= p1->real.factor.height;
		}
		else
		{	Private::Cell *p3 = p.GetCell(r-1,c);
			const double totalfactor = p3->real.factor.height + p2->real.factor.height;
			p3->real.factor.height *= 2.0/3.0;
			p2->real.factor.height *= 2.0/3.0;
			p1->real.factor.height = totalfactor - p3->real.factor.height - p2->real.factor.height;
		}
		p1->real.height = p.m_szDefWindowSize.cy;
		p1->wnd = nullptr;
		p1->splitterActive.horz = p1->splitterActive.vert = true;
	}
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
bool SplitterCtrl::InsertColumn(int c)
{	if( !p.m_Matrix.InsertColumn(c) )
		return false;
		// 
	const int row = GetNumberRow();
		// 
	for(int r=0; r<row; ++r)
	{	Private::Cell *p1 = p.GetCell(r,c);
		Private::Cell *p2 = p.GetCell(r,c+1);
		p1->real.width = p.m_szDefWindowSize.cx;
			// 
		if(c==0)
		{	p1->real.factor.width = p2->real.factor.width/2.0;
			p2->real.factor.width -= p1->real.factor.width;
		}
		else
		{	Private::Cell *p3 = p.GetCell(r,c-1);
			const double totalfactor = p3->real.factor.width + p2->real.factor.width;
			p3->real.factor.width *= 2.0/3.0;
			p2->real.factor.width *= 2.0/3.0;
			p1->real.factor.width = totalfactor - p3->real.factor.width - p2->real.factor.width;
		}
			// 
		p1->real.height = p2->real.height;
		p1->real.factor.height = p2->real.factor.height;
		p1->wnd = nullptr;
		p1->splitterActive.horz = p1->splitterActive.vert = true;
	}
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::DeleteRow(int row)
{	p.m_Matrix.DeleteRow(row);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::DeleteColumn(int col)
{	p.m_Matrix.DeleteColumn(col);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::DeleteAll()
{	p.m_Matrix.DeleteAll();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::OnPaint()
{	if(!p.m_pDrawMngr)
	{	CPaintDC dc(this);
		return;
	}
		// 
	Private::VirtualWindow virtwnd(this);
	if( !virtwnd.GetSafeHdc() )
	{	CPaintDC dc(this);
		return;
	}
		// 
	p.m_pDrawMngr->DrawBegin(this,&virtwnd);
		// 
	const int row = GetNumberRow();
	const int col = GetNumberColumn();
	if(row>0 && col>0)
	{	for(int r=0; r<row-1; ++r)
		{	const CRect rc = GetSplitterRect(true,r);
			p.m_pDrawMngr->DrawSplitter(this,&virtwnd,true,r,&rc);
		}
		for(int c=0; c<col-1; ++c)
		{	const CRect rc = GetSplitterRect(false,c);
			p.m_pDrawMngr->DrawSplitter(this,&virtwnd,false,c,&rc);
		}
	}
		// 
	p.m_pDrawMngr->DrawEnd(this,&virtwnd);
}
/////////////////////////////////////////////////////////////////////////////
//
void SplitterCtrl::OnNcPaint()
{	CWnd::OnNcPaint();
		// 
	if(p.m_bShowBorder && p.m_pDrawMngr)
	{	CWindowDC dc(this);
		CRect rc;
		CWnd::GetWindowRect(&rc);
		rc.OffsetRect(-rc.left,-rc.top);
		p.m_pDrawMngr->DrawBorder(this,&dc,&rc);
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Update()
{	p.Recalc();
	Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
void SplitterCtrl::SetDrawManager(SplitterCtrl::Draw *ptr/*or null*/)
{	assert(ptr);
	p.m_pDrawMngr = ptr;
}
// 
SplitterCtrl::Draw *SplitterCtrl::GetDrawManager()
{	return p.m_pDrawMngr;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetRecalcManager(IRecalc *ptr/*or null*/)
{	p.m_pRecalcMngr = (ptr ? ptr : &p);
}
// 
SplitterCtrl::IRecalc *SplitterCtrl::GetRecalcManager()
{	return p.m_pRecalcMngr;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
void SplitterCtrl::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
		// 
	if(p.m_bShowBorder)
	{	const int iBorderWidth = GetBorderWidth();
		::InflateRect(&lpncsp->rgrc[0],-iBorderWidth,-iBorderWidth);
	}
}
/////////////////////////////////////////////////////////////////////////////
//
void SplitterCtrl::OnSize(UINT nType, int cx, int cy)
{	CWnd::OnSize(nType, cx, cy);
	p.Recalc();
	Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
int SplitterCtrl::GetBorderWidth() const
{	return p.m_pRecalcMngr->GetBorderWidth(this,&p);
}
// 
int SplitterCtrl::GetVertSplitterWidth() const
{	return p.m_pRecalcMngr->GetVertSplitterWidth(this,&p);
}
// 
int SplitterCtrl::GetHorzSplitterHeight() const
{	return p.m_pRecalcMngr->GetHorzSplitterHeight(this,&p);
}
/////////////////////////////////////////////////////////////////////////////
//
int SplitterCtrl::Private::GetBorderWidth(SplitterCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return 1;
}
int SplitterCtrl::Private::GetVertSplitterWidth(SplitterCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return 4;
}
int SplitterCtrl::Private::GetHorzSplitterHeight(SplitterCtrl const * /*ctrl*/, IRecalc * /*base*/)
{	return 4;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::Recalc()
{	const int row = o.GetNumberRow();
	const int col = o.GetNumberColumn();
		// 
	if(row>0 && col>0)
	{	CRect rc;
		o.GetClientRect(&rc/*out*/);
			// 
		switch(m_ResizeMode)
		{	case ResizeStatic: RecalcStatic(&rc,row,col); break;
			case ResizeStaticFull: RecalcStaticFull(&rc,row,col); break;
			case ResizeDynamic: RecalcDynamic(&rc,row,col); break;
		}
	}
}
// 
void SplitterCtrl::Private::RecalcStatic(CRect *rc, int numberRow, int numberColumn)
{	const int iSplitterWidth = o.GetVertSplitterWidth();
	const int iSplitterHeight = o.GetHorzSplitterHeight();
		// 
	m_iTotalWidth = std::max(rc->Width()-(numberColumn-1)*iSplitterWidth, numberColumn*m_iMinWinWidth);
	m_iTotalHeight = std::max(rc->Height()-(numberRow-1)*iSplitterHeight, numberRow*m_iMinWinHeight);
		// 
	int r,c, pos, tail, cx,cy;
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
	{	pos = rc->left;
			// 
		for(c=0; c<numberColumn; ++c)
		{	Cell *cell = GetCell(0,c);
			tail = std::max<int>(0, rc->right-pos - (numberColumn-1-c)*(iSplitterWidth+m_iMinWinWidth));
				// 
			if(tail<cell->real.width || c==numberColumn-1)
				cx = tail;
			else
				cx = cell->real.width;
			cx = std::max(cx,m_iMinWinWidth);
				// 
			for(r=0; r<numberRow; ++r)
			{	cell = GetCell(r,c);
				cell->real.rc.left = pos;
				cell->real.rc.right = pos+cx;
				cell->real.factor.width = static_cast<double>(cx)/static_cast<double>(m_iTotalWidth);
			}
			pos += (cx+iSplitterWidth);
		}
	}
	else	// RightTop or RightBottom.
	{	pos = rc->right;
			// 
		for(c=0; c<numberColumn; ++c)
		{	Cell *cell = GetCell(0,c);
			tail = std::max<int>(0, pos-rc->left - (numberColumn-1-c)*(iSplitterWidth+m_iMinWinWidth));
				// 
			if(tail<cell->real.width || c==numberColumn-1)
				cx = tail;
			else
				cx = cell->real.width;
			cx = std::max(cx,m_iMinWinWidth);
				// 
			for(r=0; r<numberRow; ++r)
			{	cell = GetCell(r,c);
				cell->real.rc.left = pos-cx;
				cell->real.rc.right = pos;
				cell->real.factor.width = static_cast<double>(cx)/static_cast<double>(m_iTotalWidth);
			}
			pos -= (cx+iSplitterWidth);
		}
	}
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
	{	pos = rc->top;
			// 
		for(r=0; r<numberRow; ++r)
		{	Cell *cell = GetCell(r,0);
			tail = std::max<int>(0, rc->bottom-pos - (numberRow-1-r)*(iSplitterHeight+m_iMinWinHeight));
				// 
			if(tail<cell->real.height || r==numberRow-1)
				cy = tail;
			else
				cy = cell->real.height;
			cy = std::max(cy,m_iMinWinHeight);
				// 
			for(c=0; c<numberColumn; ++c)
			{	cell = GetCell(r,c);
				cell->real.rc.top = pos;
				cell->real.rc.bottom = pos+cy;
				cell->real.factor.height = static_cast<double>(cy)/static_cast<double>(m_iTotalHeight);
					// 
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
			pos += (cy+iSplitterHeight);
		}
	}
	else	// LeftBottom or RightBottom.
	{	pos = rc->bottom;
			// 
		for(r=0; r<numberRow; ++r)
		{	Cell *cell = GetCell(r,0);
			tail = std::max<int>(0, pos-rc->top - (numberRow-1-r)*(iSplitterHeight+m_iMinWinHeight));
				// 
			if(tail<cell->real.height || r==numberRow-1)
				cy = tail;
			else
				cy = cell->real.height;
			cy = std::max(cy,m_iMinWinHeight);
				// 
			for(c=0; c<numberColumn; ++c)
			{	cell = GetCell(r,c);
				cell->real.rc.top = pos-cy;
				cell->real.rc.bottom = pos;
				cell->real.factor.height = static_cast<double>(cy)/static_cast<double>(m_iTotalHeight);
					// 
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
			pos -= (cy+iSplitterHeight);
		}
	}
}
// 
void SplitterCtrl::Private::RecalcStaticFull(CRect *rc, int numberRow, int numberColumn)
{	const int iSplitterWidth = o.GetVertSplitterWidth();
	const int iSplitterHeight = o.GetHorzSplitterHeight();
		// 
	int r,c, pos, cx,cy;
		// 
		// 
	m_iTotalWidth = m_iTotalHeight=0;
		// 		
	for(c=0; c<numberColumn; ++c)
		if(c!=numberColumn-1)
			m_iTotalWidth += GetCell(0,c)->real.width;
		else	// last column.
			m_iTotalWidth += std::max(m_iMinWinWidth,rc->Width()-(numberColumn-1)*iSplitterWidth-m_iTotalWidth);
		// 
	for(r=0; r<numberRow; ++r)
		if(r!=numberRow-1)
			m_iTotalHeight += GetCell(r,0)->real.height;
		else	// last row.
			m_iTotalHeight += std::max(m_iMinWinHeight,rc->Height()-(numberRow-1)*iSplitterHeight-m_iTotalHeight);
		// 
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
	{	pos = rc->left;
			// 
		for(c=0; c<numberColumn; ++c)
		{	if(c!=numberColumn-1)
				cx = GetCell(0,c)->real.width;
			else	// last column.
				cx = rc->right-pos;
				// 
			cx = std::max(cx,m_iMinWinWidth);
				// 
			for(r=0; r<numberRow; ++r)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.left = pos;
				cell->real.rc.right = pos+cx;
				cell->real.factor.width = static_cast<double>(cx)/m_iTotalWidth;
			}
			pos += (cx+iSplitterWidth);
		}
	}
	else	// RightTop or RightBottom.
	{	pos = rc->right;
			// 
		for(c=0; c<numberColumn; ++c)
		{	if(c!=numberColumn-1)
				cx = GetCell(0,c)->real.width;
			else	// last column.
				cx = pos-rc->left;
				// 
			cx = std::max(cx,m_iMinWinWidth);
				// 
			for(r=0; r<numberRow; ++r)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.left = pos-cx;
				cell->real.rc.right = pos;
				cell->real.factor.width = static_cast<double>(cx)/m_iTotalWidth;
			}
			pos -= (cx+iSplitterWidth);
		}
	}
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
	{	pos = rc->top;
			// 
		for(r=0; r<numberRow; ++r)
		{	if(r!=numberRow-1)
				cy = GetCell(r,0)->real.height;
			else	// last row.
				cy = rc->bottom-pos;
				// 
			cy = std::max(cy,m_iMinWinHeight);
				// 
			for(c=0; c<numberColumn; ++c)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.top = pos;
				cell->real.rc.bottom = pos+cy;
				cell->real.factor.height = static_cast<double>(cy)/m_iTotalHeight;
					// 
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
			pos += (cy+iSplitterHeight);
		}
	}
	else	// LeftBottom or RightBottom.
	{	pos = rc->bottom;
			// 
		for(r=0; r<numberRow; ++r)
		{	if(r!=numberRow-1)
				cy = GetCell(r,0)->real.height;
			else	// last row.
				cy = pos-rc->top;
				// 
			cy = std::max(cy,m_iMinWinHeight);
				// 
			for(c=0; c<numberColumn; ++c)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.top = pos-cy;
				cell->real.rc.bottom = pos;
				cell->real.factor.height = static_cast<double>(cy)/m_iTotalHeight;
					// 
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
			pos -= (cy+iSplitterHeight);
		}
	}
}
// 
void SplitterCtrl::Private::RecalcDynamic(CRect *rc, int numberRow, int numberColumn)
{	const int iSplitterWidth = o.GetVertSplitterWidth();
	const int iSplitterHeight = o.GetHorzSplitterHeight();
		// 
	m_iTotalWidth = std::max(rc->Width()-(numberColumn-1)*iSplitterWidth, numberColumn*m_iMinWinWidth);
	m_iTotalHeight = std::max(rc->Height()-(numberRow-1)*iSplitterHeight, numberRow*m_iMinWinHeight);
		// 
	int cx,cy;
		// 
	int iCorrectedTail = 0;
	int iTotalOver = 0;
	int pos = rc->left;
		// 
	for(int c=0; c<numberColumn; ++c)
	{	if(c!=numberColumn-1)
			cx = static_cast<int>(static_cast<double>(m_iTotalWidth) * GetCell(0,c)->real.factor.width + 0.5);
		else	// last column.
			cx = rc->right-pos;
			// 
		if(cx<=m_iMinWinWidth)
			iCorrectedTail += m_iMinWinWidth-cx;
		else
			iTotalOver += cx-m_iMinWinWidth;
		pos += (cx+iSplitterWidth);
	}
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
	{	pos = rc->left;
			// 
		for(int c=0; c<numberColumn; ++c)
		{	if(c!=numberColumn-1)
				cx = static_cast<int>(static_cast<double>(m_iTotalWidth) * GetCell(0,c)->real.factor.width + 0.5);
			else	// last column.
				cx = rc->right-pos;
				// 
			if(c!=numberColumn-1)	// except last column.
				if(cx>m_iMinWinWidth)
					cx -= static_cast<int>(static_cast<float>(iCorrectedTail) * (static_cast<float>(cx-m_iMinWinWidth) / static_cast<float>(iTotalOver)) + 0.5f);
			cx = std::max(cx,m_iMinWinWidth);
				// 
			for(int r=0; r<numberRow; ++r)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.left = pos;
				cell->real.rc.right = pos+cx;
				cell->real.width = cx;
			}
			pos += (cx+iSplitterWidth);
		}
	}
	else	// RightTop or RightBottom.
	{	pos = rc->right;
			// 
		for(int c=0; c<numberColumn; ++c)
		{	if(c!=numberColumn-1)
				cx = static_cast<int>(static_cast<double>(m_iTotalWidth) * GetCell(0,c)->real.factor.width + 0.5);
			else	// last column.
				cx = pos-rc->left;
				// 
			if(c!=numberColumn-1)	// except last column.
				if(cx>m_iMinWinWidth)
					cx -= static_cast<int>(static_cast<float>(iCorrectedTail) * (static_cast<float>(cx - m_iMinWinWidth) / static_cast<float>(iTotalOver)) + 0.5f);
			cx = std::max(cx,m_iMinWinWidth);
				// 
			for(int r=0; r<numberRow; ++r)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.left = pos-cx;
				cell->real.rc.right = pos;
				cell->real.width = cx;
			}
			pos -= (cx+iSplitterWidth);
		}
	}
		// 
		// 
	iCorrectedTail = iTotalOver = 0;
	pos = rc->top;
		// 
	for(int r=0; r<numberRow; ++r)
	{	if(r!=numberRow-1)
			cy = static_cast<int>(static_cast<double>(m_iTotalHeight) * GetCell(r,0)->real.factor.height + 0.5);
		else	// last row.
			cy = rc->bottom-pos;
			// 
		if(cy<=m_iMinWinHeight)
			iCorrectedTail += m_iMinWinHeight-cy;
		else
			iTotalOver += cy-m_iMinWinHeight;
		pos += (cy+iSplitterHeight);
	}
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
	{	pos = rc->top;
			// 
		for(int r=0; r<numberRow; ++r)
		{	if(r!=numberRow-1)
				cy = static_cast<int>(static_cast<double>(m_iTotalHeight) * GetCell(r,0)->real.factor.height + 0.5);
			else	// last row.
				cy = rc->bottom-pos;
				// 
			if(r!=numberRow-1)	// except last row.
				if(cy>m_iMinWinHeight)
					cy -= static_cast<int>(static_cast<float>(iCorrectedTail) * (static_cast<float>(cy - m_iMinWinHeight) / static_cast<float>(iTotalOver)) + 0.5f);
			cy = std::max(cy,m_iMinWinHeight);
				// 
			for(int c=0; c<numberColumn; ++c)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.top = pos;
				cell->real.rc.bottom = pos+cy;
				cell->real.height = cy;
					// 
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
			pos += (cy+iSplitterHeight);
		}
	}
	else	// LeftBottom or RightBottom.
	{	pos = rc->bottom;
			// 
		for(int r=0; r<numberRow; ++r)
		{	if(r!=numberRow-1)
				cy = static_cast<int>(static_cast<double>(m_iTotalHeight) * GetCell(r,0)->real.factor.height + 0.5);
			else	// last row.
				cy = pos-rc->top;
				// 
			if(r!=numberRow-1)	// except last row.
				if(cy>m_iMinWinHeight)
					cy -= static_cast<int>(static_cast<float>(iCorrectedTail) * (static_cast<float>(cy - m_iMinWinHeight) / static_cast<float>(iTotalOver)) + 0.5f);
			cy = std::max(cy,m_iMinWinHeight);
				// 
			for(int c=0; c<numberColumn; ++c)
			{	Cell *cell = GetCell(r,c);
				cell->real.rc.top = pos-cy;
				cell->real.rc.bottom = pos;
				cell->real.height = cy;
					// 
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
			pos -= (cy+iSplitterHeight);
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::MoveChangedWindow(HWND wnd, CRect const *rcNew) const
{	CRect rcOld;
	::GetWindowRect(wnd,&rcOld/*out*/);
	::MapWindowPoints(HWND_DESKTOP,o.m_hWnd,reinterpret_cast<POINT *>(&rcOld),2);
	if(*rcNew!=rcOld)
		::MoveWindow(wnd,rcNew->left,rcNew->top,rcNew->Width(),rcNew->Height(),TRUE);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
LRESULT SplitterCtrl::OnNcHitTest(CPoint point)
{	CPoint pt(point);
	ScreenToClient(&pt);
		// 
	int row, col;
	HitTest(pt,&row,&col);
		// 
	if(row!=-1 && !IsHorzSplitterActive(row))
		row = -1;
	if(col!=-1 && !IsVertSplitterActive(col))
		col = -1;
		// 
	if(col==-1 && row!=-1)
		return Private::HTSplitterHorz;
	if(col!=-1 && row==-1)
		return Private::HTSplitterVert;
	if(col!=-1 && row!=-1)
		return Private::HTSplitterCross;
		// 
	return CWnd::OnNcHitTest(point);
}
/////////////////////////////////////////////////////////////////////////////
// 
BOOL SplitterCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{	if((p.m_bDragHorz || (nHitTest & 0x0000ffff)==Private::HTSplitterHorz) && p.m_hCurHorz)
	{	SetCursor(p.m_hCurHorz);
		return TRUE;
	}
	if((p.m_bDragVert || (nHitTest & 0x0000ffff)==Private::HTSplitterVert) && p.m_hCurVert)
	{	SetCursor(p.m_hCurVert);
		return TRUE;
	}
	if((p.m_bDragCross || (nHitTest & 0x0000ffff)==Private::HTSplitterCross) && p.m_hCurCross)
	{	SetCursor(p.m_hCurCross);
		return TRUE;
	}
		// 
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::OnNcLButtonDown(UINT nHitTest, CPoint point)
{	CPoint pt(point);
	ScreenToClient(&pt/*out*/);
	HitTest(pt,&p.m_iDragHorz/*out*/,&p.m_iDragVert/*out*/);
		// 
	if(p.m_iDragHorz!=-1 && !IsHorzSplitterActive(p.m_iDragHorz))
		p.m_iDragHorz = -1;
	if(p.m_iDragVert!=-1 && !IsVertSplitterActive(p.m_iDragVert))
		p.m_iDragVert = -1;
		// 
	if(p.m_iDragVert==-1 && p.m_iDragHorz!=-1)
		p.m_bDragHorz = true;
	else if(p.m_iDragVert!=-1 && p.m_iDragHorz==-1)
		p.m_bDragVert = true;
	else if(p.m_iDragVert!=-1 && p.m_iDragHorz!=-1)
		p.m_bDragCross = true;
		// 
	if( IsDragging() )
	{	p.m_ptStartDrag = pt;
			// 
		for(int r=0; r<GetNumberRow(); ++r)
			for(int c=0; c<GetNumberColumn(); ++c)
			{	Private::Cell *cell = p.GetCell(r,c);
				cell->store = cell->real;
			}
		SetCapture();
		if((Private::m_hKeyboardHook=::SetWindowsHookEx(WH_KEYBOARD,static_cast<HOOKPROC>(Private::KeyboardHookProc),nullptr,::GetCurrentThreadId()))!=nullptr)
			p.m_pFuncCancelDragging = std::make_pair(this,&SplitterCtrl::CancelDragging);
			// 
		if(p.m_DraggingMode==DraggingStatic)
		{	if(p.m_bDragHorz || p.m_bDragCross)
			{	const CRect rc = GetSplitterRect(true/*horz*/,p.m_iDragHorz);
				p.DrawDragRectHorz(&rc);
			}
			if(p.m_bDragVert || p.m_bDragCross) 
			{	const CRect rc = GetSplitterRect(false/*vert*/,p.m_iDragVert);
				p.DrawDragRectVert(&rc);
			}
		}
	}
		// 
	CWnd::OnNcLButtonDown(nHitTest, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{	CPoint pt(point);
	ScreenToClient(&pt);
	HitTest(pt,&p.m_iDragHorz,&p.m_iDragVert);
		// 
	if(p.m_iDragHorz!=-1 && !IsHorzSplitterActive(p.m_iDragHorz))
		p.m_iDragHorz = -1;
	if(p.m_iDragVert!=-1 && !IsVertSplitterActive(p.m_iDragVert))
		p.m_iDragVert = -1;
		// 
	if((p.m_iDragVert==-1 && p.m_iDragHorz!=-1) || (p.m_iDragVert!=-1 && p.m_iDragHorz!=-1))
		p.SetHalfAlignHeight(p.m_iDragHorz);
	if((p.m_iDragVert!=-1 && p.m_iDragHorz==-1) || (p.m_iDragVert!=-1 && p.m_iDragHorz!=-1))
		p.SetHalfAlignWidth(p.m_iDragVert);
		// 
	CWnd::OnNcLButtonDblClk(nHitTest, point);
}
// 
void SplitterCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{	p.StopDragging(false);
	CWnd::OnLButtonUp(nFlags, point);
}
// 
void SplitterCtrl::OnMButtonDown(UINT nFlags, CPoint point)
{	p.StopDragging(true);
	CWnd::OnMButtonDown(nFlags, point);
}
// 
void SplitterCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{	p.StopDragging(true);
	CWnd::OnRButtonDown(nFlags, point);
}
// 
void SplitterCtrl::OnCaptureChanged(CWnd *pWnd)
{	if(pWnd!=this)
		p.StopDragging(true);
	CWnd::OnCaptureChanged(pWnd);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::OnMouseMove(UINT nFlags, CPoint point)
{	if( IsDragging() )
	{	std::map<HWND,CRect> changedWindows;
			// 
		CRect rcdrag;
		if(p.m_bDragHorz || p.m_bDragCross)
		{	const int shiftY = point.y - p.m_ptStartDrag.y;
			p.SetDraggingHeight(p.m_iDragHorz,shiftY,changedWindows/*out*/,&rcdrag/*out*/);
			if(p.m_DraggingMode==DraggingStatic)
				p.DrawDragRectHorz(&rcdrag);
		}
		if(p.m_bDragVert || p.m_bDragCross)
		{	const int shiftX = point.x - p.m_ptStartDrag.x;
			p.SetDraggingWidth(p.m_iDragVert,shiftX,changedWindows/*out*/,&rcdrag/*out*/);
			if(p.m_DraggingMode==DraggingStatic)
				p.DrawDragRectVert(&rcdrag);
		}
			// 
		if( !changedWindows.empty() )
		{	const HDWP dwp = ::BeginDeferWindowPos( static_cast<int>( changedWindows.size() ) );
			if(dwp)
			{	for(std::map<HWND,CRect>::iterator i=changedWindows.begin(), e=changedWindows.end(); i!=e; ++i)
					::DeferWindowPos(dwp, i->first, nullptr, i->second.left,i->second.top,i->second.Width(),i->second.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
				::EndDeferWindowPos(dwp);
			}
			else
				for(std::map<HWND,CRect>::iterator i=changedWindows.begin(), e=changedWindows.end(); i!=e; ++i)
					::SetWindowPos(i->first, nullptr, i->second.left,i->second.top,i->second.Width(),i->second.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}
		// 
	CWnd::OnMouseMove(nFlags, point);
}
/////////////////////////////////////////////////////////////////////////////
// 
LRESULT __stdcall SplitterCtrl::Private::KeyboardHookProc(int code, WPARAM wParam, LPARAM lParam)
{	if(code==HC_ACTION &&
		!(lParam & 0x80000000))   // key is down.
	{
		if(wParam==VK_ESCAPE)
		{	(Private::m_pFuncCancelDragging.first->*Private::m_pFuncCancelDragging.second)();
			return 1;   // to prevent calling target window procedure (any nonzero value acceptable).
		}
	}
	return ::CallNextHookEx(Private::m_hKeyboardHook,code,wParam,lParam);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::StopDragging(bool reset)
{	if( !o.IsDragging() )
		return;
		// 
	if(m_DraggingMode==DraggingStatic)
	{	if(m_bDragHorz || m_bDragCross)
			DrawDragRectHorz(nullptr);
		if(m_bDragVert || m_bDragCross)
			DrawDragRectVert(nullptr);
	}
		// 
	if((m_DraggingMode==DraggingStatic && !reset) ||
		(m_DraggingMode==DraggingDynamic && reset))
	{
		for(int r=0; r<o.GetNumberRow(); ++r)
			for(int c=0; c<o.GetNumberColumn(); ++c)
			{	Cell *cell = GetCell(r,c);
				cell->real = cell->store;
				if(cell->wnd)
					MoveChangedWindow(cell->wnd,&cell->real.rc);
			}
	}
		// 
	m_bDragHorz = m_bDragVert = m_bDragCross = false;
	if(m_hKeyboardHook)
	{	::UnhookWindowsHookEx(m_hKeyboardHook);
		m_hKeyboardHook = nullptr;
	}
	if( GetCapture() )
		::ReleaseCapture();
	o.Invalidate(FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::SetDraggingWidth(int col, int offset, std::map<HWND,CRect> &changedWindows/*out*/, CRect *rcdrag/*out*/)
{	Cell *p1 = GetCell(0,col);
	Cell *p2 = GetCell(0,col+1);
		// 
	Cell::State *state1 = (m_DraggingMode==DraggingStatic ? &p1->real : &p1->store);
	Cell::State *state2 = (m_DraggingMode==DraggingStatic ? &p2->real : &p2->store);
		// 
	const int totalcx = state1->rc.Width() + state2->rc.Width();
	const double totalfactor = state1->factor.width + state2->factor.width;
		// 
	int cx1 = state1->rc.Width() + (m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom ? offset : -offset);
	if(cx1<m_iMinWinWidth)
		cx1 = m_iMinWinWidth;
	int cx2 = totalcx - cx1;
	if(cx2<m_iMinWinWidth)
	{	cx2 = m_iMinWinWidth;
		cx1 = totalcx - cx2;
	}
		// 
	const int row = o.GetNumberRow();
		// 
	for(int r=0; r<row; ++r)
	{	p1 = GetCell(r,col);
		p2 = GetCell(r,col+1);
			// 
		state1 = (m_DraggingMode==DraggingStatic ? &p1->store : &p1->real);
		state2 = (m_DraggingMode==DraggingStatic ? &p2->store : &p2->real);
			// 
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
		{	state1->rc.right = state1->rc.left + cx1;
			state2->rc.left = state2->rc.right - cx2;
		}
		else	// RightTop or RightBottom.
		{	state1->rc.left = state1->rc.right - cx1;
			state2->rc.right = state2->rc.left + cx2;
		}
		state1->width = cx1;
		state2->width = cx2;
		state1->factor.width = totalfactor * static_cast<double>(cx1)/static_cast<double>(totalcx);
		state2->factor.width = totalfactor - state1->factor.width;
		if(state2->factor.width<0.0)
			state2->factor.width = 0.0;
			// 
		if(m_DraggingMode==DraggingDynamic)
		{	if(p1->wnd)
				changedWindows[p1->wnd] = state1->rc;
			if(p2->wnd)
				changedWindows[p2->wnd] = state2->rc;
		}
	}
		// 
	if(m_DraggingMode==DraggingStatic)
	{	o.GetClientRect(rcdrag);
			// 
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
		{	rcdrag->left = GetCell(0,col)->store.rc.right;
			rcdrag->right = GetCell(0,col+1)->store.rc.left;
		}
		else	// RightTop or RightBottom.
		{	rcdrag->left = GetCell(0,col+1)->store.rc.right;
			rcdrag->right = GetCell(0,col)->store.rc.left;
		}
	}
	else
	{	CRect rc;
		o.GetClientRect(&rc);
			// 
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
		{	rc.left = state1->rc.left;
			rc.right = state2->rc.right;
		}
		else	// RightTop or RightBottom.
		{	rc.left = state2->rc.left;
			rc.right = state1->rc.right;
		}
			// 
		o.InvalidateRect(&rc,FALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::SetDraggingHeight(int row, int offset, std::map<HWND,CRect> &changedWindows/*out*/, CRect *rcdrag/*out*/)
{	Cell *p1 = GetCell(row,0);
	Cell *p2 = GetCell(row+1,0);
		// 
	Cell::State *state1 = (m_DraggingMode==DraggingStatic ? &p1->real : &p1->store);
	Cell::State *state2 = (m_DraggingMode==DraggingStatic ? &p2->real : &p2->store);
		// 
	const int totalcy = state1->rc.Height() + state2->rc.Height();
	const double totalfactor = state1->factor.height + state2->factor.height;
		// 
	int cy1 = state1->rc.Height() + (m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop ? offset : -offset);
	if(cy1<m_iMinWinHeight)
		cy1 = m_iMinWinHeight;
	int cy2 = totalcy - cy1;
	if(cy2<m_iMinWinHeight)
	{	cy2 = m_iMinWinHeight;
		cy1 = totalcy - cy2;
	}
		// 
	const int col = o.GetNumberColumn();
		// 
	for(int c=0; c<col; ++c)
	{	p1 = GetCell(row,c);
		p2 = GetCell(row+1,c);
			// 
		state1 = (m_DraggingMode==DraggingStatic ? &p1->store : &p1->real);
		state2 = (m_DraggingMode==DraggingStatic ? &p2->store : &p2->real);
			// 
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
		{	state1->rc.bottom = state1->rc.top + cy1;
			state2->rc.top = state2->rc.bottom - cy2;
		}
		else	// LeftBottom or RightBottom.
		{	state1->rc.top = state1->rc.bottom - cy1;
			state2->rc.bottom = state2->rc.top + cy2;
		}
		state1->height = cy1;
		state2->height = cy2;
		state1->factor.height = totalfactor * static_cast<double>(cy1)/static_cast<double>(totalcy);
		state2->factor.height = totalfactor - state1->factor.height;
		if(state2->factor.height<0.0)
			state2->factor.height = 0.0;
			// 
		if(m_DraggingMode==DraggingDynamic)
		{	if(p1->wnd)
				changedWindows[p1->wnd] = state1->rc;
			if(p2->wnd)
				changedWindows[p2->wnd] = state2->rc;
		}
	}
		// 
	if(m_DraggingMode==DraggingStatic)
	{	o.GetClientRect(rcdrag/*out*/);
			// 
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
		{	rcdrag->top = GetCell(row,0)->store.rc.bottom;
			rcdrag->bottom = GetCell(row+1,0)->store.rc.top;
		}
		else	// LeftBottom or RightBottom.
		{	rcdrag->top = GetCell(row+1,0)->store.rc.bottom;
			rcdrag->bottom = GetCell(row,0)->store.rc.top;
		}
	}
	else   // DraggingDynamic.
	{	CRect rc;
		o.GetClientRect(&rc/*out*/);
			// 
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
		{	rc.top = state1->rc.top;
			rc.bottom = state2->rc.bottom;
		}
		else	// LeftBottom or RightBottom.
		{	rc.top = state2->rc.top;
			rc.bottom = state1->rc.bottom;
		}
			// 
		o.InvalidateRect(&rc,FALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::SetHalfAlignWidth(int col)
{	Cell *p1 = GetCell(0,col);
	Cell *p2 = GetCell(0,col+1);
		// 
	const int totalcx = p1->real.rc.Width() + p2->real.rc.Width();
	const double totalfactor = p1->real.factor.width + p2->real.factor.width;
	int cx1 = totalcx/2;
	int cx2 = totalcx - cx1;
	if(cx1<m_iMinWinWidth)
	{	cx1 = m_iMinWinWidth;
		cx2 = totalcx - cx1;
	}
	else if(cx2<m_iMinWinWidth)
	{	cx2 = m_iMinWinWidth;
		cx1 = totalcx - cx2;
	}
		// 
	const int row = o.GetNumberRow();
		// 
	for(int r=0; r<row; ++r)
	{	p1 = GetCell(r,col);
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
			p1->real.rc.right = p1->real.rc.left + cx1;
		else	// RightTop or RightBottom.
			p1->real.rc.left = p1->real.rc.right - cx1;
		p1->real.width = cx1;
		p1->real.factor.width = totalfactor * static_cast<double>(cx1)/static_cast<double>(totalcx);
			// 
		p2 = GetCell(r,col+1);
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
			p2->real.rc.left = p2->real.rc.right - cx2;
		else	// RightTop or RightBottom.
			p2->real.rc.right = p2->real.rc.left + cx2;
		p2->real.width = cx2;
		p2->real.factor.width = totalfactor - p1->real.factor.width;
		if(p2->real.factor.width<0.0)
			p2->real.factor.width = 0.0;
			// 
		if(p1->wnd)
			::MoveWindow(p1->wnd,p1->real.rc.left,p1->real.rc.top,p1->real.rc.Width(),p1->real.rc.Height(),TRUE);
		if(p2->wnd)
			::MoveWindow(p2->wnd,p2->real.rc.left,p2->real.rc.top,p2->real.rc.Width(),p2->real.rc.Height(),TRUE);
	}
		// 
	CRect rc;
	o.GetClientRect(&rc);
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapLeftBottom)
	{	rc.left = p1->real.rc.left;
		rc.right = p2->real.rc.right;
	}
	else	// RightTop or RightBottom.
	{	rc.left = p2->real.rc.left;
		rc.right = p1->real.rc.right;
	}
		// 
	o.InvalidateRect(&rc,FALSE);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::SetHalfAlignHeight(int row)
{	Cell *p1 = GetCell(row,0);
	Cell *p2 = GetCell(row+1,0);
		// 
	const int totalcy = p1->real.rc.Height() + p2->real.rc.Height();
	const double totalfactor = p1->real.factor.height + p2->real.factor.height;
	int cy1 = totalcy/2;
	int cy2 = totalcy - cy1;
	if(cy1<m_iMinWinHeight)
	{	cy1 = m_iMinWinHeight;
		cy2 = totalcy - cy1;
	}
	else if(cy2<m_iMinWinHeight)
	{	cy2 = m_iMinWinHeight;
		cy1 = totalcy - cy2;
	}
		// 
	const int col = o.GetNumberColumn();
		// 
	for(int c=0; c<col; ++c)
	{	p1 = GetCell(row,c);
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
			p1->real.rc.bottom = p1->real.rc.top + cy1;
		else	// LeftBottom or RightBottom.
			p1->real.rc.top = p1->real.rc.bottom - cy1;
		p1->real.height = cy1;
		p1->real.factor.height = totalfactor * static_cast<double>(cy1)/static_cast<double>(totalcy);
			// 
		p2 = GetCell(row+1,c);
		if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
			p2->real.rc.top = p2->real.rc.bottom - cy2;
		else	// LeftBottom or RightBottom.
			p2->real.rc.bottom = p2->real.rc.top + cy2;
		p2->real.height = cy2;
		p2->real.factor.height = totalfactor - p1->real.factor.height;
		if(p2->real.factor.height<0.0)
			p2->real.factor.height = 0.0;
			// 
		if(p1->wnd)
			::MoveWindow(p1->wnd,p1->real.rc.left,p1->real.rc.top,p1->real.rc.Width(),p1->real.rc.Height(),TRUE);
		if(p2->wnd)
			::MoveWindow(p2->wnd,p2->real.rc.left,p2->real.rc.top,p2->real.rc.Width(),p2->real.rc.Height(),TRUE);
	}
		// 
	CRect rc;
	o.GetClientRect(&rc);
		// 
	if(m_SnapMode==SnapLeftTop || m_SnapMode==SnapRightTop)
	{	rc.top = p1->real.rc.top;
		rc.bottom = p2->real.rc.bottom;
	}
	else	// LeftBottom or RightBottom.
	{	rc.top = p2->real.rc.top;
		rc.bottom = p1->real.rc.bottom;
	}
		// 
	o.InvalidateRect(&rc,FALSE);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::HitTest(CPoint point, int *horz/*out,or null*/, int *vert/*out,or null*/) const
{	const int row = GetNumberRow();
	const int col = GetNumberColumn();
		// 
	if(row>0 && col>0)
	{	if(vert)
		{	*vert=-1;
				// 
			for(int c=0; c<col; ++c)
			{	Private::Cell const *cell = p.GetCell(0,c);
					// 
				if((point.x < cell->real.rc.left && (p.m_SnapMode==SnapLeftTop || p.m_SnapMode==SnapLeftBottom)) ||
					(point.x > cell->real.rc.right && (p.m_SnapMode==SnapRightTop || p.m_SnapMode==SnapRightBottom)))
				{
					*vert = c-1;
					break;
				}
				if(point.x>=cell->real.rc.left && point.x<=cell->real.rc.right)
					break;
			}
		}
			// 
		if(horz)
		{	*horz=-1;
				// 
			for(int r=0; r<row; ++r)
			{	Private::Cell const *cell = p.GetCell(r,0);
					// 
				if((point.y < cell->real.rc.top && (p.m_SnapMode==SnapLeftTop || p.m_SnapMode==SnapRightTop)) ||
					(point.y > cell->real.rc.bottom && (p.m_SnapMode==SnapLeftBottom || p.m_SnapMode==SnapRightBottom)))
				{
					*horz = r-1;
					break;
				}
				if(point.y>=cell->real.rc.top && point.y<=cell->real.rc.bottom)
					break;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
CRect SplitterCtrl::GetWindowRect(int row, int col) const
{	assert(row>=0 && row<GetNumberRow());
	assert(col>=0 && col<GetNumberColumn());
		// 
	return p.GetCell(row,col)->real.rc;
}
/////////////////////////////////////////////////////////////////////////////
// 
CRect SplitterCtrl::GetSplitterRect(bool horz, int idx) const
{	assert(idx>=0 && (horz && idx<GetNumberRow()-1) || (!horz && idx<GetNumberColumn()-1));
	assert(GetNumberColumn()>0 && GetNumberRow()>0);
		// 
	CRect rect;
	GetClientRect(&rect/*out*/);
		// 
	if(horz)
	{	const RECT rc = GetWindowRect(idx,0);
			// 
		if(p.m_SnapMode==SnapLeftTop || p.m_SnapMode==SnapRightTop)
		{	rect.top = rc.bottom;
			rect.bottom = rect.top + GetHorzSplitterHeight();
		}
		else
		{	rect.bottom = rc.top;
			rect.top = rect.bottom - GetHorzSplitterHeight();
		}
	}
	else
	{	const RECT rc = GetWindowRect(0,idx);
			// 
		if(p.m_SnapMode==SnapLeftTop || p.m_SnapMode==SnapLeftBottom)
		{	rect.left = rc.right;
			rect.right = rect.left + GetVertSplitterWidth();
		}
		else
		{	rect.right = rc.left;
			rect.left = rect.right - GetVertSplitterWidth();
		}
	}
	return rect;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::Private::DrawDragRectVert(CRect const *rect)
{	static CRect rectOld(0,0,0,0);
	DrawDragRectManage(&rectOld,rect,false);
}
// 
void SplitterCtrl::Private::DrawDragRectHorz(CRect const *rect)
{	static CRect rectOld(0,0,0,0);
	DrawDragRectManage(&rectOld,rect,true);
}
// 
void SplitterCtrl::Private::DrawDragRectManage(CRect *rectOld, CRect const *rectNew, bool horz)
{	CRect rcNew(0,0,0,0);
	if(rectNew)
	{	static_cast<CWnd&>(o).GetWindowRect(&rcNew);
		o.ScreenToClient(&rcNew);
		rcNew.SetRect(rectNew->left-rcNew.left,rectNew->top-rcNew.top,rectNew->right-rcNew.left,rectNew->bottom-rcNew.top);
	}
		// 
	if(m_pDrawMngr)
	{	CDC *dc = o.GetDCEx(nullptr, DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		if(dc)
		{	m_pDrawMngr->DrawDragRect(&o,dc,horz,rectOld->IsRectNull()==TRUE,rectOld,&rcNew);
			o.ReleaseDC(dc);
		}
	}
		// 
	*rectOld = (rectNew ? rcNew : CRect(0,0,0,0));
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetWindow(int row, int col, HWND wnd)
{	assert(row>=0 && row<GetNumberRow());
	assert(col>=0 && col<GetNumberColumn());
	assert(!wnd || (::IsWindow(wnd) && ::GetParent(wnd)==m_hWnd));
		// 
	p.GetCell(row,col)->wnd = wnd;
	if(wnd)   // for correct mutual repaint.
		::SetWindowLong(wnd,GWL_STYLE,::GetWindowLong(wnd,GWL_STYLE) | WS_CLIPSIBLINGS);   // add WS_CLIPSIBLINGS style.
}
// 
HWND SplitterCtrl::GetWindow(int row, int col) const
{	assert(row>=0 && row<GetNumberRow());
	assert(col>=0 && col<GetNumberColumn());
		// 
	return p.GetCell(row,col)->wnd;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Only for Static, StaticFull.
// 
void SplitterCtrl::SetColumnWidthForStatic(int col, int width)
{	assert(p.m_ResizeMode!=ResizeDynamic);
		// 
	assert(col>=0 && col<GetNumberColumn());
	assert(GetNumberRow()>0 && GetNumberColumn()>0);
	assert(width>=0);
		// 
	width = std::max(width,p.m_iMinWinWidth);
		// 
	const int row = GetNumberRow();
	for(int r=0; r<row; ++r)
		p.GetCell(r,col)->real.width = width;
}
/////////////////////////////////////////////////////////////////////////////
// Only for Static, StaticFull.
// 
void SplitterCtrl::SetRowHeightForStatic(int row, int height)
{	assert(p.m_ResizeMode!=ResizeDynamic);
		// 
	assert(row>=0 && row<GetNumberRow());
	assert(GetNumberRow()>0 && GetNumberColumn()>0);
	assert(height>=0);
		// 
	height = std::max(height,p.m_iMinWinHeight);
		// 
	const int col = GetNumberColumn();
	for(int c=0; c<col; ++c)
		p.GetCell(row,c)->real.height = height;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// For Dynamic.
// 
void SplitterCtrl::SetColumnWidthForDynamic(int col, float percent)
{	assert(p.m_ResizeMode==ResizeDynamic);
		// 
	assert(col>=0 && col<GetNumberColumn());
	assert(percent>0.1f && percent<99.9f);
		// 
	const int rowNumber = GetNumberRow();
	const int colNumber = GetNumberColumn();
	assert(rowNumber>0 && colNumber>0);
		// 
	const int totalWidth = p.m_iTotalWidth - p.GetCell(0,col)->real.width;   // without width of col.
	double factor;
		// 
	for(int c=0; c<colNumber; ++c)
	{	if(c!=col)
			factor = (1.0-static_cast<double>(percent)/100.0) * static_cast<double>(p.GetCell(0,c)->real.width)/static_cast<double>(totalWidth);
		else
			factor = static_cast<double>(percent)/100.0;
			// 
		for(int r=0; r<rowNumber; ++r)
			p.GetCell(r,c)->real.factor.width = factor;
	}
}
/////////////////////////////////////////////////////////////////////////////
// For Dynamic.
// 
void SplitterCtrl::SetRowHeightForDynamic(int row, float percent)
{	assert(p.m_ResizeMode==ResizeDynamic);
		// 
	assert(row>=0 && row<GetNumberRow());
	assert(percent>0.1f && percent<99.9f);
		// 
	const int rowNumber = GetNumberRow();
	const int colNumber = GetNumberColumn();
	assert(rowNumber>0 && colNumber>0);
		// 
	const int totalHeight = p.m_iTotalHeight - p.GetCell(row,0)->real.height;   // without height of row.
	double factor;
		// 
	for(int r=0; r<rowNumber; ++r)
	{	if(r!=row)
			factor = (1.0-static_cast<double>(percent)/100.0) * static_cast<double>(p.GetCell(r,0)->real.height)/static_cast<double>(totalHeight);
		else
			factor = static_cast<double>(percent)/100.0;
			// 
		for(int c=0; c<colNumber; ++c)
			p.GetCell(r,c)->real.factor.height = factor;
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Set width for every column (number items in 'percent'==GetNumberColumn()).
// 
void SplitterCtrl::SetColumnWidthsForDynamic(int const *percent/*in*/)
{	assert(p.m_ResizeMode==ResizeDynamic);
		// 
	const int row = GetNumberRow();
	const int col = GetNumberColumn();
	assert(row>0 && col>0);
		// 
	__try
	{	int count = 0;
		for(int c=0; c<col; ++c)
			count += percent[c];
		assert(count==100);
		if(count!=100) return;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{	assert(!"count items in 'percent' != GetNumberColumn()");
		return;
	}
		// 
	int width, tail=0;
		// 
	for(int c=0; c<col; ++c)
	{	const double factor = static_cast<double>(percent[c])/100.0;
			// 
		if(c!=col-1)
		{	width = static_cast<int>(static_cast<double>(p.m_iTotalWidth) * factor + 0.5);
			tail += width;
		}
		else
			width = p.m_iTotalWidth - tail;
			// 
		for(int r=0; r<row; ++r)
		{	p.GetCell(r,c)->real.width = width;
			p.GetCell(r,c)->real.factor.width = factor;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// Set height for every row (number items in 'percent'==GetNumberRow()).
// 
void SplitterCtrl::SetRowHeightsForDynamic(int const *percent/*in*/)
{	assert(p.m_ResizeMode==ResizeDynamic);
		// 
	const int row = GetNumberRow();
	const int col = GetNumberColumn();
	assert(row>0 && col>0);
		// 
	__try
	{	int count = 0;
		for(int r=0; r<row; ++r)
			count += percent[r];
		assert(count==100);
		if(count!=100) return;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{	assert(!"count items in 'percent' != GetNumberRow()");
		return;
	}
		// 
	int height, tail=0;
		// 
	for(int r=0; r<row; ++r)
	{	const double factor = static_cast<double>(percent[r])/100.0;
			// 
		if(r!=row-1)
		{	height = static_cast<int>(static_cast<double>(p.m_iTotalHeight) * factor + 0.5);
			tail += height;
		}
		else
			height = p.m_iTotalHeight - tail;
			// 
		for(int c=0; c<col; ++c)
		{	p.GetCell(r,c)->real.height = height;
			p.GetCell(r,c)->real.factor.height = factor;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetRowsEqualHeight()
{	const int row = GetNumberRow();
	const int col = GetNumberColumn();
	assert(row>0 && col>0);
		// 
	int height, tail=0;
	const double factor = 1.0/static_cast<double>(row);
		// 
	for(int r=0; r<row; ++r)
	{	if(r!=row-1)
		{	height = static_cast<int>(static_cast<double>(p.m_iTotalHeight) * factor + 0.5);
			tail += height;
		}
		else	// last row.
			height = p.m_iTotalHeight-tail;
			// 
		for(int c=0; c<col; ++c)
		{	p.GetCell(r,c)->real.height = height;
			p.GetCell(r,c)->real.factor.height = factor;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetColumnsEqualWidth()
{	const int row = GetNumberRow();
	const int col = GetNumberColumn();
	assert(row>0 && col>0);
		// 
	int width, tail=0;
	const double factor = 1.0/static_cast<double>(col);
		// 
	for(int c=0; c<col; ++c)
	{	if(c!=col-1)
		{	width = static_cast<int>(static_cast<double>(p.m_iTotalWidth) * factor + 0.5);
			tail += width;
		}
		else	// last column.
			width = p.m_iTotalWidth-tail;
			// 
		for(int r=0; r<row; ++r)
		{	p.GetCell(r,c)->real.width = width;
			p.GetCell(r,c)->real.factor.width = factor;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetWindowMinWidth(int width)
{	assert(width>=0);
	p.m_iMinWinWidth = width;
}
// 
int SplitterCtrl::GetWindowMinWidth() const
{	return p.m_iMinWinWidth;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetWindowMinHeight(int height)
{	assert(height>=0);
	p.m_iMinWinHeight = height;
}
// 
int SplitterCtrl::GetHeightMinWindow() const
{	return p.m_iMinWinHeight;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
int SplitterCtrl::GetNumberRow() const
{	return p.m_Matrix.GetNumberRow();
}
// 
int SplitterCtrl::GetNumberColumn() const
{	return p.m_Matrix.GetNumberColumn();
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetSnapMode(Snap mode)
{	p.m_SnapMode = mode;
}
// 
SplitterCtrl::Snap SplitterCtrl::GetSnapMode() const
{	return p.m_SnapMode;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetResizeMode(Resize mode)
{	p.m_ResizeMode = mode;
}
// 
SplitterCtrl::Resize SplitterCtrl::GetResizeMode() const
{	return p.m_ResizeMode;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SetDraggingMode(Dragging mode)
{	p.m_DraggingMode = mode;
}
// 
SplitterCtrl::Dragging SplitterCtrl::GetDraggingMode() const
{	return p.m_DraggingMode;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
bool SplitterCtrl::IsDragging() const
{	return p.m_bDragHorz || p.m_bDragVert || p.m_bDragCross;
}
// 
void SplitterCtrl::GetDraggingState(bool *dragHorz/*out,or null*/, bool *dragVert/*out,or null*/, bool *dragCross/*out,or null*/) const
{	if(dragHorz)
		*dragHorz = p.m_bDragHorz;
	if(dragVert)
		*dragVert = p.m_bDragVert;
	if(dragCross)
		*dragCross = p.m_bDragCross;
}
// 
void SplitterCtrl::CancelDragging()
{	p.StopDragging(true);
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::ActivateHorzSplitter(int idx, bool active)
{	assert(idx>=0 && idx<GetNumberRow()-1);
	assert(GetNumberColumn()>0);
		// 
	p.GetCell(idx,0)->splitterActive.horz = active;
}
// 
bool SplitterCtrl::IsHorzSplitterActive(int idx) const
{	assert(idx>=0 && idx<GetNumberRow()-1);
	assert(GetNumberColumn()>0);
		// 
	return p.GetCell(idx,0)->splitterActive.horz;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::ActivateVertSplitter(int idx, bool active)
{	assert(GetNumberRow()>0);
	assert(idx>=0 && idx<GetNumberColumn()-1);
		// 
	p.GetCell(0,idx)->splitterActive.vert = active;
}
// 
bool SplitterCtrl::IsVertSplitterActive(int idx) const
{	assert(GetNumberRow()>0);
	assert(idx>=0 && idx<GetNumberColumn()-1);
		// 
	return p.GetCell(0,idx)->splitterActive.vert;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::ShowBorder(bool show)
{	p.m_bShowBorder = show;
}
// 
bool SplitterCtrl::IsBorderVisible() const
{	return p.m_bShowBorder;
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Save state in registry.
// 
bool SplitterCtrl::SaveState(CWinApp *app, TCHAR const *section, TCHAR const *entry) const
{	assert(app && section && entry);
		// 
	try
	{	CMemFile file;
		CArchive ar(&file,CArchive::store);
		SaveState(&ar);
		ar.Flush();
			// 
		const UINT dataSize = static_cast<UINT>( file.GetLength() );
		BYTE *data = file.Detach();
			// 
		if( !app->WriteProfileBinary(section,entry,data,dataSize) )
		{	free(data);
			return false;
		}
		free(data);
	}
	catch(CMemoryException* pEx)
	{	pEx->Delete();
		return false;
	}
	catch(CArchiveException* pEx)
	{	pEx->Delete();
		return false;
	}
	catch(...)
	{	return false;
	}
		// 
	return true;
}
/////////////////////////////////////////////////////////////////////////////
// Load state from registry.
// 
bool SplitterCtrl::LoadState(CWinApp *app, TCHAR const *section, TCHAR const *entry)
{	assert(app && section && entry);
		//
	bool res = false;
	BYTE *data = nullptr;
	UINT dataSize;
		// 
	try
	{	if( app->GetProfileBinary(section,entry,&data/*out*/,&dataSize/*out*/) )
		{	CMemFile file(data,dataSize);
			CArchive ar(&file,CArchive::load);
			LoadState(&ar);
			res = true;
		}
	}
	catch(CMemoryException* pEx)
	{	pEx->Delete();
	}
	catch(CArchiveException* pEx)
	{	pEx->Delete();
	}
	catch(...)
	{
	}
	if(data)
		delete [] data;
		// 
	return res;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrl::SaveState(CArchive *ar) const
{	const int row = GetNumberRow();
	const int col = GetNumberColumn();
		// 
	*ar << row;
	*ar << col;
		// 
	if(row>0 && col>0)
	{	for(int c=0; c<col; ++c)
		{	*ar << p.GetCell(0,c)->real.width;
			*ar << p.GetCell(0,c)->real.factor.width;
		}
			// 
		for(int r=0; r<row; ++r)
		{	*ar << p.GetCell(r,0)->real.height;
			*ar << p.GetCell(r,0)->real.factor.height;
		}
	}
}
// 
void SplitterCtrl::LoadState(CArchive *ar)
{	int row, col;
		// 
	*ar >> row;
	*ar >> col;
		// 
	assert(row==GetNumberRow() && col==GetNumberColumn());
		// 
	if(row>0 && col>0)
	{	int width,height;
		double factorWidth, factorHeight;
			// 
		for(int c=0; c<col; ++c)
		{	*ar >> width;
			*ar >> factorWidth;
				// 
			for(int r=0; r<row; ++r)
			{	p.GetCell(r,c)->real.width = width;
				p.GetCell(r,c)->real.factor.width = factorWidth;
			}
		}
			// 
		for(int r=0; r<row; ++r)
		{	*ar >> height;
			*ar >> factorHeight;
				// 
			for(int c=0; c<col; ++c)
			{	p.GetCell(r,c)->real.height = height;
				p.GetCell(r,c)->real.factor.height = factorHeight;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SplitterCtrlStyle1.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle1::Install(SplitterCtrl *ctrl)
{	ctrl->SetRecalcManager(this);
	ctrl->SetDrawManager(this);
}
/////////////////////////////////////////////////////////////////////////////
// 
int SplitterCtrlStyle1::GetBorderWidth(SplitterCtrl const *ctrl, IRecalc *base)
{	return base->GetBorderWidth(ctrl,nullptr);
}
// 
int SplitterCtrlStyle1::GetVertSplitterWidth(SplitterCtrl const *ctrl, IRecalc *base)
{	return base->GetVertSplitterWidth(ctrl,nullptr) + IsInnerBorderVisible()*2/*inner borders*/;
}
// 
int SplitterCtrlStyle1::GetHorzSplitterHeight(SplitterCtrl const *ctrl, IRecalc *base)
{	return base->GetHorzSplitterHeight(ctrl,nullptr) + IsInnerBorderVisible()*2/*inner borders*/;
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle1::DrawSplitterDots(CDC *dc, CRect const *rect, bool horz, int number, int size, COLORREF color) const
{	if(horz)
	{	int x = rect->CenterPoint().x - (size*(number+number-1))/2;
		const int y = rect->CenterPoint().y - size/2;
			// 
		for(; number-->0; )
		{	dc->FillSolidRect(x,y,size,size,color);
			x += 2 * size;
		}
	}
	else
	{	const int x = rect->CenterPoint().x - size/2;
		int y = rect->CenterPoint().y - (size*(number+number-1))/2;
			// 
		for(; number-->0; )
		{	dc->FillSolidRect(x,y,size,size,color);
			y += 2 * size;
		}
	}
}
// 
void SplitterCtrlStyle1::DrawSplitter(SplitterCtrl const * /*ctrl*/, CDC *dc, bool horz, int /*idx*/, CRect const *rect)
{	dc->FillSolidRect(rect, GetBackgroundColor() );
	if( IsDotsVisible() )
		DrawSplitterDots(dc,rect, horz, 6,2, GetDotsColor() );
}
/////////////////////////////////////////////////////////////////////////////
//
void SplitterCtrlStyle1::DrawDragRect(SplitterCtrl const * /*ctrl*/, CDC *dc, bool horz, bool firstTime, CRect const *rectOld, CRect const *rectNew)
{	CRect rcOld=*rectOld, rcNew=*rectNew;
	if( IsInnerBorderVisible() )
		if(horz)
		{	rcOld.DeflateRect(0,1);
			rcNew.DeflateRect(0,1);
		}
		else
		{	rcOld.DeflateRect(1,0);
			rcNew.DeflateRect(1,0);
		}
	CBrush *brush = GetDragBrush();
	dc->DrawDragRect(&rcNew, CSize(rcNew.Width(),rcNew.Height()),
		(firstTime ? nullptr : &rcOld), CSize(rcOld.Width(),rcOld.Height()), brush,brush);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle1::DrawBorder(SplitterCtrl const * /*ctrl*/, CDC *dc, CRect const *rect)
{	const COLORREF clr = GetOuterBorderColor();
	dc->Draw3dRect(rect,clr,clr);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle1::DrawEnd(SplitterCtrl const *ctrl, CDC *dc)
{	if( IsInnerBorderVisible() )
	{	const SplitterCtrl::Snap snap = ctrl->GetSnapMode();
		const int row = ctrl->GetNumberRow();
		const int col = ctrl->GetNumberColumn();
			// 
		CPen penBorder(PS_SOLID,1, GetInnerBorderColor() );
		CPen *oldPen = dc->SelectObject(&penBorder);
			// 
		for(int r=0; r<row; ++r)
			for(int c=0; c<col; ++c)
			{	RECT rc = ctrl->GetWindowRect(r,c);
				rc.left--;
				rc.top--;
					// top.
				if((r>0 && (snap==SplitterCtrl::SnapLeftTop || snap==SplitterCtrl::SnapRightTop)) ||
					(r<row-1 && (snap==SplitterCtrl::SnapLeftBottom || snap==SplitterCtrl::SnapRightBottom)))
				{
					dc->MoveTo(rc.left, rc.top);
					dc->LineTo(rc.right, rc.top);
				}
					// bottom.
				if((r<row-1 && (snap==SplitterCtrl::SnapLeftTop || snap==SplitterCtrl::SnapRightTop)) ||
					(r>0 && (snap==SplitterCtrl::SnapLeftBottom || snap==SplitterCtrl::SnapRightBottom)))
				{
					dc->MoveTo(rc.left, rc.bottom);
					dc->LineTo(rc.right, rc.bottom);
				}
					// left.
				if((c>0 && (snap==SplitterCtrl::SnapLeftTop || snap==SplitterCtrl::SnapLeftBottom)) ||
					(c<col-1 && (snap==SplitterCtrl::SnapRightTop || snap==SplitterCtrl::SnapRightBottom)))
				{
					dc->MoveTo(rc.left, rc.top);
					dc->LineTo(rc.left, rc.bottom);
				}
					// right.
				if((c<col-1 && (snap==SplitterCtrl::SnapLeftTop || snap==SplitterCtrl::SnapLeftBottom)) ||
					(c>0 && (snap==SplitterCtrl::SnapRightTop || snap==SplitterCtrl::SnapRightBottom)))
				{
					dc->MoveTo(rc.right, rc.top);
					dc->LineTo(rc.right, rc.bottom+1);
				}
			}
			// 
		dc->SelectObject(oldPen);
	}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SplitterCtrlStyle7.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle7::DrawGradient(CDC *dc, CRect const *rc, bool horz, COLORREF clrTop, COLORREF clrBottom) const
{	GRADIENT_RECT gRect = {0,1};
	TRIVERTEX vert[2] = 
	{	{rc->left,rc->top,static_cast<COLOR16>((GetRValue(clrTop) << 8)),static_cast<COLOR16>((GetGValue(clrTop) << 8)),static_cast<COLOR16>((GetBValue(clrTop) << 8)),0},
		{rc->right,rc->bottom,static_cast<COLOR16>((GetRValue(clrBottom) << 8)),static_cast<COLOR16>((GetGValue(clrBottom) << 8)),static_cast<COLOR16>((GetBValue(clrBottom) << 8)),0}
	};
	::GradientFill(dc->m_hDC,vert,2,&gRect,1,(horz ? GRADIENT_FILL_RECT_H : GRADIENT_FILL_RECT_V));
}
// 
void SplitterCtrlStyle7::DrawSplitter(SplitterCtrl const * /*ctrl*/, CDC *dc, bool horz, int /*idx*/, CRect const *rect)
{	DrawGradient(dc,rect,!horz,RGB(245,245,245),RGB(160,165,170));
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle7::DrawEnd(SplitterCtrl const *ctrl, CDC *dc)
{	const int row = ctrl->GetNumberRow() - 1;
	const int col = ctrl->GetNumberColumn() - 1;
		// 
	for(int r=0; r<row; ++r)
		for(int c=0; c<col; ++c)
		{	RECT rcRes;
			const RECT rcH = ctrl->GetSplitterRect(true,r);
			const RECT rcV = ctrl->GetSplitterRect(false,c);
			::IntersectRect(&rcRes/*out*/,&rcH,&rcV);
				// 
			dc->FillSolidRect(&rcRes, RGB(190,190,190) );
		}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// SplitterCtrlStyle8.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle8::DrawSplitter(SplitterCtrl const * /*ctrl*/, CDC *dc, bool /*horz*/, int /*idx*/, CRect const *rect)
{	dc->DrawEdge(const_cast<CRect *>(rect),EDGE_RAISED,BF_RECT);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle8::DrawBorder(SplitterCtrl const * /*ctrl*/, CDC *dc, CRect const *rect)
{	dc->DrawEdge(const_cast<CRect *>(rect),EDGE_SUNKEN,BF_RECT);
}
/////////////////////////////////////////////////////////////////////////////
// 
void SplitterCtrlStyle8::DrawEnd(SplitterCtrl const *ctrl, CDC *dc)
{	const int row = ctrl->GetNumberRow() - 1;
	const int col = ctrl->GetNumberColumn() - 1;
	const COLORREF clr = ::GetSysColor(COLOR_BTNFACE);
		// 
	for(int r=0; r<row; ++r)
		for(int c=0; c<col; ++c)
		{	CRect rcRes;
			const CRect rcH = ctrl->GetSplitterRect(true,r);
			const CRect rcV = ctrl->GetSplitterRect(false,c);
			::IntersectRect(&rcRes/*out*/,&rcH,&rcV);
				// 
			dc->FillSolidRect(rcRes.left,rcRes.top, rcRes.Width(),rcRes.Height()-1, clr);
			dc->FillSolidRect(rcRes.left,rcRes.top, rcRes.Width()-1,rcRes.Height(), clr);
		}
}
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

















