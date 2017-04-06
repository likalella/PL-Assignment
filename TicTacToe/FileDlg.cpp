// FileDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TicTacToe.h"
#include "FileDlg.h"
#include "afxdialogex.h"


// CFileDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFileDlg, CDialog)

CFileDlg::CFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileDlg::IDD, pParent)
{
	m_fileStr = _T("");
}

CFileDlg::~CFileDlg()
{

}

void CFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FILE, m_fileStr);
}


BEGIN_MESSAGE_MAP(CFileDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CFileDlg 메시지 처리기입니다.


void CFileDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CDialog::OnOK();
}
