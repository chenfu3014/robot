
// robotQQclient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CrobotQQclientApp: 
// �йش����ʵ�֣������ robotQQclient.cpp
//

class CrobotQQclientApp : public CWinApp
{
public:
	CrobotQQclientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CrobotQQclientApp theApp;