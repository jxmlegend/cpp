#include "stdafx.h"
#include "ActivityOnArrow.h"
#include "GridDlg.h"


CAOANode::CAOANode()
{
	m_name = "";
	m_x = 0;
	m_y = 0;
}


CAOANode::~CAOANode()
{
}



void CAOANode::Serialize(CArchive& ar, double version) {
	if (ar.IsStoring()) {
		ar << m_name;
		ar << m_x;
		ar << m_y;
	}
	else {
		ar >> m_name;
		ar >> m_x;
		ar >> m_y;
	}
}



bool CAOANode::CreateOrUpdate(string menuCode) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	CDyncItemGroupDlg infd;
	infd.CXCAPTION = 50;
	infd.GROUP_NUM_PER_LINE = 3;
	int i = 0;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("����"), 64);
	if (!m_name.IsEmpty())
		infd.m_vecFindItem[0][i][0].strItem = m_name;


	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("x"), 64);
	if (m_x > 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%d", m_x);
	infd.m_vecFindItem[0][i][0].nMin = 1;
	infd.m_vecFindItem[0][i][0].nMax = 100000000;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("y"), 64);
	if (m_y > 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%d", m_y);
	infd.m_vecFindItem[0][i][0].nMin = 1;
	infd.m_vecFindItem[0][i][0].nMax = 100000000;

	infd.Init(_T("ͼ�ڵ� ��������"), _T("ͼ�ڵ� ��������"));
	if (infd.DoModal() == IDOK) {
		i = 0;
		m_name = infd.m_vecFindItem[0][i++][0].strItem;
		m_x = atoi(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		m_y = atoi(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		
		return true;
	}
	return false;
}

/**********************************************************************/

CAOAEdge::CAOAEdge()
{
	m_name = "";
	m_from_node = -1;
	m_to_node = -1;
	m_duration = -1;
}


CAOAEdge::~CAOAEdge()
{
}



void CAOAEdge::Serialize(CArchive& ar, double version) {
	if (ar.IsStoring()) {
		ar << m_name;
		ar << m_from_node;
		ar << m_to_node;
		ar << m_duration;
	}
	else {
		ar >> m_name;
		ar >> m_from_node;
		ar >> m_to_node;
		ar >> m_duration;
	}
}


bool CAOAEdge::CreateOrUpdate(string menuCode) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	CDyncItemGroupDlg infd;
	infd.CXCAPTION = 50;
	infd.GROUP_NUM_PER_LINE = 3;
	int i = 0;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("����"), 64);
	if (!m_name.IsEmpty())
		infd.m_vecFindItem[0][i][0].strItem = m_name;


	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("��ʼ�ڵ�"), 64);
	if (m_from_node >= 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%d", m_from_node);
	infd.m_vecFindItem[0][i][0].nMin = 0;
	infd.m_vecFindItem[0][i][0].nMax = 100000000;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("�����ڵ�"), 64);
	if (m_to_node >= 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%d", m_to_node);
	infd.m_vecFindItem[0][i][0].nMin = 0;
	infd.m_vecFindItem[0][i][0].nMax = 100000000;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("����ʱ��"), 64);
	if (m_duration >= 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%d", m_duration);
	infd.m_vecFindItem[0][i][0].nMin = 0;
	infd.m_vecFindItem[0][i][0].nMax = 100000000;

	
	infd.Init(_T("ͼ�� ��������"), _T("ͼ�� ��������"));
	if (infd.DoModal() == IDOK) {
		i = 0;
		m_name = infd.m_vecFindItem[0][i++][0].strItem;
		m_from_node = atoi(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		m_to_node = atoi(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		m_duration = atoi(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		
		return true;
	}
	return false;
}

/**********************************************************************/

/* ˫��������ͼ */
string CActivityOnArrow::m_ObjectCode = "01040701";
double CActivityOnArrow::m_ObjectVersion = 1.0;

CActivityOnArrow::CActivityOnArrow()
{
	m_name = "";
}


CActivityOnArrow::~CActivityOnArrow()
{
}

void CActivityOnArrow::Serialize(CArchive& ar, double version) {
	if (ar.IsStoring()) {
		ar << m_name;
		
		ar << m_nodes.size();
		for (int i = 0; i < m_nodes.size(); i++) {
			m_nodes[i].Serialize(ar, version);
		}

		ar << m_edges.size();
		for (int i = 0; i < m_edges.size(); i++) {
			m_edges[i].Serialize(ar, version);
		}
	}
	else {
		ar >> m_name;
		
		int nNum;
		ar >> nNum;
		for (int i = 0; i < nNum; i++) {
			CAOANode bs;
			bs.Serialize(ar, version);
			m_nodes.push_back(bs);
		}

		ar >> nNum;
		for (int i = 0; i < nNum; i++) {
			CAOAEdge bs;
			bs.Serialize(ar, version);
			m_edges.push_back(bs);
		}
	}
}

bool CActivityOnArrow::CreateOrUpdate(string menuCode) {

	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	CDyncItemGroupDlg infd;
	infd.CXCAPTION = 50;
	infd.GROUP_NUM_PER_LINE = 3;
	int i = 0;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("����ͼ����"), 64);
	if(!m_name.IsEmpty())
		infd.m_vecFindItem[0][i][0].strItem = m_name;

		
	infd.Init(_T("˫��������ͼ ��������"), _T("˫��������ͼ ��������"));
	if (infd.DoModal() == IDOK) {
		i = 0;
		m_name = infd.m_vecFindItem[0][i++][0].strItem;
		
		return true;
	}
	return false;
}



bool CActivityOnArrow::Draw(string menuCode, CGridCtrl* pGridCtrl, vector<CActivityOnArrow>& cols) {
	if (!pGridCtrl)
		return false;

	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	try {
		pGridCtrl->SetRowCount(cols.size() + 1);
		pGridCtrl->SetColumnCount(3 + 3);		//	������������ �� ���/�޸�/ɾ��
		pGridCtrl->SetFixedRowCount(1);
		pGridCtrl->SetFixedColumnCount(1);
		pGridCtrl->SetHeaderSort(TRUE);
	}
	catch (CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
		return false;
	}

	for (int row = 0; row < pGridCtrl->GetRowCount(); row++)
	{
		for (int col = 0; col < pGridCtrl->GetColumnCount(); col++)
		{
			GV_ITEM Item;
			Item.mask = GVIF_TEXT | GVIF_FORMAT;
			Item.row = row;
			Item.col = col;
			Item.nMargin = 10;
			string val;

			if (row < 1) {
				Item.nFormat = DT_LEFT | DT_WORDBREAK;

				if (col == 0)		val = "";
				else if (col == 1)	val = "����ͼ����";
				else if (col == 2)	val = "�ڵ���";
				else if (col == 3)	val = "����";
								
				else if (col == 4)	val = "";
				else if (col == 5)	val = "";
				
				Item.strText.Format(_T("%s"), val.c_str());
			}
			else
			{
				if (col <= 2)
					Item.nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				else
					Item.nFormat = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

				if (col >= 2 && col <= 3 || col == 0 )
				{
					if (!pGridCtrl->SetCellType(row, col, RUNTIME_CLASS(CGridCellNumeric)))
						return false;
				}
				if (col == 4 ) {
					Item.crFgClr = RGB(0, 120, 250);
					Item.mask |= GVIF_FGCLR;
				}
				if (col == 5) {
					Item.crFgClr = RGB(255, 0, 0);
					Item.mask |= GVIF_FGCLR;
				}

				
				if (col == 0)	val = "";
				else if (col == 1) 	val = cols[row - 1].m_name.GetBuffer();
				else if (col == 2)  val = Int2String(cols[row - 1].m_nodes.size());
				else if (col == 3)  val = Int2String(cols[row - 1].m_edges.size());
								
				else if (col == 4)	val = "�޸ģ�update��";
				else if (col == 5)	val = "ɾ����delete��";
		
				Item.strText.Format(_T("%s"), val.c_str());
			}
			pGridCtrl->SetItem(&Item);
		}
	}
	//pGridCtrl->AutoFill();
	pGridCtrl->Refresh();
	pGridCtrl->ExpandColumnsToFit();
	return true;
}

bool CActivityOnArrow::Update(string menuCode, int nRow, vector<CActivityOnArrow>& cols) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	if(nRow > 0 && nRow <= cols.size())
		return cols[nRow - 1].CreateOrUpdate(CActivityOnArrow::m_ObjectCode);
	return false;
}

bool CActivityOnArrow::Delete(string menuCode, int nRow, vector<CActivityOnArrow>& cols) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	if (nRow > 0 && nRow <= cols.size()) {
		int idx = 0;
		vector<CActivityOnArrow>::iterator it = cols.begin();
		for (; it != cols.end(); it++, idx++) {
			if (idx == nRow - 1)
				break;
		}
		cols.erase(it);
		return true;
	}
	return false;
}


