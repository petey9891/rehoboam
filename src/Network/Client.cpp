#include <Client.h>
#include <string>

std::string cert = "/home/pi/rehoboam/certs/client.pem";
std::string key = "/home/pi/rehoboam/certs/client.key";
std::string ca = "/home/pi/rehoboam/certs/ca.pem";

Client::Client(): SocketClient("www.rehoboamcube.com", 60000, cert, key, ca) {}

Command Client::getNextCommand() {
    return this->commands.pop_front();
}

bool Client::hasCommands() {
    return !this->commands.empty();
}

void Client::OnMessageRecieved(Message<MessageType>& msg) {
void Client::OnMessageRecieved() {
    switch (msg.header.id) {
        case ServerPing:
            break;
        case ServerShutdown:           
            system("sudo shutdown -P now");
            break;
        case CubeDisplayOnOff:
            if (!this->power) {
                this->power = true;
                // this->commands.push_front({ DisplayOn, 0, nullptr});
            } else {
                this->power = false;
                // this->commands.push_front({ DisplayOff, 0, nullptr});
            }
            break;
        case CubeBrightness: {
            // uint8_t value[1];
            // msg >> value;

            // this->commands.push_front({ Brightness, 1, value });
            break;
        }
        case CubePulse:
            // this->commands.push_front({ ColorPulseMode, 0, nullptr });
            break;
        case CubeRehoboam:
            // this->commands.push_front({ RehoboamMode, 0, nullptr });
            break;
        case SetSolidColor: {
            // std::vector<uint8_t> rgb;
            uint8_t rgb[3] = { 0, 0, 0 };
            msg >> rgb;
            uint8_t length = 3;
            Command _cmd = { StaticColor, length, rgb };
            this->commands.push_front(_cmd);
            break;
        }
        case Success:
            break;
    }
}
