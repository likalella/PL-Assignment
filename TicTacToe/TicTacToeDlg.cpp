
// TicTacToeDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "TicTacToe.h"
#include "TicTacToeDlg.h"
#include "afxdialogex.h"
#include "FileDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

														// �����Դϴ�.
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


// CTicTacToeDlg ��ȭ ����

CTicTacToeDlg::CTicTacToeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTicTacToeDlg::IDD, pParent)
	, m_startCom(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTicTacToeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_UNDO_B, m_undoB);
	DDX_Control(pDX, IDC_COMBO_A, m_comboA);
	DDX_Control(pDX, IDC_EDIT_A, m_listA);
}

BEGIN_MESSAGE_MAP(CTicTacToeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CTicTacToeDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CTicTacToeDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CTicTacToeDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_UNDO_B, &CTicTacToeDlg::OnBnClickedButtonUndoB)
	ON_WM_CTLCOLOR()
	ON_COMMAND_RANGE(IDC_B1, IDC_B16, &CTicTacToeDlg::OnBtnClick)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CTicTacToeDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CTicTacToeDlg �޽��� ó����

BOOL CTicTacToeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

									// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_hAccelTable = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	m_BKColor = CreateSolidBrush(RGB(255, 255, 255));

	m_isLoad = 0;

	SetGame();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

BOOL CTicTacToeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.	 
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
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CTicTacToeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HBRUSH CTicTacToeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_A)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 0));
		return m_BKColor;
	}

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}

void CTicTacToeDlg::OnBnClickedButtonExit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int conclusion;

	if (m_board.state == GameBoard::STATE_PLAY)
	{
		conclusion = MessageBox(L"���� �������Դϴ�.\n������ �ߴ� �Ͻðڽ��ϱ�?", L"���� �ߴ�", MB_OKCANCEL);
		if (conclusion == IDOK)
			m_board.state = GameBoard::STATE_STOP;
	}
	else {
		conclusion = MessageBox(L"������ ���� �Ͻðڽ��ϱ�?", L"���� ����", MB_OKCANCEL);
		if (conclusion == IDOK)
			exit(0);
	}
}


void CTicTacToeDlg::OnBnClickedButtonUndoB()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//m_checkUndo = 1;
	if (m_board.moveCnt > 1) {
		for (int i = 1; i <= 2; i++) {
			int x = m_board.preMoves[m_board.moveCnt - i].x;
			int y = m_board.preMoves[m_board.moveCnt - i].y;
			int n = (x * 4) + y;
			if (!GetDlgItem(IDC_A1 + n)->IsWindowEnabled()) {
				GetDlgItem(IDC_A1 + n)->EnableWindow(TRUE);
				SetDlgItemInt(IDC_A1 + n, n + 1);
			}
			if (!GetDlgItem(IDC_B1 + n)->IsWindowEnabled()) {
				GetDlgItem(IDC_B1 + n)->EnableWindow(TRUE);
				SetDlgItemInt(IDC_B1 + n, n + 1);
			}
		}
		m_board.UndoMove();
		m_board.UndoMove();
	}
}

void CTicTacToeDlg::OnBnClickedButtonInit()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	for (int i = 0; i < 16; i++) {
		if (!GetDlgItem(IDC_A1 + i)->IsWindowEnabled()) {
			GetDlgItem(IDC_A1 + i)->EnableWindow(TRUE);
			SetDlgItemInt(IDC_A1 + i, i + 1);
		}
		if (!GetDlgItem(IDC_B1 + i)->IsWindowEnabled()) {
			GetDlgItem(IDC_B1 + i)->EnableWindow(TRUE);
			SetDlgItemInt(IDC_B1 + i, i + 1);
		}
	}
	if (!m_undoB.IsWindowEnabled())
		m_undoB.EnableWindow(TRUE);
	ResetGame();
}

void CTicTacToeDlg::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SaveGame();
}

void CTicTacToeDlg::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	LoadGame();
}

int CTicTacToeDlg::CheckReady()
{
	UpdateData(TRUE);

	int level_a = m_comboA.GetCurSel();

	if (level_a == -1)
		return -1;

	else
	{
		switch (level_a)
		{
		case 0: m_levelA = 1; break;
		case 1: m_levelA = 3; break;
		case 2: m_levelA = 5; break;
		}

		return 1;
	}
}