unsigned int CActivityOnArrow::PopupMenuId(string menuCode) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return 0;
	return IDR_POPUP_COMPONENT;
}


bool CActivityOnArrow::DrawGraph(CGraphCtrl* pCtrl) {
	if (!pCtrl)
		return false;

	pCtrl->initGraph();
	for (int i = 0; i < m_nodes.size(); i++) {
		pCtrl->AddNode(m_nodes[i].m_x, m_nodes[i].m_y, m_nodes[i].m_name.GetBuffer());
	}

	for (int i = 0; i < m_edges.size(); i++) {
		if (m_edges[i].m_earliest_start < 0) {
			pCtrl->AddEdge(m_edges[i].m_from_node, m_edges[i].m_to_node,
				m_edges[i].m_name.GetBuffer(),
				m_edges[i].m_duration >= 0 ? Int2String(m_edges[i].m_duration) : "");
		}
		else {
			string tips = "�� : " + string(m_edges[i].m_name.GetBuffer()) + "\n";
			if (m_edges[i].m_earliest_start >= 0) tips += "���翪ʼʱ�� : " + Int2String(m_edges[i].m_earliest_start) + "\n";
			if (m_edges[i].m_earliest_finish >= 0) tips += "�������ʱ�� : " + Int2String(m_edges[i].m_earliest_finish) + "\n";
			if (m_edges[i].m_latest_start >= 0) tips += "������ʼʱ�� : " + Int2String(m_edges[i].m_latest_start) + "\n";
			if (m_edges[i].m_latest_finish >= 0) tips += "�������ʱ�� : " + Int2String(m_edges[i].m_latest_finish) + "\n";
			if (m_edges[i].m_total_float >= 0) tips += "��ʱ�� : " + Int2String(m_edges[i].m_total_float) + "\n";
			tips += "\r\n\r\n";

			pCtrl->AddEdge(m_edges[i].m_from_node, m_edges[i].m_to_node,
				m_edges[i].m_name.GetBuffer(),
				m_edges[i].m_duration >= 0 ? Int2String(m_edges[i].m_duration) : "", tips);
		}
		
	}
	pCtrl->Refresh();
	return true;
}

