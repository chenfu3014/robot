#include "stdafx.h"
#include "socketClient.h"



/*
23.245.202.74
root
AH@&823rfgk
这是linux服务器
*/
socketClient::socketClient()
{
    initResoure();
}


socketClient::~socketClient()
{
    deinitResoure();
}


void socketClient::initResoure()
{
    msocketFd = 0;
}



void socketClient::deinitResoure()
{
    msocketFd = 0;
}



int socketClient::initSocketForClient( const char  *serverIP, const int serverPort )
{
    //
    int addr_len = 0;
    int error_no = 0;
    struct sockaddr_in servAddrInfo;
    addr_len = sizeof(servAddrInfo);

    if (NULL == serverIP || 0 == serverPort)
    {
        printf("input  error for initSocketForClient. \r\n");
        return -1;
    }

    msocketFd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (0 == msocketFd)
    {
        //error
        return -1;
    }

    memset(&servAddrInfo, 0, sizeof(servAddrInfo));
    servAddrInfo.sin_family = AF_INET;
    servAddrInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddrInfo.sin_port = htons(serverPort);
    // get ip(uint32) from ip(string).    
    if (inet_pton(AF_INET, serverIP, &servAddrInfo.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", serverIP);
        return -1;
    }

    //  connect server
    printf("[%s]%s:%d.\r\n", __FILE__, __FUNCTION__, __LINE__);
    while (1)
    {
        error_no = connect(msocketFd, (struct sockaddr*)&servAddrInfo, sizeof(servAddrInfo));
        if (error_no < 0)
        {
            //printf("connect %s server_ip error, retry: %s(errno: %d)\n", server_ip, strerror(error_no), error_no);
            Sleep(1);
            continue;
        }
        //printf("connect %s sucess!!!!.%s(errno: %d)\n", server_ip, strerror(error_no), error_no);
        break;
    }
	return 0;
}



int socketClient::sendDataToServer(const char *sendBuf, const int bufSize)
{   
    int sendLen = 0;
    if (NULL == sendBuf || 0 == bufSize)
    {
        return -1;
    }
    while (sendLen < bufSize)
    {
        int i = send(msocketFd, sendBuf, bufSize, 0);
        if (0 > i)
        {
            return -1;
        }
        sendLen = sendLen + i;
    }

    return sendLen;
}

int socketClient::deinitSocketForClient()
{
    closesocket(msocketFd);
    return 0;
}
