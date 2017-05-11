
// TestChatClientDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "TestChatClient.h"
#include "TestChatClientDlg.h"
#include "afxdialogex.h"

#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// диалоговое окно CTestChatClientDlg



CTestChatClientDlg::CTestChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestChatClientDlg::IDD, pParent)
	, m_PortV(22345)
	, m_AddressV(_T("127.0.0.1"))
	, m_LoginV(_T(""))
	, m_MessageV(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_PortV);
	DDV_MinMaxUInt(pDX, m_PortV, 1, 65535);
	//DDX_Text(pDX, IDC_EDIT_ADDRESS, m_AddressV);
	//DDV_MaxChars(pDX, m_AddressV, 15);
	DDX_Control(pDX, IDC_IPADDRESS_CTRL, m_AdressCtrl);
	DDX_Text(pDX, IDC_EDIT_LOGIN, m_LoginV);
	DDV_MaxChars(pDX, m_LoginV, 20);
	DDX_Control(pDX, IDC_LIST_LOG, m_ListLog);
	DDX_Control(pDX, IDC_LIST_MAPS, m_ListMaps);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_MessageV);
	DDV_MaxChars(pDX, m_MessageV, 100);
	DDX_Control(pDX, IDC_EDIT_PORT, m_PortCtrl);
	DDX_Control(pDX, IDC_EDIT_LOGIN, m_LoginCtrl);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ConnectCtrl);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT, m_DisconnectCtrl);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_MessageCtrl);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_SendCtrl);
	DDX_Control(pDX, IDC_BUTTON_MAP_CHANGE, m_MapCh);
}

BEGIN_MESSAGE_MAP(CTestChatClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CTestChatClientDlg::OnBnClickedButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT, &CTestChatClientDlg::OnBnClickedButtonDisconnect)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CTestChatClientDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_MAP_CHANGE, &CTestChatClientDlg::OnBnClickedButtonMapChange)
END_MESSAGE_MAP()


// обработчики сообщений CTestChatClientDlg

BOOL CTestChatClientDlg::OnInitDialog()
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
	m_AdressCtrl.SetWindowText(m_AddressV);

	m_ListLog.SetExtendedStyle(m_ListLog.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);
	m_ListLog.InsertColumn(0, L"Сообщения", LVCFMT_CENTER, 323);

	m_ListMaps.SetExtendedStyle(m_ListMaps.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_FLATSB | LVS_EX_INFOTIP | LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE);
	m_ListMaps.InsertColumn(0, L"Группы", LVCFMT_CENTER, 128);

	errorMessage = "";

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CTestChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestChatClientDlg::OnPaint()
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
HCURSOR CTestChatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestChatClientDlg::ShowConnected(){
	m_ConnectCtrl.EnableWindow(FALSE);
	m_PortCtrl.EnableWindow(FALSE);
	m_LoginCtrl.EnableWindow(FALSE);
	m_AdressCtrl.EnableWindow(FALSE);
	

	m_DisconnectCtrl.EnableWindow(TRUE);
	m_MessageCtrl.EnableWindow(TRUE);
	m_SendCtrl.EnableWindow(TRUE);
	m_MapCh.EnableWindow(TRUE);

	errorMessage = "";
	//m_ListLog	
}
void CTestChatClientDlg::ShowDisconnected(){
	m_ConnectCtrl.EnableWindow(TRUE);
	m_PortCtrl.EnableWindow(TRUE);
	m_LoginCtrl.EnableWindow(TRUE);
	m_AdressCtrl.EnableWindow(TRUE);	

	m_DisconnectCtrl.EnableWindow(FALSE);
	m_MessageCtrl.EnableWindow(FALSE);
	m_SendCtrl.EnableWindow(FALSE);
	m_MapCh.EnableWindow(FALSE);


	m_ListLog.DeleteAllItems();
	m_ListMaps.DeleteAllItems();
}

