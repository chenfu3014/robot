/**************************************************************************
  Copyright (C), 

  File:  

  Author:  tony.chen
  
    Version: 1.0  
  
  Date:    2017
  
  Description:   

** History:
**Author (core ID)        Date          Number     Description of Changes

**--------------------------------------------------------------------------------------------------------------------
**                    --           --
** 
** --------------------------------------------------------------------------------------------------------------------
**************************************************************************/

#inlcude "stdio.h"
#include "string.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <linux/tcp.h>
#include <netdb.h>
#include <errno.h>
#include <net/if.h>






/**
 **************************************************************
 * @function name      : 
 * @brief              : 
 * @input param        : 
 * @output param       : 
 * @retval             : 
 * @author             : 
 * @date               : 
 * @others             :
 **************************************************************
*/
int BAL_TestTcpClientSocketInterface()
{
#if 0
    printf("[%s]%s:%d. Enter .\r\n", __FILE__, __FUNCTION__,  __LINE__  );
    int error_no = 0;
    struct sockaddr_in servaddr;
    int socketid = 0;
    char server_ip[32] = {"183.15.88.54"};   //  szgps.xicp.net
    short server_port = 3001;
    char sendbuf[] = {"hello, this is tony~~~~."};    
    char host_name[] = "szgps.xicp.net";
    //char host_name[] = "www.baidu.com";
    char **pptr = NULL;
    struct hostent *hptr = NULL;
    char recv_buf[32] = { 0 };
    int recv_len = 0;


    // get ip from domain
    //CFW_GetHostByName(host_name);
    
    while(1)
    {
        //   get ip adrress... 
        if((hptr = CFW_TcpipSocketGetHostByName(host_name)) == NULL)
        {            
            printf(".\r\n gethostbyname error(10s retry) for host:%s.errno:%d. info:%s.\r\n", host_name, errno, strerror(errno));
            printf("\r\n gethostbyname error for host:%s.\r\n", strerror(h_errno));
            sleep(10);
            continue;
        }

        printf("official hostname:%s\n",hptr->h_name);
        for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
        {
            printf(" alias:%s\n",*pptr);
        }

        switch(hptr->h_addrtype)
        {
            case AF_INET:
            case AF_INET6:
            {
                pptr=hptr->h_addr_list;
                for(; *pptr!=NULL; pptr++)
                {
                    printf(" address:%s\n",inet_ntop(hptr->h_addrtype, *pptr, server_ip, sizeof(server_ip)));
                }
                 printf(" first address: %s\n",inet_ntop(hptr->h_addrtype, hptr->h_addr, server_ip, sizeof(server_ip)));
            }         
            break;
            default:
                printf("unknown address type\n");
            break;
        }
        break;
        
    }
   

    //  create socket .
    socketid = CFW_TcpipSocket( AF_INET ,SOCK_STREAM, IPPROTO_TCP);
    if( 0 > socketid )
    {
        return -1;
    }

    // bind socket.
    BindSocketToInterface(socketid, "wan1");
    

    // get ip(uint32) from ip(string).    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    if( inet_pton(AF_INET, server_ip, &servaddr.sin_addr) <= 0)
    {
        printf("inet_pton error for %s\n",server_ip);
        CFW_TcpipSocketClose(socketid);
        return -1;

    }

    //  connect server just mdl ...not for ufi.
    printf("[%s]%s:%d.\r\n", __FILE__, __FUNCTION__,  __LINE__  );
    while(1)
    {       
        error_no = CFW_TcpipSocketConnectEx(socketid, (struct sockaddr*)&servaddr, sizeof(servaddr));
        if( error_no < 0)
        {
            printf("connect %s server_ip error, retry: %s(errno: %d)\n", server_ip, strerror(error_no),error_no);
            sleep(3);
            continue;
        }
        printf("connect %s sucess!!!!.%s(errno: %d)\n",server_ip, strerror(error_no),error_no); 
        break;
        
    }

    //
    printf("send msg to server: \n");
    error_no = CFW_TcpipSocketSend(socketid, sendbuf, strlen(sendbuf), 0);
    if( error_no < 0)
    {
        printf("send msg error: %s(errno: %d)\n", strerror(error_no), error_no);
        CFW_TcpipSocketClose(socketid);
        return -1;

    }

    recv_len = CFW_TcpipSocketRecv(socketid, recv_buf, 32, 0);   
    if( 0 > recv_len )
    {
        //error....
        CFW_TcpipSocketClose(socketid);
       
        
    }    
    printf("[%s]%s:%d. recv_buf:%s .\r\n", __FILE__, __FUNCTION__,  __LINE__ , recv_buf );

    
    CFW_TcpipSocketClose(socketid);
    printf("[%s]%s:%d. Exit .\r\n", __FILE__, __FUNCTION__,  __LINE__  );



#endif


#if 0
    //
    unsigned int wait_time = 180;
    while(wait_time)
    {
        sleep(1);
        printf("\r\n %d \r\n", wait_time--);
        
    }
    BAL_Test_Elec_MYNETCON();


    //
    BAL_Test_Elec_MYNETACT();
#endif


}




/**
 **************************************************************
 * @function name      : 
 * @brief              : 
 * @input param        : 
 * @output param       : 
 * @retval             : 
 * @author             : 
 * @date               : 
 * @others             :
 **************************************************************
*/
VOID BAL_TestTcpServersSocketInterface()
{
    printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface enter..!!!!!!!!! \r\n");
    sleep(60);
    printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface begin..!!!!!!!!! \r\n");
    int isReused = 1;
    int addr_len = 0;
    struct sockaddr_in servaddr;
    struct sockaddr_in new_clientaddr;
    addr_len = sizeof(new_clientaddr);
    
    int listenfd1 = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&new_clientaddr, 0, sizeof(new_clientaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(10000);

    setsockopt(listenfd1, SOL_SOCKET, SO_REUSEADDR,  &isReused, sizeof (isReused));  
    bind(listenfd1, (struct sockaddr *)&servaddr, sizeof(servaddr));

    int iret = listen(listenfd1,10);
    if( 0 != iret )
    {
        printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface listen failed.!!!!!!!!! \r\n"); 
        return -1;
    }
    
    printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface before listenfd1:%d.!!!!!!!!! \r\n", listenfd1); 
    int accecptfd1 = accept(listenfd1, (struct sockaddr*)&new_clientaddr, &addr_len);
    if( accecptfd1 > 0 )
    {      
        printf("BAL_TestTcpServersSocketInterface sin_port(new):%d, DestIP(new):%s. \r\n", 
                ntohs(new_clientaddr.sin_port), inet_ntoa(new_clientaddr.sin_addr)); 
        printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface new client incomming new socketfd:%d .!!!!!!!!! \r\n", accecptfd1); 
        
    }
    else
    {
        printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface error:%d.!!!!!!!!! \r\n", accecptfd1);
        return -1;
    }
}




/********************************** (C) *****END OF FILE**************************/