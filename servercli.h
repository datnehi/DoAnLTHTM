#ifndef SERVERCLI_H
#define SERVERCLI_H

#include "cli.h"
#include "httpserver.h"
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

class ServerCLI : public CmdLineInterfac
{
private:
    HTTPServer* http;
public:
    ServerCLI();
    ~ServerCLI();
protected:
    virtual void initCmd();
private:
    void doStart(string cmd_argv[], int cmd_argc);
    void doStop(string cmd_argv[], int cmd_argc);
    void doStatus(string cmd_argv[], int cmd_argc);
    void doRestart(string cmd_argv[], int cmd_argc);
    void doHelp(string cmd_argv[], int cmd_argc);
};

#endif // SERVERCLI_H
