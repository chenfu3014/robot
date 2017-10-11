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



#define MAX_READ_BUF            10240
#define MAX_PHP_STORE_DATA      20480


char gRecvBufStoreData[MAX_READ_BUF] = {0};
char gRecvBufStoreDataUTF8[MAX_READ_BUF] = {0};

char gPHPWriteData[MAX_PHP_STORE_DATA] = {0};
char gPHPDataHead[128] = {0};
char gPHPDataTrail[32] = {0};
char gPHPDataBody[MAX_PHP_STORE_DATA] = {0};


int gIsRecvDataFlag = 0;

char gBufStart[] = {"六码两期"};
char gBufStop[] = {"对错勿怪"};
char gBufStartUtf8[32] = {0};
char gBufStopUtf8[32] = {0};
char gBufBreakLine[] = {"\r\n"};
char gBufBreakLineUtf8[16] = {0};


/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void convertStartFlagToUtf8()
{
    char *pIn = gBufStart;
    char *pOut = gBufStartUtf8;   
    memset(gBufStartUtf8, 0, 32);
    size_t iInLen = strlen(gBufStart);                                
    size_t iOutLen = 32; 

    iconv_t hIconv = iconv_open("UTF-8", "GBK");  
    if( 0 == hIconv )
    {   
        printf("start-flag: iconv_open error.\n" );
        return ;
    }

    printf("start-flag:iconv_open hIconv:%d,iInLen:%d.\n", hIconv, iInLen );
    size_t iRet = iconv(hIconv, (&pIn), &iInLen, &pOut, &iOutLen); 
    printf("start-flag:iconv iRet:%d.\n", iRet );
    if( -1 != iRet )
    {
        printf("start-flag:iconv data successfull.len:%d,gBufStartUtf8:%s.\n", strlen(gBufStartUtf8), gBufStartUtf8);
    }           
    else
    {
        printf("start-flag:iconv data failed....\n");                                        
    }
    iconv_close(hIconv);    
}






/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void convertStopFlagToUtf8()
{
    char *pIn = gBufStop;
    char *pOut = gBufStopUtf8;   
    memset(gBufStopUtf8, 0, 32);
    size_t iInLen = strlen(gBufStop);                                
    size_t iOutLen = 32; 

    iconv_t hIconv = iconv_open("UTF-8", "GBK");  
    if( 0 == hIconv )
    {   
        printf("stop-flag: iconv_open error.\n" );
        return ;
    }

    printf("stop-flag:iconv_open hIconv:%d,iInLen:%d.\n", hIconv, iInLen );
    size_t iRet = iconv(hIconv, (&pIn), &iInLen, &pOut, &iOutLen); 
    printf("stop-flag:iconv iRet:%d.\n", iRet );
    if( -1 != iRet )
    {
        printf("stop-flag:iconv data successfull.len:%d, gBufStopUtf8:%s.\n", strlen(gBufStopUtf8),  gBufStopUtf8);
    }           
    else
    {
        printf("stop-flag:iconv data failed....\n");                                        
    }
    iconv_close(hIconv);    
}





/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void convertBreakLineToUtf8()
{
    #if 0
    char *pIn = gBufBreakLine;
    char *pOut = gBufBreakLineUtf8;   
    memset(gBufBreakLineUtf8, 0, 16);
    size_t iInLen = strlen(gBufBreakLine);                                
    size_t iOutLen = 16; 

    iconv_t hIconv = iconv_open("UTF-8", "GBK");  
    if( 0 == hIconv )
    {   
        printf("breakline-flag: iconv_open error.\n" );
        return ;
    }

    printf("breakline-flag:iconv_open hIconv:%d,iInLen:%d.\n", hIconv, iInLen );
    size_t iRet = iconv(hIconv, (&pIn), &iInLen, &pOut, &iOutLen); 
    printf("breakline-flag:iconv iRet:%d.\n", iRet );
    if( -1 != iRet )
    {
        printf("breakline-flag:iconv data successfull.len:%d,gBufStartUtf8:%s.\n", strlen(gBufBreakLineUtf8), gBufBreakLineUtf8);
    }           
    else
    {
        printf("breakline-flag:iconv data failed....\n");                                        
    }
    iconv_close(hIconv);    
    #else
    memset(gBufBreakLineUtf8, 0, 16);
    memcpy(gBufBreakLineUtf8, "\n", 1);
    #endif
}






/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
int saveToFile(  const char *dataBuf, int dataSize )
{
    //
    FILE *fp = NULL;
    fp = fopen( "test.php", "a+");
    if( NULL ==  fp )
    {
        return -1;
    }

    fwrite( dataBuf, dataSize, 1, fp );
    //fflush(fp);
    fclose(fp);
    fp = NULL;
    
}


/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void combineThePHPHead()
{
    sprintf(gPHPDataHead, "%s", "<!DOCTYPE html>\r\n\
<html lang=\"en\">\r\n\
<head>\r\n\
    <meta charset=\"UTF-8\">\r\n\
    <title>Title</title>\r\n\
</head>\r\n\
<body>\r\n\
<?php\r\n");


    printf("%s", gPHPDataHead);
}




