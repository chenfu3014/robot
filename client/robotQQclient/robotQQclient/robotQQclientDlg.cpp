
// robotQQclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "robotQQclient.h"
#include "robotQQclientDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


bool gFlagCheckFileSize = FLAGE_STOP_THREAD_PROC;
bool gOnOffSound = 0;



DWORD WINAPI  ThreadProcCheckFileSize(LPVOID lpParam)
{
    while (1)
    {
        if (FLAGE_STOP_THREAD_PROC == gFlagCheckFileSize)
        {
            Sleep(1);
            continue;
        }
        else
        {
            if (gOnOffSound)
            {   
                MessageBeep(MB_OK);
            }
            
            // 1: read file 
            // 2: send file to server.
            // 3: 

        }
    }
    return 0;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CrobotQQclientDlg 对话框



CrobotQQclientDlg::CrobotQQclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROBOTQQCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CrobotQQclientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_FILE_PATH, mEditCtlPath);
}

BEGIN_MESSAGE_MAP(CrobotQQclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BTN_START, &CrobotQQclientDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(ID_BTN_STOP, &CrobotQQclientDlg::OnBnClickedBtnStop)
    ON_BN_CLICKED(IDC_BTN_SELECT_FILE, &CrobotQQclientDlg::OnBnClickedBtnSelectFile)
    ON_BN_CLICKED(IDC_BTN_HELP, &CrobotQQclientDlg::OnBnClickedBtnHelp)
    ON_BN_CLICKED(IDC_BTN_ONOFF_SOUNED, &CrobotQQclientDlg::OnBnClickedBtnOnoffSouned)
END_MESSAGE_MAP()


// CrobotQQclientDlg 消息处理程序

BOOL CrobotQQclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    gOnOffSound = 1;
    mHandleCheckFile = CreateThread(NULL, 0, ThreadProcCheckFileSize, NULL, 0, NULL);
    if ( NULL == mHandleCheckFile )
    {
        AfxMessageBox( _T("Monitor Log File Failed!!!"));
    }
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CrobotQQclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CrobotQQclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CrobotQQclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CrobotQQclientDlg::OnBnClickedBtnStart()
{
	// TODO: 在此添加控件通知处理程序代码

    gFlagCheckFileSize = FLAGE_START_THREAD_PROC;

}


void CrobotQQclientDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
<<<<<<< HEAD
    gFlagCheckFileSize = FLAGE_STOP_THREAD_PROC;
=======
    m_flagCheckFileSize = FLAGE_STOP_THREAD_PROC;
	//
>>>>>>> f798954d5911e635df81a4481fbe1dd215e6eab6
}


void CrobotQQclientDlg::OnBnClickedBtnSelectFile()
{
    // TODO: 在此添加控件通知处理程序代码
    CFileDialog fpdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
    if (fpdlg.DoModal() == IDOK)
    {
        //m_filename = fpdlg.GetFileName();
        //m_fileext = fpdlg.GetFileExt();//文件扩展名
        m_filepathname = fpdlg.GetPathName();
        //UpdateData(FALSE);
        //m_fileFullPath = m_filepathname + m_filename + "." + m_fileext;
        //
        mEditCtlPath.SetWindowTextW(m_filepathname);
    }
}


void CrobotQQclientDlg::OnBnClickedBtnHelp()
{
    // TODO: 在此添加控件通知处理程序代码

    CFile logMsgFile(m_filepathname.GetBuffer(), CFile::modeRead);
    mCurrentFileSize = logMsgFile.GetLength();

    AfxMessageBox( _T("~~~点了也没啥卵用~~~") );
}



void CrobotQQclientDlg::OnBnClickedBtnOnoffSouned()
{
    // TODO: 在此添加控件通知处理程序代码
    if (gOnOffSound == 0)
    {
        gOnOffSound = 1;
    }
    else
    {
        gOnOffSound = 0;
    }

}
