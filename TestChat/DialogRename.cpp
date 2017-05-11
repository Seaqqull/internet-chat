// DialogRename.cpp: файл реализации
//

#include "stdafx.h"
#include "TestChat.h"
#include "DialogRename.h"
#include "afxdialogex.h"


// диалоговое окно CDialogRename

IMPLEMENT_DYNAMIC(CDialogRename, CDialogEx)

CDialogRename::CDialogRename(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogRename::IDD, pParent)
	, m_StringRename(_T(""))
{	
}

CDialogRename::~CDialogRename()
{
}

void CDialogRename::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_StringRename);
	DDV_MaxChars(pDX, m_StringRename, 20);
}


BEGIN_MESSAGE_MAP(CDialogRename, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDialogRename::OnBnClickedOk)
END_MESSAGE_MAP()


// обработчики сообщений CDialogRename


void CDialogRename::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений	
	if (UpdateData(TRUE)){
		if (modeRename){
			if (m_StringRename.GetLength() > 0){
				if (theMFrame->RnmGroup(oldMapTitle, m_StringRename))	CDialogEx::OnOK();
				else
					MessageBox(L"Група с данным названием уже существует");
			}
			else
				MessageBox(L"Введите название группы");
		}
		else{
			if (m_StringRename.GetLength() > 2){
				if (theMFrame->RnmUser(oldMapTitle, m_StringRename))	CDialogEx::OnOK();
				else
					MessageBox(L"Пользователь с данным ником уже существует");
			}
			else
				MessageBox(L"Введите название имя пользователя");
		}			
	}
	else
		MessageBox(L"Ошибка");
}
