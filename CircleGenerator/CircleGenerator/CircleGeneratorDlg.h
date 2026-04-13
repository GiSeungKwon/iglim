
// CircleGeneratorDlg.h: 헤더 파일
//

#pragma once


// CCircleGeneratorDlg 대화 상자
class CCircleGeneratorDlg : public CDialogEx
{
// 생성입니다.
public:
	CCircleGeneratorDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCLEGENERATOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	// --- 여기서부터 추가하세요 ---
	CPoint m_ptClick[3];    // 클릭한 세 점의 위치를 저장할 메모리
	int m_nClickCount;      // 현재까지 클릭한 횟수 (0, 1, 2, 3...)
	int m_nDragIndex;       // 어떤 점을 마우스로 끌고 있는지 확인용 (-1은 안 끌고 있음)
	int m_nRadius;          // 입력받은 점의 반지름
	int m_nThickness;       // 입력받은 정원의 테두리 두께
	// --------------------------
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedRandom();
};
