
// TestChatDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TestChat.h"
#include "TestChatDlg.h"
#include "afxdialogex.h"

#include "ServerSocket.h"
#include "ClientSocket.h"
#include "DialogRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CTestChatDlg* CClientSocket::m_Dlg = NULL;

// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// диалоговое окно CTestChatDlg



CTestChatDlg::CTestChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestChatDlg::IDD, pParent)
	, m_PortV(22345)
	, m_RoomV(_T(""))
	, m_MessageV(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BEGIN, m_BeginButton);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_StopButton);
	DDX_Text(pDX, IDC_EDIT_PORT, m_PortV);
	DDV_MinMaxUInt(pDX, m_PortV, 1, 65535);
	DDX_Control(pDX, IDC_LIST_MAPS, m_ListMaps);
	DDX_Control(pDX, IDC_LIST_USERS, m_ListUsers);
	DDX_Control(pDX, IDC_LIST_LOG, m_ListLog);
	DDX_Text(pDX, IDC_EDIT_ROOM, m_RoomV);
	DDV_MaxChars(pDX, m_RoomV, 20);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_MessageCtrl);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_MessageV);
	DDV_MaxChars(pDX, m_MessageV, 100);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_SendCtrl);
}

BEGIN_MESSAGE_MAP(CTestChatDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CTestChatDlg::OnBnClickedButtonBegin)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTestChatDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MAP_CREATE, &CTestChatDlg::OnBnClickedButtonMapCreate)
	ON_BN_CLICKED(IDC_BUTTON_MAP_DELETE, &CTestChatDlg::OnBnClickedButtonMapDelete)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CTestChatDlg::OnBnClickedButtonSend)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAPS, &CTestChatDlg::OnNMClickListMaps)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_USERS, &CTestChatDlg::OnNMRClickListUsers)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MAPS, &CTestChatDlg::OnNMRClickListMaps)
	ON_WM_CONTEXTMENU()
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST_MAPS, &CTestChatDlg::OnNMKillfocusListMaps)
	ON_NOTIFY(NM_CLICK, IDC_LIST_USERS, &CTestChatDlg::OnNMClickListUsers)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST_USERS, &CTestChatDlg::OnNMKillfocusListUsers)
	ON_WM_MOUSEMOVE()
ON_COMMAND(ID_ELEMENTS_COMMAND1, &CTestChatDlg::OnElementsCommandDelete)
ON_COMMAND(ID_ELEMENTS_COMMAND3, &CTestChatDlg::OnElementsCommandClear)
ON_COMMAND(ID_ELEMENTS_COMMAND2, &CTestChatDlg::OnElementsCommandRename)
ON_COMMAND(ID_ELEMENTS_USER3, &CTestChatDlg::OnElementsUserExpellFromGroup)
ON_COMMAND(ID_ELEMENTS_USER1, &CTestChatDlg::OnElementsUserDelete)
ON_COMMAND(ID_ELEMENTS_USER2, &CTestChatDlg::OnElementsUserRename)
ON_COMMAND(ID_ELEMENTS_32777, &CTestChatDlg::OnElementsUserWsp)
END_MESSAGE_MAP()


// обработчики сообщений CTestChatDlg

