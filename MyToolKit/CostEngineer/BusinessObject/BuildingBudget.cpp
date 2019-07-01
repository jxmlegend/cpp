#include "stdafx.h"
#include "BuildingBudget.h"
#include "GridDlg.h"


CBuildingBudgetObj::CBuildingBudgetObj()
{
	m_unit_price = 0;
	m_quantity = 0;
	m_name = "";
	m_unit = "";
}


CBuildingBudgetObj::~CBuildingBudgetObj()
{
}



void CBuildingBudgetObj::Serialize(CArchive& ar, double version) {
	if (ar.IsStoring()) {
		ar << m_unit_price;
		ar << m_quantity;
		ar << m_name;
		ar << m_unit;
	}
	else {
		ar >> m_unit_price;
		ar >> m_quantity;
		ar >> m_name;
		ar >> m_unit;
	}
}




bool CBuildingBudgetObj::CreateOrUpdate(string menuCode) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return false;

	CDyncItemGroupDlg infd;
	infd.CXCAPTION = 50;
	infd.GROUP_NUM_PER_LINE = 3;
	int i = 0;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("��������"), 64);
	if (!m_name.IsEmpty())
		infd.m_vecFindItem[0][i][0].strItem = m_name;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("��λ"), 64);
	if (!m_unit.IsEmpty())
		infd.m_vecFindItem[0][i][0].strItem = m_unit;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("������"), 64);
	if (m_quantity > 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%.2f", m_quantity);
	infd.m_vecFindItem[0][i][0].dbMin = 0.01;
	infd.m_vecFindItem[0][i][0].dbMax = 100000;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("���ϵ���"), 64);
	if (m_unit_price > 0 )
		infd.m_vecFindItem[0][i][0].strItem.Format("%.2f", m_unit_price);
	infd.m_vecFindItem[0][i][0].dbMin = 0;
	infd.m_vecFindItem[0][i][0].dbMax = 10000;

	
	infd.Init(_T("�������� ��������"), _T("�������� ��������"));
	if (infd.DoModal() == IDOK) {
		i = 0;
		m_name = infd.m_vecFindItem[0][i++][0].strItem;
		m_unit = infd.m_vecFindItem[0][i++][0].strItem;
		m_quantity = String2Double(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		m_unit_price = String2Double(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		
		return true;
	}
	return false;
}

bool CBuildingBudgetObj::Draw(CGridCtrl* pGridCtrl, vector<CBuildingBudgetObj>& cols) {
	if (!pGridCtrl)
		return false;


	try {
		pGridCtrl->SetRowCount(cols.size() + 1);
		pGridCtrl->SetColumnCount(4 + 3);		//	������������ �� ���/�޸�/ɾ��
		pGridCtrl->SetFixedRowCount(1);
		pGridCtrl->SetFixedColumnCount(1);
		pGridCtrl->SetHeaderSort(TRUE);
		pGridCtrl->SetEditable(FALSE);
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

				if (col == 0)		val = "��Դ����";
				else if (col == 1)	val = "����";
				else if (col == 2)	val = "��λ";
				else if (col == 3)	val = "������";
				else if (col == 4)	val = "���ϵ���";
				else if (col == 5)	val = "";
				else if (col == 6)	val = "";


				Item.strText.Format(_T("%s"), val.c_str());
			}
			else
			{
				if (col <= 2)
					Item.nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				else
					Item.nFormat = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

				if (col >= 3 && col <= 4 || col == 0)
				{
					if (!pGridCtrl->SetCellType(row, col, RUNTIME_CLASS(CGridCellNumeric)))
						return false;
				}
				if (col == 5) {
					Item.crFgClr = RGB(0, 120, 250);
					Item.mask |= GVIF_FGCLR;
				}
				if (col == 6) {
					Item.crFgClr = RGB(255, 0, 0);
					Item.mask |= GVIF_FGCLR;
				}

				if (col == 0)	val = Int2String(row);
				else if (col == 1) 	val = cols[row - 1].m_name.GetBuffer();
				else if (col == 2) 	val = cols[row - 1].m_unit.GetBuffer();
				else if (col == 3)  val = Double2String(cols[row - 1].m_quantity);
				else if (col == 4)  val = Double2String(cols[row - 1].m_unit_price);
				else if (col == 5)	val = "�޸ģ�update��";
				else if (col == 6)	val = "ɾ����delete��";

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

bool CBuildingBudgetObj::Update(string menuCode, int nRow, vector<CBuildingBudgetObj>& cols) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return false;

	if (nRow > 0 && nRow <= cols.size())
		return cols[nRow - 1].CreateOrUpdate(menuCode);
	return false;
}


bool CBuildingBudgetObj::Delete(string menuCode, int nRow, vector<CBuildingBudgetObj>& cols) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return false;

	if (nRow > 0 && nRow <= cols.size()) {
		int idx = 0;
		vector<CBuildingBudgetObj>::iterator it = cols.begin();
		for (; it != cols.end(); it++, idx++) {
			if (idx == nRow - 1)
				break;
		}
		cols.erase(it);
		return true;
	}
	return false;
}


/***********************************************************************************/


/* ʩ��ͼԤ��-  ʵ������ */
string CBuildingBudget::m_ObjectCode = "01030401";
double CBuildingBudget::m_ObjectVersion = 1.0;

CBuildingBudget::CBuildingBudget()
{
	m_name = "";
	m_other_material_cost_percent = 0;
	m_machine_tool_cost_percent = 0;
}


CBuildingBudget::~CBuildingBudget()
{
}




void CBuildingBudget::Serialize(CArchive& ar, double version) {
	if (ar.IsStoring()) {
		ar << m_other_material_cost_percent;
		ar << m_machine_tool_cost_percent;
		ar << m_name;
		ar << m_materials.size();
		for (int i = 0; i < m_materials.size(); i++) {
			m_materials[i].Serialize(ar, version);
		}
	}
	else {
		ar >> m_other_material_cost_percent;
		ar >> m_machine_tool_cost_percent;
		ar >> m_name;
		int nNum;
		ar >> nNum;
		for (int i = 0; i < nNum; i++) {
			CBuildingBudgetObj bs;
			bs.Serialize(ar, version);
			m_materials.push_back(bs);
		}
	}
}

bool CBuildingBudget::CreateOrUpdate(string menuCode) {

	if (menuCode != CBuildingBudget::m_ObjectCode)
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
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("�������Ϸ�ռ�����Ϸѵı���(%)"), 64);
	if (m_other_material_cost_percent > 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%.2f", m_other_material_cost_percent);
	infd.m_vecFindItem[0][i][0].dbMin = 0.0001;
	infd.m_vecFindItem[0][i][0].dbMax = 100000;

	i++;
	infd.m_vecFindItem[0][i][0].nType = CDlgTemplateBuilder::EDIT;
	memcpy(infd.m_vecFindItem[0][i][0].caption, _T("����ʹ�÷�ռ�˲Ļ�֮�͵ı���(%)"), 64);
	if (m_machine_tool_cost_percent > 0)
		infd.m_vecFindItem[0][i][0].strItem.Format("%.2f", m_machine_tool_cost_percent);
	infd.m_vecFindItem[0][i][0].dbMin = 1;
	infd.m_vecFindItem[0][i][0].dbMax = 1000000;

	infd.Init(_T("����ָ�귨 ��������"), _T("����ָ�귨 ��������"));
	if (infd.DoModal() == IDOK) {
		i = 0;
		m_name = infd.m_vecFindItem[0][i++][0].strItem;
		m_other_material_cost_percent = String2Double(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());
		m_machine_tool_cost_percent = String2Double(infd.m_vecFindItem[0][i++][0].strItem.GetBuffer());

		return true;
	}
	return false;
}




bool CBuildingBudget::Draw(string menuCode, CGridCtrl* pGridCtrl, vector<CBuildingBudget>& cols) {
	if (!pGridCtrl)
		return false;

	if (menuCode != CBuildingBudget::m_ObjectCode)
		return false;

	try {
		pGridCtrl->SetRowCount(cols.size() + 1);
		pGridCtrl->SetColumnCount(3 + 4);		//	��������4�� �� ���/�޸�/ɾ��/����
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

				if (col == 0)		val = "���ƹ���Ԥ�㷨";
				else if (col == 1)	val = "����";
				else if (col == 2)	val = "�������Ϸ�ռ�����Ϸѵı���(%)";
				else if (col == 3)	val = "����ʹ�÷�ռ�˲Ļ�֮�͵ı���(%)";
				else if (col == 4)	val = "";
				else if (col == 5)	val = "";
				else if (col == 6)	val = "";

				Item.strText.Format(_T("%s"), val.c_str());
			}
			else
			{
				if (col <= 2)
					Item.nFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;
				else
					Item.nFormat = DT_RIGHT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

				if (col >= 2 && col <= 3 || col == 0)
				{
					if (!pGridCtrl->SetCellType(row, col, RUNTIME_CLASS(CGridCellNumeric)))
						return false;
				}
				if (col == 4 || col == 6) {
					Item.crFgClr = RGB(0, 120, 250);
					Item.mask |= GVIF_FGCLR;
				}
				if (col == 5) {
					Item.crFgClr = RGB(255, 0, 0);
					Item.mask |= GVIF_FGCLR;
				}

				if (col == 0)	val = Int2String(row);
				else if (col == 1) 	val = cols[row - 1].m_name.GetBuffer();
				else if (col == 2)  val = Double2String(cols[row - 1].m_other_material_cost_percent, "%.2f");
				else if (col == 3)  val = Double2String(cols[row - 1].m_machine_tool_cost_percent, "%.2f");
				
				else if (col == 4)	val = "�޸ģ�update��";
				else if (col == 5)	val = "ɾ����delete��";
				else if (col == 6)	val = "���ӣ�create��";

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

bool CBuildingBudget::Update(string menuCode, int nRow, vector<CBuildingBudget>& cols) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return false;

	if (nRow > 0 && nRow <= cols.size())
		return cols[nRow - 1].CreateOrUpdate(CBuildingBudget::m_ObjectCode);
	return false;
}

bool CBuildingBudget::Delete(string menuCode, int nRow, vector<CBuildingBudget>& cols) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return false;

	if (nRow > 0 && nRow <= cols.size()) {
		int idx = 0;
		vector<CBuildingBudget>::iterator it = cols.begin();
		for (; it != cols.end(); it++, idx++) {
			if (idx == nRow - 1)
				break;
		}
		cols.erase(it);
		return true;
	}
	return false;
}


unsigned int CBuildingBudget::PopupMenuId(string menuCode) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return 0;
	return IDR_POPUP_COMPONENT;
}



