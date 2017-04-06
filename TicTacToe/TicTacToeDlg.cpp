
// TicTacToeDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TicTacToe.h"
#include "TicTacToeDlg.h"
#include "afxdialogex.h"
#include "FileDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTicTacToeDlg 대화 상자

CTicTacToeDlg::CTicTacToeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTicTacToeDlg::IDD, pParent)
	, m_startCom(-1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTicTacToeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UNDO_A, m_undoA);
	DDX_Control(pDX, IDC_BUTTON_UNDO_B, m_undoB);
	DDX_Control(pDX, IDC_COMBO_A, m_comboA);
	DDX_Control(pDX, IDC_COMBO_B, m_comboB);
	DDX_Control(pDX, IDC_EDIT_A, m_listA);
	DDX_Control(pDX, IDC_EDIT_B, m_listB);
	DDX_Radio(pDX, IDC_RADIO_A, m_startCom);
}

BEGIN_MESSAGE_MAP(CTicTacToeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CTicTacToeDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CTicTacToeDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CTicTacToeDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CTicTacToeDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_UNDO_A, &CTicTacToeDlg::OnBnClickedButtonUndoA)
	ON_BN_CLICKED(IDC_BUTTON_UNDO_B, &CTicTacToeDlg::OnBnClickedButtonUndoB)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTicTacToeDlg 메시지 처리기

BOOL CTicTacToeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	GetDlgItem(IDC_BUTTON_UNDO_A)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_UNDO_B)->EnableWindow(false);
	m_BKColor = CreateSolidBrush(RGB(255, 255, 255));

	m_isLoad = 0;
	m_checkUndo = 0;

	SetGame();
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTicTacToeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

BOOL CTicTacToeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.	 
	if (m_hAccelTable != NULL)
    {
		if (TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg))
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTicTacToeDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTicTacToeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CTicTacToeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
	if(pWnd->GetDlgCtrlID() == IDC_EDIT_A)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return m_BKColor;
	}

	if(pWnd->GetDlgCtrlID() == IDC_EDIT_B)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return m_BKColor;
	}
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}

void CTicTacToeDlg::OnBnClickedButtonExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int conclusion;

	if(m_board.state == GameBoard::STATE_PLAY)
	{
		conclusion = MessageBox(L"현재 게임중입니다.\n게임을 중단 하시겠습니까?", L"게임 중단", MB_OKCANCEL);
		if(conclusion == IDOK)
			m_board.state = GameBoard::STATE_STOP;
	}
	else{
		conclusion = MessageBox(L"게임을 종료 하시겠습니까?", L"게임 종료", MB_OKCANCEL);
		if(conclusion == IDOK)
			exit(0);
	}
}

void CTicTacToeDlg::OnBnClickedButtonUndoA()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_checkUndo = 1;
}

void CTicTacToeDlg::OnBnClickedButtonUndoB()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_checkUndo = 1;
}

void CTicTacToeDlg::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StartGame();
}

void CTicTacToeDlg::OnBnClickedButtonInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ResetGame();
}

void CTicTacToeDlg::OnBnClickedButtonLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	LoadGame();
}

int CTicTacToeDlg::CheckReady()
{
	UpdateData(TRUE);

	int level_a = m_comboA.GetCurSel();
	int level_b = m_comboB.GetCurSel();

	if(level_a == -1 || level_b == -1)
		return -1;
	else if(m_startCom == -1)
		return 0;
	else
	{
		switch(level_a)
		{
		case 0 : m_levelA = 1; break;			
		case 1 : m_levelA = 3; break;			
		case 2 : m_levelA = 5; break;		
		case 3 : m_levelA = 7; break;			
		}

		switch(level_b)
		{
		case 0 : m_levelB = 2; break;			
		case 1 : m_levelB = 4; break;			
		case 2 : m_levelB = 7; break;		
		case 3 : m_levelB = 8; break;			
		}

		return 1;
	}	
}

void CTicTacToeDlg::SetGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_comboA.AddString(L"Level 1");
	m_comboA.AddString(L"Level 3");
	m_comboA.AddString(L"Level 5");
	m_comboA.AddString(L"Level 7");

	m_comboB.AddString(L"Level 2");
	m_comboB.AddString(L"Level 4");
	m_comboB.AddString(L"Level 6");
	m_comboB.AddString(L"Level 8");	

	GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<게임 트리>");
	GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"<게임 트리>");

	m_levelA = 0;
	m_levelB = 0;
}

