#include <Client.h>
#include <string>

std::string cert = "/home/pi/rehoboam/certs/client.pem";
std::string key = "/home/pi/rehoboam/certs/client.key";
std::string ca = "/home/pi/rehoboam/certs/ca.pem";

Client::Client(): SocketClient("www.rehoboamcube.com", 60000, cert, key, ca, CUBE) {}

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
                this->commands.push_front({ DisplayOn });
            } else {
                this->power = false;
                this->commands.push_front({ DisplayOff });
            }
            break;
        case CubeBrightness: {
            uint8_t value;
            msg >> value;

            this->commands.push_front({ Brightness, { value } });
            break;
        }
        case CubePulse:
            this->commands.push_front({ ColorPulseMode });
            break;
        case CubeRehoboam:
            this->commands.push_front({ RehoboamMode });
            break;
        case SetSolidColor: {
            uint8_t rgb[3];
            msg >> rgb;
            std::vector<uint8_t> _rgb(&rgb[0], &rgb[3]);
            
            this->commands.push_front({ StaticColor, _rgb });
            break;
        }
        case CubeChristmas:
            this->commands.push_front({ ChristmasMode });
        case Success:
            break;
    }
}
