#include <SocketServer.h>

SocketServer::SocketServer(uint16_t port): RehoboamServer(port) {}

Command SocketServer::getNextCommand() {
    return this->commands.pop_front();
}

bool SocketServer::hasCommands() {
    return !this->commands.empty();
}

bool SocketServer::OnClientConnect(std::shared_ptr<connection<MessageType>> client) {
    return true;
}

void SocketServer::OnMessageRecieved(std::shared_ptr<connection<MessageType>> client, Message<MessageType>& msg) {
    switch (msg.header.id) {
        case ServerPing:
            // Simply bounce back the message
            client->Send(msg);
            break;
        case CubeDisplayOnOff:
            if (!this->power) {
                this->power = true;
                this->commands.push_front({ DisplayOn });
            } else {
                this->power = false;
                this->commands.push_front({ DisplayOff });
            }
            this->Acknowledge(client);
            break;
        case CubeBrightness:
            uint8_t value;
            msg >> value;

            this->commands.push_front({ Brightness, { value }});
            this->Acknowledge(client);
            break;
        case CubePulse:
            printf("The cube is pulsing\n");
            this->Acknowledge(client);
            break;
        case CubeRehoboam:
            printf("The cube is rehoboaming\n");
            this->Acknowledge(client);
            break;
        case Success:
            break;
    }
}

void SocketServer::Acknowledge(std::shared_ptr<connection<MessageType>> client) {
    Message<MessageType> res;
    res.header.id = Success;

    this->MessageClient(client, res);
}
