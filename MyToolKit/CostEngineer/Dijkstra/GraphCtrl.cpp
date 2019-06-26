// Dijkstra.cpp : Implementation of CGraphCtrl

#include "stdafx.h"
#include "GraphCtrl.h"
#include "Node.h"
#include "../GridCtrl_src/MemDC.h"
//#include <atlbase.h>
//#include <atlctl.h>
//#include <atlcom.h>
//#include <comutil.h>
//#include <comdef.h>

#define MAXDOUBLE   0xfffffffffffffffe

/////////////////////////////////////////////////////////////////////////////
// CGraphCtrl

IMPLEMENT_DYNCREATE(CGraphCtrl, CWnd)

BEGIN_MESSAGE_MAP(CGraphCtrl, CWnd)
	//EFW - Added ON_WM_RBUTTONUP
	//{{AFX_MSG_MAP(CGraphCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GRAPH_ADD_EDGE, &CGraphCtrl::OnGraphAddEdge)
	ON_COMMAND(ID_GRAPH_ADD_NODE, &CGraphCtrl::OnGraphAddNode)
	ON_COMMAND(ID_GRAPH_DELETE, &CGraphCtrl::OnGraphDelete)
	ON_COMMAND(ID_GRAPH_UPDATE, &CGraphCtrl::OnGraphUpdate)
END_MESSAGE_MAP()

void CGraphCtrl::OnPaint()
{
	CPaintDC dc(this);      // device context for painting

	if (m_bDoubleBuffer)    // Use a memory DC to remove flicker
	{
		CMemDC1 MemDC(&dc);
		OnDraw(&MemDC);
	}
	else                    // Draw raw - this helps in debugging vis problems.
		OnDraw(&dc);
}

void CGraphCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO : Add Code for message handler. Call DefWindowProc if necessary.
	return;
}


void CGraphCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_LastLButtonDownPosition.x = point.x;
	m_LastLButtonDownPosition.y = point.y;
	
	if (m_mode_addNodes)
		OnAddNode(point.x, point.y);
	else if (m_mode_addEdges)
		OnAddEdge(point.x, point.y);
	else if (m_mode_update)
		OnUpdate(point.x, point.y);
	else if (m_mode_delete)
		OnDelete(point.x, point.y);
	return;
}

void CGraphCtrl::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// TODO : Add Code for message handler. Call DefWindowProc if necessary.
	return;
}

void CGraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	return;
}

void CGraphCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO : Add Code for message handler. Call DefWindowProc if necessary.
	return;
}

void CGraphCtrl::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	CPoint pt = point;
	CMenu menu;
	CMenu* popupMenu = NULL;

	ScreenToClient(&pt);

	//加载菜单
	menu.LoadMenu(IDR_POPUP_COMPONENT);

	//子菜单项
	//右键点击 弹出此子菜单项
	popupMenu = menu.GetSubMenu(1);
	popupMenu->CheckMenuItem(ID_GRAPH_ADD_NODE, MF_BYCOMMAND | m_mode_addNodes ? MF_CHECKED : MF_UNCHECKED);
	popupMenu->CheckMenuItem(ID_GRAPH_ADD_EDGE, MF_BYCOMMAND | m_mode_addEdges ? MF_CHECKED : MF_UNCHECKED);
	popupMenu->CheckMenuItem(ID_GRAPH_UPDATE, MF_BYCOMMAND | m_mode_update ? MF_CHECKED : MF_UNCHECKED);
	popupMenu->CheckMenuItem(ID_GRAPH_DELETE, MF_BYCOMMAND | m_mode_delete ? MF_CHECKED : MF_UNCHECKED);

	popupMenu->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
}

// creates the control - use like any other window create control
BOOL CGraphCtrl::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	ASSERT(pParentWnd->GetSafeHwnd());

	if (!CWnd::Create(GRAPHCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID))
		return FALSE;

	//Initialise(); - called in PreSubclassWnd

	// The number of rows and columns will only be non-zero if the constructor
	// was called with non-zero initialising parameters. If this window was created
	// using a dialog template then the number of rows and columns will be 0 (which
	// means that the code below will not be needed - which is lucky 'cause it ain't
	// gonna get called in a dialog-template-type-situation.

	return TRUE;
}

