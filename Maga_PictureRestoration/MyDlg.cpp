
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Maga_PictureRestoration.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MAGA_PICTURERESTORATION_DIALOG, pParent)
	, N(7)
	, S(1)
	, ErrBefore(0)
	, ErrorAfter(0)
	, ErrorAfterSwap(0)
	, ErrorBlured(0)
	, Cutter(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SOURCE_PICTURE, SourcePicture);
	DDX_Control(pDX, IDC_CORRUPTED_PICTURE, CorruptedPicture);
	DDX_Control(pDX, IDC_RESTORED_PICTURE, RestoredPicture);
	DDX_Text(pDX, IDC_EDIT1, N);
	DDX_Text(pDX, IDC_EDIT4, S);
	DDX_Text(pDX, IDC_EDIT5, ErrBefore);
	DDX_Text(pDX, IDC_EDIT6, ErrorAfter);
	DDX_Text(pDX, IDC_EDIT7, ErrorAfterSwap);
	DDX_Text(pDX, IDC_EDIT8, ErrorBlured);
	DDX_Control(pDX, IDC_EDIT5, EBctrl);
	DDX_Control(pDX, IDC_EDIT8, EBLctrl);
	DDX_Control(pDX, IDC_EDIT6, ERctrl);
	DDX_Control(pDX, IDC_EDIT7, ERWctrl);
	DDX_Text(pDX, IDC_EDIT9, Cutter);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_SOURCE, &MyDlg::OnBnClickedButtonSource)
	ON_BN_CLICKED(IDC_BUTTON_SWAP_QUADRANTS, &MyDlg::OnBnClickedButtonSwapQuadrants)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	SourcePicture.ShowPicture(false);
	SourcePicture.SetTitle(L"Исходное изображение");
	SourcePicture.SetPadding(12, 5, 5, 8);

	CorruptedPicture.ShowPicture(false);
	CorruptedPicture.SetTitle(L"Размытое изображение");
	CorruptedPicture.SetPadding(0, 0, 0, 0);

	RestoredPicture.ShowPicture(false);
	RestoredPicture.SetTitle(L"H");
	RestoredPicture.SetPadding(0, 0, 0, 0);
	RestoredPicture.SetLogarithmic(false);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void MyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void MyDlg::OnPaint()
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
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	if (!init)return;
	UpdateData();
	mj = MainJob();
	mj.SetPath(targetpath, IsPicture);
	mj.SetGauss(N,S);
	mj.SetCutter(Cutter);
	mj.main();



	SourcePicture.SetData(mj.GetStretchedData());
	SourcePicture.Invalidate();

	CorruptedPicture.SetData(mj.GetBluredData());
	CorruptedPicture.Invalidate();

	RestoredPicture.SetData(mj.GetRestoredData());
	RestoredPicture.Invalidate();

	/*ErrBefore = mj.GetEstSource();
	ErrorAfter = mj.GetEstPurified();
	ErrorAfterSwap = mj.GetEstPurifiedSwap();
	ErrorBlured = mj.GetEstBlured();
	UpdateData(FALSE);*/
	CString str;

	str.Format(L"%.3f", mj.GetEstSource());
	EBctrl.SetWindowTextW(str);

	str.Format(L"%.3f", mj.GetEstBlured());
	EBLctrl.SetWindowTextW(str);

	str.Format(L"%.3f", mj.GetEstPurified());
	ERctrl.SetWindowTextW(str);

	str.Format(L"%.3f", mj.GetEstPurifiedSwap());
	ERWctrl.SetWindowTextW(str);
}


void MyDlg::OnBnClickedButtonSource()
{
	// TODO: добавьте свой код обработчика уведомлений
	SourceDialog dialog;
	if (dialog.DoModal() != IDOK) return;

	IsPicture = dialog.IsPicture;
	targetpath = dialog.targetpath;
	SourcePicture.ShowPicture(IsPicture);
	if (IsPicture)
	{
		/*SourcePicture.ShowPicture(false);
		ImageConverter conv;
		conv.SetImagePath(targetpath);
		conv.Convert();
		RangeStretcher rstr;
		rstr.ConvertRange(conv.GetData());
		SourcePicture.SetData(rstr.GetStretched());*/
		SourcePicture.SetPicturePath(targetpath);
	}
	else
	{
		GaussPreloader gauss;
		gauss.SetPath(targetpath);
		gauss.Load();
		SourcePicture.SetData(gauss.GetData());
	}
	SourcePicture.Invalidate();
	init = true;
}


void MyDlg::OnBnClickedButtonSwapQuadrants()
{
	// TODO: добавьте свой код обработчика уведомлений
	mj.SwapQuadrants();
	RestoredPicture.SetData(mj.GetRestoredData());
	RestoredPicture.Invalidate();
}
