//*********************************************************************************************************************
//
// File Name: Subject.h
//
// Description:
//    TODO: Add description.
//
// History
//---------------------------------------------------------------------------------------------------------------------
// Matthew Yorke             | 08/DD/2019 | TODO: Add description
//
//*********************************************************************************************************************

#ifndef Subject_H
#define Subject_H

#include "Observer.h"

class Subject {

//*********************************************************************************************************************
// Start - Methods
//*********************************************************************************************************************

   public:

      //***************************************************************************************************************
      //
      // Method: registerObserver
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
      virtual void RegisterObserver(Observer* thepObserver) = 0;

      //***************************************************************************************************************
      //
      // Method: removeObserver
      //
      // Description:
      //    Unregister an observer
      //
      // Arguments:
      //    thepObserver - the observer object to be unregistered
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual void RemoveObserver(Observer* thepObserver) = 0;

      //***************************************************************************************************************
      //
      // Method: notifyObservers
      //
      // Description:
      //    Notify all the registered observers when a change happens
      //
      // Arguments:
      //    thepParent - TODO: Add description.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual void NotifyObservers() = 0;

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

#endif // Subject_H