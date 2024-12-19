
// MyDlg.h: файл заголовка
//

#pragma once
#include"SourceDialog.h"
#include"MainJob.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAGA_PICTURERESTORATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	bool IsPicture = false;
	CString targetpath;
	bool init = false;
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	Drawer SourcePicture;
	afx_msg void OnBnClickedButtonSource();
	Drawer CorruptedPicture;
	Drawer RestoredPicture;
	int N;
	double S;
	MainJob mj;

	afx_msg void OnBnClickedButtonSwapQuadrants();
	double ErrBefore;
	double ErrorAfter;
	double ErrorAfterSwap;
	double ErrorBlured;
	CEdit EBctrl;
	CEdit EBLctrl;
	CEdit ERctrl;
	CEdit ERWctrl;
	int Cutter;
};
