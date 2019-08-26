//*********************************************************************************************************************
//
// File Name: Server.h
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

#ifndef Server_H
#define Server_H

#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <mutex>
#include "Database.h"
#include "Subject.h"

#pragma comment (lib, "ws2_32.lib")

class Server : Subject
{

//*********************************************************************************************************************
// Start - Methods
//*********************************************************************************************************************

   public:

      //***************************************************************************************************************
      //
      // Method: Server
      //
      // Description:
      //    The constructor of the server object that kicks off the initialization of the server by setting up the
      //    listener socket.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      Server();

      //***************************************************************************************************************
      //
      // Method: ~Server
      //
      // Description:
      //    Destructor of the Server object that will join all threads and then close the listener socket, finally
      //    cleaning up windows sockets.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual ~Server();

      //***************************************************************************************************************
      //
      // Method: RegisterObserver
      //
      // Description:
      //    Register an observer
      //
      // Arguments:
      //    thepObserver - the observer object to be registered.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void RegisterObserver(Observer* const thepObserver) override;

      //***************************************************************************************************************
      //
      // Method: RemoveObserver
      //
      // Description:
      //    Unregister an observer
      //
      // Arguments:
      //    thepObserver - the observer object to be unregistered.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void RemoveObserver(Observer* const thepObserver) override;

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      bool StartServer();

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      void TerminateServer();

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      bool ConnectToDatabase(const std::string theHost, const int thePortNumber, const std::string theUser,
                             const std::string thePassword, const std::string theDatabaseName);

   protected:

      // There are currently no protected methods.

   private:

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
      void NotifyObservers(Information* const thepServerInformation) override;

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      bool InitializeWindowsSocket();

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      bool CreateListenerSocket();

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      bool BindListenerSocket();

      //***************************************************************************************************************
      //
      // Method: ListenForConnections
      //
      // Description:
      //    Continuously listen for a connection. If a connection comes in, create a new thread to handle the
      //    connection messaging.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void ListenForConnections();

      //***************************************************************************************************************
      //
      // Method: HandleClient
      //
      // Description:
      //    Thread function to handle receiving messages and then calling to broadcast the message back out to the
      //    clients.
      //
      // Arguments:
      //    theClientSocket - The socket that is being handled for this thread.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void HandleClient(const SOCKET theClientSocket);

      //***************************************************************************************************************
      //
      // Method: BroadcastSend
      //
      // Description:
      //    Thread function to handle receiving messages and then calling to broadcast the message back out to the
      //     clients.
      //
      // Arguments:
      //    theBuffer         - The buffer containing the message to be broadcasted back to the clients.
      //    theBystesRecieved - The number of bytes received to be sent back out.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void BroadcastSend(const char* const theBuffer, const int theBystesRecieved);

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      void SendObserversMessage(Information* const thepMessage);

      //***************************************************************************************************************
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
      //***************************************************************************************************************
      void SendResponseMessage(const std::string theMessage, const SOCKET theSocket);

//*********************************************************************************************************************
// End - Methods
//*********************************************************************************************************************

//*********************************************************************************************************************
// Start - Member Variables
//*********************************************************************************************************************

   public:

      // There are currently no public member variables.

   protected:

      // There are currently no protected member variables.

   private:

      // The socket to listen for connections.
      SOCKET mListeningSocket;

      // Thread to handle listening for connections.
      std::thread* mListenThread;

      // Vector containing the list of all active threads.
      std::vector<std::unique_ptr<std::thread>> mClientThreads;

      // Vector containing the list of all connected clients.
      std::vector<SOCKET> mConnectedClients;

      // Mutex to block critical data.
      std::mutex mMutex;

      // List of objects observing this class.
      std::vector<Observer*> mObservers;

      // TODO: Add description.
      Database* mpDatabase;

      // Boolean to track if the listener socket is active (true) or not (false).
      bool mListernerSocketActive;

//*********************************************************************************************************************
// End - Member Variables
//*********************************************************************************************************************

};

#endif // Server_H