void CActivityOnArrow::InvalidateCaculate() {
	/* ��ʼ�� ���ݽṹ */
	for (int n = 0; n < m_nodes.size(); n++) {
		m_nodes[n].m_earliest_event_time = -1;
		m_nodes[n].m_latest_event_time = -1;
	}
	for (int e = 0; e < m_edges.size(); e++) {
		m_edges[e].m_earliest_start = -1;
		m_edges[e].m_earliest_finish = -1;
		m_edges[e].m_latest_start = -1;
		m_edges[e].m_latest_finish = -1;
		m_edges[e].m_total_float = -1;
		m_edges[e].m_free_float = -1;
	}
}

bool CActivityOnArrow::AddNode(string menuCode, int x, int y) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	CAOANode node;
	node.m_x = x;
	node.m_y = y;
	node.m_name.Format("%d", m_nodes.size() + 1);
	if (node.CreateOrUpdate(menuCode)) {
		m_nodes.push_back(node);
	}
	return true;
}

string DecimalTo26System(int x) {
	if (x < 0) return "";
	int a = x / 26;
	int b = x % 26;
	return DecimalTo26System(a - 1) + (char)(b + 'A');
}

bool CActivityOnArrow::AddEdge(string menuCode, int from, int to) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	CAOAEdge edge;
	edge.m_from_node = from;
	edge.m_to_node = to;
	edge.m_name = DecimalTo26System(m_edges.size()).c_str();
	if (edge.CreateOrUpdate(menuCode)) {
		m_edges.push_back(edge);
		InvalidateCaculate();
	}
	return true;
}

