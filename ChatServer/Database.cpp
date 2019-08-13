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

#include "stdafx.h"
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
   mHost = "";
   mUser = "";
   mPassword = "";
   mDatabase = "";
   mPortNumber = -1;
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
//    theHost - TODO: Add description.
//    thePortNumber - TODO: Add description.
//    theUser - TODO: Add description.
//    thePassword - TODO: Add description.
//    theDatabase - TODO: Add description.
//
// Return:
//    N/A
//
//*********************************************************************************************************************
bool Database::Connect(std::string theHost, int thePortNumber, std::string theUser, std::string thePassword,
                       std::string theDatabase)
{   
      try
      {
         mpSession = new mysqlx::Session(mysqlx::SessionOption::HOST, theHost,
                                         mysqlx::SessionOption::PORT, thePortNumber,
                                         mysqlx::SessionOption::USER, theUser,
                                         mysqlx::SessionOption::PWD, thePassword);

         try
         {
            mpSession->sql("USE " + theDatabase).execute();
         }
         catch(const mysqlx::Error& err)
         {
            delete mpSession;
            mpSession = nullptr;
            return false;
         }
      }
      catch(const mysqlx::Error& err)
      {
         return false;
      }

   return true;
}

bool Database::CheckUserLogin(std::string theUsername, std::string thePassword)
{
   mysqlx::RowResult myRows;
   //std::vector<mysqlx::Schema> myScehma = mpSession->getSchemas();
   //for(auto a = myScehma.begin(); a != myScehma.end(); ++a)
   //{
   //   std::vector<mysqlx::Table> table = a->getTables();
   //}
   mysqlx::Schema mySchema = mpSession->getSchema(L"chatdatabase");
   mysqlx::Table usersTable = mySchema.getTable(L"users");
   myRows = usersTable.select("handle", "password")
                      .where("handle like :handle AND password like :password")
                      .bind("handle", theUsername)
                      .bind("password", thePassword)
                      .execute();

   if (myRows.begin() == myRows.end())
   {
      return false;
   }
   else
   {
      return true;
   }
}