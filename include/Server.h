#pragma once

#include <rehoboam-server/common.h>
#include <rehoboam-server/SocketServer.h>
#include <rehoboam-server/connection.h>
#include <Command.h>

class Server: public SocketServer<MessageType> {
public:
    Server(uint16_t port);

public:
    Command getNextCommand();
    bool hasCommands();

protected:
    bool OnClientConnect(std::shared_ptr<connection<MessageType>> client) override;
    void OnMessageRecieved(std::shared_ptr<connection<MessageType>> client, Message<MessageType>& msg) override;

private:
    void Acknowledge(std::shared_ptr<connection<MessageType>> client);

private: 
    bool power = true;

    // Thread safe queue for commands
    tsqueue<Command> commands;
};