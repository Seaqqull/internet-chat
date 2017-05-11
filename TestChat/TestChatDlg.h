
// TestChatDlg.h : файл заголовка
//

#pragma once
#include "afxwin.h"
//#include "afxcmn.h"

#include "Map.h"
//#include "ClientSocket.h"

using namespace std;

class CServerSocket;
//class CClientSocket;
class ManageMySocket;


enum MySystemMessage
{
	EnterUser = 0,	
	ExitUser = 1,
	AddGroup = 2,
	EraseGroup = 3,
	ChangeNick = 4,
	ClearGroup = 5,
	RenameGroup = 7
};
enum MyUserMessage
{
	UserSendMessage = 0,
	UserChangeNick = 1,
	UserChangeGroup = 2,
	UserAddGroup = 3,
	UserDeleteGroup = 4
};
enum ErrClient{
	NoError = 0,
	ErrorNick = 1,
	Expelled = 2
};
// диалоговое окно CTestChatDlg
class CTestChatDlg : public CDialogEx
{
// Создание
public:
	CTestChatDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_TESTCHAT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV

public:// Новые
	void NewServerData();
	void ClearServerData();
	void ShowAllMaps();
	void ShowAllUsers();
	void AddUser();
	void DeleteUser(ErrClient errAlert, CString incomingUser, UINT incomingId, CString incominTmpNick = L"");
	void SetUserNick(CString incomingUser, UINT incomingId, CString incomingNick);
	void ChnUserNick(CString incomingUser, CString incomingNick);
	void ChngUserGroup(CString incomingUser, UINT incomingId, CString incomingGroup);
	void SndSystemMessage(MySystemMessage SystemMessageType, ErrClient errAlert, CString* incomingMessage);

	void SndUserMessage(MyUserMessage UserMessageType, CString incomeNick, UINT incomeGroupId, CString incomingMessage);
	void SndAddGroup(UINT incomeIdUser, CString incomeTitleGroup);
	void SndDeleteGroup(UINT incomeIdUser, CString incomeTitleGroup);
	void SndRenameGroup(UINT incomeIdUser, CString incomeOldTitleGroup, CString incomeNewTitleGroup);
	void ChnUserGroup(UINT incomeIdUser, CString incomeTitleGroup);

	BOOL ShowPopup(UINT nID, int x, int y, CWnd* parent);
	BOOL IsInDiap(CRect incomeWindowRect, CPoint incomeMousePoints);
	BOOL RnmGroup(CString incomeOldGroup, CString incomeNewGroup);
	BOOL RnmUser(CString incomeOldUser, CString incomeNewUser);
// Реализация
protected:
	HICON m_hIcon;

	CServerSocket* m_sSocket;
	UINT m_CntRooms, m_CntUsers, m_CurSelGroup;
	CMFCPopupMenu *contextMenu;

	//ErrClient errClient;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBegin();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMapCreate();
	afx_msg void OnBnClickedButtonMapDelete();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnNMClickListMaps(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListUsers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickListMaps(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnNMKillfocusListMaps(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListUsers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusListUsers(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnElementsCommandDelete();
	afx_msg void OnElementsCommandClear();
	afx_msg void OnElementsCommandRename();
	afx_msg void OnElementsUserExpellFromGroup();
	afx_msg void OnElementsUserDelete();
	afx_msg void OnElementsUserRename();
	afx_msg void OnElementsUserWsp();

	CButton m_BeginButton;
	CButton m_StopButton;
	
	UINT m_PortV;
	CString m_RoomV;

	CListCtrl m_ListMaps;
	CListCtrl m_ListUsers;
	CListCtrl m_ListLog;

	//vector<MyMap> ServerMapList;
	ManageMyMap ServerMapList;
	ManageMySocket* ServerSocketList;
	CEdit m_MessageCtrl;
	CString m_MessageV;
	CButton m_SendCtrl;			
	


};
