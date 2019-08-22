//*********************************************************************************************************************
//
// File Name: Database.cpp
//
// Description:
//    This class handles the connection between the server and the database. Functionality allows for connecting to
//    different databases dynamically, but under the assumption of the same schema required for the chat database.
//    This class handles verifying user connection, manipulating users, and logging chat messages.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 07/31/2019 | TODO: Add description.
//
//*********************************************************************************************************************

#include "stdafx.h"
#include "Database.h"
#include <iostream>
#include <string>

//*********************************************************************************************************************
// Start - Public Methods
//*********************************************************************************************************************

//*********************************************************************************************************************
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
//*********************************************************************************************************************
Database::Database()
{
   mpSession = nullptr;
   mpSchema = nullptr;
}

//*********************************************************************************************************************
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
//*********************************************************************************************************************
Database::~Database()
{
   Terminate();
}

//*********************************************************************************************************************
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
//*********************************************************************************************************************
bool Database::Connect(const std::string& theHost, const int& thePortNumber, const std::string& theUser, 
                       const std::string& thePassword, const std::string& theDatabase)
{   
      try
      {
         // Create connection to the database.
         mpSession = new mysqlx::Session(mysqlx::SessionOption::HOST, theHost,
                                         mysqlx::SessionOption::PORT, thePortNumber,
                                         mysqlx::SessionOption::USER, theUser,
                                         mysqlx::SessionOption::PWD, thePassword);

         // Track connection to the database schema.
         mpSchema = new mysqlx::Schema(mpSession->getSchema(theDatabase));
         if(mpSchema->existsInDatabase() == false)
         {
            Terminate();
            return false;
         }
      }
      catch([[maybe_unused]]const mysqlx::Error& err)
      {
         return false;
      }

   return true;
}

//*********************************************************************************************************************
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
//*********************************************************************************************************************
bool Database::CheckUserLogin(const std::string& theUsername, const std::string& thePassword)
{
   // Query the username and password against the database table.
   mysqlx::Table usersTable = mpSchema->getTable("users");
   mysqlx::RowResult myRows = usersTable.select("handle", "password")
                                        .where("handle like :handle AND password like :password")
                                        .bind("handle", theUsername)
                                        .bind("password", thePassword)
                                        .execute();

   // If the query resulted in no items, then return false that the user/password combination didn't work. Otherwise,
   // the item existed in the table and return true.
   if (myRows.begin() == myRows.end())
   {
      return false;
   }
   else
   {
      return true;
   }
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

//*********************************************************************************************************************
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
//*********************************************************************************************************************
void Database::Terminate()
{
   delete mpSession;
   mpSession = nullptr;
   delete mpSchema;
   mpSchema = nullptr;
}

//*********************************************************************************************************************
// End - Private Methods
//*********************************************************************************************************************