BOOL CTestChatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	SetWindowText(L"Chat: Server Disabled");
	
	m_ListMaps.SetExtendedStyle(m_ListMaps.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);
	m_ListMaps.InsertColumn(0, L"Группы", LVCFMT_CENTER, 90);	
	m_ListMaps.InsertColumn(1, L"Size", LVCFMT_CENTER, 50);

	//m_ListMaps.InsertItem(m_ListMaps.GetItemCount(), L"Все");

	m_ListUsers.SetExtendedStyle(m_ListUsers.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);
	m_ListUsers.InsertColumn(0, L"Пользователи", LVCFMT_CENTER, 140);

	m_ListLog.SetExtendedStyle(m_ListLog.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);
	m_ListLog.InsertColumn(0, L"Лог", LVCFMT_CENTER, 305);

	/*m_CntRooms = m_CntUsers = 0;
	ServerMapList.Add(L"Все", m_CntRooms++);
	ShowAllMaps();*/
	//errClient = ErrClient::NoError;
	NewServerData();
	ClearServerData();


	CClientSocket::m_Dlg = this;
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTestChatDlg::ClearServerData(){
	m_CntRooms = m_CntUsers = m_CurSelGroup = 0;

	ServerMapList.Clear();
	ServerSocketList->Clear();

	m_ListMaps.DeleteAllItems();
	m_ListUsers.DeleteAllItems();
	m_ListLog.DeleteAllItems();

	ServerMapList.Add(L"Все", m_CntRooms++);
	m_ListMaps.InsertItem(m_ListMaps.GetItemCount(), L"Все");

	ShowAllMaps();
}
void CTestChatDlg::NewServerData(){
	ServerSocketList = new ManageMySocket(this);
}
void CTestChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CTestChatDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CTestChatDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestChatDlg::OnBnClickedButtonBegin()
{	
	// TODO: добавьте свой код обработчика уведомлений
	if (!UpdateData(TRUE)) return;

	m_sSocket = new CServerSocket(this);	
	if (!m_sSocket->Create(m_PortV)) MessageBox(L"Socket was chosen before");	
	else{
		if (!m_sSocket->Listen()) MessageBox(L"Error beginning of server");
		else{
			
			SetWindowText(L"Chat: Server Enabled");
			//ShowAllMaps();//
			m_SendCtrl.EnableWindow(TRUE);
			m_MessageCtrl.EnableWindow(TRUE);
			m_StopButton.EnableWindow(TRUE);
	
			m_BeginButton.EnableWindow(FALSE);
		}
	}	
}

void CTestChatDlg::OnBnClickedButtonStop()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_sSocket->Close();
	
	/*ServerMapList.Clear();
	ServerSocketList->Clear();*/

	//ServerSocketList->~ManageMySocket();

	/*m_ListMaps.DeleteAllItems();*/
	ClearServerData();

	SetWindowText(L"Chat: Server Disabled");
	
	m_BeginButton.EnableWindow(TRUE);

	m_SendCtrl.EnableWindow(FALSE);
	m_MessageCtrl.EnableWindow(FALSE);
	m_StopButton.EnableWindow(FALSE);
}


void CTestChatDlg::OnBnClickedButtonMapCreate()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!UpdateData(TRUE)) return;

	if (m_RoomV.GetLength() > 0){
		if (ServerMapList.Add(m_RoomV, m_CntRooms)){
			//Рассылка груп на клиенты
			m_CntRooms++;
			int pos = m_ListMaps.InsertItem(m_ListMaps.GetItemCount(), m_RoomV);	
			CString myMessage;
			myMessage.Format(L": ChatRoom [%s] was created", m_RoomV);
			//!!!ServerSocketList->SendToAll(CTime::GetCurrentTime(), L"NewChat", L"System", myMessage);//New Chat
			SndSystemMessage(MySystemMessage::AddGroup, ErrClient::NoError, new CString[1]{m_RoomV});
			
			for (int i = 0; i < ServerSocketList->GetCount(); i++)
				SndAddGroup(ServerSocketList->Ar[i]->userId, m_RoomV);			

			m_RoomV = "";
			UpdateData(FALSE);	
			
			ShowAllMaps();//!!!
			//!!!Отправка групп
		}
		else
			MessageBox(L"Група с данным названием уже существует");		
	}
	else
		MessageBox(L"Введите название группы");
}
BOOL CTestChatDlg::RnmGroup(CString incomeOldGroup, CString incomeNewGroup){	
	int pos = ServerMapList.GetIdMap(incomeOldGroup);
	int tmpPos = ServerMapList.GetIdMap(incomeNewGroup);
	if (tmpPos == -1){		
		ServerMapList.GetMap(ServerMapList.Get(incomeOldGroup)).TitleMap = incomeNewGroup;
		SndSystemMessage(MySystemMessage::AddGroup, ErrClient::NoError, new CString[2]{incomeOldGroup, incomeNewGroup});
		for (int i = 0; i < ServerSocketList->GetCount(); i++)
			SndRenameGroup(ServerSocketList->Ar[i]->userId, incomeOldGroup, incomeNewGroup);
		ShowAllMaps();//!!!
		return true;
	}
	else
		return false;
}
BOOL CTestChatDlg::RnmUser(CString incomeOldUser, CString incomeNewUser){
	int pos = ServerSocketList->FindUser(incomeOldUser);
	int tmpPos = ServerSocketList->FindUser(incomeNewUser); 
	if (tmpPos == -1){
		CString serverQuery;
		serverQuery.Format(L"RenameUser %s", incomeNewUser);
		ServerSocketList->Ar[ServerSocketList->FindUser(incomeOldUser)]->Send(serverQuery, serverQuery.GetLength() * 2 + 1);
		ServerSocketList->Ar[pos]->Nick = incomeNewUser;		
		ShowAllUsers();//!!!
		return true;
	}
	else
		return false;
}

