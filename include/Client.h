#pragma once

#include <SocketServer/common.h>
#include <SocketServer/SocketClient.h>
#include <SocketServer/SocketConnection.h>
#include <Command.h>

// class Client {
// public:
//     Client();

// public:
//     Command getNextCommand();
//     bool hasCommands();

// protected:
//     void OnMessageRecieved();

// private: 
//     bool power = true;
// };


class Client: public SocketClient<MessageType> {
public:
    Client();

public:
    Command getNextCommand();
    bool hasCommands();

protected:
    void OnMessageRecieved(Message<MessageType>& msg) override;

private: 
    bool power = true;

    // Thread safe queue for commands
    tsqueue<Command> commands;
};
