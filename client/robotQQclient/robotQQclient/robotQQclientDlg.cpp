
// robotQQclientDlg.cpp : ʵ���ļ�
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
unsigned long long gCurrentFileSize = 0;   //  ��ǰ���ļ���С
unsigned long long gLastFileSize = 0;        //  �ϴε��ļ���С
unsigned long long gFileSeekPoint = 0;      // ��ǰ�ļ����ݶ�ȡ�����
CString gLogFilePath;                               //  QQ��־�ļ���·��

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
            sysInfo.Format(_T("����Ƿ����µ�����???"));
            ::SetWindowTextW(hSysInfo, sysInfo);
            Sleep(1 * 1000);
            CFile logMsgFile(gLogFilePath.GetBuffer(), CFile::modeRead);
            gCurrentFileSize = logMsgFile.GetLength();

            int changeSize = (int)(gCurrentFileSize - gLastFileSize);
            if (changeSize > 0)
            {
                //  ������Ϣ                
                sysInfo.Format(_T("��⵽�µ�����!  new:%d > old:%d."), gCurrentFileSize, gLastFileSize);
                ::SetWindowTextW(hSysInfo, sysInfo);
                Sleep(1 * 1000);

                // 1: read file 
                char readBuf[READ_EACH_TIME_MAX_DATA] = { 0 };
                logMsgFile.Seek(gLastFileSize, 0);
                int readOKCount = logMsgFile.Read(readBuf, READ_EACH_TIME_MAX_DATA);
                if (readOKCount)
                {
                    sysInfo.Format(_T("��ȡ���ݳɹ�!  ���ݴ�С:%d."), readOKCount);
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    gLastFileSize += readOKCount;

                }
                else
                {
                    sysInfo.Format(_T("��ȡ����ʧ��."));
                    ::SetWindowTextW(hSysInfo, sysInfo);
                }

                // close file
                logMsgFile.Close();


                // 2: connect to sever.
                socketClient tcpClient;
                while (1)
                {
                    sysInfo.Format(_T("��ʼ���ӷ�����...."));
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    if (0 == tcpClient.initSocketForClient(gTcpServerAddr, gTcpServerPort))
                    {
                        sysInfo.Format(_T("���ӷ������ɹ�."));
                        ::SetWindowTextW(hSysInfo, sysInfo);
                        Sleep(1 * 1000);
                        break;
                    }
                    else
                    {
                        sysInfo.Format(_T("���ӷ�����ʧ�ܣ�10������� "));
                        ::SetWindowTextW(hSysInfo, sysInfo);
                        Sleep(10 * 1000);
                        continue;
                    }
                }


                // 3: send file to server.
                if ( 0 < tcpClient.sendDataToServer(readBuf, readOKCount))
                {
                    sysInfo.Format(_T("�������ݳɹ�, ���ݴ�С:%d."), readOKCount);
                    ::SetWindowTextW(hSysInfo, sysInfo);
                    Sleep(1 * 1000);
                }
                else
                {
                    sysInfo.Format(_T("��������ʧ��."));
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
                sysInfo.Format(_T("û�м�⵽�µ�����,10s������."));
                ::SetWindowTextW(hSysInfo, sysInfo);
                Sleep(10 * 1000);
            }
        }
    }
    return 0;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CrobotQQclientDlg �Ի���  ��������ַ��



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


// CrobotQQclientDlg ��Ϣ�������

BOOL CrobotQQclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
    gOnOffSound = 1;
    gCurrentFileSize = 0;
    gLastFileSize = 0;
    mHandleCheckFile = CreateThread(NULL, 0, ThreadProcCheckFileSize, NULL, 0, NULL);
    if ( NULL == mHandleCheckFile )
    {
        AfxMessageBox( _T("Monitor Log File Failed!!!"));
    }

    mCtrlIPAdress.SetWindowTextW(_T("23.245.202.74"));   // 
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CrobotQQclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CrobotQQclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CrobotQQclientDlg::OnBnClickedBtnStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

    gFlagCheckFileSize = FLAGE_START_THREAD_PROC;
    mCtrlSysInfo.SetWindowTextW( _T(" ����ʼ����...."));    
}


void CrobotQQclientDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    gFlagCheckFileSize = FLAGE_STOP_THREAD_PROC;
    mCtrlSysInfo.SetWindowTextW( _T(" ����ֹͣ���� !!!"));
}


void CrobotQQclientDlg::OnBnClickedBtnSelectFile()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    CFileDialog fpdlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
    if (fpdlg.DoModal() == IDOK)
    {
        //m_filename = fpdlg.GetFileName();
        //m_fileext = fpdlg.GetFileExt();//�ļ���չ��
        m_filepathname = fpdlg.GetPathName();
        //UpdateData(FALSE);
        //m_fileFullPath = m_filepathname + m_filename + "." + m_fileext;
        
        gLogFilePath = m_filepathname;

        mEditCtlPath.SetWindowTextW(gLogFilePath);


        CIPAddressCtrl * pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDR_SRV);
        BYTE nf1, nf2, nf3, nf4;
        pIP->GetAddress(nf1, nf2, nf3, nf4);
        CString cstrServerAddr;
        cstrServerAddr.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);//�����nf�õ���ֵ��IPֵ��

        int strLength = cstrServerAddr.GetLength() + 1;
        TCHAR bufIPAddr[32] = { _T("0") };
        wcsncpy_s(bufIPAddr, (cstrServerAddr.GetBuffer()), strLength);
        WideCharToMultiByte(CP_ACP, 0, bufIPAddr, wcslen(bufIPAddr), gTcpServerAddr, 32, NULL, NULL);
    }
}


void CrobotQQclientDlg::OnBnClickedBtnHelp()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    AfxMessageBox( _T("~~~����Ҳûɶ����~~~") );
}



void CrobotQQclientDlg::setSysInfoToStatusBar(CString sysInfo)
{
    mEditCtlPath.SetWindowTextW(sysInfo);
}

void CrobotQQclientDlg::OnBnClickedBtnOnoffSouned()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (gOnOffSound == 0)
    {
        gOnOffSound = 1;
    }
    else
    {
        gOnOffSound = 0;
    }

}
