#include "stdafx.h"
#include "dataFile.h"
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/locking.h>  
#include <share.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <io.h> 


dataFile::dataFile()
{
}


dataFile::~dataFile()
{
}


// 0 is success.
int dataFile::openFile(const char *filePath)
{
    errno_t err = _sopen_s(&m_fileHandle, filePath, _O_RDONLY, _SH_DENYNO,_S_IREAD | _S_IWRITE);
    printf("%d %d\n", err, m_fileHandle);
    if (err != 0)
    {
        exit(1);
    }
    return 0;
}


int dataFile::closeFile()
{
    return 0;
}


int dataFile::readData(char *destBuf, int readLen)
{
    int  retReadLen = 0;
    long lseek_ret;
    memset(destBuf, 0, readLen);
    printf("No one can change these bytes while I'm reading them\n");
    retReadLen = _read(m_fileHandle, destBuf, readLen);
    printf("%d bytes read: %.30s\n", readLen, destBuf);
    lseek_ret = _lseek(m_fileHandle, 0L, SEEK_SET);
    
    return retReadLen;
}