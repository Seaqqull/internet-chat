// ClientSocket.cpp: файл реализации
//

#include "stdafx.h"
#include "TestChatClient.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket(CTestChatClientDlg* Dlg)
{
	m_Dlg = Dlg;
}

CClientSocket::~CClientSocket()
{
}


// функции-члены CClientSocket


void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	char recStr[400];
	int r = Receive(recStr, 400);
	recStr[r] = '\0';
	CString incomeMessage, serverQuery;
	incomeMessage.Format(L"%s", recStr);

	int posQuery = incomeMessage.FindOneOf(L" ");
	serverQuery = incomeMessage.Left(posQuery);
	CString userCommand = incomeMessage.Mid(posQuery + 1, incomeMessage.GetLength() - posQuery + 1);
	
	if (serverQuery == "Registration")// 8 3(4) 8 - 3 = 5; 9 reg nikol ' '=3 4+?	+++
		m_Dlg->m_ListLog.InsertItem(m_Dlg->m_ListLog.GetItemCount(), userCommand);
	else if (serverQuery == "ErrorRegistration"){//Алерт
		m_Dlg->errorMessage = userCommand;
		m_Dlg->m_ListLog.InsertItem(m_Dlg->m_ListLog.GetItemCount(), userCommand);
	}
	else if (serverQuery == "Message")// +++
		m_Dlg->m_ListLog.InsertItem(m_Dlg->m_ListLog.GetItemCount(), userCommand);
	else if (serverQuery == "DeleteGroup")
		m_Dlg->DeleteGroup(userCommand);	
	else if (serverQuery == "AddGroup")
		m_Dlg->AddGroup(userCommand);
	else if (serverQuery == "ChangeGroup"){
		m_Dlg->m_ListLog.DeleteAllItems();
		m_Dlg->ChangeWindowTtle(userCommand);
	}
	else if (serverQuery == "RenameGroup"){		
		CString userCommand2;		
		int posQuery2 = incomeMessage.Find(L" ", posQuery + 1);
		userCommand2 = incomeMessage.Mid(posQuery2 + 1, incomeMessage.GetLength() - posQuery2 + 1);
		userCommand = incomeMessage.Mid(posQuery + 1, incomeMessage.GetLength() - posQuery2 - 1);
		m_Dlg->RenameGroup(userCommand, userCommand2);		
	}
	else if (serverQuery == "RenameUser"){
		m_Dlg->m_LoginV = userCommand;
		m_Dlg->UpdateData(FALSE);

		CString titleWdw;
		m_Dlg->GetWindowText(titleWdw);
		int pos1, pos2;
		pos1 = titleWdw.FindOneOf(L"[");
		pos2 = titleWdw.Find(L"]", pos1 + 1);
		titleWdw = titleWdw.Mid(pos1 + 1, pos2 - (pos1 + 1));

		m_Dlg->ChangeWindowTtle(titleWdw);
	}
	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	//m_Dlg->m_SendButton.EnableWindow(FALSE);
	//AfxMessageBox(L"Server was closed");
	if (m_Dlg->errorMessage != ""){
		m_Dlg->MessageBox(m_Dlg->errorMessage);
		m_Dlg->errorMessage = "";
	}
	else
		m_Dlg->MessageBox(L"Сервер отключен");
	m_Dlg->ShowDisconnected();
	m_Dlg->SetWindowText(L"Chat");
	CSocket::OnClose(nErrorCode);
}
