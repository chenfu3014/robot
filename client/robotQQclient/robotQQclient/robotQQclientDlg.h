
// robotQQclientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


#define FLAGE_START_THREAD_PROC   1
#define FLAGE_STOP_THREAD_PROC   0


// CrobotQQclientDlg �Ի���
class CrobotQQclientDlg : public CDialogEx
{
// ����
public:
	CrobotQQclientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOTQQCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
    afx_msg void OnBnClickedBtnSelectFile();


private:
    CString m_filename;
    CString m_fileext;//�ļ���չ��
    CString m_filepathname;
    CString m_fileFullPath;
    CEdit mEditCtlPath;
public:
    afx_msg void OnBnClickedBtnHelp();
    void setSysInfoToStatusBar(CString sysInfo);
private:


    HANDLE  mHandleCheckFile;
    bool mOnOffSound;

public:
    afx_msg void OnBnClickedBtnOnoffSouned();
    CStatic mCtrlSysInfo;
};
