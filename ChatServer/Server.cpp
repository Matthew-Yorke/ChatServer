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

   for(auto iter = mClientThreads.begin(); iter != mClientThreads.end(); ++iter)
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
   auto iterator = std::find(mObservers.begin(), mObservers.end(), thepObserver);
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
   auto iterator = std::find(mObservers.begin(), mObservers.end(), thepObserver);
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
//    TODO: Add description.
//
// Arguments:
//    N/A
//
// Return:
//    N/A
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
   mListenThread = new std::thread(&Server::ListenForConnections, this);

   // No failures happened, return true.
   return true;
}

//*********************************************************************************************************************
//
// Method: TerminateServer
//
// Description:
//    TODO: Add description.
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
//    thepObserver - the observer object to be unregistered
//
// Return:
//    True  - The connection to the database is successful.
//    False - The connection to the database has failed.
//
//*********************************************************************************************************************
bool Server::ConnectToDatabase(const std::string theHost, const int thePortNumber, const std::string theUser,
                               const std::string thePassword, const std::string theDatabaseName)
{
   return mpDatabase->Connect(theHost, thePortNumber, theUser, thePassword, theDatabaseName);;
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
//    Notify all the registered observers when a change happens
//
// Arguments:
//    thepServerInformation - 
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

   if (WSAStartup(version, &wsData) != 0)
   {
      AfxMessageBox(_T("Failed to initialize the listener socket."), MB_OK | MB_ICONERROR);
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
      AfxMessageBox(_T("Failed to create the socket."), MB_OK | MB_ICONERROR);
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
   hint.sin_port = htons(54000);
   hint.sin_addr.S_un.S_addr = INADDR_ANY;

   if (bind(mListeningSocket, reinterpret_cast<sockaddr*>(&hint), sizeof(hint)) == SOCKET_ERROR)
   {
      return false;
   }

   if (listen(mListeningSocket, SOMAXCONN) == SOCKET_ERROR)
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
      SOCKET clientSocket = accept(mListeningSocket, reinterpret_cast<sockaddr*>(&client), &clientSize);
      // Invalid socket tried to connect.
      if (clientSocket == INVALID_SOCKET)
      {
         std::cerr << "Client socket is invalid." << std::endl;
      }
      // Valid socket connected.
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

         // Add the connection to the vector of connected clients.
         mMutex.lock();
         mConnectedClients.push_back(clientSocket);
         mMutex.unlock();

         // Send a message to observers that the client has connected
         Information* pConnectionInformation = new Information();
         pConnectionInformation->type = Information::Connection;
         pConnectionInformation->message = "";
         pConnectionInformation->user = host;
         mMutex.lock();
         NotifyObservers(pConnectionInformation);
         mMutex.unlock();
         delete pConnectionInformation;
         
         mClientThreads.emplace_back(new std::thread(&Server::HandleClient, this, clientSocket));
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
void Server::HandleClient(SOCKET theClientSocket)
{
   char buffer[8192];
   std::string string;
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

      string = buffer;

      if(string == "message")
      {
         // Wait to receive the actual message.
         ZeroMemory(buffer, 8192);
         bytesRecieved = recv(theClientSocket, buffer, 8192, MSG_WAITALL);

         // Send message information to observers.
         Information* pConnectionInformation = new Information();
         pConnectionInformation->type = Information::Message;
         pConnectionInformation->message = buffer;
         pConnectionInformation->user = "";
         mMutex.lock();
         NotifyObservers(pConnectionInformation);
         mMutex.unlock();
         delete pConnectionInformation;

         // Echo message back to client.
         BroadcastSend(buffer, bytesRecieved);
      }
      else if(string == "connection")
      {
         // Wait to receive the actual message.
         ZeroMemory(buffer, 8192);
         bytesRecieved = recv(theClientSocket, buffer, 8192, MSG_WAITALL);

         std::string message = buffer;
         std::vector<std::string> tokens;
         std::stringstream ss(message);
         while( ss.good() )
         {
            std::string substr;
            std::getline(ss, substr, ',' );
            tokens.push_back( substr );
         }

         // TODO: Query database
         bool userExist = mpDatabase->CheckUserLogin(tokens[0], tokens[1]);
         
         ZeroMemory(buffer, 8192);
         // Successful connection.
         if (userExist == true)
         {
            std::string message = "ACK";
            for (int i = 0; i < message.length(); ++i)
            {
               buffer[i] = message[i];
            }
            send(theClientSocket, buffer, 8192, 0);
         }
         // Unsuccessful connection.
         else
         {
            std::string message = "NACK";
            for (int i = 0; i < message.length(); ++i)
            {
               buffer[i] = message[i];
            }
            send(theClientSocket, buffer, 8192, 0);
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
   mMutex.lock();
   for (auto iter = mConnectedClients.begin(); iter != mConnectedClients.end(); ++iter)
   {
      send(*iter, theBuffer, theBystesRecieved, 0);
   }
   mMutex.unlock();
}

//*********************************************************************************************************************
// End - Private Methods
//*********************************************************************************************************************