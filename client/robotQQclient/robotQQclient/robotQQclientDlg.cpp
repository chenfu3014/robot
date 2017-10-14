
// robotQQclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "robotQQclient.h"
#include "robotQQclientDlg.h"
#include "afxdialogex.h"
#include "socketClient.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define  READ_EACH_TIME_MAX_DATA  20480

bool gFlagCheckFileSize = FLAGE_STOP_THREAD_PROC;
bool gOnOffSound = 0;
unsigned long long gCurrentFileSize = 0;   //  当前的文件大小
unsigned long long gLastFileSize = 0;        //  上次的文件大小
unsigned long long gFileSeekPoint = 0;      // 当前文件数据读取的起点
CString gLogFilePath;                               //  QQ日志文件的路径

char gTcpServerAddr[32] = {"0"};
unsigned short gTcpServerPort = 54321;

DWORD WINAPI  ThreadProcCheckFileSize(LPVOID lpParam)
{
    HWND hMainDlg = AfxGetApp()->GetMainWnd()->GetSafeHwnd();
    HWND hSysInfo = ::GetDlgItem(hMainDlg, IDC_SYS_INFO);
    CString sysInfo;
    while (1)
    {
        if (FLAGE_STOP_THREAD_PROC == gFlagCheckFileSize)
        {
#if 1
            Sleep(5 * 1000);
#else
            Sleep(60);
#endif
            continue;
        }
        else
        {
            // 0: check file has been modify ....
            sysInfo.Format(_T("检测是否有新的数据???"));
            ::SetWindowTextW(hSysInfo, sysInfo);
            Sleep(1 * 1000);
            CFile logMsgFile(gLogFilePath.GetBuffer(), CFile::modeRead);
            gCurrentFileSize = logMsgFile.GetLength();

            int changeSize = (int)(gCurrentFileSize - gLastFileSize);
            if (changeSize > 0)
            {
                //  程序信息                
                sysInfo.Format(_T("检测到新的数据!  new:%d > old:%d."), gCurrentFileSize, gLastFileSize);
                ::SetWindowTextW(hSysInfo, sysInfo);
                Sleep(1 * 1000);

                // 1: read file 
                char readBuf[READ_EACH_TIME_MAX_DATA] = { 0 };
                logMsgFile.Seek(gLastFileSize, 0);
                int readOKCount = logMsgFile.Read(readBuf, READ_EACH_TIME_MAX_DATA);
                if (readOKCount)
                {
                    sysInfo.Format(_T("读取数据成功!  数据大小:%d."), readOKCount);
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    gLastFileSize += readOKCount;

                }
                else
                {
                    sysInfo.Format(_T("读取数据失败."));
                    ::SetWindowTextW(hSysInfo, sysInfo);
                }

                // close file
                logMsgFile.Close();


                // 2: connect to sever.
                socketClient tcpClient;
                while (1)
                {
                    sysInfo.Format(_T("开始连接服务器...."));
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    if (0 == tcpClient.initSocketForClient(gTcpServerAddr, gTcpServerPort))
                    {
                        sysInfo.Format(_T("连接服务器成功."));
                        ::SetWindowTextW(hSysInfo, sysInfo);
                        Sleep(1 * 1000);
                        break;
                    }
                    else
                    {
                        sysInfo.Format(_T("连接服务器失败，10秒后重试 "));
                        ::SetWindowTextW(hSysInfo, sysInfo);
                        Sleep(10 * 1000);
                        continue;
                    }
                }


                // 3: send file to server.
                if ( 0 < tcpClient.sendDataToServer(readBuf, readOKCount))
                {
                    sysInfo.Format(_T("发送数据成功, 数据大小:%d."), readOKCount);
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    Sleep(1 * 1000);
                }
                else
                {
                    sysInfo.Format(_T("发送数据失败."));
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    Sleep(1 * 1000);
                }

                // 4: close tcp client.
                Sleep(3 * 1000);
                tcpClient.deinitSocketForClient();
            }
            else
            {
                // close file
                logMsgFile.Close();
                sysInfo.Format(_T("没有检测到新的数据,10s后重试."));
                ::SetWindowTextW(hSysInfo, sysInfo);
                Sleep(10 * 1000);
            }
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


// CrobotQQclientDlg 对话框  服务器地址：



CrobotQQclientDlg::CrobotQQclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROBOTQQCLIENT_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDB_MAINICON);
}

void CrobotQQclientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_FILE_PATH, mEditCtlPath);
    DDX_Control(pDX, IDC_SYS_INFO, mCtrlSysInfo);
    DDX_Control(pDX, IDC_IPADDR_SRV, mCtrlIPAdress);
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
    gCurrentFileSize = 0;
    gLastFileSize = 0;
    mHandleCheckFile = CreateThread(NULL, 0, ThreadProcCheckFileSize, NULL, 0, NULL);
    if ( NULL == mHandleCheckFile )
    {
        AfxMessageBox( _T("Monitor Log File Failed!!!"));
    }

    mCtrlIPAdress.SetWindowTextW(_T("23.245.202.74"));   // 
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
    mCtrlSysInfo.SetWindowTextW( _T(" 程序开始运行...."));    
}


void CrobotQQclientDlg::OnBnClickedBtnStop()
{
	// TODO: 在此添加控件通知处理程序代码
    gFlagCheckFileSize = FLAGE_STOP_THREAD_PROC;
    mCtrlSysInfo.SetWindowTextW( _T(" 程序停止运行 !!!"));
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
        
        gLogFilePath = m_filepathname;

        mEditCtlPath.SetWindowTextW(gLogFilePath);


        CIPAddressCtrl * pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDR_SRV);
        BYTE nf1, nf2, nf3, nf4;
        pIP->GetAddress(nf1, nf2, nf3, nf4);
        CString cstrServerAddr;
        cstrServerAddr.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);//这里的nf得到的值是IP值了

        int strLength = cstrServerAddr.GetLength() + 1;
        TCHAR bufIPAddr[32] = { _T("0") };
        wcsncpy_s(bufIPAddr, (cstrServerAddr.GetBuffer()), strLength);
        WideCharToMultiByte(CP_ACP, 0, bufIPAddr, wcslen(bufIPAddr), gTcpServerAddr, 32, NULL, NULL);
    }
}


void CrobotQQclientDlg::OnBnClickedBtnHelp()
{
    // TODO: 在此添加控件通知处理程序代码

    AfxMessageBox( _T("~~~点了也没啥卵用~~~") );
}



void CrobotQQclientDlg::setSysInfoToStatusBar(CString sysInfo)
{
    mEditCtlPath.SetWindowTextW(sysInfo);
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
