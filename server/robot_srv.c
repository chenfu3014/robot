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
void store_data_to_file()
{
    //
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
bool BAL_ApsTaskSetSocketToFD (fd_set *fd_list)
{
    FD_ZERO(fd_list);//首先将fd_list清0  

    //将每一个socket加入fd_list中  
    int i = 0;
    for (i=0;i<FD_SETSIZE;i++)  
    {  
        if (socketIdArray[i]>0)  
        {  
            FD_SET(socketIdArray[i],fd_list);  
            //printf("BAL_ApsTaskSetSocketToFD fd_set %d into. \r\n", socketIdArray[i]);
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
bool BAL_ApsTaskInsertSocket (int sockfd )
{
    int i = 0;
    printf("[%s:%d] insert socketfd:%d into array.... \r\n", __FUNCTION__ , __LINE__, sockfd );
    if( sockfd > max_socketfd )
    {
        max_socketfd = sockfd;
    }
    for ( i; i<FD_SETSIZE; i++)  
    {  
        //如果某一个socketArray[i]为0，表示哪一个位可以放入socket  
        if (socketIdArray[i]==0)  
        {  
            socketIdArray[i]=sockfd;  
            sockCurNumCounts++;  
            break;//这里一定要加上break，不然一个socket会放在socketArray的多个位置上  
        }  
    } 
    return 0;
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
bool BAL_ApsTaskInsertServerSocket (int sockfd )
{
    int i = 0;
    printf("[%s:%d] insert server socketfd:%d into array.... \r\n", __FUNCTION__ , __LINE__, sockfd );
    if( sockfd > max_socketfd )
    {
        max_socketfd = sockfd;
    }

    for ( i; i<FD_SETSIZE; i++)  
    {  
        //如果某一个socketArray[i]为0，表示哪一个位可以放入socket  
        if (serverSocketIdArray[i]==0)  
        {  
            serverSocketIdArray[i]=sockfd;  
            serversocketCurNumCounts++;  
            break;//这里一定要加上break，不然一个socket会放在socketArray的多个位置上  
        }  
    } 
    return 0;
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
BOOL BAL_ApsTaskDeleteSocket (int s)
{
    int i = 0;
    printf("[%s:%d] delete socketfd:%d from array.... \r\n", __FUNCTION__ , __LINE__, s );
    for ( i;i<FD_SETSIZE;i++)  
    {  
        if (socketIdArray[i]==s)  
        {  
            socketIdArray[i]=0;  
            sockCurNumCounts--;  
            break;  
        }  
    }  
    return 0;
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
BOOL BAL_ApsTaskDeleteSeverSocket (int s)
{
    int i = 0;
    printf("[%s:%d] delete server socketfd:%d from array.... \r\n", __FUNCTION__ , __LINE__, s );
    for ( i;i<FD_SETSIZE;i++)  
    {  
        if (serverSocketIdArray[i]==s)  
        {  
            serverSocketIdArray[i]=0;  
            serversocketCurNumCounts--;  
            break;  
        }  
    }  
    return 0;
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
void recv_data_proc()
{
    //
    int select_ret=0;  
    int recv_ret = 0;
    fd_set fdread;//存在读文件的set，select会检测这个set中是否可以从某些socket中读入信息  
    //fd_set fdwrite;//存在写文件的set，select会检测这个set中是否可以从某些socket中读入信息      
    SOCKET socketfd = 0; 
    
    struct timeval timeout;//设置select超时的时间  

    printf("[%s:%d] enter. \r\n", __FUNCTION__ , __LINE__);
    
    while(1)  
    {              
        //must here......
        BAL_ApsTaskSetSocketToFD(&fdread);

        /* add begin by jinyf, 2017-09-13, 原因: select完毕定时器自动清零，需重新设置。
            超时时间不宜过长，否则有可能出现新建立的socket无法及时加入到fd_list中*/
        timeout.tv_sec= 2;
        timeout.tv_usec= 0;
        /* add end by jinyf, 2017-09-13 */
        
        select_ret = select( max_socketfd+1, &fdread, NULL, NULL, &timeout);
        
        if( 0 > select_ret )
        {
            //error
            printf("[%s:%d] CFW_TcpipSocketSelect error.... \r\n", __FUNCTION__ , __LINE__);
            sleep(3);
            continue; 
                      
        }
        if ( 0 == select_ret)//select返回0表示超时  
        {
            /* delete begin by jinyf, 2017-09-13, 原因: select为非阻塞。超时无需sleep*/
            //sleep(1);
            /* delete end by jinyf, 2017-09-13 */
            //printf("aps select is running...max_socketfd+1:%d.\r\n", max_socketfd+1);
            continue;  
        }  
        else  
        {  
            // 0 < slelct_ret
            //遍历socketList中的每一个socket，查看那些socket是可读的，处理可读的socket  
            //从中读取数据到缓冲区，并发送数据给客户端  
            int i = 0;
            for (i;i<FD_SETSIZE;i++)  
            {   
                //读取有效的socket  
                if ( 0 == socketIdArray[i])  
                {
                    continue;  
                }
                socketfd=socketIdArray[i];  
                printf("===>i:%d, socketfd:%d, select_ret:%d.\r\n", i, socketIdArray[i], select_ret);
                
                if ( FD_ISSET(socketfd,&fdread))  
                {  
                    int j = 0;
                    int isServerSockfd = 0;
                    for (j;j<FD_SETSIZE;j++) 
                    { 
                        if ( socketfd == serverSocketIdArray[j])  
                        {
                           isServerSockfd  = 1;
                           printf("!!!!!!!!!! wo got a listen server socketfd:%d. !!!!!!!!!!.\r\n" ,socketfd);     
                           break;
                        }
                        else
                        {
                            continue;
                        }
                    }
                    
                    if(0 == isServerSockfd )  //  判  断  该  套  接  字  是  否  为  监  听 套  接  字
                    {
                        //判断哪些socket是可读的，如果这个socket是可读的，从它里面读取数据  
                        char recv_buf[1024] = {0};
                        int recv_buf_size = sizeof(recv_buf)/sizeof(char);
                        recv_ret=recv(socketfd,recv_buf,recv_buf_size,MSG_DONTWAIT  | MSG_PEEK);  
                        //如果返回值表示要关闭这个连接，那么关闭它，并将它从sockeList中去掉  
                        if( 0 > recv_ret )
                        {
                            //
                            printf("[%s:%d] tcp connection is error...., socketfd:%d,recv_ret:%d.\r\n", __FUNCTION__ , __LINE__,socketfd, recv_ret);                         
                            BAL_ApsTaskDeleteSocket(socketfd);
                            //sys_post_event_to_APP(EV_CFW_TCPIP_ERR_IND, socket, 0, 0, 0, 0);
                        }                      
                        else if (0 == recv_ret)
                        {  
                            // there is 2 situation. 1: remote pc close the socket; 2: network is broken....
                            printf("[%s:%d] tcp connection is close.... socketfd:%d, recv_ret:%d.\r\n", __FUNCTION__ , __LINE__,socketfd, recv_ret);
                            BAL_ApsTaskDeleteSocket(socketfd); //must be delete for ftp.....
                            sys_post_event_to_APP(EV_CFW_TCPIP_CLOSE_IND, socketfd, 0, 0, 0, 0);    
                            //FD_CLR();
                        }
                        else  
                        {      
                            // recv_ret > 0
                            printf(" [%s:%d]!!!!!!!!!! wo recv data from server on socketfd:%d, recv_ret:%d. !!!!!!!!!! \r\n", __FUNCTION__ , __LINE__, socketfd, recv_ret);
                            COS_EVENT ev;
                            int recv_len = 0;
                            int ret = 0;
                            memset( &ev, 0, sizeof(ev) );
                            ev.nEventId = EV_CFW_GPRS_DATA_IND ;
                            ev.nParam1 = socketfd;
                            BAL_ApsTaskTcpipProc( &ev );
                                                                                  
                            sys_post_event_to_APP(EV_CFW_TCPIP_REV_DATA_IND, socketfd, recv_len, 0, 0, 0);
                            
                            sleep(1); //尝试将此延时去掉，发现时间发送完毕。可能会导致接受线程来不及处理最终导致命令全部阻塞
                        }  
                    }
                    else
                    {
                        // 该  套  接  字  为  监  听  套  接  字  
                        int iret = 0;
                        int addr_len = sizeof(g_accept_new_sockaddr_in);
                        
                        iret = accept(socketfd, (struct sockaddr*)&g_accept_new_sockaddr_in, &addr_len);
                        if( iret > 0 )
                        {      
                            g_accept_new_socketfd = iret;
                            printf("BAL_CFWApsTask accept sin_port(new):%d, DestIP(new):%s. \r\n", ntohs(g_accept_new_sockaddr_in.sin_port), inet_ntoa(g_accept_new_sockaddr_in.sin_addr)); 
                            printf("!!!!!!!!!!BAL_CFWApsTask new client incomming new socketfd:%d .!!!!!!!!! \r\n", g_accept_new_socketfd); 
                                    
                            sys_post_event_to_APP(EV_CFW_TCPIP_ACCEPT_IND, socketfd, 0, 0, 0, 0);

                        }
                        else
                        {
                            printf("!!!!!!!!!!BAL_CFWApsTask accept error:%d.!!!!!!!!! \r\n", iret); 
                        }
                    }
                    
                }//end if  

            }//end for  

        }//end if (err==0)  

    }
    
    
    return ret; 
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
int BindSocketToInterface( int socket_id, char *if_name )
{
    printf("[%s]%s:%d. Enter .\r\n", __FILE__, __FUNCTION__,  __LINE__  );
    int err_num = 0;
    int name_len = 0;
    if( NULL ==  if_name  || 0 > socket_id )
    {
        printf("[%s]%s:%d. input error .\r\n", __FILE__, __FUNCTION__,  __LINE__  );
        return -1;
    }
    struct ifreq ifr;
    memset( &ifr, 0, sizeof( struct ifreq) );
    strcpy( ifr.ifr_name, if_name );
    name_len = strlen(if_name);
    ifr.ifr_name[name_len+1] = "\0";
    err_num = setsockopt(socket_id, SOL_SOCKET, SO_BINDTODEVICE, &ifr, sizeof(ifr) );
    if( 0 > err_num )
    {
        printf("[%s]%s:%d. setsockopt error .\r\n", __FILE__, __FUNCTION__,  __LINE__  );
        return -1;
    }
    printf("[%s]%s:%d. Exit .\r\n", __FILE__, __FUNCTION__,  __LINE__  );
    
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
int create_task_recv_data()
{
    //
    pthread_t thread_id;
    pthread_attr_t attr;
    int ret = 0;
    
    memset(&attr, 0x0, sizeof(pthread_attr_t));
    
    pthread_attr_init(&attr); /*初始化线程属性*/
    
    ret = pthread_create(&thread_id, &attr, pTaskEntry, pParameter);
    if(0 != ret)
    {
        printf("Failed to create thread %s", pTaskName);
        return ret;
    }
    
    pthread_attr_destroy(&attr);

    return ret; 
}


vodi main()
{
    printf{"hello"};
}


/********************************** (C) *****END OF FILE**************************/