
// robotQQclientDlg.cpp : ʵ���ļ�
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


// CrobotQQclientDlg �Ի���



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
    mHandleCheckFile = CreateThread(NULL, 0, ThreadProcCheckFileSize, NULL, 0, NULL);
    if ( NULL == mHandleCheckFile )
    {
        AfxMessageBox( _T("Monitor Log File Failed!!!"));
    }
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

}


void CrobotQQclientDlg::OnBnClickedBtnStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
<<<<<<< HEAD
    gFlagCheckFileSize = FLAGE_STOP_THREAD_PROC;
=======
    m_flagCheckFileSize = FLAGE_STOP_THREAD_PROC;
	//
>>>>>>> f798954d5911e635df81a4481fbe1dd215e6eab6
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
        //
        mEditCtlPath.SetWindowTextW(m_filepathname);
    }
}


void CrobotQQclientDlg::OnBnClickedBtnHelp()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    CFile logMsgFile(m_filepathname.GetBuffer(), CFile::modeRead);
    mCurrentFileSize = logMsgFile.GetLength();

    AfxMessageBox( _T("~~~����Ҳûɶ����~~~") );
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
