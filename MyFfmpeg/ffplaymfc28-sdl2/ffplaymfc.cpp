/* 
 * FFplay for MFC
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 *
 * http://blog.csdn.net/leixiaohua1020
 * 
 * �����̽�ffmpeg��Ŀ�е�ffplay��������ffplay.c����ֲ����VC�Ļ����¡�
 * ����ʹ��MFC����һ�׼򵥵Ľ��档
 * This software transplant ffplay to Microsoft VC++ environment. 
 * And use MFC to build a simple Graphical User Interface. 
 */

#include "stdafx.h"
#include "ffplaymfc.h"
#include "ffplaymfcDlg.h"
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CffplaymfcApp

BEGIN_MESSAGE_MAP(CffplaymfcApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CffplaymfcApp ����

CffplaymfcApp::CffplaymfcApp()
{
	// ֧����������������
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CffplaymfcApp ����

CffplaymfcApp theApp;


// CffplaymfcApp ��ʼ��

BOOL CffplaymfcApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	//�����������
	//SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
	LoadLaguage();

	CffplaymfcDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô�����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

void CffplaymfcApp::LoadLaguage()
{
	//�����ļ�·��
	char conf_path[300]={0};
	//���exe����·��
	GetModuleFileNameA(NULL,(LPSTR)conf_path,300);//
	//���exe�ļҼ�·��
	strrchr( conf_path, '\\')[0]= '\0';//
	strcat(conf_path,"\\configure.ini");
	//�洢���Ե��ַ���
	char conf_val[300]={0};

	if((_access(conf_path, 0 )) == -1 ){  
		//�����ļ������ڣ�ֱ�ӷ���
	}else{
		GetPrivateProfileStringA("Settings","language",NULL,conf_val,300,conf_path);
		if(strcmp(conf_val,"Chinese")==0){
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT));
		}else if(strcmp(conf_val,"English")==0){
			SetThreadUILanguage(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT));
		}

	}

	return;
}