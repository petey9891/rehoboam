#include <Server.h>

Server::Server(uint16_t port): SocketServer(port, "/home/pi/rehoboam/certs/server.pem", "/home/pi/rehoboam/certs/server-key.pem") {}

Command Server::getNextCommand() {
    return this->commands.pop_front();
}

bool Server::hasCommands() {
    return !this->commands.empty();
}

bool Server::OnClientConnect(std::shared_ptr<connection<MessageType>> client) {
    return true;
}

void Server::OnMessageRecieved(std::shared_ptr<connection<MessageType>> client, Message<MessageType>& msg) {
    switch (msg.header.id) {
        case ServerPing:
            // Simply bounce back the message
            client->Send(msg);
            break;
        case ServerShutdown:           
            this->Acknowledge(client);
            system("sudo shutdown -P now");
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

void Server::Acknowledge(std::shared_ptr<connection<MessageType>> client) {
    Message<MessageType> res;
    res.header.id = Success;

    this->MessageClient(client, res);
}