/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void combineThePHPTrail()
{
        sprintf(gPHPDataTrail, "%s", "?>\r\n\
</body>\r\n\
</html>\r\n");
    
    
        printf("%s", gPHPDataTrail);

}




/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void combineThePHPAllData()
{
    printf("combineThePHPAllData enter.\r\n");
    combineThePHPHead();
    strcat(gPHPWriteData, gPHPDataHead);

    // 
    strcat(gPHPWriteData, gPHPDataBody);

    combineThePHPTrail();
    strcat(gPHPWriteData, gPHPDataTrail);

    printf("\r\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\r\n");
    //printf("\r\n%s\r\n", gPHPWriteData);
    saveToFile( gPHPWriteData, strlen(gPHPWriteData) );
    printf("\r\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\r\n");
}



/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
int formatEachLineDataToBuf( const char *eachLineBuf, int eachLineLen )
{
    printf("formatEachLineDataToBuf enter.\r\n");
    char eachPHPLine[64] = {0};   //   echo '125-126期 3-8名【04】 挂(2)<br>';
    char eachDataLine[64] = {0};   //  125-126期 3-8名【04】 挂(2)
    memcpy( eachDataLine, eachLineBuf, eachLineLen );
    printf("eachDataLine(%d)==>>\r\n%s\r\n<<==\r\n", eachLineLen, eachDataLine);


    // combine body data.
    //  echo '六码两期<br>';
    sprintf(eachPHPLine, "echo '%s<br>';",eachDataLine );

    int phpLen = (int)strlen(eachPHPLine);
    printf("eachPHPLine(%d)==>>\r\n%s\r\n<<==\r\n", phpLen, eachPHPLine);
    strcat(gPHPDataBody, eachPHPLine);
    strcat(gPHPDataBody, "\n");   //  linux is just \n


    printf("formatEachLineDataToBuf exit.\r\n");
    return 0;
}














/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
#if 0
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <title>Title</title>
                    </head>
                    <body>
                    <?php
                        echo '六码两期<br>';
                        echo '------------------------------<br>';
                        echo '125-126期 3-8名【04】 挂(2)<br>';
                        echo '127-128期 3-8名【07】 中(1)<br>';
                        echo '128-129期 3-8名【01】 中(1)<br>';
                        echo '129-130期 3-8名【02】 中(1)<br>';
                        echo '130-131期 3-8名【02】 中(2)<br>';
                        echo '132-133期 3-8名【09】 中(1)<br>';
                        echo '133-134期 3-8名【02】 中(2)<br>';
                        echo '135-136期 3-8名【03】 中(1)<br>';
                        echo '136-137期 3-8名【06】 中(2)<br>';
                        echo '138-139期 3-8名【05】 等开(1)<br>';
                        echo '------------------------------<br>';
                        echo '仅供参考   对错勿怪<br>';
                    ?>
                    </body>
                    </html>
#endif












* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
#if 0
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <title>Title</title>
                    </head>
                    <body>
                    <?php
                        echo '六码两期<br>
                              ------------------------------<br>
                              127-128期 3-8名【07】 中(1)<br>
                              128-129期 3-8名【01】 中(1)<br>
                              129-130期 3-8名【02】 中(1)<br>
                              130-131期 3-8名【02】 中(2)<br>
                              132-133期 3-8名【09】 中(1)<br>
                              133-134期 3-8名【02】 中(2)<br>
                              135-136期 3-8名【03】 中(1)<br>
                              136-137期 3-8名【06】 中(2)<br>
                              138-139期 3-8名【05】 中(1)<br>
                              139-140期 3-8名【09】 等开(2)<br>
                              ------------------------------<br>
                              仅供参考   对错勿怪<br>';
                    ?>
                    </body>
                    </html>
#endif

***************************************************************/
int formatEachRecordDataToBuf( const char *data, const int dataLen )
{
    if( NULL == data || 0 == dataLen )
    {
        return -1;
    }
    char eachRecordBuf[1024] = {"0"};
    memcpy( eachRecordBuf, data, dataLen );
    printf("eachRecordData(%d)\r\n+++++++++>>>\r\n%s\r\n<<<+++++++++\r\n", dataLen, eachRecordBuf);

    char *pStartPoint = NULL;
    char *pEndPoint = NULL;
    char *pCurPoint = (char *)eachRecordBuf;
    
    while( pCurPoint - eachRecordBuf <= dataLen)
    {
        printf("formatEachRecordDataToBuf:(%d),(%d).\r\n", pCurPoint - eachRecordBuf , dataLen);
        
        pStartPoint = pCurPoint;
        pEndPoint = strstr( pCurPoint, gBufBreakLineUtf8 );   // 指向   \r\n  中的  \r  .....       linux is just \n
        if( NULL == pEndPoint )
        {
            printf("formatEachRecordDataToBuf break.\r\n");
            break;
        }

        pCurPoint = pEndPoint + 1;    // 指向   \r\n  的下个字符  .....          linux is just \n

        int eachLineDataLen = pEndPoint-pStartPoint;
        formatEachLineDataToBuf(pStartPoint,eachLineDataLen);  // data without \r\n
    }

    printf("formatEachRecordDataToBuf exit.\r\n");
    return 0;
}











