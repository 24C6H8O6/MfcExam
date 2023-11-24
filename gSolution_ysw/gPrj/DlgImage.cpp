// DlgImage.cpp: 구현 파일
//

#include "pch.h"
#include "gPrj.h"
#include "afxdialogex.h"
#include "DlgImage.h"
#include "gPrjDlg.h"

// CDlgImage 대화 상자
IMPLEMENT_DYNAMIC(CDlgImage, CDialogEx)

CDlgImage::CDlgImage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGIMAGE, pParent)
{

}

CDlgImage::~CDlgImage()
{
}

void CDlgImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgImage, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgImage 메시지 처리기

BOOL CDlgImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(0, 0, n_Child_Width, n_Child_Height);
	InitImage();

	return TRUE;
}

void CDlgImage::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_Image) {
		m_Image.Draw(dc, 0, 0);
	}
}

void CDlgImage::InitImage()
{
	int nBpp = 8;
	m_Image.Create(n_Child_Width, -n_Child_Height, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_Image.SetColorTable(0, 256, rgb);
	}
	int nPitch = m_Image.GetPitch();
	unsigned char* fm = (unsigned char*)m_Image.GetBits();

	memset(fm, 0xff, n_Child_Width * n_Child_Height);
}