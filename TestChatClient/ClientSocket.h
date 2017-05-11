#pragma once

#include "TestChatClientDlg.h"
// конечный объект команды CClientSocket

class CClientSocket : public CSocket
{
public:
	CClientSocket(CTestChatClientDlg* Dlg);
	virtual ~CClientSocket(); 
protected:
	CTestChatClientDlg* m_Dlg;
public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