/**
	함 수 : StartGame()
	기 능 : 게임 시작 버튼을 눌렀을때 실행하는 함수로써, 기본적으로
			게임이 시작되기전 옵션 설정이 준비되었는지 체크하고, 추가적으로 불러온 
			게임판인지 여부를 검사해서 게임판을 초기화 시켜줌. 
			설정이 완료되면 게임이 끝날때까지 게임판을 진행 함
*/
void CTicTacToeDlg::StartGame()
{
	int checkErr;				/* 에러 종류를 알려주는 변수 */

	checkErr = CheckReady();	/* 게임을 시작할 준비 되었는지 검사 */
	if(checkErr == 1)			/* 리턴값이 1이면 준비 완료 */
	{
		if(m_isLoad!=0)			/* 불러온 게임이라면, */
		{						/* 불러온 게임 정보로 보드판 초기화 */
			m_board.InitBoard(m_startCom, m_isLoad, m_levelA, m_levelB);	
			m_isLoad = 0;
		}
		else
			m_board.InitBoard(m_startCom, 0, m_levelA, m_levelB);	/* 아니라면, 새로운 판으로 초기화 */

		UpdateGame();
		m_board.state = GameBoard::STATE_PLAY;						/* 보드판 상태를 플레이 중으로 변경 */

 		while(m_board.state == GameBoard::STATE_PLAY)		/* 게임 중이라면 */
		{
			TicTacToeAI* tttAI = new TicTacToeAI(m_board);	/* 새로운 AI 객체를 생성 */

			tttAI->GetBestMove();							/* 최적의 좌표를 구함 */
			m_board.DoMove(tttAI->bestX, tttAI->bestY);		/* 해당 좌표에 수를 둠 */
			
			Node* node = tttAI->GetRootNode();			/* 최적의 좌표를 구하는동안 저장한 트리 중 루트노드 반환 */
			this->PrintTreeNode(node);					/* 트리 출력 */
			
			UpdateGame();							/* 게임판 업데이트 */
		
			while(WaitUndo())							/* 무르기 기다림 */
			{
				m_board.RandomMove();	/* 남아있는 좌표중 랜덤한 곳으로 수를 둠 */
				UpdateGame();						/* 게임판 업데이트 */
			}

			delete tttAI;
			delete node;

			m_board.CheckState();			/* 게임판 상태를 점검 */
			if(m_board.state != GameBoard::STATE_PLAY)
				EndGame();					/* 플레이 중이 아닌 상태면 게임 종료 */
		}
		UpdateGame();	/* 상대방 보드판에도 출력 */
	}
	else if(checkErr == -1)	/* 레벨 설정이 안되어있을때 오류 출력 */
	{
		MessageBox(L"ERROR : LA-Level 설정을 확인하세요!", L"Error!", MB_ICONERROR);
	}
	else					/* 시작 순서 설정이 안되어있을때 오류 출력 */
		MessageBox(L"ERROR : 시작순서 설정을 확인하세요!", L"Error!", MB_ICONERROR);
}