void CBuildingBudget::Calculate(string menuCode, vector<CBuildingBudget>& cols) {
	if (menuCode != CBuildingBudget::m_ObjectCode)
		return;

	CGridDlg gridDlg;
	gridDlg.m_vecHeader.push_back("����");
	gridDlg.m_vecHeader.push_back("�������");
	

	for (int i = 0; i < cols.size(); i++)
	{
		CBuildingBudget seb = cols[i];

		string str = seb.m_name.GetBuffer();
		vector<string> vec;
		vec.push_back(str);
		
		// ���ķ�
		double material = 0;
		double people = 0;
		double machine = 0;

		for (auto e : seb.m_materials)
		{
			if (e.m_unit.Find("����")>=0) {
				people += e.m_quantity * e.m_unit_price;
			} else if (e.m_unit.Find("̨��") >= 0) {
				machine += e.m_quantity * e.m_unit_price;
			} else 
				material += e.m_quantity * e.m_unit_price ;
		}
		// ��ȫ����ʩ����
		double  measure1 = (people + machine) * 0.12;
		// �����ܼ۴�ʩ��
		double	measure2 = (people + machine) * 0.08;
		// ������
		double manage = (people + machine + measure2 * 0.45) * 0.15;
		// ����
		double net = (people + machine + measure2 * 0.45) * 0.1;
		// ���
		double government = (people + measure2 * 0.35) * 0.15;
		// ��ֵ˰
		double tax = net * 0.11;
		
		double total = material + people + machine + measure1 + measure2 + manage + net + government + tax + 3000;
		
		vec.push_back(Double2String(total));
		gridDlg.m_vecData.push_back(vec);
	}
	gridDlg.DoModal();
}