void CGraphCtrl::OnDraw(CDC* pDC)
{
	//RECT& rc = *(RECT*)di.prcBounds;
	CRect rc;
	GetClientRect(rc);
	
	pDC->Rectangle(rc.left, rc.top, rc.right, rc.bottom);
	HPEN pen=CreatePen(PS_SOLID,0,RGB(0,0,0));
	HPEN penred=CreatePen(PS_SOLID,2,RGB(255,0,0));
	HBRUSH brush=CreateSolidBrush(RGB(0,0,0));
	HPEN oldpen;
	HPEN oldbrush;
	oldpen=(HPEN)pDC->SelectObject(pen);

	HFONT OldFont = (HFONT)pDC->SelectObject(m_lmfont);

	// draw the nodes and the text on the nodes
	long nr = 0;
	VTYPE_NODE::iterator kl;
	for(kl=g.m_nodes.begin(); kl<g.m_nodes.end(); kl++)
	{
		char s[10];
		ltoa((*kl).m_NodeNr + 1, s, 10);
		pDC->Ellipse((*kl).m_p.x-20, (*kl).m_p.y-20, (*kl).m_p.x+20, (*kl).m_p.y+20);
		if(nr<9)
			pDC->TextOut((*kl).m_p.x-5, (*kl).m_p.y-8, s, 1);
		else
			pDC->TextOut((*kl).m_p.x-5, (*kl).m_p.y-8, s, 2);
		nr++;
	}	
	oldbrush=(HPEN)pDC->SelectObject(brush);

	// draw the edges and the text on the edges
	VTYPE_EDGE::iterator kll;
	for(kll=g.m_edges.begin(); kll<g.m_edges.end(); kll++)
	{
		HPEN temp;
		if((*kll).m_red)
			temp=(HPEN)pDC->SelectObject(penred);
		pDC->MoveTo((*kll).m_firstPct.x, (*kll).m_firstPct.y);
		pDC->LineTo((*kll).m_secondPct.x, (*kll).m_secondPct.y);
		pDC->Ellipse((*kll).m_secondPct.x-5, (*kll).m_secondPct.y-5, (*kll).m_secondPct.x+5, (*kll).m_secondPct.y+5);
		// calcul middle pint
		POINT po;
		po.x = ((*kll).m_firstPct.x+(*kll).m_secondPct.x)/2;
		po.y = ((*kll).m_firstPct.y+(*kll).m_secondPct.y)/2;
		char s[5];
		ltoa((*kll).m_cost, s, 10);
		pDC->TextOut(po.x, po.y, s, 1);
		if((*kll).m_red)
			pDC->SelectObject(temp);
	}
	pDC->SelectObject(OldFont);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
	DeleteObject(pen);
	DeleteObject(brush);

}

void CGraphCtrl::ReleaseAll()
{
	if(m_lmfont)
		::DeleteObject(m_lmfont);
}

void CGraphCtrl::initGraph() {
	g.m_edges.clear();
	g.m_nodes.clear();
}

void CGraphCtrl::AddNode(long x, long y) {
	CNode node;
	node.m_p.x = x;
	node.m_p.y = y;
	node.m_NodeNr = g.GetNrNodes();
	g.m_nodes.push_back(node);
}


void  CGraphCtrl::AddEdge(int from, int to) {
	CEdge ed;
	ed.m_firstNode = from;
	ed.m_secondNode = to;

	ed.m_firstPct.x = g.m_nodes[from].m_p.x;
	ed.m_firstPct.y = g.m_nodes[from].m_p.y;
	ed.m_secondPct.x = g.m_nodes[to].m_p.x;
	ed.m_secondPct.y = g.m_nodes[to].m_p.y;
	g.m_edges.push_back(ed);
}


void CGraphCtrl::Refresh()
{
	if (GetSafeHwnd())
		Invalidate();
}



// used in getedge to find graphically the node
int CGraphCtrl::GetNode(long x, long y)
{
	VTYPE_NODE::iterator kl;
	for(kl=g.m_nodes.begin(); kl<g.m_nodes.end(); kl++)
	{
		if(x<(*kl).m_p.x+15 && x>(*kl).m_p.x-15 && y<(*kl).m_p.y+15 && y>(*kl).m_p.y-15)
			return (*kl).m_NodeNr;
	}
	return -1;
}


void CGraphCtrl::Relax(CNode u, CNode v, double w)
{
	if(g.d[v.m_NodeNr-1]>g.d[u.m_NodeNr-1]+w)
	{
		g.d[v.m_NodeNr-1] = g.d[u.m_NodeNr-1]+w;
		g.pi[v.m_NodeNr-1] = u.m_NodeNr;
	}
}

double CGraphCtrl::GetEdgeVal(CNode u, CNode v)
{
	VTYPE_EDGE::iterator kl;
	for(kl=g.m_edges.begin(); kl<g.m_edges.end(); kl++)
	{
		if(((*kl).m_firstNode == u.m_NodeNr && (*kl).m_secondNode == v.m_NodeNr))/* ||
			((*kl).m_firstNode == v.m_NodeNr && (*kl).m_secondNode == u.m_NodeNr))*/
			return (*kl).m_cost;
	}
	return 0;
}

bool CGraphCtrl::ExistEdge(CNode u, CNode v)
{
	VTYPE_EDGE::iterator kl;
	for(kl=g.m_edges.begin(); kl<g.m_edges.end(); kl++)
	{
		if(((*kl).m_firstNode == u.m_NodeNr && (*kl).m_secondNode == v.m_NodeNr))/* ||
			((*kl).m_firstNode == v.m_NodeNr && (*kl).m_secondNode == u.m_NodeNr))*/
			return true;
	}
	return false;
}