void CTestChatDlg::OnBnClickedButtonMapDelete()
{
	// TODO: добавьте свой код обработчика уведомлений
	POSITION CurSelPos = m_ListMaps.GetFirstSelectedItemPosition();	

	if (CurSelPos != NULL){
		UINT pos = m_ListMaps.GetNextSelectedItem(CurSelPos);//-1
		if (pos > 0){
			//Рассылка груп на клиенты, перемещение клиентов из удаленной группы в общую группу
			CString myMessage;
			myMessage.Format(L": ChatRoom [%s] was deleted", m_ListMaps.GetItemText(pos, 0));
			//!!!ServerSocketList->SendToAll(CTime::GetCurrentTime(), L"DeleteChat", L"System", myMessage);//New Chat
			SndSystemMessage(MySystemMessage::EraseGroup, ErrClient::NoError, new CString[1]{m_ListMaps.GetItemText(pos, 0)});
			
			for (int i = 0; i < ServerSocketList->GetCount(); i++)
				SndDeleteGroup(ServerSocketList->Ar[i]->userId, m_ListMaps.GetItemText(pos, 0));
			
			ServerMapList.Remove(m_ListMaps.GetItemText(pos, 0));						
			m_ListMaps.DeleteItem(pos);	

			ShowAllMaps();//!!!
			//!!!Отправка клиентов из группы в общую группу
		}
		else
			MessageBox(L"Нельзя удалить данную группу");		
	}
	else
		MessageBox(L"Выберите группу");
	
}

void CTestChatDlg::ShowAllMaps(){
	m_ListMaps.DeleteAllItems();
	POSITION curPos = ServerMapList.GetHead();
	int tmpPos, counterUs;
	MyMap tmpMap;
	CString tmpStr;
	for (int i = 0; i < ServerMapList.Maps.GetCount(); i++){
		counterUs = 0;
		tmpMap = ServerMapList.GetNext(curPos);
		tmpPos = m_ListMaps.InsertItem(m_ListMaps.GetItemCount(), tmpMap.TitleMap);
		for (int i = 0; i < ServerSocketList->GetCount(); i++)
			if (ServerSocketList->Ar[i]->curMapId == tmpMap.IdMap) counterUs++;		
		tmpStr.Format(L"%d", counterUs);
		m_ListMaps.SetItemText(tmpPos, 1, tmpStr);
	}
	ShowAllUsers();
}
void CTestChatDlg::ShowAllUsers(){
	m_ListUsers.DeleteAllItems();
	for (int i = 0; i < ServerSocketList->GetCount(); i++)
		if (ServerSocketList->Ar[i]->curMapId == this->m_CurSelGroup)
			m_ListUsers.InsertItem(m_ListUsers.GetItemCount(), ServerSocketList->Ar[i]->Nick);
}
//
void CTestChatDlg::SndAddGroup(UINT incomeIdUser, CString incomeTitleGroup){
	CString sendMessage;
	sendMessage.Format(L"AddGroup %s", incomeTitleGroup);
	ServerSocketList->GetUser(incomeIdUser)->Send(sendMessage, sendMessage.GetLength() * 2 + 1);
}

