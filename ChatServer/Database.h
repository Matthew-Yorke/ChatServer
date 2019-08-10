//*********************************************************************************************************************
//
// File Name: Database.h
//
// Description:
//    TODO: Add description.
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
      //    TODO: Add description.
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
      //    TODO: Add description.
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
      //    True - TODO: Add description.
      //    False - TODO: Add description.
      //
      //***************************************************************************************************************
      bool Connect(std::string theHost, int thePortNumber, std::string theUser, std::string thePassword,
                   std::string theDatabase);

   protected:

      // There are currently no protected methods.

   private:

      // There are currently no private methods.

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

      // TODO: Add description.
      mysqlx::Session* mpSession;

      // TODO: Add description.
      std::string mHost;

      // TODO: Add description.
      std::string mUser;

      // TODO: Add description.
      std::string mPassword;

      // TODO: Add description.
      std::string mDatabase;

      // TODO: Add description.
      int mPortNumber;

//*********************************************************************************************************************
// End - Member Variables
//*********************************************************************************************************************

};

#endif // Database_H