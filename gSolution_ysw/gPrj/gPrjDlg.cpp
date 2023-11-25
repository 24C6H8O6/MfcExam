
// gPrjDlg.cpp: 구현 파일

#include "pch.h"
#include "framework.h"
#include "gPrj.h"
#include "gPrjDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


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


// CgPrjDlg 대화 상자
CgPrjDlg::CgPrjDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GPRJ_DIALOG, pParent)

{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgPrjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_edit_radius);
	DDX_Control(pDX, IDC_EDIT_X, m_edit_X);
	DDX_Control(pDX, IDC_EDIT_Y, m_edit_Y);
}

BEGIN_MESSAGE_MAP(CgPrjDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_MAKE_CIRCLE, &CgPrjDlg::OnBnClickedBtnMakeCircle)
END_MESSAGE_MAP()


// CgPrjDlg 메시지 처리기

BOOL CgPrjDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	// DlgImage
	MoveWindow(0, 0, n_Parent_Width, n_Parent_Height);
	m_pDlgImage = new CDlgImage;
	m_pDlgImage->Create(IDD_DLGIMAGE, this);
	m_pDlgImage->ShowWindow(SW_SHOW);

	return TRUE;
}

void CgPrjDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CgPrjDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CgPrjDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 메모리 릭 제거
void CgPrjDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (m_pDlgImage) delete m_pDlgImage;
}

// 반지름에 맞는 원을 그리는 버튼
void CgPrjDlg::OnBnClickedBtnMakeCircle()
{
	unsigned char* fm = (unsigned char*)m_pDlgImage->m_Image.GetBits();

	// 입력한 반지름 값 가져오기
	CString n_radius_str;
	GetDlgItemText(IDC_EDIT_RADIUS, n_radius_str);
	int n_radius = _ttoi(n_radius_str);

	// 랜덤으로 좌표 설정
	int x = rand() % (m_pDlgImage->m_Image.GetWidth() - 2 * n_radius) + n_radius;
	int y = rand() % (m_pDlgImage->m_Image.GetHeight() - 2 * n_radius) + n_radius;

	// DlgImage 크기에 그려지도록 설정
	if ((x - n_radius < 0) || (y - n_radius < 0) || (x + n_radius > (m_pDlgImage->m_Image.GetWidth())) || (y + n_radius > (m_pDlgImage->m_Image.GetHeight())))
		AfxMessageBox(_T("원이 이미지 범위를 벗어납니다."));
	else {
		memset(fm, 0xff, (m_pDlgImage->m_Image.GetWidth()) * (m_pDlgImage->m_Image.GetHeight()));
		int nGray = 80;
		drawInCircle(fm, x, y, n_radius, nGray);

		CClientDC dc(m_pDlgImage);
		m_pDlgImage->m_Image.Draw(dc, 0, 0);
		
		drawCross(dc, x, y, n_radius);
		// 바깥 원 색깔
		COLORREF color = RGB(255, 255, 0);
		// 바깥 원 두께
		int outBorder = 2;
		drawOutCircle(dc, x, y, n_radius + 5, outBorder, color);

	}
}

// 내부 원 그리기
void CgPrjDlg::drawInCircle(unsigned char* fm, int x, int y, int nRadius, int nGray)
{
	int nPitch = m_pDlgImage->m_Image.GetPitch();

	for (int j = y - nRadius; j <= y + nRadius; j++)
	{
		for (int i = x - nRadius; i <= x + nRadius; i++)
		{
			if (isInCircle(i, j, x, y, nRadius))
			{
				fm[j * nPitch + i] = nGray;
			}
		}
	}
}

bool CgPrjDlg::isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius)
{
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = sqrt(dX * dX + dY * dY);

	if (dDist >= nRadius - inBorder && dDist <= nRadius) {
		bRet = true;
	}

	return bRet;
}

// 십자 표시 그리기
void CgPrjDlg::drawCross(CDC &pDC, int x, int y, int nRadius) {
	pDC.MoveTo(x- n_Cross_Size, y);
	pDC.LineTo(x+ n_Cross_Size+1, y);
	pDC.MoveTo(x, y- n_Cross_Size);
	pDC.LineTo(x, y+ n_Cross_Size+3);
}

// 외부 원 그리기
void CgPrjDlg::drawOutCircle(CDC &pDC, int x, int y, int nRadius, int outBorder, COLORREF color) {
	CString strX;
	CString strY;
	strX.Format(_T("%d"), x);
	strY.Format(_T("%d"), y);
	m_edit_X.SetWindowTextW(strX);
	m_edit_Y.SetWindowTextW(strY);
	// 원 내부 투명하게 하기
	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	CBrush* pBrush = pDC.SelectObject(&brush);
	// 펜으로 원 그리기
	CPen n_Pen(PS_SOLID, outBorder, color);
	CPen* pPen = pDC.SelectObject(&n_Pen);
	pDC.Ellipse(x - nRadius - outBorder, y - nRadius - outBorder, x + nRadius + outBorder, y + nRadius + outBorder);
	pDC.SelectObject(pBrush);
	pDC.SelectObject(pPen);
}

