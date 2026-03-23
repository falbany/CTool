/**
 * @file ct_net.hpp
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Object-oriented networking suite for TCP/UDP communication.
 * @version 1.1
 * @date 2026-03-20
 */

#ifndef CT_NET_HPP
#define CT_NET_HPP

#include <string>
#include <memory>

namespace ct {
    /**
     * @namespace ct::net
     * @brief High-level networking interfaces for C++11.
     */
    namespace net {

        /** @brief Error codes for networking operations. */
        enum class Status { Ok = 0, Error, Timeout, Disconnected };

        /**
         * @class TcpClient
         * @brief A synchronous TCP client for point-to-point communication.
         */
        class TcpClient {
          public:
            TcpClient();
            ~TcpClient();

            /** @brief Connects to a remote host. */
            Status connect(const std::string& host, int port);

            /** @brief Sends a string to the connected host. */
            Status send(const std::string& data);

            /** @brief Receives data with a blocking wait. */
            std::string receive(size_t max_len = 4096);

            /** @brief Checks if the client is currently connected. */
            bool isConnected() const { return _socket != -1; }

            void disconnect();

          private:
            friend class TcpServer;            // Allow TcpServer to access private constructor
            explicit TcpClient(int socket);    // Constructor for accepted connections
            int _socket;
        };

        /**
         * @class TcpServer
         * @brief A simple TCP listener that can accept incoming connections.
         */
        class TcpServer {
          public:
            TcpServer();
            ~TcpServer();

            /** @brief Starts listening on a specific port. */
            Status listen(int port);

            /** @brief Accepts the next incoming client. Blocks until a client connects. */
            std::unique_ptr<TcpClient> accept();

            void stop();

          private:
            int _server_sock;
        };

    }    // namespace net
}    // namespace ct

#endif    // CT_NET_HPP