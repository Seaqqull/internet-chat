
// TestChatClientDlg.h : файл заголовка
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CClientSocket;
// диалоговое окно CTestChatClientDlg
class CTestChatClientDlg : public CDialogEx
{
// Создание
public:
	CTestChatClientDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TESTCHATCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	CClientSocket* m_cSocket;		
	
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void ShowConnected();
	void ShowDisconnected();

	void DeleteGroup(CString incomeTitleGroup);
	void AddGroup(CString incomeTitleGroup);
	void ChangeGroup(CString incomeTitleGroup);
	void ChangeWindowTtle(CString incomeTitleGroup);
	void RenameGroup(CString incomeOldGroup, CString incomeNewGroup);

	CString errorMessage;
	UINT m_PortV;
	CString m_AddressV;
	CString m_MessageV;
	CString m_LoginV;
	
	afx_msg void OnBnClickedButtonConnect();
	afx_msg void OnBnClickedButtonDisconnect();
	afx_msg void OnBnClickedButtonSend();
	
	CIPAddressCtrl m_AdressCtrl;	
	CListCtrl m_ListLog;
	CListCtrl m_ListMaps;
	
	CEdit m_PortCtrl;
	CEdit m_LoginCtrl;	
	CEdit m_MessageCtrl;

	CButton m_SendCtrl;
	CButton m_ConnectCtrl;
	CButton m_DisconnectCtrl;
	afx_msg void OnBnClickedButtonMapChange();
	CButton m_MapCh;
};
