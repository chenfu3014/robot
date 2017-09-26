#pragma once
class socketClient
{
public:
	socketClient();
	~socketClient();

public:
    int initSocketForClient(const char  *serverIP, const int serverPort);
    int sendDataToServer(const char *sendBuf, const int bufSize);

private:
    void initResoure();
    void deinitResoure();
    int socketFD;

};