void CTestChatClientDlg::DeleteGroup(CString incomeTitleGroup){
	for (int i = 0; i < m_ListMaps.GetItemCount(); i++)
		if (m_ListMaps.GetItemText(i, 0) == incomeTitleGroup)	m_ListMaps.DeleteItem(i);	
}
void CTestChatClientDlg::AddGroup(CString incomeTitleGroup){
	m_ListMaps.InsertItem(m_ListMaps.GetItemCount(), incomeTitleGroup);
}
void CTestChatClientDlg::ChangeGroup(CString incomeTitleGroup){
	m_ListLog.DeleteAllItems();
	ChangeWindowTtle(incomeTitleGroup);
}
void CTestChatClientDlg::RenameGroup(CString incomeOldGroup, CString incomeNewGroup){
	CString titleWdw; 
	GetWindowText(titleWdw);
	int pos1, pos2;
	pos1 = titleWdw.FindOneOf(L"[");
	pos2 = titleWdw.Find(L"]", pos1 + 1);
	titleWdw = titleWdw.Mid(pos1 + 1, pos2 - (pos1 + 1));
	for (int i = 0; i < m_ListMaps.GetItemCount(); i++)
		if (m_ListMaps.GetItemText(i, 0) == incomeOldGroup){
			m_ListMaps.SetItemText(i, 0, incomeNewGroup);
			break;
		}
	if (titleWdw == incomeOldGroup) ChangeWindowTtle(incomeNewGroup);
	
}
void CTestChatClientDlg::ChangeWindowTtle(CString incomeTitleGroup){
	CString title;
	title.Format(L"Chat: [%s] [%s]", incomeTitleGroup, m_LoginV);
	SetWindowText(title);
}


void CTestChatClientDlg::OnBnClickedButtonConnect()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!UpdateData(TRUE)) return;
	
	m_AdressCtrl.GetWindowText(m_AddressV);
	m_LoginV = m_LoginV.Trim();
	
	if (m_LoginV.GetLength() > 2){
		m_cSocket = new CClientSocket(this);
		m_cSocket->Create();
		if (m_cSocket->Connect(m_AddressV, m_PortV)){
			CString clientQuery;
			clientQuery.Format(L"Registration %s", m_LoginV);
			m_cSocket->Send(clientQuery, clientQuery.GetLength() * 2 + 1);
			ShowConnected();
			ChangeWindowTtle(L"Все");
		}
		else
			MessageBox(_T("Error: Server enabled"));
	}
	/*if ()*/
		//m_SendButton.EnableWindow(TRUE);
}


void CTestChatClientDlg::OnBnClickedButtonDisconnect()
{
	// TODO: добавьте свой код обработчика уведомлений
	m_cSocket->Close();
	ShowDisconnected();
	SetWindowText(L"Chat");
}


void CTestChatClientDlg::OnBnClickedButtonSend()
{	
	//// TODO: добавьте свой код обработчика уведомлений
	if (!UpdateData(TRUE)) return;
	if (m_MessageV.GetLength() > 1){
		CString myQuery;
		myQuery.Format(L"Message %s", m_MessageV);
		m_cSocket->Send(myQuery, myQuery.GetLength() * 2 + 1);
		m_MessageV = "";
		UpdateData(FALSE);
	}
}


void CTestChatClientDlg::OnBnClickedButtonMapChange()
{
	// TODO: добавьте свой код обработчика уведомлений
	CString clientQuery;
	POSITION CurSelPos = m_ListMaps.GetFirstSelectedItemPosition();
	int pos = m_ListMaps.GetNextSelectedItem(CurSelPos);//-1	
	if (pos > -1){
		clientQuery.Format(L"ChangeGroup %s", m_ListMaps.GetItemText(pos, 0));
		m_cSocket->Send(clientQuery, clientQuery.GetLength() * 2 + 1);
	}
	else
		MessageBox(L"Выберите группу");
}
