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


#include "stdio.h"
#include "string.h"
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <netdb.h>
#include <errno.h>
#include <net/if.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <iconv.h>
#include <stdlib.h>




#define RETURN_TCP_SUCCESS    0
#define RETURN_TCP_FAILED    -1

#define MAX_READ_BUF            10240
extern int gIsRecvDataFlag;
extern char gRecvBufStoreData[MAX_READ_BUF];


int maxSocketFD = 0;                                // max socketid for select .
int monitorSocketIdArray[FD_SETSIZE];  // tht socket is create by scket(), and use recv data for client and server.. and  accepct a new connection
int gServerListentSocketID = 0;             //  the socket is used  for  judge whether is a new client connection.
int gCurClientConnectCounts = 0;             // current clients numbers  connecting on server.

struct sockaddr_in gAcceptNewSockaddInfo;


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
void setSocketIdToFD (fd_set *fd_list)
{
    FD_ZERO(fd_list);//首先将fd_list清0  

    //将每一个socket加入fd_list中  
    int i = 0;

    for (i=0;i<FD_SETSIZE;i++)  
    {  
        if (monitorSocketIdArray[i]>0)  
        {  
            FD_SET(monitorSocketIdArray[i],fd_list);  
            //printf("setSocketIdToFD fd_set %d into. \r\n", monitorSocketIdArray[i]);
        }  
    } 
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
void insertSocketIdToMonitorArray (int sockfd )
{
    int i = 0;
    printf("insertSocketIdToMonitorArray:%d into array.... \r\n",  sockfd );
    if( sockfd > maxSocketFD )
    {
        maxSocketFD = sockfd;
    }
    for ( i; i<FD_SETSIZE; i++)  
    {  
        if (monitorSocketIdArray[i]==0)  
        {  
            monitorSocketIdArray[i]=sockfd;  
            break;
        }  
    } 
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
void setServerListenSocketID (int sockfd )
{
    gServerListentSocketID = sockfd;
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
void deleteSocketIdFromMonitorArray (int s)
{
    int i = 0;
    printf("deleteSocketIdFromMonitorArray:%d from array.... \r\n", s );
    for ( i;i<FD_SETSIZE;i++)  
    {  
        if (monitorSocketIdArray[i]==s)  
        {  
            monitorSocketIdArray[i]=0;  
            break;  
        }  
    }  
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
void cleanServerListenSocketID (int s)
{
    gServerListentSocketID = 0;
}



/**
***************************************************************
 * @function name      : 
 * @brief              : 
  * @input param        : 
 * @output param       : 
 * @retval             : 
 * @author             : 
  * @date               : 
 * @others             :
 **************************************************************
 **/
void recvDataAndNewConnectionFromClient()
{
    //
    int select_ret=0;  
    int recv_ret = 0;
    fd_set fdread;
    int socketfd = 0; 
    
    struct timeval timeout;
    printf("recvDataAndNewConnectionFromClient enter. \r\n");
    
    while(1)  
    {              
        //must here......
        setSocketIdToFD(&fdread);

        timeout.tv_sec= 3;
        timeout.tv_usec= 0;		
        printf("recvDataAndNewConnectionFromClient select waiting...\n");
        select_ret = select( maxSocketFD+1, &fdread, NULL, NULL, &timeout);
        
        if( 0 > select_ret )    //error
        {
            printf("[%s:%d] CFW_TcpipSocketSelect error.... \r\n", __FUNCTION__ , __LINE__);
            sleep(3);
            continue; 
                      
        }
        if ( 0 == select_ret)   //select is timeout 
        {
        	printf("recvDataAndNewConnectionFromClient select timeout.\n");
            continue;  
        }  
        else    // 0 < slelct_ret  
        {  
            int i = 0;
			printf("recvDataAndNewConnectionFromClient select_ret > 0.\n");
            for (i;i<FD_SETSIZE;i++)  
            {   
                if ( 0 == monitorSocketIdArray[i])  
                {
                    continue;  
                }
                socketfd=monitorSocketIdArray[i];  
                printf("===>i:%d, socketfd:%d, select_ret:%d, gServerListentSocketID:%d.\r\n", i, monitorSocketIdArray[i], select_ret, gServerListentSocketID);
                
                if ( FD_ISSET(socketfd,&fdread))  
                {  
                    printf("FD_ISSET socketfd:%d has event.\n", socketfd );
                    if( socketfd != gServerListentSocketID )                           // socketfd not for accept new connection......
                    {
                        char recv_buf[MAX_READ_BUF] = {0};
                        int recv_buf_size = sizeof(recv_buf)/sizeof(char);
                        recv_ret=recv(socketfd,recv_buf,recv_buf_size,MSG_DONTWAIT  | MSG_PEEK);  
                        if( 0 > recv_ret )
                        {
                            //
                            printf("[%s:%d] tcp connection is error...., socketfd:%d,recv_ret:%d.\r\n", __FUNCTION__ , __LINE__,socketfd, recv_ret);                         
                            deleteSocketIdFromMonitorArray(socketfd);
                            close(socketfd);
                            //sys_post_event_to_APP(EV_CFW_TCPIP_ERR_IND, socket, 0, 0, 0, 0);
                        }                      
                        else if (0 == recv_ret)
                        {  
                            // there is 2 situation. 1: remote pc close the socket; 2: network is broken....
                            printf("[%s:%d] tcp connection is close.... socketfd:%d, recv_ret:%d.\r\n", __FUNCTION__ , __LINE__,socketfd, recv_ret);
                            deleteSocketIdFromMonitorArray(socketfd); //must be delete for ftp.....
                            close(socketfd);
                            //sys_post_event_to_APP(EV_CFW_TCPIP_CLOSE_IND, socketfd, 0, 0, 0, 0);    
                            //FD_CLR();
                        }
                        else  
                        {      
                            // recv_ret > 0
                            printf(" [%s:%d]!!!!!!!!!! wo recv data from server on socketfd:%d, datasize:%d. !!!!!!!!!! \r\n", __FUNCTION__ , __LINE__, socketfd, recv_ret); 
						
                            //sys_post_event_to_APP(EV_CFW_TCPIP_REV_DATA_IND, socketfd, recv_len, 0, 0, 0);                            
                            sleep(1);

                            recv_ret=recv(socketfd,recv_buf,recv_buf_size, 0);  
                            if( 0 < recv_ret )
                            {
                                char *pIn = recv_buf;
                                char *pOut = gRecvBufStoreData;   
                                memset(gRecvBufStoreData, 0, MAX_READ_BUF);
                                size_t iInLen = strlen(recv_buf);                                
                                size_t iOutLen = MAX_READ_BUF; 

                                iconv_t hIconv = iconv_open("UTF-8", "GBK");  
                                if( 0 == hIconv )
                                {   
                                    printf("iconv_open error.\n" );
                                    return ;
                                }
                                
                                printf("iconv_open hIconv:%d,iInLen:%d.\n", hIconv, iInLen );
                                size_t iRet = iconv(hIconv, (&pIn), &iInLen, &pOut, &iOutLen); 
                                printf("iconv iRet:%d.\n", iRet );
                                if( -1 != iRet )
                                {
                                    gIsRecvDataFlag = 1;
                                    printf("iconv data successfull....\n");
                                }           
                                else
                                {
                                    printf("iconv data failed....\n");                                        
                                }
                                iconv_close(hIconv);                                                                                                           
                            }               
                            
                        }  
                    }
                    else                        // socketfd for server listen .
                    {
                        int iRet = 0;
                        socklen_t addr_len = sizeof(gAcceptNewSockaddInfo);
                        
                        iRet = accept(socketfd, (struct sockaddr*)&gAcceptNewSockaddInfo, &addr_len);
                        if( iRet > 0 )
                        {      
                            printf("BAL_CFWApsTask accept sin_port(new):%d, DestIP(new):%s. \r\n", ntohs(gAcceptNewSockaddInfo.sin_port), inet_ntoa(gAcceptNewSockaddInfo.sin_addr)); 
                            printf("!!!!!!!!!! new client incomming new socketfd:%d .!!!!!!!!! \r\n", iRet); 
                            insertSocketIdToMonitorArray(iRet);                            
                                    
                            //sys_post_event_to_APP(EV_CFW_TCPIP_ACCEPT_IND, socketfd, 0, 0, 0, 0);

                        }
                        else
                        {
                            printf("!!!!!!!!!!BAL_CFWApsTask accept error:%d.!!!!!!!!! \r\n", iRet); 
                        }
                    }
                    
                }//end if  

            }//end for  

        }//end if (err==0)  

    }       
}


/****************************************************************
 * @function name      : 
 * @brief              : 
  * @input param        : 
 * @output param       :
 * @retval             :    0 is success...
 * @author             : 
  * @date               : 
 * @others             :
 ***************************************************************/
int initAsTcpClientMode()
{
    return 0;
}





/****************************************************************
 * @function name      : 
 * @brief              : 
  * @input param        : 
 * @output param       :
 * @retval             :    0 is success...
 * @author             : 
  * @date               : 
 * @others             :
 ***************************************************************/
int initAsTcpServerMode()
{
    int iRet = 0;
    int addr_len = 0;
    int serverListenFD = 0;
    struct sockaddr_in srvAdrrInfo;
    memset(&srvAdrrInfo, 0, sizeof(srvAdrrInfo));
    
    serverListenFD = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if( 0 > serverListenFD )
    {
        printf("!!!!!!!!!!initServerSocket socket failed.!!!!!!!!! \r\n");         
    }
	printf("!!!!!!!!!!initServerSocket serverListenFD %d.!!!!!!!!! \r\n", serverListenFD); 
    srvAdrrInfo.sin_family = AF_INET;
    srvAdrrInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAdrrInfo.sin_port = htons(54321);
    iRet = bind(serverListenFD, (struct sockaddr *)&srvAdrrInfo, sizeof(srvAdrrInfo));
    if( 0 != iRet )
    {
        printf("!!!!!!!!!!initServerSocket bind failed.!!!!!!!!! \r\n"); 
        close(serverListenFD);        
        serverListenFD = -1;
        return RETURN_TCP_FAILED;
    }
    printf("!!!!!!!!!!initServerSocket bind success.!!!!!!!!! \r\n"); 
    iRet = listen(serverListenFD,10);
    if( 0 != iRet )
    {
        printf("!!!!!!!!!!initServerSocket listen failed.!!!!!!!!! \r\n"); 
        close(serverListenFD);
        serverListenFD = -1;
        return RETURN_TCP_FAILED;
    }
	printf("!!!!!!!!!!initServerSocket listen 54321 success.!!!!!!!!! \r\n");
	
    insertSocketIdToMonitorArray( serverListenFD );
    setServerListenSocketID(serverListenFD);
    return RETURN_TCP_SUCCESS;
}



/****************************************************************
 * @function name      : 
 * @brief              : 
  * @input param        : 
 * @output param       :
 * @retval             :
 * @author             : 
  * @date               : 
 * @others             :
 ***************************************************************/
 void *netCommThreadProc(void *arg)
{
	printf("netCommThreadProc enter.\n");
    recvDataAndNewConnectionFromClient();
}





/****************************************************************
 * @function name      : 
 * @brief              : 
  * @input param        : 
 * @output param       :
 * @retval             :
 * @author             : 
  * @date               : 
 * @others             :
 ***************************************************************/
int createTaskNetCommService()
{
    int errNum = 0;
    pthread_t threadNetCommID = 0;
    
    if (initAsTcpClientMode() != 0)
    {
        printf("initAsTcpClientMode failed.\n");
        return RETURN_TCP_FAILED;
    }
    
    if (initAsTcpServerMode() != 0)
    {
        printf("initAsTcpServerMode failed.\n");
        return RETURN_TCP_FAILED;
    } 

    
    errNum = pthread_create(&threadNetCommID, NULL, netCommThreadProc, NULL);
    if (errNum != 0)
    {
        printf("createTaskNetCommService failed: %s.\n", strerror(errNum));
    }    
    else
    {
        printf("createTaskNetCommService successfully: %s.\n", strerror(errNum));
    }
    return RETURN_TCP_SUCCESS; 
}





#if 0
void BAL_TestTcpServersSocketInterface()
{
    printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface enter..!!!!!!!!! \r\n");
    sleep(60);
    printf("!!!!!!!!!!BAL_TestTcpServersSocketInterface begin..!!!!!!!!! \r\n");
    int isReused = 1;
    int addr_len = 0;
    struct sockaddr_in srvAdrrInfo;
    struct sockaddr_in new_clientaddr;
    addr_len = sizeof(new_clientaddr);
    
    int listenfd1 = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    
    memset(&srvAdrrInfo, 0, sizeof(srvAdrrInfo));
    memset(&new_clientaddr, 0, sizeof(new_clientaddr));
    srvAdrrInfo.sin_family = AF_INET;
    srvAdrrInfo.sin_addr.s_addr = htonl(INADDR_ANY);
    srvAdrrInfo.sin_port = htons(10000);

    setsockopt(listenfd1, SOL_SOCKET, SO_REUSEADDR,  &isReused, sizeof (isReused));  
    bind(listenfd1, (struct sockaddr *)&srvAdrrInfo, sizeof(srvAdrrInfo));

    int iRet = listen(listenfd1,10);
    if( 0 != iRet )
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
#endif

/********************************** (C) *****END OF FILE**************************/