void CGraphCtrl::OnGraphAddEdge()
{
	// TODO: 在此添加命令处理程序代码
	m_mode_addEdges = true;
	m_mode_addNodes = false;
	m_mode_update = false;
	m_mode_delete = false;
}


void CGraphCtrl::OnGraphAddNode()
{
	// TODO: 在此添加命令处理程序代码
	m_mode_addEdges = false;
	m_mode_addNodes = true;
	m_mode_update = false;
	m_mode_delete = false;
}


void CGraphCtrl::OnGraphDelete()
{
	// TODO: 在此添加命令处理程序代码
	m_mode_addEdges = false;
	m_mode_addNodes = false;
	m_mode_update = false;
	m_mode_delete = true;
}


void CGraphCtrl::OnGraphUpdate()
{
	// TODO: 在此添加命令处理程序代码
	m_mode_addEdges = false;
	m_mode_addNodes = false;
	m_mode_update = true;
	m_mode_delete = false;
}

void CGraphCtrl::OnAddNode(long x, long y)
{
	NM_GRAPH_ADD_NODE_STRUCT nmgv;
	nmgv.hdr.hwndFrom = m_hWnd;
	nmgv.hdr.idFrom = GetDlgCtrlID();
	nmgv.hdr.code = NM_GRAPH_ADD_NODE;

	nmgv.x = x;
	nmgv.y = y;

	CWnd *pOwner = GetOwner();
	if (pOwner && IsWindow(pOwner->m_hWnd))
		pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
}

void CGraphCtrl::OnAddEdge(long x, long y)
{
	MSG msg;
	HDC hdc = ::GetDC(m_hWnd);
	SetROP2(hdc, R2_NOTXORPEN);
	HPEN pen = CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
	HPEN oldpen;
	//POINT Apct;
	long pox = -1, poy = -1;
	oldpen = (HPEN)SelectObject(hdc, pen);
	bool TrackFinished = false;

	long firstnode = GetNode(x, y);
	long secondnode;

	// while mouse not up try to find the nodes between which it will draw the edge
	while (!TrackFinished)
	{
		GetMessage(&msg, m_hWnd, 0x200, 0x204);
		if (msg.message == WM_MOUSEMOVE)
		{
			int poxT = ((int)LOWORD(msg.lParam));
			int poyT = ((int)HIWORD(msg.lParam));
			if (pox >= 0 || poy >= 0) {
				/* 擦除旧线 */
				MoveToEx(hdc, x, y, NULL);
				LineTo(hdc, pox, poy);
			}
			
			/* 画新线 */
			MoveToEx(hdc, x, y, NULL);
			LineTo(hdc, poxT, poyT);
			short fwKeys = short(UINT(msg.wParam));
			pox = poxT;
			poy = poyT;
		}
		if (msg.message == WM_LBUTTONUP)
		{
			int poxT = ((int)LOWORD(msg.lParam));
			int poyT = ((int)HIWORD(msg.lParam));
			secondnode = GetNode(poxT, poyT);
			TrackFinished = true;
			if (firstnode >= 0 && secondnode >= 0)
			{
				NM_GRAPH_ADD_EDGE_STRUCT ed;
				ed.hdr.hwndFrom = m_hWnd;
				ed.hdr.idFrom = GetDlgCtrlID();
				ed.hdr.code = NM_GRAPH_ADD_EDGE;

				ed.m_firstNode = firstnode;
				ed.m_firstPct.x = x;
				ed.m_firstPct.y = y;
				ed.m_secondNode = secondnode;
				ed.m_secondPct.x = poxT;
				ed.m_secondPct.y = poyT;

				CWnd *pOwner = GetOwner();
				if (pOwner && IsWindow(pOwner->m_hWnd))
					pOwner->SendMessage(WM_NOTIFY, ed.hdr.idFrom, (LPARAM)&ed);
			}
			else {
				Refresh();
			}
		}
	}

	SelectObject(hdc, oldpen);
	SetROP2(hdc, R2_COPYPEN);
	DeleteObject(pen);
	::ReleaseDC(m_hWnd, hdc);

	// Refresh();
}


void CGraphCtrl::OnUpdate(long x, long y) {
	NM_GRAPH_DEL_EDIT_STRUCT nmgv;
	nmgv.hdr.hwndFrom = m_hWnd;
	nmgv.hdr.idFrom = GetDlgCtrlID();
	nmgv.hdr.code = NM_GRAPH_EDIT_NODE;

	nmgv.idx = GetNode(x, y);
	if (nmgv.idx >= 0) {
		CWnd *pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd))
			pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
	}
}

void CGraphCtrl::OnDelete(long x, long y) {
	NM_GRAPH_DEL_EDIT_STRUCT nmgv;
	nmgv.hdr.hwndFrom = m_hWnd;
	nmgv.hdr.idFrom = GetDlgCtrlID();
	nmgv.hdr.code = NM_GRAPH_DEL_NODE;

	nmgv.idx = GetNode(x, y);
	if (nmgv.idx >= 0) {
		CWnd *pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd))
			pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
	}
}