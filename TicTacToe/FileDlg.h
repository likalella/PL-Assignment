#pragma once


// CFileDlg 대화 상자입니다.

class CFileDlg : public CDialog
{
	DECLARE_DYNAMIC(CFileDlg)

public:
	CFileDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CFileDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_FILEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_fileStr;			/* 파일명 저장할 변수 */
};
