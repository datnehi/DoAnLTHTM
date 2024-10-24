#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "tcpserver.h"
#include <string>

class HttpTcpServer : public TCPServer
{
public:
    HttpTcpServer(unsigned short port = 80);
    ~HttpTcpServer();

protected:
    void startNewSession(TcpSocket slave);
    int processHttpRequest(TcpSocket& slave);

    // Các hàm xử lý phương thức HTTP
    void handlePostRequest(TcpSocket& slave, const std::string& request);
    void handlePutRequest(TcpSocket& slave, const std::string& request);
    void handleUnknownRequest(TcpSocket& slave);
};

#endif // _HTTP_SERVER_H_
