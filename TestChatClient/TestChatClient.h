
// TestChatClient.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTestChatClientApp:
// О реализации данного класса см. TestChatClient.cpp
//

class CTestChatClientApp : public CWinApp
{
public:
	CTestChatClientApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTestChatClientApp theApp;