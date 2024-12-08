
// Maga_PictureRestoration.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMagaPictureRestorationApp:
// Сведения о реализации этого класса: Maga_PictureRestoration.cpp
//

class CMagaPictureRestorationApp : public CWinApp
{
public:
	CMagaPictureRestorationApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMagaPictureRestorationApp theApp;
