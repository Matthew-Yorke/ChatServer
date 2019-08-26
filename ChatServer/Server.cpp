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

#include "stdafx.h"
#include "Server.h"
#include "ServerConstants.h"
#include "MessageConstants.h"
#include <iostream>

//*********************************************************************************************************************
// Start - Public Methods
//*********************************************************************************************************************

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
   mListernerSocketActive = false;
}

//*********************************************************************************************************************
//
// Method: ~Server
//
// Description:
//    Destructor of the Server object that will join all threads and then close the listener socket, finally cleaning
//    up windows sockets. Additionally, at the end any allocated memory is freed.
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
   TerminateServer();

   for (auto iter = mClientThreads.begin();
        iter != mClientThreads.end();
        ++iter)
   {
      if ((*iter)->joinable() == true)
      {
         (*iter)->join();
      }
   }

   delete mpDatabase;
}

//*********************************************************************************************************************
//
// Method: RegisterObserver
//
// Description:
//    Register an observer by adding it to the vector of registered observers.
//
// Arguments:
//    thepObserver - Pointer to the observer object that is registering for information.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::RegisterObserver(Observer* const thepObserver)
{
   auto iterator = std::find(mObservers.begin(), 
                             mObservers.end(),
                             thepObserver);
   if (iterator == mObservers.end())
   {
       mObservers.push_back(thepObserver);
   }
}

//*********************************************************************************************************************
//
// Method: RemoveObserver
//
// Description:
//    Unregister an observer by removing it from the vector of registered observers.
//
// Arguments:
//    thepObserver - Pointer to the observer object that is unregistering.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::RemoveObserver(Observer* const thepObserver)
{
   auto iterator = std::find(mObservers.begin(),
                             mObservers.end(),
                             thepObserver);
   if (iterator != mObservers.end())
   {
       mObservers.erase(iterator);
   }
}

//*********************************************************************************************************************
//
// Method: StartServer
//
// Description:
//    Attempts to start the server up by creating a listener socket thread to wait for client connections.
//
// Arguments:
//    N/A
//
// Return:
//    True  - The server successfully started.
//    False - The server did not successfully start.
//
//*********************************************************************************************************************
bool Server::StartServer()
{
   // Set up a listener socket and on any failure clean up sockets then return false.
   if (InitializeWindowsSocket() == false ||
       CreateListenerSocket() == false ||
       BindListenerSocket() == false)
   {
      TerminateServer();
      return false;
   }

   // Start a thread for listening for socket connections.
   mListenThread = new std::thread(&Server::ListenForConnections,
                                   this);

   // No failures happened, return true.
   return true;
}

//*********************************************************************************************************************
//
// Method: TerminateServer
//
// Description:
//    Terminates the listener thread so the server object can gracefully cleanup memory and shutdown.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::TerminateServer()
{
   mListernerSocketActive = false;
   closesocket(mListeningSocket);

   if (mListenThread != nullptr &&
       mListenThread->joinable() == true)
   {
      mListenThread->join();
   }
   delete mListenThread;
   mListenThread = nullptr;

   WSACleanup();
}

//*********************************************************************************************************************
//
// Method: ConnectToDatabase
//
// Description:
//    Attempts to connect to the database using the passed in credentials.
//
// Arguments:
//    theHost         - The hostname/IP address of the database.
//    thePortNumber   - The port number of the database.
//    theUser         - The user to log into the database.
//    thePassword     - The password for the user to log into the database.
//    theDatabaseName - The name of the database schema to connect to.
//
// Return:
//    True  - The connection to the database is successful.
//    False - The connection to the database has failed.
//
//*********************************************************************************************************************
bool Server::ConnectToDatabase(const std::string theHost, const int thePortNumber, const std::string theUser,
                               const std::string thePassword, const std::string theDatabaseName)
{
   return mpDatabase->Connect(theHost,
                              thePortNumber,
                              theUser,
                              thePassword,
                              theDatabaseName);;
}

//*********************************************************************************************************************
// End - Public Methods
//*********************************************************************************************************************

//*********************************************************************************************************************
// Start - Protected Methods
//*********************************************************************************************************************

// There are currently no protected methods.

//*********************************************************************************************************************
// End - Protected Methods
//*********************************************************************************************************************

//*********************************************************************************************************************
// Start - Private Methods
//*********************************************************************************************************************

