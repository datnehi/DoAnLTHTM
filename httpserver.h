#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "tcpserver.h"
#include "httpsession.h"
#include "httpserverconfig.h"

class SMTPServer: public TCPServer
{

public:
    HTTPServer(unsigned short port=1708);
    ~HTTPServer();
    bool configServer();
protected:

    void cleanServer();
    SMTPServerConfig* getServerConfig() {return (SMTPServerConfig*)conf;}
    bool loadServerConfig(const string& confFileName);
    void startNewSession(TcpSocket slave);
    void initCmd();
private:
    int readCmd(TcpSocket& slave, string& cmdLine);
    unsigned short parseCmd(const string& cmdLine, string cmd_argv[], int& cmd_argc);
protected:
};

#endif // HTTPSERVER_H
