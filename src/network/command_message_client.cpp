//
// Created by Andrew Peterson on 11/2/23.
//

#include <network/command_message_client.h>
#include <state/application_state.h>

//std::string cert = "/home/pi/rehoboam/certs/client_cert.pem";
//std::string key = "/home/pi/rehoboam/certs/client_key.pem";
//std::string ca = "/home/pi/rehoboam/certs/ca_cert.pem";

std::string cert = "/Users/andrewpeterson/git/crypto/rehoboam/dev/v2/server_cert.pem";
std::string key = "/Users/andrewpeterson/git/crypto/rehoboam/dev/v2/server_key.pem";
std::string ca = "/Users/andrewpeterson/git/crypto/rehoboam/dev/v2/ca_cert.pem";


CommandMessageClient::CommandMessageClient()
    : SocketClient<MessageType>("localhost", 60000, cert, key, ca, CUBE)
{}

void CommandMessageClient::initialize() {
    this->Connect();
    this->HandleMessages();
}

void CommandMessageClient::processNextCommand(ApplicationState &state) {
    if (this->hasCommands()) {
        Command cmd = this->getNextCommand();
        state.applyCommand(cmd);
    }
}

bool CommandMessageClient::hasCommands() {
    return !this->m_commands.empty();
}

Command CommandMessageClient::getNextCommand() {
    return this->m_commands.pop_front();
}

void CommandMessageClient::OnMessageReceived(Message<Rehoboam::SocketLibrary::MessageType> &msg) {
    if (msg.header.id == MessageType::Success) return;

    if (msg.header.id == MessageType::ServerPing) {
        LOG(INFO, "Heartbeat has been acknowledge");
    } else if (msg.header.id == MessageType::ServerShutdown) {
        system("sudo shutdown -P now");
    } else if (!msg.body.empty()) {
        uint8_t value;
        msg >> value;
        this->m_commands.push_front({ msg.header.id, { value } });
    } else {
        this->m_commands.push_front({ msg.header.id, {} });
    }
}