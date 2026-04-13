
// CircleGeneratorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CircleGenerator.h"
#include "CircleGeneratorDlg.h"
#include "afxdialogex.h"
#include <cmath> // pow, sqrt 등을 사용하기 위함

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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


// CCircleGeneratorDlg 대화 상자



CCircleGeneratorDlg::CCircleGeneratorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCLEGENERATOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// --- 여기서부터 추가: 변수들을 깨끗한 상태로 만듭니다 ---
	m_nClickCount = 0;      // 클릭 횟수 초기화
	m_nDragIndex = -1;      // 아무것도 드래그하지 않는 상태 (-1)
	m_nRadius = 10;         // 기본 점 반지름 (원하는 값)
	m_nThickness = 2;       // 기본 정원 두께 (원하는 값)
	// ----------------------------------------------
}

void CCircleGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	// UI 컴포넌트 ID와 멤버 변수를 연결합니다.
	DDX_Text(pDX, EDIT_RADIUS, m_nRadius);       // 반지름 입력창 연결
	DDX_Text(pDX, EDIT_THICKNESS, m_nThickness); // 두께 입력창 연결
}

BEGIN_MESSAGE_MAP(CCircleGeneratorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(BTN_RESET, &CCircleGeneratorDlg::OnBnClickedReset)
	ON_BN_CLICKED(BTN_RANDOM, &CCircleGeneratorDlg::OnBnClickedRandom)
END_MESSAGE_MAP()


// CCircleGeneratorDlg 메시지 처리기

BOOL CCircleGeneratorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	srand((unsigned int)time(NULL));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCircleGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCircleGeneratorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 그리기를 위한 도구(DC)를 가져옵니다.

		// 1. 배경을 흰색으로 깨끗하게 지웁니다.
		CRect rect;
		GetClientRect(&rect);
		dc.FillSolidRect(rect, RGB(255, 255, 255));

		// 2. 클릭한 점(작은 원)들을 그립니다.
		for (int i = 0; i < m_nClickCount; i++) {
			int cx = m_ptClick[i].x;
			int cy = m_ptClick[i].y;
			int r = m_nRadius;

			// 원을 감싸는 사각형 영역만 검사해서 효율을 높입니다.
			for (int y = cy - r; y <= cy + r; y++) {
				for (int x = cx - r; x <= cx + r; x++) {
					// 피타고라스 정리: (x-cx)^2 + (y-cy)^2 <= r^2 이면 원 내부
					if (pow(x - cx, 2) + pow(y - cy, 2) <= pow(r, 2)) {
						dc.SetPixel(x, y, RGB(0, 0, 0)); // 검은색 점 찍기
					}
				}
			}

			// --- [추가] 점 옆에 좌표 표시 ---
			CString strPos;
			strPos.Format(_T("P%d: (%d, %d)"), i + 1, cx, cy);
			dc.SetTextColor(RGB(100, 100, 100)); // 가독성을 위해 회색으로 표시
			dc.TextOutW(cx + r + 5, cy - r, strPos); // 점의 오른쪽 상단에 좌표 출력
			// ----------------------------
		}

		// 2. 점이 3개라면, 세 점을 지나는 '정원'을 그릴 차례입니다.
		if (m_nClickCount == 3) {
			double x1 = m_ptClick[0].x, y1 = m_ptClick[0].y;
			double x2 = m_ptClick[1].x, y2 = m_ptClick[1].y;
			double x3 = m_ptClick[2].x, y3 = m_ptClick[2].y;

			// 외접원 공식 적용 (분모 D 계산)
			double D = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));

			if (abs(D) > 0.000001) { // 세 점이 일직선이 아닐 때만 계산
				double xc = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) + (x3 * x3 + y3 * y3) * (y1 - y2)) / D;
				double yc = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) + (x3 * x3 + y3 * y3) * (x2 - x1)) / D;
				double R = sqrt(pow(x1 - xc, 2) + pow(y1 - yc, 2));

				// 화면 전체를 돌며 원의 테두리에 해당하는 픽셀만 칠함
				for (int y = rect.top; y < rect.bottom; y++) {
					for (int x = rect.left; x < rect.right; x++) {
						double dist = sqrt(pow(x - xc, 2) + pow(y - yc, 2));
						// 거리가 반지름(R)과 거의 같으면 (두께 고려) 칠함
						if (abs(dist - R) < (double)m_nThickness / 2.0) {
							dc.SetPixel(x, y, RGB(0, 0, 0)); // 외접원은 빨간색으로 구분
						}
					}
				}

				// --- [추가] 외접원의 중심점(파란색) 그리기 ---
				int nCenterSize = 3; // 중심점 크기
				for (int i = -nCenterSize; i <= nCenterSize; i++) {
					for (int j = -nCenterSize; j <= nCenterSize; j++) {
						dc.SetPixel((int)xc + i, (int)yc + j, RGB(0, 0, 255));
					}
				}

				// --- [추가] 중심점 좌표 텍스트 표시 ---
				CString strCenter;
				strCenter.Format(_T("Center: (%.1f, %.1f)"), xc, yc);
				dc.SetTextColor(RGB(0, 0, 255)); // 파란색 글씨
				dc.TextOutW((int)xc + 10, (int)yc - 10, strCenter); // 중심점 살짝 옆에 표시
				// ------------------------------------------

			}
		}
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCircleGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCircleGeneratorDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	UpdateData(TRUE);

	// 1. 이미 찍힌 점들 중 마우스 클릭 위치와 가까운 점이 있는지 확인
	m_nDragIndex = -1;
	for (int i = 0; i < m_nClickCount; i++) {
		double dist = sqrt(pow(m_ptClick[i].x - point.x, 2) + pow(m_ptClick[i].y - point.y, 2));
		if (dist <= m_nRadius) { // 점의 반지름 안에 클릭했다면
			m_nDragIndex = i; // 드래그 시작!
			return;
		}
	}

	// 2. 드래그가 아니고, 아직 3점을 다 안 찍었다면 새로 추가
	if (m_nClickCount < 3) {
		m_ptClick[m_nClickCount] = point;
		m_nClickCount++;
		Invalidate();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

void CCircleGeneratorDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// 드래그 중인 점이 있다면 좌표 업데이트
	if (m_nDragIndex != -1) {
		m_ptClick[m_nDragIndex] = point;
		Invalidate(); // 실시간으로 다시 그리기
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CCircleGeneratorDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_nDragIndex = -1; // 드래그 종료
	CDialogEx::OnLButtonUp(nFlags, point);
}

void CCircleGeneratorDlg::OnBnClickedReset()
{
	// 1. 데이터 초기화
	m_nClickCount = 0;
	m_nDragIndex = -1;

	// 2. 화면을 새로 그리라고 명령 (그려진 점과 원이 사라집니다)
	Invalidate();
}

void CCircleGeneratorDlg::OnBnClickedRandom()
{
	CRect rect;
	GetClientRect(&rect); // 그릴 수 있는 화면 크기 가져오기

	for (int i = 0; i < 3; i++) {
		// 여백을 고려하여 화면 크기 내에서 랜덤 좌표 생성
		m_ptClick[i].x = rand() % (rect.Width() - 40) + 20;
		m_ptClick[i].y = rand() % (rect.Height() - 100) + 20;
	}

	m_nClickCount = 3; // 점 3개가 생겼음을 선언
	UpdateData(TRUE);  // 현재 설정된 반지름/두께 값 반영
	Invalidate();      // 원 그리기!
}
