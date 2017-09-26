#pragma once
class dataFile
{
public:
    dataFile();
    ~dataFile();

public:
    int openFile(const char *filePath);
    int readData(char *destBuf, int readLen);
    int closeFile();

private:
    int m_fileHandle;
};