void CTicTacToeDlg::SetGame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_comboA.AddString(L"Level 1");
	m_comboA.AddString(L"Level 3");
	m_comboA.AddString(L"Level 5");

	GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<���� Ʈ��>");

	m_levelA = 3;
	m_startCom = 0;
	m_comboA.SetCurSel(1);
}

/**
�� �� : StartGame()
�� �� : ���� ���� ��ư�� �������� �����ϴ� �Լ��ν�, �⺻������
������ ���۵Ǳ��� �ɼ� ������ �غ�Ǿ����� üũ�ϰ�, �߰������� �ҷ���
���������� ���θ� �˻��ؼ� �������� �ʱ�ȭ ������.
������ �Ϸ�Ǹ� ������ ���������� �������� ���� ��
*/
// lika : ���� ���� ������ �и�.
void CTicTacToeDlg::StartGame()
{
	int checkErr;				/* ���� ������ �˷��ִ� ���� */

	checkErr = CheckReady();	/* ������ ������ �غ� �Ǿ����� �˻� */
	if (checkErr == 1)			/* ���ϰ��� 1�̸� �غ� �Ϸ� */
	{
		if (m_isLoad != 0)			/* �ҷ��� �����̶��, */
		{						/* �ҷ��� ���� ������ ������ �ʱ�ȭ */
								//m_board.InitBoard(m_startCom, m_isLoad, m_levelA);	
			m_isLoad = 0;
		}
		else
			m_board.InitBoard(m_startCom, 0, m_levelA);	/* �ƴ϶��, ���ο� ������ �ʱ�ȭ */

		UpdateGame();
		m_board.state = GameBoard::STATE_PLAY;						/* ������ ���¸� �÷��� ������ ���� */

	}
	else if (checkErr == -1)	/* ���� ������ �ȵǾ������� ���� ��� */
	{
		MessageBox(L"ERROR : LA-Level ������ Ȯ���ϼ���!", L"Error!", MB_ICONERROR);
	}
}

/**
�� �� : PrintTreeNode(Node* root)
�� �� : AI�� ���ؼ� ������ ��ǥ�� ���ϴµ����� Eval ���� ������ ����� ������
Ʈ���� �ڷᱸ�� ť�� �̿��� �ʺ�켱 Ž������ Edit Box�� ������ִ� �Լ�
*/
void CTicTacToeDlg::PrintTreeNode(Node* root)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int preDepth = 0;			/* ���� ����� ���� */
	bool preParent = false;		/* �θ� ������ ���� ���� */
	Node *pNode = root;			/* ��Ʈ��带 ���� */
	CString temp, temp2;
	queue <Node* > que;			/* ť ���� */

	que.push(pNode);			/* ť�� ��Ʈ��带 �ְ� */
	while (!que.empty())			/* ť�� ������� �� ���� ��� */
	{
		pNode = que.front();	/* �տ��� ������ */
		que.pop();
		if (pNode != NULL)		/* NULL ���� �ƴϰ� */
		{
			if (preDepth != pNode->depth)	/* ���� ��尪�� ���̰� �ٸ��� */
				temp = temp + (L"\r\n");	/* ���� */

			if (preParent)					/* �θ��尡 ������ */
				temp = temp + (L", ");		/* �̾ ��� */
			else
				temp = temp + (L"(");		/* �ٸ��� '(' �� ���� */

			temp2.Format(L"%d", pNode->eval);
			temp = temp + temp2;

			preParent = true;

			if (pNode->childCnt != 0)		/* �ڽĳ�� ������ 0�� �ƴϸ� */
			{
				que.push(NULL);				/* NULL ������ �θ��� ���� ���ְ� */
				for (int j = 0; j < pNode->childCnt; j++)
					que.push(pNode->next[j]);		/* �ڽĳ�带 ť�� ���� */

			}
			preDepth = pNode->depth;				/* ��� ���̸� ���� */
		}
		else							/* ť���� ������ NULL �� �̶�� */
		{
			temp = temp + (L")");		/* ')' �� �θ��带 ���� */
			preParent = false;
		}
	}
	temp = temp + (L")");

	GetDlgItem(IDC_EDIT_A)->SetWindowTextW(temp);
}

