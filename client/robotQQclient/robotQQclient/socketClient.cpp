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
    int addr_len = 0;
    int error_no = 0;
    char server_ip[64] = { "23.245.202.74" };
    unsigned int server_port = 10000;
    struct sockaddr_in servAddrInfo;   
    addr_len = sizeof(servAddrInfo);

    int clientSockFd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if ( 0 == clientSockFd)
    {
        //error
    }

    memset(&servAddrInfo, 0, sizeof(servAddrInfo));
    servAddrInfo.sin_family = AF_INET;
    servAddrInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddrInfo.sin_port = htons(server_port);
    // get ip(uint32) from ip(string).    
    if (inet_pton(AF_INET, server_ip, &servAddrInfo.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n", server_ip);
        return;
    }

    //  connect server
    printf("[%s]%s:%d.\r\n", __FILE__, __FUNCTION__, __LINE__);
    while (1)
    {
        error_no = connect(clientSockFd, (struct sockaddr*)&servAddrInfo, sizeof(servAddrInfo));
        if (error_no < 0)
        {
            printf("connect %s server_ip error, retry: %s(errno: %d)\n", server_ip, strerror(error_no), error_no);
            sleep(3);
            continue;
        }
        printf("connect %s sucess!!!!.%s(errno: %d)\n", server_ip, strerror(error_no), error_no);
        break;

    }
}


socketClient::~socketClient()
{
}
