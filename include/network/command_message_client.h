//
// Created by Andrew Peterson on 11/2/23.
//

#pragma once

#define RSL_LOG_LEVEL INFO

#include <Rehoboam/SocketLibrary/common.h>
#include <Rehoboam/SocketLibrary/SocketClient.h>

#include "command.h"

// Forward declare
class ApplicationState;

using namespace Rehoboam::SocketLibrary;

/**
 * @class CommandMessageClient
 *
 * @brief A class that is able to handle connections and communicate over SSL/TLS.
 */
class CommandMessageClient: private SocketClient<MessageType> {
public:
    /**
     * @brief Constructor for SocketClient.
     */
    CommandMessageClient();

    /**
     * @brief Initialize server connection and start message thread
     */
    void initialize();

    /**
     * @brief Process the next incoming command and update application state
     *
     * @param state The application state
     */
    void processNextCommand(ApplicationState& state);

protected:
    /**
     * @brief Override function to handle incoming messages.
     *
     * @param msg The incoming message.
     */
    void OnMessageReceived(Message<MessageType>& msg) override;

private:
    /**
     * @brief Determine if there are any unprocessed commands
     *
     * @return If any commands have been received and not yet processed
     */
    bool hasCommands();

    /**
     * @brief Retrieve the next command
     *
     * @return The first command to have been received, FIFO queue.
     */
    Command getNextCommand();


    // Thread safe queue for commands
    tsqueue<Command> m_commands;
};