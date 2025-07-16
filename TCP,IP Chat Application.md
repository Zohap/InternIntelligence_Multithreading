# 🧵 Simple TCP/IP Chat Application (C++)

This repository contains a simple **TCP/IP chat application** implemented in C++ using POSIX sockets and multithreading. The application consists of two programs:

1. **server.cpp**: A multi-client chat server that accepts connections, broadcasts messages from clients, and allows server-side admin messages.  
2. **client.cpp**: A chat client that connects to the server, sends user messages, and displays incoming messages from other clients or the server.

## 🚀 Features

- Handles multiple clients concurrently using `std::thread`.  
- Broadcasts messages from any client to all other connected clients.  
- Supports server-side admin messages (type in server terminal).  
- Graceful handling of client disconnections.  
- Synchronized console output to prevent message interleaving.

## 🛠 Technologies

- C++17 or later  
- POSIX sockets (`<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`)  
- Multithreading (`<thread>`, `<mutex>`, `<condition_variable>`)  

## 📦 Project Structure

```
/tcp_chat/
├── server.cpp    # Chat server with admin input feature
├── client.cpp    # Chat client
└── README.md     # This file
```

## 🎯 Usage

1. **Compile** the server and client:

   ```bash
   g++ server.cpp -o server -pthread
   g++ client.cpp -o client -pthread
   ```

2. **Run the server** in its own terminal:

   ```bash
   ./server
   ```

   - The server will start listening on port `8080`.  
   - You can type admin messages directly into the server terminal to broadcast to all clients.  
   - Type `exit` and press Enter in the server terminal to stop sending admin messages (server continues running).

3. **Run one or more clients** in separate terminals:

   ```bash
   ./client
   ```

   - After connecting, type messages and press Enter to send.  
   - Type `exit` and press Enter to disconnect the client.

## 🔧 Example

- **Server** terminal:

  ```
  🟢 Server is running on port 8080...
  Hello from admin!
  ```

- **Client A** terminal:

  ```
  ✅ Connected to server!
  [Server]: Hello from admin!
  Hi everyone!
  ```

- **Client B** terminal:

  ```
  ✅ Connected to server!
  [Server]: Hello from admin!
  [Client 5]: Hi everyone!
  ```

## 📌 Notes

- Ensure the server is running **before** starting any clients.  
- You can change the listening port by modifying `#define PORT` in both `server.cpp` and `client.cpp`.  
- This application is intended for educational purposes and basic local testing.

## 📄 License

This project is licensed under the MIT License.