void CTestChatDlg::SndDeleteGroup(UINT incomeIdUser, CString incomeTitleGroup){
	CString sendMessage;
	sendMessage.Format(L"DeleteGroup %s", incomeTitleGroup);
	ServerSocketList->GetUser(incomeIdUser)->Send(sendMessage, sendMessage.GetLength() * 2 + 1);

	/*for (int i = 0; i < ServerSocketList->GetCount(); i++)
		if (ServerSocketList->Ar[i]->curMapId == ServerMapList.GetIdMap(incomeTitleGroup)){*/
	if (ServerSocketList->GetUser(incomeIdUser)->curMapId == ServerMapList.GetIdMap(incomeTitleGroup)){
		Sleep(50);
		ChnUserGroup(incomeIdUser, L"Все");
	}
			//ChnUserGroup(ServerSocketList->Ar[i]->userId, L"Все");
		//}
}
void CTestChatDlg::SndRenameGroup(UINT incomeIdUser, CString incomeOldTitleGroup, CString incomeNewTitleGroup) {
	CString sendMessage;//111
	sendMessage.Format(L"RenameGroup %s %s", incomeOldTitleGroup, incomeNewTitleGroup);
	ServerSocketList->GetUser(incomeIdUser)->Send(sendMessage, sendMessage.GetLength() * 2 + 1);	
}
void CTestChatDlg::ChnUserGroup(UINT incomeIdUser, CString incomeTitleGroup){
	int pos = ServerMapList.GetIdMap(incomeTitleGroup);
	if (pos > -1 && pos != ServerSocketList->GetUser(incomeIdUser)->curMapId){
		CString myMessage;
		CTime currentTime = CTime::GetCurrentTime();
		myMessage.Format(L": User [%s] has joined to ChatRoom [%s]", ServerSocketList->GetUser(incomeIdUser)->Nick, incomeTitleGroup);
		ServerSocketList->SendToGroup(currentTime, L"Registration", L"System", pos, myMessage);// оповещание о новом пользователе
		ServerSocketList->GetUser(incomeIdUser)->curMapId = pos;

		CString queryServer, logMessage;
		queryServer.Format(L"ChangeGroup %s", incomeTitleGroup);
		Sleep(50);
		ServerSocketList->GetUser(incomeIdUser)->Send(queryServer, queryServer.GetLength() * 2 + 1);// Подтверждение смены чата

		logMessage.Format(L"%02i:%02i:%02i [System] %s", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), myMessage);
		m_ListLog.InsertItem(m_ListLog.GetItemCount(), logMessage);// лог сервера

		ShowAllMaps();//!!!
	}
}

void CTestChatDlg::AddUser(){ // Регистрация нового пользователя
	m_sSocket->Accept(*(ServerSocketList->Ar[ServerSocketList->AddUser(L"Unknown", m_CntUsers++)]));
	//SndSystemMessage(MySystemMessage::EnterUser, ErrClient::NoError, new CString[1]{L"Unknown"});
}

void CTestChatDlg::DeleteUser(ErrClient errAlert, CString incomingUser, UINT incomingId, CString incominTmpNick){ // Дисконект клиента от сервера	
	if (errAlert == ErrClient::NoError){ // Дисконект от клиента
		int pos = ServerSocketList->FindUser(incomingUser, incomingId);
		CString myMessage;
		myMessage.Format(L": User [%s] has left the ChatRoom", ServerSocketList->Ar[pos]->Nick);
		ServerSocketList->SendToGroup(CTime::GetCurrentTime(), L"UserExit", L"System", ServerSocketList->Ar[pos]->curMapId, myMessage);
		SndSystemMessage(MySystemMessage::ExitUser, errAlert, new CString[1]{incomingUser});		
	}
	else if (errAlert == ErrClient::ErrorNick){ // Ошибка регистрации пользователя
		ServerSocketList->SendToUser(CTime::GetCurrentTime(), L"ErrorRegistration", L"System", incomingUser, incomingId, L": You has not been added because of the similarity nicknames");
		SndSystemMessage(MySystemMessage::ExitUser, errAlert, new CString[1]{incominTmpNick});
	}
	else if (errAlert == ErrClient::Expelled){
		ServerSocketList->SendToUser(CTime::GetCurrentTime(), L"ErrorRegistration", L"System", incomingUser, incomingId, L": You was expelled from ChatRoom");
		SndSystemMessage(MySystemMessage::ExitUser, errAlert, new CString[1]{incominTmpNick});
	}
	ServerSocketList->DeleteUser(incomingUser, incomingId);
	ShowAllMaps();//!!!
}

