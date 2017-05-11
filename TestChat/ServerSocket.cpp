// ServerSocket.cpp: файл реализации
//

#include "stdafx.h"
#include "TestChat.h"
#include "ServerSocket.h"


// CServerSocket

CServerSocket::CServerSocket(CTestChatDlg* Dlg)
{
	m_Dlg = Dlg;
}

CServerSocket::~CServerSocket()
{
}


// функции-члены CServerSocket


void CServerSocket::OnAccept(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	//AfxMessageBox(L"New connection accepted");
	m_Dlg->AddUser();
	CSocket::OnAccept(nErrorCode);
}
