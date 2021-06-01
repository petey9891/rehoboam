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
    switch (msg.header.id) {
        case ServerPing:
            break;
        case ServerShutdown:           
            system("sudo shutdown -P now");
            break;
        case CubeDisplayOnOff:
            if (!this->power) {
                this->power = true;
                this->commands.push_front({ DisplayOn, nullptr, 0});
            } else {
                this->power = false;
                this->commands.push_front({ DisplayOff, nullptr, 0});
            }
            break;
        case CubeBrightness: {
            uint8_t value[1];
            msg >> value;

            this->commands.push_front({ Brightness, value, 1 });
            break;
        }
        case CubePulse:
            this->commands.push_front({ ColorPulseMode, nullptr, 0 });
            break;
        case CubeRehoboam:
            this->commands.push_front({ RehoboamMode, nullptr, 0 });
            break;
        case SetSolidColor: {
            // std::vector<uint8_t> rgb;
            uint8_t rgb[3];
            msg >> rgb;

            this->commands.push_front({ StaticColor, rgb, 3 });
            break;
        }
        case Success:
            break;
    }
}
