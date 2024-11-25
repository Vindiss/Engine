#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP
#pragma once

#include "Socket.h"

#include <string>
#include <vector>

/**
 * @brief Represents a TCP socket for communication.
 */
class TCPSocket
{
public:
    /**
     * @brief Constructor.
     */
    TCPSocket();

    /**
     * @brief Destructor.
     */
    ~TCPSocket();

    /**
     * @brief Connect to a remote endpoint.
     * @param ipaddress The IP address of the remote endpoint.
     * @param port The port of the remote endpoint.
     * @return True if the connection is successful, false otherwise.
     */
    bool Connect(const std::string& ipaddress, unsigned short port);

    /**
     * @brief Send data over the socket.
     * @param data Pointer to the data to send.
     * @param len Length of the data to send.
     * @return True if the data is sent successfully, false otherwise.
     */
    bool Send(const unsigned char* data, unsigned short len);

    /**
     * @brief Receive data from the socket.
     * @param buffer The buffer to store the received data.
     * @return True if data is received successfully, false otherwise.
     */
    bool Receive(std::vector<unsigned char>& buffer);

private:
    SOCKET mSocket; /**< The underlying socket handle. */
};

#endif // TCPSOCKET_HPP
