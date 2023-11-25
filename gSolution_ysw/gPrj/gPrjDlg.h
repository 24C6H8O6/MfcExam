
// gPrjDlg.h: 헤더 파일

#include "DlgImage.h"

#pragma once


// CgPrjDlg 대화 상자
class CgPrjDlg : public CDialogEx
{
public:
	CgPrjDlg(CWnd* pParent = nullptr);
	CDlgImage *m_pDlgImage;
// 대화 상자 데이터
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GPRJ_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	int m_edit_radius=0;
	// 메인 다이얼로그의 크기 설정
	int n_Parent_Width = 960;
	int n_Parent_Height = 640;
	int inBorder = 1;                                                                         // 내부 원 테두리 두께
	int n_Cross_Size = 5;                                                                     // 십자표시 크기 설정
	afx_msg void OnBnClickedBtnMakeCircle();                                                  // 반지름에 맞는 원을 그리는 버튼
	void drawCross(CDC &pDC, int x, int y, int nRadius);                                      // 십자 표시 그리는 함수
	void drawInCircle(unsigned char* fm, int i, int j, int nRadius, int nGray);               // 안쪽 원 그리는 함수
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	void drawOutCircle(CDC &pDC, int x, int y, int nRadius, int outBorder, COLORREF color);   // 바깥쪽 원 그리는 함수
	CEdit m_edit_X;
	CEdit m_edit_Y;

};