bool CActivityOnArrow::MoveNode(string menuCode, int nRow, int x, int y) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return false;

	m_nodes[nRow].m_x = x;
	m_nodes[nRow].m_y = y;
		
	return true;
}

bool CActivityOnArrow::UpdateNode(string menuCode, int nRow) {
	if (m_nodes[nRow].CreateOrUpdate(menuCode)) {
		InvalidateCaculate();
		return true;
	}
	return false;
}

bool CActivityOnArrow::UpdateEdge(string menuCode, int nRow) {
	if (m_edges[nRow].CreateOrUpdate(menuCode)) {
		InvalidateCaculate();
		return true;
	}
	return false;
}



bool CActivityOnArrow::DeleteNode(int nRow) {
	bool bDelete;
	do {
		bDelete = false;
		vector<CAOAEdge>::iterator it;
		for (it = m_edges.begin(); it != m_edges.end(); it++) {
			if (it->m_from_node == nRow || it->m_to_node == nRow) {
				m_edges.erase(it);
				bDelete = true;
				break;
			}
		}
	} while (bDelete);

	for (int i = 0; i < m_edges.size(); i++) {
		if (m_edges[i].m_from_node > nRow)
			m_edges[i].m_from_node = m_edges[i].m_from_node - 1;
		if (m_edges[i].m_to_node > nRow)
			m_edges[i].m_to_node = m_edges[i].m_to_node - 1;
	}

	vector<CAOANode>::iterator it;
	int idx = 0;
	for (it = m_nodes.begin(); it != m_nodes.end(); it++, idx++) {
		if (idx == nRow) {
			m_nodes.erase(it);
			bDelete = true;
			break;
		}
	}

	InvalidateCaculate();
	return true;
}

bool CActivityOnArrow::DeleteEdge(int nRow) {

	vector<CAOAEdge>::iterator it;
	int idx = 0;
	for (it = m_edges.begin(); it != m_edges.end(); it++, idx++) {
		if (idx == nRow) {
			m_edges.erase(it);
			break;
		}
	}
	InvalidateCaculate();
	return true;
}

void CActivityOnArrow::SteelQuantity(string menuCode, vector<CActivityOnArrow>& cols) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return;

}