/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
                        2017-09-27 16:46:41 正在连接服务器：123.151.77.183
                        2017-09-27 16:46:46 成功连接到服务器
                        2017-09-27 16:46:46 下载QQ列表
                        2017-09-27 16:46:46 登陆成功!
                        2017-09-27 16:46:47 协议版本：QQ_8_9
                        2017-09-27 16:46:55 发送队列启动完成
                        2017-09-27 16:50:45 拖泥(2353617310) 说：    六码两期
                        六码两期
                        ------------------------------
                        125-126期 3-8名【04】 挂(2)
                        127-128期 3-8名【07】 中(1)
                        128-129期 3-8名【01】 中(1)
                        129-130期 3-8名【02】 中(1)
                        130-131期 3-8名【02】 中(2)
                        132-133期 3-8名【09】 中(1)
                        133-134期 3-8名【02】 中(2)
                        135-136期 3-8名【03】 中(1)
                        136-137期 3-8名【06】 中(2)
                        138-139期 3-8名【05】 等开(1)
                        ------------------------------
                        仅供参考   对错勿怪
                        六码两期
                        ------------------------------
                        125-126期 3-8名【04】 挂(2)
                        127-128期 3-8名【07】 中(1)
                        128-129期 3-8名【01】 中(1)
                        129-130期 3-8名【02】 中(1)
                        130-131期 3-8名【02】 中(2)
                        132-133期 3-8名【09】 中(1)
                        133-134期 3-8名【02】 中(2)
                        135-136期 3-8名【03】 中(1)
                        136-137期 3-8名【06】 中(2)
                        138-139期 3-8名【05】 等开(1)
                        ------------------------------
                        仅供参考   对错勿怪
***************************************************************/
int splitSegementData( /*const*/ char *dataBuf, /*const*/ int dataLen )
{    
    printf("splitSegementData enter.\r\n");
    if( NULL == dataBuf || 0 == dataLen )
    {    
        return -1;
    }
    char *pStartPoint = NULL;  // 指向   六码两期    的开始
    char *pEndPoint = NULL;    // 指向    对错勿怪    的末尾 
    char *pCurPoint = (char *)dataBuf;
    
    while( pCurPoint-dataBuf < dataLen )
    {        
        printf("splitSegementData %d:%d.\r\n", pCurPoint-dataBuf, dataLen);
        pStartPoint = strstr(pCurPoint, gBufStartUtf8);
        if( NULL == pStartPoint )
        {
            printf("splitSegementData pStartPoint is null, so break.\r\n");
            break;
        }

        pEndPoint = strstr(pCurPoint, gBufStopUtf8);
        if( NULL == pEndPoint )
        {
            printf("splitSegementData pEndPoint is null, so break.\r\n");        
            break;
        }    

        int eachDataLen = (pEndPoint + 14) - pStartPoint;           // modifiy from 8 to 10 bls include \r\n            4*3=12;   12+2=14
        formatEachRecordDataToBuf( pStartPoint, eachDataLen );
        pCurPoint = pEndPoint;
        pCurPoint = pCurPoint + 12;   // 4 * 3
    }
    
    //
    combineThePHPAllData();
    
    printf("splitSegementData exit.\r\n");
    return 0; // success.
}
















/****************************************************************
* @function name      : 
* @brief              : 
 * @input param        : 
* @output param       : 
* @retval              : 
* @author              : 
 * @date               : 
* @others              :
***************************************************************/
void *stroeDataFunProc(void *arg)
{
    printf("stroeDataFunProc enter.\r\n");
    while(1)
    {
        if( 0 == gIsRecvDataFlag )
        {            
            printf("stroeDataFunProc no data.\r\n");
            sleep(2);
            continue;
        }
        else
        {
            printf("==>gRecvBufStoreData(%d):\r\n%s. \r\n", (int)strlen(gRecvBufStoreData),  gRecvBufStoreData );
            splitSegementData(gRecvBufStoreData, (int)strlen(gRecvBufStoreData));                        
        }
        
        //clean gRecvBufStoreData and data flag.
        memset( gRecvBufStoreData, 0, MAX_READ_BUF );   
        gIsRecvDataFlag = 0;
        sleep(3);
    }
    return ((void *)0);
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
int createTaskStoreData()
{
    //
    // for test.
    //combineThePHPHead();
    //combineThePHPTrail();
    //splitSegementData( (char *)"abc", strlen("abc"));
    //
    
    int ret;
    int errNum = 0;
    pthread_t writeXmlId;    
    errNum = pthread_create(&writeXmlId, NULL, stroeDataFunProc, NULL);
    if (errNum != 0)
    {
        printf("createTaskSaveXMLToLocal failed: %s\n", strerror(errNum));
    }
    else
    {
        printf("createTaskSaveXMLToLocal successfully: %s\n", strerror(errNum));
        convertStartFlagToUtf8();
        convertStopFlagToUtf8();
        convertBreakLineToUtf8();
    }
    return ret; 
}


/********************************** (C) *****END OF FILE**************************/