/**
�� �� : ResetGame()
�� �� : �������� �� �ʱ�ȭ ��Ű�� �Լ�
*/
void CTicTacToeDlg::ResetGame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString tempStr, str;
	int count = 0;

	if (m_board.state == GameBoard::STATE_PLAY)	/* �ҷ����� ������ ������������ �˻� */
	{
		EndGame();
	}

	m_startCom = 0;
	UpdateData(FALSE);

	m_board.state = GameBoard::STATE_INIT;
	m_board.InitBoard(m_startCom, 0, m_levelA);

	for (int i = 0; i<3; i++)
	{
		for (int j = 0; j<3; j++)
		{
			str.Format(L"%d", count + 1);
			SetDlgItemText(2001 + count, str);
			SetDlgItemText(2101 + count, str);
			count++;
		}
	}

	GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<���� Ʈ��>");
	m_comboA.SetCurSel(1);
	m_levelA = 3;
}

/**
�� �� : EndGame()
�� �� : ������ �����Ű�� �ش� ������ ���¿����� Edit Box�� �������
������ִ� �Լ�
*/
void CTicTacToeDlg::EndGame()
{
	switch (m_board.state)
	{
	case GameBoard::STATE_WINA:
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"��ǻ�Ͱ� ���ӿ� �̰���ϴ�.");
		break;

	case GameBoard::STATE_WINB:
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"��ǻ�Ͱ� ���ӿ� �����ϴ�.");
		break;

	case GameBoard::STATE_DRAW:
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"���ӿ� �����ϴ�.");
		break;

	case GameBoard::STATE_INIT:
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<���� Ʈ��>");
		break;

	case GameBoard::STATE_STOP:
		GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"������ �ߴ܉���ϴ�.");
		break;
	}
	m_undoB.EnableWindow(FALSE);
}

/**
�� �� : UpdateGame()
�� �� : �ش� �������� ȭ������ ������Ʈ ���ִ� �Լ�
*/
// lika : 3->4
void CTicTacToeDlg::UpdateGame()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int count = 0;
	int comButton = 0;
	CString str;

	if (m_board.moveCnt % 2 == 1)
	{
		if (m_board.starterCom == 'X')
			comButton = IDC_A1;
		else
			comButton = IDC_B1;
	}
	else
	{
		if (m_board.starterCom == 'X')
			comButton = IDC_B1;
		else
			comButton = IDC_A1;
	}

	for (int i = 0; i<4; i++)
	{
		for (int j = 0; j<4; j++)
		{
			if (m_board.board[i][j] == 'X')
				SetDlgItemText(comButton + count, L"X");
			else if (m_board.board[i][j] == 'O')
				SetDlgItemText(comButton + count, L"O");
			else
			{
				str.Format(L"%d", count + 1);
				SetDlgItemText(comButton + count, str);
			}
			count++;
		}
	}

	count = 0;
	if (m_board.state != GameBoard::STATE_PLAY)
	{
		for (int i = 0; i<4; i++)
		{
			for (int j = 0; j<4; j++)
			{
				if (m_board.board[i][j] == 'X')
				{
					SetDlgItemText(IDC_A1 + count, L"X");
					SetDlgItemText(IDC_B1 + count, L"X");
				}
				else if (m_board.board[i][j] == 'O')
				{
					SetDlgItemText(IDC_A1 + count, L"O");
					SetDlgItemText(IDC_B1 + count, L"O");
				}
				else
				{
					str.Format(L"%d", count + 1);
					SetDlgItemText(IDC_A1 + count, str);
					SetDlgItemText(IDC_B1 + count, str);
				}
				count++;
			}
		}
	}
}