/**
	함 수 : PrintTreeNode(Node* root)
	기 능 : AI를 통해서 최적의 좌표를 구하는동안의 Eval 값을 저장한 노드들로 구성된
		   트리를 자료구조 큐를 이용해 너비우선 탐색으로 Edit Box에 출력해주는 함수
*/
void CTicTacToeDlg::PrintTreeNode(Node* root)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int preDepth = 0 ;			/* 이전 노드의 깊이 */
	bool preParent = false;		/* 부모가 같은지 여부 점검 */
	Node *pNode = root;			/* 루트노드를 복사 */
	CString temp, temp2;	
	queue <Node* > que;			/* 큐 생성 */

	que.push(pNode);			/* 큐에 루트노드를 넣고 */
	while(!que.empty())			/* 큐가 비어있을 때 까지 출력 */
	{		
		pNode = que.front();	/* 앞에서 꺼내서 */
		que.pop();
		if(pNode != NULL)		/* NULL 값이 아니고 */
		{
			if( preDepth != pNode->depth )	/* 이전 노드값과 깊이가 다르면 */
				temp = temp + (L"\r\n");	/* 개행 */

			if(preParent)					/* 부모노드가 같으면 */
				temp = temp + (L", ");		/* 이어서 출력 */
			else
				temp = temp + (L"(");		/* 다르면 '(' 로 구분 */
			
			temp2.Format(L"%d", pNode->eval);
			temp = temp + temp2;			

			preParent = true;				

			if(pNode->childCnt != 0)		/* 자식노드 개수가 0이 아니면 */
			{
				que.push(NULL);				/* NULL 값으로 부모노드 구분 해주고 */
				for(int j=0; j < pNode->childCnt; j++)
					que.push(pNode->next[j]);		/* 자식노드를 큐에 넣음 */
				
			}
			preDepth = pNode->depth;				/* 노드 깊이를 변경 */
		}
		else							/* 큐에서 꺼낸게 NULL 값 이라면 */
		{
			temp = temp + (L")");		/* ')' 로 부모노드를 구분 */
			preParent = false;
		}
	}
	temp = temp + (L")");

	if(m_board.moveCnt % 2 == 1)
	{
		if(m_board.starterCom == 'X')
		{
			GetDlgItem(IDC_EDIT_A)->SetWindowTextW(temp);
			GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"<게임 트리>");
		}	
		else
		{
			GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<게임 트리>");
			GetDlgItem(IDC_EDIT_B)->SetWindowTextW(temp);
		}
	}
	else
	{
		if(m_board.starterCom == 'X')
		{
			GetDlgItem(IDC_EDIT_B)->SetWindowTextW(temp);
			GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<게임 트리>");
		}	
		else
		{
			GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"<게임 트리>");
			GetDlgItem(IDC_EDIT_A)->SetWindowTextW(temp);
		}
	}
}

/**
	함 수 : ResetGame()
	기 능 : 게임판을 재 초기화 시키는 함수
*/
void CTicTacToeDlg::ResetGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tempStr, str;
	int count = 0;

	m_startCom = -1;
	UpdateData(FALSE);

	m_board.state = GameBoard::STATE_INIT;
	m_board.InitBoard(m_startCom, 0, m_levelA, m_levelB);	

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			str.Format(L"%d", count+1);
			SetDlgItemText(IDC_A1+count, str);
			SetDlgItemText(IDC_B1+count, str);
			count++;
		}
	}

	GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<게임 트리>");
	GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"<게임 트리>");
	m_undoA.EnableWindow(FALSE);
	m_undoB.EnableWindow(FALSE);
	m_comboA.SetCurSel(-1);
	m_comboB.SetCurSel(-1);
}

/**
	함 수 : EndGame()
	기 능 : 게임을 종료시키고 해당 게임판 상태에따라 Edit Box에 결과값을
		    출력해주는 함수 
*/
void CTicTacToeDlg::EndGame()
{
	switch(m_board.state)
	{
	case GameBoard::STATE_WINA :
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"게임에 이겼습니다.");
		GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"게임에 졌습니다.");
		break;

	case GameBoard::STATE_WINB :
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"게임에 졌습니다.");
		GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"게임에 이겼습니다.");
		break;
		
	case GameBoard::STATE_DRAW :
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"게임에 비겼습니다.");
		GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"게임에 비겼습니다.");
		break;

	case GameBoard::STATE_INIT :
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<게임 트리>");
		GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"<게임 트리>");
		break;
	
	case GameBoard::STATE_STOP :
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"게임이 중단됬습니다.");
		GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"게임이 중단됬습니다.");
		break;		
	}
	m_undoA.EnableWindow(FALSE);
	m_undoB.EnableWindow(FALSE);
}

/**
	함 수 : WaitUndo()
	기 능 : 컴퓨터가 수를 해당 좌표에 놓고 무르기를 하기까지 5초동안 기다리는
			함수. 무르기를 시전하면 1을 리턴하고 그렇지 않으면 0을 리턴
*/
int CTicTacToeDlg::WaitUndo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();

	if(m_board.moveCnt % 2 == 1)
	{
		if(m_board.starterCom == 'X')
		{
			m_undoA.EnableWindow(TRUE);
			m_undoB.EnableWindow(FALSE);
		}
		else
		{
			m_undoA.EnableWindow(FALSE);
			m_undoB.EnableWindow(TRUE);
		}
	}
	else
	{
		if(m_board.starterCom == 'X')
		{
			m_undoA.EnableWindow(FALSE);
			m_undoB.EnableWindow(TRUE);
		}
		else
		{
			m_undoA.EnableWindow(TRUE);
			m_undoB.EnableWindow(FALSE);
		}
	}

	while(GetTickCount() - dwStart < 1000)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			PreTranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(m_checkUndo == 1)		/* 무르기를 시전했다면 */
		{
			m_checkUndo = 0;		/* 체크값을 0으로 바꾼디 1을 반환 */
			return 1;
		}
	}
	return 0;						/* 아니라면, 0을 반환 */
}

