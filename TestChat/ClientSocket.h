#pragma once

#include "TestChatDlg.h"
// конечный объект команды CClientSocket

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	CClientSocket(CString incomingStr, UINT incomingId);
	virtual ~CClientSocket(); 
public:
	static CTestChatDlg* m_Dlg;
	CString Nick;
	UINT curMapId, userId;
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

class ManageMySocket {
public:
	CArray<CClientSocket*> Ar;
	CTestChatDlg* m_Dlg;
public:
	ManageMySocket(CTestChatDlg* Dlg);
	~ManageMySocket();

	CClientSocket* operator[](UINT index);

	// Новые
	int AddUser(CString incomingNick, UINT userId, bool checkIden = false);
	bool AddUser(CClientSocket* incomingUser, bool checkIden = false);
	bool DeleteUser(CString incomingNick, UINT userId);
	UINT GetCount();

	CClientSocket* GetUser(UINT userId);

	int FindUser(CString incomingNick);
	int FindUser(CString incomingNick, UINT userId);
	int FindUser(UINT userId);

	bool ChangeGroup(CString incomingNick, UINT userId, UINT nMap);
	bool ChangeNick(CString incomingUser, UINT incomingId, CString incomingNick);
	
	void SendToAll(CTime timeEvent, CString sendCommand, CString typeCommand, CString incomingMessage);//All Chat, Admin Message
	//void SendSystemAll(CTime timeEvent, CString sendCommand, CString incomingMessage);//Server Commands
	void SendToGroup(CTime timeEvent, CString sendCommand, CString typeCommand, UINT IdGroup, CString incomingMessage);//Group Chat, User System Message
	//void SendSystemGroup(CTime timeEvent, UINT IdGroup, CString sendCommand, CString incomingMessage);//Group Chat, User System Message
	void SendToUser(CTime timeEvent, CString sendCommand, CString typeCommand, CString incomingUser, UINT incomingId, CString incomingMessage);// User Message
	
	void Clear();
};
