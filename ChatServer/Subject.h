//*********************************************************************************************************************
//
// File Name: Subject.h
//
// Description:
//    The subject interface of the observer pattern that sets the methods needed to be implemented by the child class
//    to register, unregister, and notify observers.
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
      // Method: RegisterObserver
      //
      // Description:
      //    Register an observer.
      //
      // Arguments:
      //    thepObserver - Pointer to an Observer object that is being registered.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual void RegisterObserver(Observer* const thepObserver) = 0;

      //***************************************************************************************************************
      //
      // Method: RemoveObserver
      //
      // Description:
      //    Unregister an observer.
      //
      // Arguments:
      //    thepObserver - Pointer of the Observer object that is no longer observing changes.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual void RemoveObserver(Observer* const thepObserver) = 0;

      //***************************************************************************************************************
      //
      // Method: NotifyObservers
      //
      // Description:
      //    Notify all the registered observers when a change happens
      //
      // Arguments:
      //    thepServerInformation - Pointer to the information the client sent.
      //
      // Return:
      //    N/A
      //
      //***************************************************************************************************************
      virtual void NotifyObservers(Information* const thepServerInformation) = 0;

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