/**
	함 수 : UpdateGame()
	기 능 : 해당 게임판을 화면으로 업데이트 해주는 함수
*/
void CTicTacToeDlg::UpdateGame()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int count = 0;
	int comButton = 0;
	CString str;
	
	if(m_board.moveCnt % 2 == 1)
	{
		if(m_board.starterCom == 'X')
			comButton = IDC_A1;
		else
			comButton = IDC_B1;
	}
	else
	{
		if(m_board.starterCom == 'X')
			comButton = IDC_B1;
		else
			comButton = IDC_A1;
	}

	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			if(m_board.board[i][j] == 'X')
				SetDlgItemText(comButton+count, L"X");
			else if(m_board.board[i][j] == 'O')
				SetDlgItemText(comButton+count, L"O");
			else
			{
				str.Format(L"%d", count+1);
				SetDlgItemText(comButton+count, str);
			}
			count++;
		}
	}

	count = 0;
	if(m_board.state != GameBoard::STATE_PLAY)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				if(m_board.board[i][j] == 'X')
				{
					SetDlgItemText(IDC_A1+count, L"X");
					SetDlgItemText(IDC_B1+count, L"X");
				}
				else if(m_board.board[i][j] == 'O')
				{
					SetDlgItemText(IDC_A1+count, L"O");
					SetDlgItemText(IDC_B1+count, L"O");
				}
				else
				{
					str.Format(L"%d", count+1);
					SetDlgItemText(IDC_A1+count, str);
					SetDlgItemText(IDC_B1+count, str);
				}
				count++;
			}
		}
	}
}

/**
	함 수 : LoadGame()
	기 능 : 게임을 불러왔을때 해당 게임판 정보를 화면에 업데이트 해주는 함수
*/
void CTicTacToeDlg::LoadGame()
{
	CFileDlg dlg;

	if(m_board.state == GameBoard::STATE_PLAY)	/* 불러오는 시점이 게임중인지를 검사 */
	{
		MessageBox(L"지금은 게임중입니다! 종료하고 시도하세요.", L"경 고", MB_ICONEXCLAMATION);
		return;
	}

	if(dlg.DoModal()==IDOK)
	{		
		FILE *fp;						/* 파일 포인터 선언 */
		CStringA name(dlg.m_fileStr);

		if(!(fp = fopen(name, "r")))
		{
			MessageBox(L"파일을 열 수 없습니다! 파일명을 확인하세요.", L"ERROR", MB_ICONERROR);
			return;
		}
		else		/* 제대로 열린 파일이라면 */
		{
			int i, j, stoneCount=0;
			int Acnt = 0 , Bcnt = 0;
			char temp[5];

			for(i=0; i<4; i++)
			{
				fscanf_s(fp, "%s", temp, 5);	/* 해당파일에서 한줄을 읽은뒤 */
				for(j=0; j<4; j++)				/* 문자에 맞게 게임판에 입력 */
				{				
					if(temp[j] == 'X')			
					{
						m_board.board[i][j] = 'X';
						Acnt++;
					}						
					else if(temp[j] == 'O')
					{
						m_board.board[i][j] = 'O';
						Bcnt++;
					}
					else
						m_board.board[i][j] = ' ';
				}
			}

			if(Acnt < Bcnt)			/* 'X'와 'O' 문자 개수를 비교 */
				m_startCom = 1;		/* A가 작으면 B가 시작 컴퓨터 */
			else					/* 동일하면, A가 시작 컴퓨터 */
				m_startCom = 0;

			UpdateData(FALSE);
			m_isLoad = Acnt + Bcnt;
			UpdateGame();
			GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<게임 트리>");
			GetDlgItem(IDC_EDIT_B)->SetWindowTextW(L"<게임 트리>");
			fclose(fp);
		}
	}
}