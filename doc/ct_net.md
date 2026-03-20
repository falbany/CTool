# ct_net Module

The `ct_net` module provides a C++11 object-oriented interface for TCP networking. It abstracts platform-specific socket code (Winsock/POSIX) into manageable classes.

## Classes
- **`TcpClient`**: For connecting to and communicating with remote servers.
- **`TcpServer`**: For creating listeners that accept incoming `TcpClient` connections.

## API Reference

### `TcpClient` Methods
| Method                | Description                                 |
| :-------------------- | :------------------------------------------ |
| `connect(host, port)` | Establishes a connection. Returns `Status`. |
| `send(string)`        | Sends data to the host.                     |
| `receive(max_size)`   | Blocking receive of data.                   |
| `disconnect()`        | Closes the connection gracefully.           |

### `Status` Enums
- `Ok`: Success.
- `Error`: Socket-level failure.
- `Disconnected`: Remote host closed the connection.

### Server-Client Workflow
The `TcpServer::accept()` method is a blocking call. When a remote client connects:
1. It creates a new underlying system socket.
2. It wraps that socket into a `std::unique_ptr<ct::net::TcpClient>`.
3. You can then use that pointer to communicate specifically with that client.

```cpp
auto connectedClient = myServer.accept();
if (connectedClient) {
    connectedClient->send("Hello from Server!");
}
```
---

## Usage Examples

### Client: Connecting to a Server
```cpp
#include "ct_net.hpp"
#include <iostream>

ct::net::TcpClient client;

if (client.connect("127.0.0.1", 8080) == ct::net::Status::Ok) {
    client.send("GET /data HTTP/1.1\r\n\r\n");
    std::string response = client.receive(1024);
    std::cout << "Server said: " << response << std::endl;
}
```

### Server: Simple Listener
```cpp
#include "ct_net.hpp"

ct::net::TcpServer server;
server.listen(8080);

// Accept a client connection
auto client = server.accept();
if (client && client->isConnected()) {
    client->send("Welcome to CTool Server");
}
```

---

## 🛠 Platform Notes
- **Windows**: Automatically handles `WSAStartup` and `WSACleanup` via the constructor/destructor.
- **Linux**: Uses standard BSD sockets.
- **Firewall**: Ensure the selected ports are open in your OS firewall.
