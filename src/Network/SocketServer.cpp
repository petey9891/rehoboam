#include <SocketServer.h>

SocketServer::SocketServer(uint16_t port): RehoboamServer(port) {}

bool SocketServer::OnClientConnect(std::shared_ptr<connection<MessageType> > client) {
    return true;
}

void SocketServer::OnMessageRecieved(std::shared_ptr<connection<MessageType> > client, Message<MessageType>& msg) {
    switch (msg.header.id) {
        case ServerPing:
            // Simply bounce back the message
            client->Send(msg);
            break;
        case CubeDisplayOnOff:
            if (!power) {
                power = true;
                printf("The power is on\n");
            } else {
                power = false;
                printf("The power is off\n");
            }
            this->Acknowledge(client);
            break;
        case CubeBrightness:
            uint8_t value;
            msg >> value;
            printf("Brightness: %d\n", value);
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

void SocketServer::Acknowledge(std::shared_ptr<connection<MessageType> > client) {
    Message<MessageType> res;
    res.header.id = Success;

    this->MessageClient(client, res);
}