void CTestChatDlg::ChnUserNick(CString incomingUser, CString incomingNick){
	CString serverQuery;
	serverQuery.Format(L"RenameUser %s", incomingNick);
	ServerSocketList->Ar[ServerSocketList->FindUser(incomingUser)]->Send(serverQuery, serverQuery.GetLength() * 2 + 1);	
}

void CTestChatDlg::SetUserNick(CString incomingUser, UINT incomingId, CString incomingNick){ // Переименование клиента
	if (ServerSocketList->ChangeNick(incomingUser, incomingId, incomingNick)){
		if (incomingUser == "Unknown"){	// Подтверждение регистрации пользователя
			int pos = ServerSocketList->FindUser(incomingNick, incomingId);
			CString myMessage;
			myMessage.Format(L": User [%s] has joined to ChatRoom [%s]", incomingNick, ServerMapList.GetTitleMap(ServerSocketList->Ar[pos]->curMapId));
			ServerSocketList->SendToGroup(CTime::GetCurrentTime(), L"Registration", L"System", ServerSocketList->Ar[pos]->curMapId, myMessage);//new user send to curr group
			SndSystemMessage(MySystemMessage::EnterUser, ErrClient::NoError, new CString[1]{incomingNick});			
			//!!!Отправка групп
			for (int i = 0; i < m_ListMaps.GetItemCount(); i++){
				Sleep(50);
				SndAddGroup(ServerSocketList->Ar[pos]->userId, m_ListMaps.GetItemText(i, 0));				
			}
			ShowAllMaps();//!!!
		}
		else{ // Подтверждение переименования пользователя			
			int pos = ServerSocketList->FindUser(incomingNick, incomingId);
			CString myMessage;
			myMessage.Format(L": User [%s] change his name to [%s]", incomingUser,incomingNick);
			ServerSocketList->SendToGroup(CTime::GetCurrentTime(), L"Rename", L"System", ServerSocketList->Ar[pos]->curMapId, myMessage);//new user send to curr group
			SndSystemMessage(MySystemMessage::ChangeNick, ErrClient::NoError, new CString[2]{incomingUser, incomingNick});
		}
	}
	else{		
		if (incomingUser == "Unknown"){	// Ошибка регистрации пользователя
			DeleteUser(ErrClient::ErrorNick, incomingUser, incomingId, incomingNick);
			ShowAllMaps();//!!!
		}
		else // Ошибка переименования пользователя
		{			
			int pos = ServerSocketList->FindUser(incomingNick, incomingId);
			CString myMessage;
			myMessage.Format(L": Change nick error", incomingUser, incomingNick);
			ServerSocketList->SendToUser(CTime::GetCurrentTime(), L"ErrorRename", L"System", ServerSocketList->Ar[pos]->Nick, ServerSocketList->Ar[pos]->userId, myMessage);//new user send to curr group
			SndSystemMessage(MySystemMessage::ChangeNick, ErrClient::ErrorNick, new CString[1]{incomingUser});
		}
	}
}

