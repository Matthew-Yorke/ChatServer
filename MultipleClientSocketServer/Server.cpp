//*********************************************************************************************************************
//
// File Name; Server.cpp
//
// Description:
//    This object handles the overall TCP server functionality. It allows the set up and destruction of the server. The
//    main functionality is to listen for any client connection and use multithreading to handle individual message
//    handling of client commands and messages.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 07/31/2019 | The basic functionality of the server are available. It will allow
//                           |            | listening for client connections and receiving chat messages to be
//                           |            | broadcasted to all connected clients. Functionality will be expanded in the
//                           |            | future.
//
//*********************************************************************************************************************

#include "Server.h"
#include <iostream>

//*********************************************************************************************************************
//
// Method: Server
//
// Description:
//    The constructor of the server object that kicks off the initialization of the server by setting up the listener
//    socket.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
Server::Server()
{
   mpDatabase = new Database();

   InitializeWindowsSocket();
   CreateListenerSocket();
   BindListenerSocket();
   ListenForConnections();
}

//*********************************************************************************************************************
//
// Method: ~Server
//
// Description:
//    Destructor of the Server object that will join all threads and then close the listener socket, finally cleaning
//    up windows sockets.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
Server::~Server()
{
   for(auto iter = mClientThreads.begin(); iter != mClientThreads.end(); ++iter)
   {
      (*iter)->join();
   }

   closesocket(mListeningSocket);

   WSACleanup();
}

//*********************************************************************************************************************
//
// Method: InitializeWindowsSocket
//
// Description:
//    Initialize the window socket library.
//
// Arguments:
//    N/A
//
// Return:
//    True  - Initialization successful.
//    False - Initialization failed.
//
//*********************************************************************************************************************
bool Server::InitializeWindowsSocket()
{
   WSADATA wsData;
   WORD version = MAKEWORD(2, 2);

   int winSocketOk = WSAStartup(version, &wsData);

   if (winSocketOk != 0)
   {
      std::cerr << "Can't initialize winsock. Quitting." << std::endl;
      return false;
   }

   return true;
}

//*********************************************************************************************************************
//
// Method: CreateListenerSocket
//
// Description:
//    Create the socket used for listening for connections.
//
// Arguments:
//    N/A
//
// Return:
//    True  - Socket creation successful.
//    False - Socket creation failed.
//
//*********************************************************************************************************************
bool Server::CreateListenerSocket()
{
   mListeningSocket = socket(AF_INET, SOCK_STREAM, 0);
   if (mListeningSocket == INVALID_SOCKET)
   {
      std::cerr << "Can't create socket. Quitting." << std::endl;
      return false;
   }

   return true;
}

//*********************************************************************************************************************
//
// Method: BindListenerSocket
//
// Description:
//    Binds the socket to the server's IP address and the designated port number.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::BindListenerSocket()
{
   sockaddr_in hint;
   hint.sin_family = AF_INET;
   hint.sin_port = htons(54000);
   hint.sin_addr.S_un.S_addr = INADDR_ANY;

   bind(mListeningSocket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));

   listen(mListeningSocket, SOMAXCONN);
}

//*********************************************************************************************************************
//
// Method: ListenForConnections
//
// Description:
//    Continuously listen for a connection. If a connection comes in, create a new thread to handle the connection
//    messaging.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::ListenForConnections()
{
   sockaddr_in client;
   int clientSize = sizeof(client);
   for(;;)
   {
      SOCKET clientSocket = accept(mListeningSocket, reinterpret_cast<sockaddr*>(&client), &clientSize);
      if (clientSocket == INVALID_SOCKET)
      {
         std::cerr << "Client socket is invalid." << std::endl;
      }
      else
      {
         char host[NI_MAXHOST];      // Client's remote name.
         char service[NI_MAXHOST];   // Service (i.e. port) client is connected on.

         ZeroMemory(host, NI_MAXHOST);
         ZeroMemory(service, NI_MAXHOST);

         if (getnameinfo(reinterpret_cast<sockaddr*>(&client), sizeof(client), host, NI_MAXHOST, service, NI_MAXHOST, 0) == 0)
         {
            std::cout << host << " connected on port: " << service << std::endl;
         }
         else
         {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
            std::cout << host << " connected on port: " << ntohs(client.sin_port) << std::endl;
         }

         mMutex.lock();
         mConnectedClients.push_back(clientSocket);
         mMutex.unlock();
         mClientThreads.emplace_back(new std::thread(&Server::HandleClient, this, clientSocket));
      }
   }
}

//*********************************************************************************************************************
//
// Method: HandleClient
//
// Description:
//    Thread function to handle receiving messages and then calling to broadcast the message back out to the clients.
//
// Arguments:
//    theClientSocket - The socket that is being handled for this thread.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::HandleClient(SOCKET theClientSocket)
{
   char buffer[8192];
   int bytesRecieved;

   while (true)
   {
      ZeroMemory(buffer, 8192);

      // Wait for client to send data.
      bytesRecieved = recv(theClientSocket, buffer, 8192, MSG_WAITALL);
      if (bytesRecieved == SOCKET_ERROR)
      {
         std::cerr << "Error in receiving. Quitting." << std::endl;
         break;
      }

      if (bytesRecieved == 0)
      {
         std::cout << "Client disconnected." << std::endl;
         mMutex.lock();
         for(auto iter = mConnectedClients.begin(); iter != mConnectedClients.end();)
         {
            if(*iter == theClientSocket)
            {
               iter = mConnectedClients.erase(iter);
            }
            else
            {
               ++iter;
            }
         }
         mMutex.unlock();
         break;
      }

      // Echo message back to client.
      BroadcastSend(buffer, bytesRecieved);
   }

   closesocket(theClientSocket);
}

//*********************************************************************************************************************
//
// Method: BroadcastSend
//
// Description:
//    Thread function to handle receiving messages and then calling to broadcast the message back out to the clients.
//
// Arguments:
//    theSendingClient  - The socket of the client that sent the message to be broadcasted so it can be ignored for
//                        broadcasting.
//    theBuffer         - The buffer containing the message to be broadcasted back to the clients.
//    theBystesRecieved - The number of bytes received to be sent back out.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::BroadcastSend(char* theBuffer, int theBystesRecieved)
{
   std::string test = "String Test!";
   mMutex.lock();
   for (auto iter = mConnectedClients.begin(); iter != mConnectedClients.end(); ++iter)
   {
      send(*iter, theBuffer, theBystesRecieved, 0);
   }
   mMutex.unlock();
}