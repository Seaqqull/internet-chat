#pragma once

#include "TestChatDlg.h"
// диалоговое окно CDialogRename

class CDialogRename : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogRename)
public:
	BOOL modeRename;
	CString oldMapTitle;
	CTestChatDlg* theMFrame;
	CDialogRename(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~CDialogRename();

// Данные диалогового окна
	enum { IDD = IDD_DIALOG_RENAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_StringRename;
};
