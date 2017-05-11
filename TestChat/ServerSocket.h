#pragma once

// конечный объект команды CServerSocket
#include "TestChatDlg.h"

class CServerSocket : public CSocket
{
private:
	CTestChatDlg* m_Dlg;
	UINT m_port;
public:
	CServerSocket(CTestChatDlg* Dlg);
	virtual ~CServerSocket();
	virtual void OnAccept(int nErrorCode);
};


