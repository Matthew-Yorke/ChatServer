//*********************************************************************************************************************
//
// File Name: Database.h
//
// Description:
//    This class handles the connection between the server and the database. Functionality allows for connecting to
//    different databases dynamically, but under the assumption of the same schema required for the chat database.
//    This class handles verifying user connection, manipulating users, and logging chat messages.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | MM/DD/2019 | TODO: Add description
//
//*********************************************************************************************************************

#ifndef Database_H
#define Database_H

#include <mysqlx/xdevapi.h>

class Database
{

//*********************************************************************************************************************
// Start - Methods
//*********************************************************************************************************************

   public:

      //***************************************************************************************************************
      //
      // Method: Database
      //
      // Description:
      //    Standard constructor for the database class that sets member variables to their default values.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      Database();

      //***************************************************************************************************************
      //
      // Method: ~Database
      //
      // Description:
      //    Destructor for the database class that frees any allocated dynamic memory.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual ~Database();

      //***************************************************************************************************************
      //
      // Method: Connect
      //
      // Description:
      //    Attempts to connect to the database using the passed in credentials.
      //
      // Arguments:
      //    theHost       - The IP address or hostname used to connect to the  MySQL database.
      //    thePortNumber - The port number used to connect to the MySQL database.
      //    theUser       - The username for connecting to the MySQL database.
      //    thePassword   - The password that accompanies the username for connecting to the MySQL database.
      //    theDatabase   - The name of the database schema.
      //
      // Return:
      //    True  - The connection to the database is successful.
      //    False - The connection to the database is not successful.
      //
      //***************************************************************************************************************
      bool Connect(const std::string& theHost, const int& thePortNumber, const std::string& theUser,
                   const std::string& thePassword, const std::string& theDatabase);

      //***************************************************************************************************************
      //
      // Method: CheckUserLogin
      //
      // Description:
      //    Queries the database with the username and password the client sent for logging into the chat.
      //
      // Arguments:
      //    theUsername - The username the client sent for logging in.
      //    thePassword - The password the client sent for logging in.
      //
      // Return:
      //    True  - The username/password combination exist in the database and the user can log in to the chat server.
      //    False - The username/password combination does not exist and the user will be denied access to the server.
      //
      //***************************************************************************************************************
      bool CheckUserLogin(const std::string& theUsername, const std::string& thePassword);

   protected:

      // There are currently no protected methods.

   private:

      //***************************************************************************************************************
      //
      // Method: Terminate
      //
      // Description:
      //    Frees any dynamically allocated member variables.
      //
      // Arguments:
      //    N/A
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      void Terminate();

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

      // Pointer to the session of the MySQL database.
      mysqlx::Session* mpSession;

      // Pointer to the schema within the MySQL database.
      mysqlx::Schema* mpSchema;

//*********************************************************************************************************************
// End - Member Variables
//*********************************************************************************************************************

};

#endif // Database_H