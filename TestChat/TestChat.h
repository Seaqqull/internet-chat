
// TestChat.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTestChatApp:
// О реализации данного класса см. TestChat.cpp
//

class CTestChatApp : public CWinApp
{
public:
	CTestChatApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestChatApp theApp;