//
// Created by delinoz on 19-5-22.
//

#ifndef DIM_FRONT_H
#define DIM_FRONT_H

#include <map>
#include <string>
#include <vector>
#include "DimClient.h"

#define MESSAGE_HEADER        "msg"
#define MESSAGE_SHOW_FUNC     "-show"
#define MESSAGE_SEND_FUNC     "-send"
#define MESSAGE_PEER_ARG      "-peer"
#define MESSAGE_PEER_ALL_ARG  "-all"
#define CONNECT_HEADER        "connect"
#define CONNECT_HOST_ARG      "-host"
#define CONNECT_PORT_ARG      "-port"

#define IPFINDER_RESPONSE_CONSUMER_ID 1
using std::map;
using std::string;
using std::vector;

//Process Response
class IpFinderResponseConsumer : public ResponseConsumer {
public:
    IpFinderResponseConsumer(long id) : ResponseConsumer(id) {}

    void consume(DimPacket *packet, DimClient *client);
};

class IpFinderRequestConsumer : public ResponseConsumer {
public:
    IpFinderRequestConsumer(long id) : ResponseConsumer(id) {}

    void consume(DimPacket *packet, DimClient *client);
};

class TextMessageConsumer : public ResponseConsumer {
public:
    TextMessageConsumer(long id) : ResponseConsumer(id) {}

    void consume(DimPacket *packet, DimClient *client);
};
class Command;

class CommandResolver {
public:
    void listenCommand(DimClient *client);

    void resolveHeader(char *token, Command &command);
};

//对‘typeinfo for Action’未定义的引用
class Action {
public:
    virtual void action(Command &command) = 0;
};

class MessageAction : public Action {
    void action(Command &command);
};

class ConnectAction : public Action {
    void action(Command &command);
};

/**
 * 参数解析器
 */
class ParameterResolver {
public:
    virtual void resolve(vector<string> &args, Command &command) = 0;
};

class MessageCommandParameterResolver : public ParameterResolver {
public:
    void resolve(vector<string> &args, Command &command);
};

class ConnectCommandParameterResolver : public ParameterResolver {
public:
    void resolve(vector<string> &args, Command &command);
};

class Command {
protected:
    int id;       //命令id
    char name[20];//命令名称
    map<string, string> parameters;
    Action *action;
    //field has incomplete type
    //前向声明的结构体或者类只能用来定义指针或者引用
    ParameterResolver *resolver;
    DimClient *client;
public:
    Command(DimClient *client);

    const char *getName();

    Action *getAction();

    int getId();

    void setId(int id);

    void setAction(const Action *action);

    void setName(char *name, int len);

    void put(string name, string value);

    string get(string name);

    const ParameterResolver *getResolver() const;

    void setResolver(const ParameterResolver *resolver);

    DimClient *getClient() const;

    void complete();
};

#endif //DIM_FRONT_H
