
// robotQQclientDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


#define FLAGE_START_THREAD_PROC   1
#define FLAGE_STOP_THREAD_PROC   0


// CrobotQQclientDlg 对话框
class CrobotQQclientDlg : public CDialogEx
{
// 构造
public:
	CrobotQQclientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROBOTQQCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
    CString m_fileext;//文件扩展名
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
