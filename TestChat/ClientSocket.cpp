// ClientSocket.cpp: файл реализации
//

#include "stdafx.h"
#include "TestChat.h"
#include "ClientSocket.h"


// CClientSocket

CClientSocket::CClientSocket()
{
	Nick = "Unknown";
	curMapId = 0;
	userId = 0;
}
CClientSocket::CClientSocket(CString incomingStr, UINT incomingId){
	Nick = incomingStr;
	curMapId = 0;
	userId = incomingId;
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
	if (serverQuery == "Registration"){// 8 3(4) 8 - 3 = 5; 9 reg nikol ' ' = 3 4+?
		CString newUserReg = incomeMessage.Mid(posQuery + 1, incomeMessage.GetLength() - posQuery + 1);
		m_Dlg->SetUserNick(this->Nick, this->userId, newUserReg);
	}
	else if (serverQuery == "Message") {
		CString newUserMess = incomeMessage.Mid(posQuery + 1, incomeMessage.GetLength() - posQuery + 1);
		m_Dlg->SndUserMessage(MyUserMessage::UserSendMessage, this->Nick, this->curMapId, newUserMess);
	}
	else if (serverQuery == "ChangeGroup"){
		CString newUserGroup = incomeMessage.Mid(posQuery + 1, incomeMessage.GetLength() - posQuery + 1);
		m_Dlg->ChnUserGroup(this->userId, newUserGroup);
	}
	else if (serverQuery == "CreateGroup"){
		
	}
		
	// Смена ника ?
	// Создание группы 
	


	//m_Dlg->SetDlgItemTextW(IDC_STATIC_OUTPUT, str);

	CSocket::OnReceive(nErrorCode);
}


void CClientSocket::OnClose(int nErrorCode)
{
	// TODO: добавьте специализированный код или вызов базового класса
	//m_Dlg->m_SendButton.EnableWindow(FALSE);
	//AfxMessageBox(L"Client was closed");
	// При ошике регистрации отправить ошибку перед закрытием
	m_Dlg->DeleteUser(ErrClient::NoError, Nick, userId);
	CSocket::OnClose(nErrorCode);
}


ManageMySocket::ManageMySocket(CTestChatDlg* Dlg){
	m_Dlg = Dlg;
}

ManageMySocket::~ManageMySocket(){ this->Clear(); delete this->m_Dlg; /*this->m_Dlg = NULL;*/ }

CClientSocket* ManageMySocket::operator[](UINT index){
	return Ar[index];
}
// Новые use
int ManageMySocket::AddUser(CString incomingNick, UINT userId, bool checkIden){
	int tempPos = (checkIden) ? FindUser(incomingNick, userId) : 0;
	if (tempPos == -1 || !checkIden){
		int pos = Ar.Add(new CClientSocket(incomingNick, userId));
		return pos;
	}
	return -1;
}
bool ManageMySocket::AddUser(CClientSocket* incomingUser, bool checkIden){
	int tempPos = (checkIden) ? FindUser(incomingUser->Nick, incomingUser->userId) : 0;
	if (tempPos == -1 || !checkIden){
		Ar.Add(incomingUser);
		return true;
	}
	return false;
}

bool ManageMySocket::DeleteUser(CString incomingNick, UINT userId){
	int tempPos = FindUser(incomingNick, userId);
	if (tempPos > -1){
		Ar[tempPos]->Close();
		Ar.RemoveAt(tempPos);
		return true;
	}
	return false;
}

CClientSocket* ManageMySocket::GetUser(UINT userId){	
	return Ar[FindUser(userId)];
}

int ManageMySocket::FindUser(CString incomingNick){
	for (int i = 0; i < Ar.GetCount(); i++)
		if (Ar[i]->Nick == incomingNick) return i;
	return -1;
}
int ManageMySocket::FindUser(CString incomingNick, UINT userId){
	for (int i = 0; i < Ar.GetCount(); i++)
		if (Ar[i]->Nick == incomingNick && Ar[i]->userId == userId) return i;
	return -1;
}
int ManageMySocket::FindUser(UINT userId){
	for (int i = 0; i < Ar.GetCount(); i++)
		if (Ar[i]->userId == userId) return i;
	return -1;
}

bool ManageMySocket::ChangeGroup(CString incomingNick, UINT userId, UINT nMap){
	int uPos = FindUser(incomingNick, userId);
	if (uPos > -1){
		Ar[uPos]->curMapId = nMap;
		return true;
	}
	return false;
}

bool ManageMySocket::ChangeNick(CString incomingUser, UINT incomingId, CString incomingNick){
	int tempPos = FindUser(incomingUser, incomingId);
	int tempIden = FindUser(incomingNick);
	if (tempIden == -1 && tempPos > -1){
		Ar[tempPos]->Nick = incomingNick;
		return true;
	}
	return false;
}

UINT ManageMySocket::GetCount(){
	return Ar.GetCount();
}

void ManageMySocket::Clear(){ 
	for (int i = 0; i < Ar.GetCount(); i++)
		Ar[i]->Close();
	Ar.RemoveAll();
}
// Отправка
void ManageMySocket::SendToAll(CTime timeEvent, CString sendCommand, CString typeCommand, CString incomingMessage){
	CString message;
	message.Format(L"%s %02i:%02i:%02i [%s] %s", sendCommand, timeEvent.GetHour(), timeEvent.GetMinute(), timeEvent.GetSecond(), typeCommand, incomingMessage);
	for (int i = 0; i < Ar.GetCount(); i++)
		Ar[i]->Send(message, message.GetLength() * 2 + 1);
}

void ManageMySocket::SendToGroup(CTime timeEvent, CString sendCommand, CString typeCommand, UINT IdGroup, CString incomingMessage){
	CString message;
	//m_Dlg->ServerMapList.
	message.Format(L"%s %02i:%02i:%02i [%s] %s", sendCommand, timeEvent.GetHour(), timeEvent.GetMinute(), timeEvent.GetSecond(), typeCommand, incomingMessage);
	for (int i = 0; i < Ar.GetCount(); i++)
		if (Ar[i]->curMapId == IdGroup)
			Ar[i]->Send(message, message.GetLength() * 2 + 1);
}

void ManageMySocket::SendToUser(CTime timeEvent, CString sendCommand, CString typeCommand, CString incomingUser, UINT incomingId, CString incomingMessage){
	int tempPos = FindUser(incomingUser, incomingId);
	CString message;
	message.Format(L"%s %02i:%02i:%02i [%s] %s", sendCommand, timeEvent.GetHour(), timeEvent.GetMinute(), timeEvent.GetSecond(), typeCommand, incomingMessage);
	Ar[tempPos]->Send(message, message.GetLength() * 2 + 1);
}