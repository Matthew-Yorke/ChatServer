//*********************************************************************************************************************
//
// File Name; Database.cpp
//
// Description:
//    TODO: Add description.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 07/31/2019 | TODO: Add description.
//
//*********************************************************************************************************************

#include "Database.h"
#include <iostream>
#include <string>

//*********************************************************************************************************************
//
// Method: Database
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
Database::Database()
{
   mpSession = nullptr;
   Connect();
}

//*********************************************************************************************************************
//
// Method: ~Database
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
Database::~Database()
{
}

//*********************************************************************************************************************
//
// Method: Connect
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
void Database::Connect()
{
   do
   {
      std::string host, user, password, database;
      std::cout << "Enter the hostname/IP address of the database: ";
      std::cin >> host;
      std::cout << "Enter the user of the database: ";
      std::cin >> user;
      std::cout << "Enter the password of the database: ";
      std::cin >> password;
      std::cout << "Enter the database name: ";
      std::cin >> database;
   
      try
      {
         mpSession = new mysqlx::Session(mysqlx::SessionOption::HOST, host,
                                         mysqlx::SessionOption::PORT, 33060,
                                         mysqlx::SessionOption::USER, user,
                                         mysqlx::SessionOption::PWD, password);

         try
         {
            mpSession->sql("USE " + database).execute();
         }
         catch(const mysqlx::Error& err)
         {
            std::cerr << "ERROR: Unable to connect to the schema with the entered data." << std::endl;
         }
      }
      catch(const mysqlx::Error& err)
      {
         std::cerr << "ERROR: Unable to connect to the database with the entered data." << std::endl;
      }
   } while (mpSession == nullptr);

   std::cout << "SUCCESS: Connected to the database." << std::endl;
}