/**
�� �� : SaveGame()
�� �� : ���� �������� ������ ������ �����ϴ� �Լ�.
*/
// lika 3->4, 4->5
void CTicTacToeDlg::SaveGame() {
	CFileDlg dlg;
	int checkErr;				/* ���� ������ �˷��ִ� ���� */
	if (dlg.DoModal() == IDOK)
	{
		FILE *fp;						/* ���� ������ ���� */
		CStringA name(dlg.m_fileStr);

		if (!(fp = fopen(name, "w")))
		{
			MessageBox(L"������ �� �� �����ϴ�! ���ϸ��� Ȯ���ϼ���.", L"ERROR", MB_ICONERROR);
			return;
		}
		else		/* ����� ���� �����̶�� */
		{
			int i, j, stoneCount = 0;
			int Acnt = 0, Bcnt = 0;
			char temp[21];

			for (i = 0; i<4; i++)
			{
				for (j = 0; j < 4; j++)
				{
					if (m_board.board[i][j] == ' ')
						temp[(i * 5) + j] = 'B';
					else
						temp[(i * 5) + j] = m_board.board[i][j];
				}
				if (i < 3)
					temp[((i + 1) * 5) - 1] = '\n';
				else
					temp[((i + 1) * 5) - 1] = '\0';
			}
			fprintf_s(fp, "%s", temp);	/* �ش����Ͽ��� ������ ������ */
			fclose(fp);
		}
	}

}

/**
�� �� : LoadGame()
�� �� : ������ �ҷ������� �ش� ������ ������ ȭ�鿡 ������Ʈ ���ִ� �Լ�
*/
// lika 3->4, 4->5

void CTicTacToeDlg::LoadGame()
{
	CFileDlg dlg;
	int checkErr;				/* ���� ������ �˷��ִ� ���� */

	if (m_board.state == GameBoard::STATE_PLAY)	/* �ҷ����� ������ ������������ �˻� */
	{
		EndGame();
	}

	checkErr = CheckReady();	/* ������ ������ �غ� �Ǿ����� �˻� */
	if (checkErr == -1)	/* ���� ������ �ȵǾ������� ���� ��� */
	{
		MessageBox(L"ERROR : LA-Level ������ Ȯ���ϼ���!", L"Error!", MB_ICONERROR);
		return;
	}

	if (dlg.DoModal() == IDOK)
	{
		FILE *fp;						/* ���� ������ ���� */
		CStringA name(dlg.m_fileStr);

		if (!(fp = fopen(name, "r")))
		{
			MessageBox(L"������ �� �� �����ϴ�! ���ϸ��� Ȯ���ϼ���.", L"ERROR", MB_ICONERROR);
			return;
		}
		else		/* ����� ���� �����̶�� */
		{
			int i, j, stoneCount = 0;
			int Acnt = 0, Bcnt = 0;
			char temp[5];

			for (i = 0; i<4; i++)
			{
				fscanf_s(fp, "%s", temp, 5);	/* �ش����Ͽ��� ������ ������ */
				for (j = 0; j<4; j++)				/* ���ڿ� �°� �����ǿ� �Է� */
				{
					if (temp[j] == 'X')
					{
						m_board.board[i][j] = 'X';
						Acnt++;
						int n = (i * 4) + j;
						if (GetDlgItem(IDC_A1 + n)->IsWindowEnabled()) {
							SetDlgItemText(IDC_A1 + n, L"X");
							GetDlgItem(IDC_A1 + n)->EnableWindow(FALSE);
						}
						if (GetDlgItem(IDC_B1 + n)->IsWindowEnabled()) {
							SetDlgItemText(IDC_B1 + n, L"X");
							GetDlgItem(IDC_B1 + n)->EnableWindow(FALSE);

						}
					}
					else if (temp[j] == 'O')
					{
						m_board.board[i][j] = 'O';
						Bcnt++;
						int n = (i * 4) + j;
						if (GetDlgItem(IDC_A1 + n)->IsWindowEnabled()) {
							GetDlgItem(IDC_A1 + n)->EnableWindow(FALSE);
							SetDlgItemText(IDC_A1 + n, L"O");
						}
						if (GetDlgItem(IDC_B1 + n)->IsWindowEnabled()) {
							GetDlgItem(IDC_B1 + n)->EnableWindow(FALSE);
							SetDlgItemText(IDC_B1 + n, L"O");
						}
					}
					else {
						m_board.board[i][j] = ' ';
						int n = (i * 4) + j;
						if (!GetDlgItem(IDC_A1 + n)->IsWindowEnabled()) {
							GetDlgItem(IDC_A1 + n)->EnableWindow(TRUE);
							SetDlgItemInt(IDC_A1 + n, n + 1);
						}
						if (!GetDlgItem(IDC_B1 + n)->IsWindowEnabled()) {
							GetDlgItem(IDC_B1 + n)->EnableWindow(TRUE);
							SetDlgItemInt(IDC_B1 + n, n + 1);
						}
					}
				}
			}

			UpdateData(FALSE);
			m_isLoad = Acnt + Bcnt;
			m_board.moveCnt = Acnt + Bcnt;
			//UpdateGame();
			GetDlgItem(IDC_EDIT_A)->SetWindowTextW(L"<���� Ʈ��>");
			if (m_board.state != GameBoard::STATE_PLAY) {

				m_board.state = GameBoard::STATE_PLAY;
			}
			if (!m_undoB.IsWindowEnabled())
				m_undoB.EnableWindow(TRUE);
			if (!(Acnt - Bcnt == 0 || Acnt - Bcnt == 1)) {
				// err
			}
			if (Acnt != Bcnt)
				DoCom();
			fclose(fp);
		}
	}
}