//***************************************************************************************************************
//
// Method: NotifyObservers
//
// Description:
//    Notify all the registered observers about when a change happens.
//
// Arguments:
//    thepServerInformation - The information to notify observers.
//
// Return:
//    N/A
//
//***************************************************************************************************************
void Server::NotifyObservers(Information* const thepServerInformation)
{
   for (Observer* pObserver : mObservers)
   {
       pObserver->Notify(thepServerInformation);
   }
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

   if (WSAStartup(version, &wsData) != ServerConstants::WSA_STARTUP_SUCCESS)
   {
      AfxMessageBox(GuiMessageConstants::INITIALIZE_LISTEN_SOCKET_ERROR,
                    MB_OK | MB_ICONERROR);
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
   mListeningSocket = socket(AF_INET,
                             SOCK_STREAM,
                             IPPROTO_TCP);
   if (mListeningSocket == INVALID_SOCKET)
   {
      AfxMessageBox(GuiMessageConstants::CREATE_SOCKET_ERROR,
                    MB_OK | MB_ICONERROR);
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
bool Server::BindListenerSocket()
{
   sockaddr_in hint;
   hint.sin_family = AF_INET;
   hint.sin_port = htons(ServerConstants::SERVER_PORT_NUMBER);
   hint.sin_addr.S_un.S_addr = INADDR_ANY;

   if (bind(mListeningSocket,
            reinterpret_cast<sockaddr*>(&hint),
            sizeof(hint)) == SOCKET_ERROR)
   {
      return false;
   }

   if (listen(mListeningSocket,
              SOMAXCONN) == SOCKET_ERROR)
   {
      return false;
   }

   return true;
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
   mListernerSocketActive = true;
   sockaddr_in client;
   int clientSize = sizeof(client);

   do
   {
      // Listen for a socket connection.
      SOCKET clientSocket = accept(mListeningSocket,
                                   reinterpret_cast<sockaddr*>(&client),
                                   &clientSize);
      // Valid socket connected.
      if (clientSocket != INVALID_SOCKET)
      {
         char host[NI_MAXHOST];      // Client's remote name.
         char service[NI_MAXHOST];   // Service (i.e. port) client is connected on.

         ZeroMemory(host,
                    NI_MAXHOST);
         ZeroMemory(service,
                    NI_MAXHOST);

         if (getnameinfo(reinterpret_cast<sockaddr*>(&client),
                         sizeof(client),
                         host,
                         NI_MAXHOST,
                         service,
                         NI_MAXHOST,
                         0) == 0)
         {
            std::cout << host << " connected on port: " << service << std::endl;
         }
         else
         {
            inet_ntop(AF_INET,
                      &client.sin_addr,
                      host,
                      NI_MAXHOST);
            std::cout << host << " connected on port: " << ntohs(client.sin_port) << std::endl;
         }

         // Add the connection to the vector of connected clients.
         mMutex.lock();
         mConnectedClients.push_back(clientSocket);
         mMutex.unlock();
         
         mClientThreads.emplace_back(new std::thread(&Server::HandleClient,
                                                     this,
                                                     clientSocket));
      }
   } while (mListernerSocketActive == true);
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
void Server::HandleClient(const SOCKET theClientSocket)
{
   char buffer[ServerConstants::MESSAGE_BUFFER_SIZE];
   std::string string = ServerConstants::BLANK_MESSAGE;
   int bytesRecieved = 0;

   while (true)
   {
      ZeroMemory(buffer,
                 ServerConstants::MESSAGE_BUFFER_SIZE);

      // Wait to receive data from the client.
      bytesRecieved = recv(theClientSocket,
                           buffer,
                           ServerConstants::MESSAGE_BUFFER_SIZE,
                           MSG_WAITALL);
      if (bytesRecieved == SOCKET_ERROR)
      {
         break;
      }
      // Will happen if a client disconnects.
      if (bytesRecieved == 0)
      {
         // Remove the client from list of connected clients and then break to end the thread.
         mMutex.lock();
         for (auto iter = mConnectedClients.begin();
              iter != mConnectedClients.end();)
         {
            if (*iter == theClientSocket)
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

      // Convert message char array to string.
      string = buffer;

      // Received a standard message from the client.
      if (string.compare(ServerConstants::MESSAGE_TYPE_MESSAGE) == ServerConstants::STRINGS_ARE_EQUAL)
      {
         // Wait to receive the actual message.
         ZeroMemory(buffer,
                    ServerConstants::MESSAGE_BUFFER_SIZE);
         bytesRecieved = recv(theClientSocket,
                              buffer,
                              ServerConstants::MESSAGE_BUFFER_SIZE,
                              MSG_WAITALL);

         // Send message information to observers.
         Information* pConnectionInformation = new Information();
         pConnectionInformation->type = Information::Message;
         pConnectionInformation->message = buffer;
         pConnectionInformation->user = ServerConstants::BLANK_MESSAGE;
         SendObserversMessage(pConnectionInformation);

         // Echo message back to client.
         BroadcastSend(buffer,
                       bytesRecieved);
      }
      // Received a connection request from the client.
      else if (string.compare(ServerConstants::MESSAGE_TYPE_CONNECTION) == ServerConstants::STRINGS_ARE_EQUAL)
      {
         // Wait to receive the actual message.
         ZeroMemory(buffer,
                    ServerConstants::MESSAGE_BUFFER_SIZE);
         bytesRecieved = recv(theClientSocket,
                              buffer,
                              ServerConstants::MESSAGE_BUFFER_SIZE,
                              MSG_WAITALL);

         std::string message = buffer;
         std::vector<std::string> tokens;
         std::stringstream ss(message);
         while (ss.good())
         {
            std::string substr;
            std::getline(ss,
                         substr,
                         ServerConstants::PARSE_DELIMIETER);
            tokens.push_back(substr);
         }

         // TODO: Query database
         bool userExist = mpDatabase->CheckUserLogin(tokens[0],
                                                     tokens[1]);
         
         // Successful connection.
         if (userExist == true)
         {
            SendResponseMessage(ServerConstants::ACKNOWLEDGE_CONNECTION, theClientSocket);

            // Send a message to observers that the client has connected
            Information* pConnectionInformation = new Information();
            pConnectionInformation->type = Information::Connection;
            pConnectionInformation->message = ServerConstants::BLANK_MESSAGE;
            pConnectionInformation->user = tokens[0];
            SendObserversMessage(pConnectionInformation);
         }
         // Unsuccessful connection.
         else
         {
            SendResponseMessage(ServerConstants::UNACKNOWLEDGE_CONNECTION, theClientSocket);
         }
      }
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
//    theBuffer         - The buffer containing the message to be broadcasted back to the clients.
//    theBystesRecieved - The number of bytes received to be sent back out.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::BroadcastSend(const char* const theBuffer, const int theBystesRecieved)
{
   mMutex.lock();
   for (auto iter = mConnectedClients.begin();
        iter != mConnectedClients.end();
        ++iter)
   {
      send(*iter,
           theBuffer,
           theBystesRecieved,
           ServerConstants::SEND_NO_FLAG);
   }
   mMutex.unlock();
}

//*********************************************************************************************************************
//
// Method: SendObserversMessage
//
// Description:
//    This method sends the passed in information to all observers and frees up memory.
//
// Arguments:
//    thepMessage - The message to send to observers.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::SendObserversMessage(Information* const thepMessage)
{
   mMutex.lock();
   NotifyObservers(thepMessage);
   mMutex.unlock();
   delete thepMessage;
}

//*********************************************************************************************************************
//
// Method: SendResponseMessage
//
// Description:
//    This methods converts the message into a fixed sized buffer and sends it to the passed in socket.
//
// Arguments:
//    theMessage - The message to be sent.
//    theSocket  - The connected socket to receive the message.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
void Server::SendResponseMessage(const std::string theMessage, const SOCKET theSocket)
{
   char buffer[ServerConstants::MESSAGE_BUFFER_SIZE];

   // TODO: Add code for handling messages larger than the buffer size. A.K.A. Send multiple packets.

   ZeroMemory(buffer,
              ServerConstants::MESSAGE_BUFFER_SIZE);

   for (int i = 0;
        i < theMessage.length();
        ++i)
   {
      buffer[i] = theMessage[i];
   }
   send(theSocket,
        buffer,
        ServerConstants::MESSAGE_BUFFER_SIZE,
        ServerConstants::SEND_NO_FLAG);
}

//*********************************************************************************************************************
// End - Private Methods
//*********************************************************************************************************************