void CActivityOnArrow::Calculate(string menuCode, vector<CActivityOnArrow>& cols) {
	if (menuCode != CActivityOnArrow::m_ObjectCode)
		return;

	for (int i = 0; i < cols.size(); i++) {
		/* Ѱ��ÿ��ͼ�� ��ʼ�ڵ� �� �����ڵ� */
		vector<int> beginNode;
		vector<int> endNode;
		for (int n = 0; n < cols[i].m_nodes.size(); n++) {
			bool bStart = false;
			bool bEnd = false;
			for (int e = 0; e < cols[i].m_edges.size(); e++) {
				if (cols[i].m_edges[e].m_from_node == n) bStart = true;
				if (cols[i].m_edges[e].m_to_node == n) bEnd = true;
			}

			if (bStart && !bEnd) {
				beginNode.push_back(n);
			}
			if (!bStart && bEnd) {
				endNode.push_back(n);
			}
		}
		/* �ж����ʼ�ڵ� �� �����ڵ㣬��ͼ���ϸ� */
		if (beginNode.size() > 1 || endNode.size() > 1 || beginNode.size() == 0 || endNode.size() == 0) continue;

		cols[i].InvalidateCaculate();

		vector<int> begin;
		vector<int> end;
		/* ������ڵ㡢����� ���翪ʼʱ�� */
		cols[i].m_nodes[beginNode[0]].m_earliest_event_time = 0;
		begin.push_back(beginNode[0]);
		do {
			end.clear();
			for (auto n : begin) {
				for (int e = 0; e < cols[i].m_edges.size(); e++) {
					if (cols[i].m_edges[e].m_from_node == n) {
						cols[i].m_edges[e].m_earliest_start = cols[i].m_nodes[n].m_earliest_event_time;
						cols[i].m_edges[e].m_earliest_finish = cols[i].m_nodes[n].m_earliest_event_time + cols[i].m_edges[e].m_duration;
						end.push_back(cols[i].m_edges[e].m_to_node);
					}
				}
			}
			
			/*  */
			begin.clear();
			for (auto n : end) {
				int earliest_event_time = -1;
				for (int e = 0; e < cols[i].m_edges.size(); e++) {
					if (cols[i].m_edges[e].m_to_node == n) {
						if (cols[i].m_edges[e].m_earliest_finish >= 0) {
							if (earliest_event_time < cols[i].m_edges[e].m_earliest_finish)
								earliest_event_time = cols[i].m_edges[e].m_earliest_finish;
						}
						else {
							/*  */
							earliest_event_time = -1;
							break;
						}
					}
				}

				if (earliest_event_time >= 0) {
					cols[i].m_nodes[n].m_earliest_event_time = earliest_event_time;
					begin.push_back(n);
				}
			}

			/*  */
			if (cols[i].m_nodes[endNode[0]].m_earliest_event_time >= 0) {
				end.push_back(endNode[0]);
				cols[i].m_nodes[endNode[0]].m_latest_event_time = cols[i].m_nodes[endNode[0]].m_earliest_event_time;
				break;
			}

		} while (1);

		/*  */
		do {
			begin.clear();
			for (auto n : end) {
				for (int e = 0; e < cols[i].m_edges.size(); e++) {
					if (cols[i].m_edges[e].m_to_node == n) {
						cols[i].m_edges[e].m_latest_finish = cols[i].m_nodes[n].m_latest_event_time;
						cols[i].m_edges[e].m_latest_start = cols[i].m_nodes[n].m_latest_event_time - cols[i].m_edges[e].m_duration;
						begin.push_back(cols[i].m_edges[e].m_from_node);
					}
				}
			}
			

			/*  */
			end.clear();
			for (auto n : begin) {
				int latest_event_time = cols[i].m_nodes[endNode[0]].m_latest_event_time + 100;
				for (int e = 0; e < cols[i].m_edges.size(); e++) {
					if (cols[i].m_edges[e].m_from_node == n) {
						if (cols[i].m_edges[e].m_latest_start >= 0) {
							if (latest_event_time > cols[i].m_edges[e].m_latest_start)
								latest_event_time = cols[i].m_edges[e].m_latest_start;
						}
						else {
							/*  */
							latest_event_time = -1;
							break;
						}
					}
				}

				if (latest_event_time >= 0) {
					cols[i].m_nodes[n].m_latest_event_time = latest_event_time;
					end.push_back(n);
				}
			}

			/*  */
			if (cols[i].m_nodes[beginNode[0]].m_latest_event_time >= 0) {
				
				break;
			}
		} while (1);

		/*  */
		for (int e = 0; e < cols[i].m_edges.size(); e++) {
			cols[i].m_edges[e].m_total_float = cols[i].m_edges[e].m_latest_start - cols[i].m_edges[e].m_earliest_start;
		}
	}

}