//*********************************************************************************************************************
//
// File Name: Observer.h
//
// Description:
//    The observer interface for the observer pattern used to create the abstract method to notify classes that the
//    subject they are observing is notifying changes.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 08/DD/2019 | TODO: Add description
//
//*********************************************************************************************************************

#ifndef Observer_H
#define Observer_H

//*********************************************************************************************************************
//
// Structure: Information
//
// Description:
//    The information structure of what information is contained for when the server has updated with a new message
//    from the client. The client will send what type of message, who sent the message, and what the message is.
//
//*********************************************************************************************************************
struct Information
{
   enum MessageType {Connection, Disconnect, Message};
   MessageType type;
   std::string user;
   std::string message;
};

class Observer {

//*********************************************************************************************************************
// Start - Methods
//*********************************************************************************************************************

   public:

   //***************************************************************************************************************
   //
   // Method: Notify
   //
   // Description:
   //    Sets the notify pure virtual method so any class that inherits this class must define the functionality.
   //
   // Arguments:
   //    thepServerInformation - TODO: Add description.
   //
   // Return:
   //    N/A
   //
   //***************************************************************************************************************
   virtual void Notify(Information* thepServerInformation) = 0;

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

      // There are currently no private member variables.

//*********************************************************************************************************************
// End - Member Variables
//*********************************************************************************************************************

};

#endif // Observer_H