//
// Created by delinoz on 19-5-22.
//

#include "debug.h"
#include "Front.h"
#include "StringUtils.h"
#include "IteratorHelper.h"
#include <cstdlib>
#include <iostream>

using std::cin;
using std::getline;

void CommandResolver::listenCommand(DimClient *client) {
    int len = 100;
    char line[100], testc;
    IpFinderResponseConsumer ipFinderResponseConsumer((char) PACKET_TYPE_IP_FINDER_RES);
    IpFinderRequestConsumer ipFinderRequestConsumer((char) PACKET_TYPE_IP_FINDER);
    TextMessageConsumer textMessageConsumer((char) PACKET_TYPE_TEXT);
    client->addConsumer(ipFinderResponseConsumer);
    client->addConsumer(ipFinderRequestConsumer);
    client->addConsumer(textMessageConsumer);
    while (1) {
        cin.getline(line, 100);
        //如何split
        vector<string> args;
        string input = line;
        StringUtils::regexSplit(input, args, "\\s+");
        //解析第一个参数
        string header = args.front();
        Command command(client);
        Action *action;
        ParameterResolver *resolver;
        command.setName((char *) header.data(), header.length());
        //根据参数new Action
        if (header.compare(MESSAGE_HEADER) == 0) {
            cout << "message header" << endl;
            action = new MessageAction;
            resolver = new MessageCommandParameterResolver;
            command.setAction(action);
            command.setResolver(resolver);
        } else if (header.compare(CONNECT_HEADER) == 0) {
            cout << "connect header" << endl;
            action = new ConnectAction;
            resolver = new ConnectCommandParameterResolver;
            command.setAction(action);
            command.setResolver(resolver);
        }
        //解析required和optional parameters，参数名和值存储在map中
        resolver->resolve(args, command);
        //Action获取参数进行操作

    }
}


void CommandResolver::resolveHeader(char *token, Command &command) {
    cout << "resolveHeader token:" << token << endl;
    if (strcmp(token, MESSAGE_HEADER) == 0) {
        command.setName(token, strlen(token));
    }
}

Command::Command(DimClient *client) {
    this->client = client;
}

const char *Command::getName() {
    return this->name;
}

void Command::setName(char *name, int len) {
    strncpy(this->name, name, len);
}

Action *Command::getAction() {
    return this->action;
}

int Command::getId() {
    return this->id;
}

void Command::setId(int id) {
    Command::id = id;
}

void Command::setAction(const Action *action) {
    Command::action = (Action *) action;
}

void Command::put(string name, string value) {
    //不用make_pair出错
    parameters.insert(make_pair(name, value));
}

string Command::get(string name) {
    std::map<string, string>::iterator iter;

    iter = parameters.find(name);
    if (iter == parameters.end()) {
        return nullptr;
    }
    return iter->second;
}

void Command::setResolver(const ParameterResolver *resolver) {
    Command::resolver = (ParameterResolver *) resolver;
}

DimClient *Command::getClient() const {
    return client;
}

const ParameterResolver *Command::getResolver() const {
    return resolver;
}

void Command::complete() {
    action->action(*this);
}

//msg
//-show
//-send -peer xx
void MessageCommandParameterResolver::resolve(vector<string> &args, Command &command) {
    vector<string>::iterator iterator = args.begin();
    string arg;
    IteratorHelper::pop(args, iterator, &arg);
    if (IteratorHelper::pop(args, iterator, &arg)) {
        if (arg.compare(MESSAGE_SHOW_FUNC) == 0) {

        } else if (arg.compare(MESSAGE_SEND_FUNC) == 0) {
            if (IteratorHelper::pop(args, iterator, &arg)) {
                if (arg.compare(MESSAGE_PEER_ARG) == 0) {
                    if (IteratorHelper::pop(args, iterator, &arg)) {
                        cout << " -peer " << arg << endl;
                        command.put("peer", arg);
                    } else {
                        perror("lack parameter of peer arg");
                        return;
                    }
                    command.complete();
                }
            }
        }
    }

    cout << "MessageCommandParameterResolver" << endl;
}

//connect -host ip
void ConnectCommandParameterResolver::resolve(vector<string> &args, Command &command) {
    vector<string>::iterator iterator = args.begin();
    string arg;
    IteratorHelper::pop(args, iterator, &arg);

    if (IteratorHelper::pop(args, iterator, &arg)) {
        if (arg.compare(CONNECT_HOST_ARG) == 0) {
            if (IteratorHelper::pop(args, iterator, &arg)) {
                cout << " -host " << arg << endl;
                command.put("host", arg);
            } else {
                perror("lack parameter of host arg");
                return;
            }
        }
    }

    if (IteratorHelper::pop(args, iterator, &arg)) {
        if (arg.compare(CONNECT_PORT_ARG) == 0) {
            if (IteratorHelper::pop(args, iterator, &arg)) {
                cout << " -port " << arg << endl;
                command.put("port", arg);
            } else {
                perror("lack parameter of port arg");
                return;
            }
        }
    }
    command.complete();
}

//action
void MessageAction::action(Command &command) {
    long peerHashID = StringUtils::toLong(command.get("peer"));
    DimTextMessage msg;
    msg.destHashId = peerHashID;
    msg.type = PACKET_TYPE_TEXT;
    string content = "sbbbbbbbbbb";
    msg.text = (char *) content.c_str();
    cout << "to:" << msg.destHashId << endl;
    cout << "content:" << msg.text << endl;
    if (command.getClient()->sendText(msg) > 0) {
        cout << "send success" << endl;
    } else {
        cout << "send failed" << endl;
    }
}

void ConnectAction::action(Command &command) {
    cout << "ConnectAction.action" << endl;
    int connectRes = command.getClient()->connectByIpAndPort((char *) command.get("host").c_str(),
                                                             StringUtils::toInt(command.get("port")));
    if (connectRes < 0) {
        perror("Connect Error!\n");
        return;
    }
}

/////////////////////////////
void IpFinderResponseConsumer::consume(DimPacket *packet, DimClient *client) {
    cout << "=====================IpFinderResponseConsumer" << endl;
    client->debugIpFinderPacket(packet);
    Peer peer;
    DimIpFinderContent *ipFinderContent = (DimIpFinderContent *) packet->content;
    strncpy(peer.ipv4, ipFinderContent->destIpv4, IPV4_ADDR_LENGTH);
    peer.hashId = ipFinderContent->destHashId;
    peer.startTime = DateTimeUtils::getCurrentTime();
    peer.port = ipFinderContent->destPort;
    client->addPeer(peer);
}

void IpFinderRequestConsumer::consume(DimPacket *packet, DimClient *client) {
    client->responseConnectByIpAndPort(*packet);
}

void TextMessageConsumer::consume(DimPacket *packet, DimClient *client) {
    cout << "from:" << packet->srcHashId << endl;
    cout << "content:" << packet->content << endl;
}