/*
�� �� : OnBtnClick(UINT btnID)
�� �� : ����ڰ� ��ư�� ������, �ش��ϴ� ��ư�� ���� �δ� �Լ�.
*/
void CTicTacToeDlg::OnBtnClick(UINT btnID)
{
	if (m_board.state != GameBoard::STATE_PLAY) {
		for (int i = 0; i < 16; i++) {
			if (!GetDlgItem(IDC_A1 + i)->IsWindowEnabled()) {
				GetDlgItem(IDC_A1 + i)->EnableWindow(TRUE);
				SetDlgItemInt(IDC_A1 + i, i + 1);
			}
			if (!GetDlgItem(IDC_B1 + i)->IsWindowEnabled()) {
				GetDlgItem(IDC_B1 + i)->EnableWindow(TRUE);
				SetDlgItemInt(IDC_B1 + i, i + 1);
			}
		}
		if (!m_undoB.IsWindowEnabled())
			m_undoB.EnableWindow(TRUE);
		StartGame();

	}
	if (m_board.state == GameBoard::STATE_PLAY) {
		int x = (btnID - IDC_B1) / 4;
		int y = (btnID - IDC_B1) % 4;
		m_board.DoMove(x, y);
		GetDlgItem(btnID)->EnableWindow(FALSE);
		GetDlgItem(btnID - 100)->EnableWindow(FALSE);
		UpdateGame();
		m_board.CheckState();			// ������ ���¸� ���� 
		if (m_board.state != GameBoard::STATE_PLAY)
			EndGame();					// �÷��� ���� �ƴ� ���¸� ���� ���� 

										//��ǻ�� ����
		DoCom();
	}
}

/*


*/
void CTicTacToeDlg::DoCom() {
	TicTacToeAI* tttAI = new TicTacToeAI(m_board);	// ���ο� AI ��ü�� ���� 
	tttAI->GetBestMove();							// ������ ��ǥ�� ���� 
	m_board.DoMove(tttAI->bestX, tttAI->bestY);		// �ش� ��ǥ�� ���� �� 
	int position = (4 * tttAI->bestX) + tttAI->bestY;
	GetDlgItem(IDC_A1 + position)->EnableWindow(FALSE);
	GetDlgItem(IDC_B1 + position)->EnableWindow(FALSE);

	Node* node = tttAI->GetRootNode();			// ������ ��ǥ�� ���ϴµ��� ������ Ʈ�� �� ��Ʈ��� ��ȯ 
	this->PrintTreeNode(node);					// Ʈ�� ��� 

	UpdateGame();							// ������ ������Ʈ 

	delete tttAI;
	delete node;
	m_board.CheckState();			// ������ ���¸� ���� 
	if (m_board.state != GameBoard::STATE_PLAY)
		EndGame();					// �÷��� ���� �ƴ� ���¸� ���� ���� 
}