void CTestChatDlg::SndSystemMessage(MySystemMessage SystemMessageType, ErrClient errAlert, CString* incomingMessage){ // Системные оповещания 
	CString systemMessage;
	CTime currentTime = CTime::GetCurrentTime();
	switch (SystemMessageType)
	{// Команды
	case MySystemMessage::AddGroup:
		systemMessage.Format(L"%02i:%02i:%02i [System]: Group [%s] was added", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		break;
	case MySystemMessage::EraseGroup:
		systemMessage.Format(L"%02i:%02i:%02i [System]: Group [%s] was deleted", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		break;
	case MySystemMessage::EnterUser:
		systemMessage.Format(L"%02i:%02i:%02i [System]: User [%s] has entered to chat", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		break;
	case MySystemMessage::ExitUser:
		if (errAlert == ErrClient::NoError)
			systemMessage.Format(L"%02i:%02i:%02i [System]: User [%s] has left the chat", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		else if (errAlert == ErrClient::ErrorNick)
			systemMessage.Format(L"%02i:%02i:%02i [System]: User [%s] has not been added because of the similarity nicknames", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		break;
	case MySystemMessage::ChangeNick:	
		if (errAlert == ErrClient::NoError)
			systemMessage.Format(L"%02i:%02i:%02i [System]: User [%s] change his name to [%s]", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0], incomingMessage[1]);
		else if (errAlert == ErrClient::ErrorNick)
			systemMessage.Format(L"%02i:%02i:%02i [System]: User [%s] change nick error", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		break;
	case MySystemMessage::ClearGroup:
		systemMessage.Format(L"%02i:%02i:%02i [System]: Group [%s] was cleared", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0]);
		break;
	case MySystemMessage::RenameGroup:
		systemMessage.Format(L"%02i:%02i:%02i [System]: ChatRoom [%s] was renamed to [%s]", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0], incomingMessage[1]);		
		break;
	/*case MySystemMessage::ChangeGroup:
		systemMessage.Format(L"%02i:%02i:%02i [System]: User [%s] has change the chat room from [%s] to [%s]", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), incomingMessage[0], incomingMessage[1], incomingMessage[2]);
		break;*/
	default:
		break;
	}
	m_ListLog.InsertItem(m_ListLog.GetItemCount(), systemMessage);
	//delete incomingMessage;
}

void CTestChatDlg::SndUserMessage(MyUserMessage UserMessageType, CString incomeNick, UINT incomeGroupId, CString incomingMessage){ // Отправка сообщений
	CString sendMessage, logMessage;
	CTime currentTime = CTime::GetCurrentTime();
	switch (UserMessageType)
	{
	case UserSendMessage:
		sendMessage.Format(L"[%s]: %s", incomeNick, incomingMessage);
		if (incomeGroupId == 0)
			ServerSocketList->SendToAll(currentTime, L"Message", ServerMapList.GetTitleMap(incomeGroupId), sendMessage);
		else		
			ServerSocketList->SendToGroup(currentTime, L"Message", ServerMapList.GetTitleMap(incomeGroupId), incomeGroupId, sendMessage);
		break;
	case UserChangeNick:
		break;
	case UserChangeGroup:
		break;
	case UserAddGroup:
		break;
	case UserDeleteGroup:
		break;
	default:
		break;
	}
	logMessage.Format(L"%02i:%02i:%02i [%s] %s", currentTime.GetHour(), currentTime.GetMinute(), currentTime.GetSecond(), ServerMapList.GetTitleMap(incomeGroupId), sendMessage);
	m_ListLog.InsertItem(m_ListLog.GetItemCount(), logMessage);
}

void CTestChatDlg::OnBnClickedButtonSend()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!UpdateData(TRUE)) return;
	if (m_MessageV.GetLength() > 1){
		CString myQuery;
		myQuery.Format(L"[Admin]: %s", m_MessageV);
		ServerSocketList->SendToAll(CTime::GetCurrentTime(), L"Message", L"Все", myQuery);
		
		myQuery.Format(L"[Все] %s", m_MessageV);
		m_ListLog.InsertItem(m_ListLog.GetItemCount(), myQuery);
		m_MessageV = "";
		UpdateData(FALSE);
	}
}

BOOL CTestChatDlg::ShowPopup(UINT nID, int x, int y, CWnd* parent)
{
	if (contextMenu != NULL && contextMenu->IsShown()){ contextMenu->DestroyWindow(); contextMenu = NULL; }

	/*contextMenu = new CMFCPopupMenu();
	CMenu menu;
	menu.LoadMenu(nID);
	if (!contextMenu->Create(parent, x, y, menu.GetSubMenu(0)->m_hMenu, 0, TRUE))
		return FALSE;
	menu.Detach();
	return contextMenu->ShowWindow(SW_SHOW);*/
	CMenu menu;
	CMenu* poupMenu;
	menu.LoadMenu(nID);
	poupMenu = menu.GetSubMenu(0);
	ASSERT(poupMenu);
	poupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, x, y, parent);
	return TRUE;
}
BOOL CTestChatDlg::IsInDiap(CRect incomeWindowRect, CPoint incomeMousePoints){
	CRgn tmpRgn;
	CPoint* tmpPts = new CPoint[4]{
		{incomeWindowRect.left, incomeWindowRect.top}, {incomeWindowRect.right, incomeWindowRect.top}, {incomeWindowRect.right, incomeWindowRect.bottom}, {incomeWindowRect.left, incomeWindowRect.bottom}
	};
	tmpRgn.CreatePolygonRgn(tmpPts, 4, ALTERNATE);
	delete[]tmpPts;
	return tmpRgn.PtInRegion(incomeMousePoints);
}

void CTestChatDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	if (contextMenu != NULL && contextMenu->IsShown()){
		CRect rectWindow1, rectWindow2, rectWindow3;
		m_ListMaps.GetWindowRect(&rectWindow1);
		m_ListUsers.GetWindowRect(&rectWindow2);
		contextMenu->GetWindowRect(&rectWindow3);
		if (!IsInDiap(rectWindow1, point) && !IsInDiap(rectWindow2, point) && !IsInDiap(rectWindow3, point)){ contextMenu->DestroyWindow(); contextMenu = NULL; }
	}
	CDialogEx::OnMouseMove(nFlags, point);
}

void CTestChatDlg::OnNMClickListMaps(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);	
	if (pNMItemActivate->iItem > -1 && pNMItemActivate->iItem != m_CurSelGroup)	{ m_CurSelGroup = pNMItemActivate->iItem; ShowAllUsers(); }
	if (contextMenu != NULL && contextMenu->IsShown())	{ contextMenu->DestroyWindow(); contextMenu = NULL; }
	// TODO: добавьте свой код обработчика уведомлений
	*pResult = 0;
}

void CTestChatDlg::OnNMRClickListMaps(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	
	if (pNMItemActivate->iItem > -1){
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		m_ListMaps.SetFocus();
		//ShowPopup(IDR_MENU_GROUPS, cursorPos.x, cursorPos.y, (CWnd*)&m_ListMaps);
		//OnContextMenu((CWnd*)&m_ListMaps, cursorPos);
	}
	else
		if (contextMenu != NULL && contextMenu->IsShown())	{ contextMenu->DestroyWindow(); contextMenu = NULL; }
	*pResult = 0;
}

void CTestChatDlg::OnNMRClickListUsers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	if (pNMItemActivate->iItem > -1){				
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		m_ListUsers.SetFocus();
		//ShowPopup(IDR_MENU_USERS, cursorPos.x, cursorPos.y, (CWnd*)&m_ListUsers);
		//OnContextMenu((CWnd*)&m_ListUsers, cursorPos);
	}
	else
		if (contextMenu != NULL && contextMenu->IsShown())	{ contextMenu->DestroyWindow(); contextMenu = NULL; }
	*pResult = 0;
}

void CTestChatDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений  theApp.GetContextMenuManager()->ShowPopupMenu(	
	/*if (contextMenuType)
		ShowPopup(IDR_MENU_USERS, point.x, point.y, pWnd);
	else
		ShowPopup(IDR_MENU_GROUPS, point.x, point.y, pWnd);*/
	//theApp.cloe
	CMenu menu;
	CMenu* poupMenu;
	if ((pWnd == (CWnd*)&m_ListMaps && m_ListMaps.GetFirstSelectedItemPosition() != NULL) || (pWnd == (CWnd*)&m_ListUsers && m_ListUsers.GetFirstSelectedItemPosition() != NULL)){
		menu.LoadMenu((pWnd == (CWnd*)&m_ListMaps) ? IDR_MENU_GROUPS : IDR_MENU_USERS);
		poupMenu = menu.GetSubMenu(0);
		ASSERT(poupMenu);
		poupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	//return TRUE;
}

void CTestChatDlg::OnNMKillfocusListMaps(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений
	if (contextMenu != NULL && contextMenu->IsShown())	{ contextMenu->DestroyWindow(); contextMenu = NULL; }
	*pResult = 0;
}

void CTestChatDlg::OnNMClickListUsers(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: добавьте свой код обработчика уведомлений
	if (contextMenu != NULL && contextMenu->IsShown())	{ contextMenu->DestroyWindow(); contextMenu = NULL; }
	*pResult = 0;
}

void CTestChatDlg::OnNMKillfocusListUsers(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: добавьте свой код обработчика уведомлений
	if (contextMenu != NULL && contextMenu->IsShown())	{ contextMenu->DestroyWindow(); contextMenu = NULL; }
	*pResult = 0;
}

void CTestChatDlg::OnElementsCommandDelete()
{
	// TODO: добавьте свой код обработчика команд
	this->OnBnClickedButtonMapDelete();
}

void CTestChatDlg::OnElementsCommandClear()
{
	// TODO: добавьте свой код обработчика команд
	POSITION CurSelPos = m_ListMaps.GetFirstSelectedItemPosition();
	UINT pos = m_ListMaps.GetNextSelectedItem(CurSelPos);//-1
	if (pos > 0){
		for (int i = 0; i < ServerSocketList->GetCount(); i++)
			if (ServerSocketList->Ar[i]->curMapId == ServerMapList.GetIdMap(m_ListMaps.GetItemText(pos, 0))){
				Sleep(50);
				ChnUserGroup(ServerSocketList->Ar[i]->userId, L"Все");
			}
		SndSystemMessage(MySystemMessage::ClearGroup, ErrClient::NoError, new CString[1]{m_ListMaps.GetItemText(pos, 0)});
		//MessageBox(L"Ок");
	}
	else
		MessageBox(L"Нельзя очистить данную группу");
}

void CTestChatDlg::OnElementsCommandRename()
{
	// TODO: добавьте свой код обработчика команд
	CDialogRename tmpDlg;
	tmpDlg.modeRename = true;
	POSITION CurSelPos = m_ListMaps.GetFirstSelectedItemPosition();
	tmpDlg.oldMapTitle = m_ListMaps.GetItemText(m_ListMaps.GetNextSelectedItem(CurSelPos), 0);
	tmpDlg.theMFrame = this;
	tmpDlg.DoModal();	
}


void CTestChatDlg::OnElementsUserExpellFromGroup()
{
	// TODO: добавьте свой код обработчика команд
	POSITION CurSelPos = m_ListUsers.GetFirstSelectedItemPosition();
	int pos = m_ListUsers.GetNextSelectedItem(CurSelPos);//-1
	if (pos > -1)
		ChnUserGroup(ServerSocketList->Ar[ServerSocketList->FindUser(m_ListUsers.GetItemText(pos, 0))]->userId, L"Все");	
}

void CTestChatDlg::OnElementsUserDelete()
{
	// TODO: добавьте свой код обработчика команд
	POSITION CurSelPos = m_ListUsers.GetFirstSelectedItemPosition();
	int pos = m_ListUsers.GetNextSelectedItem(CurSelPos);//-1
	if (pos > -1)
		DeleteUser(ErrClient::Expelled, m_ListUsers.GetItemText(pos, 0), ServerSocketList->Ar[ServerSocketList->FindUser(m_ListUsers.GetItemText(pos, 0))]->userId);
}

void CTestChatDlg::OnElementsUserRename()
{
	// TODO: добавьте свой код обработчика команд
	CDialogRename tmpDlg;
	tmpDlg.modeRename = false;
	POSITION CurSelPos = m_ListUsers.GetFirstSelectedItemPosition();
	tmpDlg.oldMapTitle = m_ListUsers.GetItemText(m_ListUsers.GetNextSelectedItem(CurSelPos), 0);
	tmpDlg.theMFrame = this;
	tmpDlg.DoModal();
}

void CTestChatDlg::OnElementsUserWsp()
{
	// TODO: добавьте свой код обработчика команд
}
