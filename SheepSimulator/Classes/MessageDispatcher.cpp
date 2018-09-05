#include "MessageDispatcher.h"
#include "BaseGameEntity.h"
#include "EntityManager.h"
#include "Locations.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

using std::set;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif



//------------------------------ Instance -------------------------------------

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher instance;

	return &instance;
}


//----------------------------- Dispatch ---------------------------------
//  
//  see description in header
//------------------------------------------------------------------------
void MessageDispatcher::Discharge(BaseGameEntity* pReceiver,
	const Telegram& telegram)
{
	if (!pReceiver->HandleMessage(telegram))
	{
		//telegram could not be handled
		cout << "Message not handled";
	}
}

//---------------------------- DispatchMessage ---------------------------
//
//  given a message, a receiver, a sender and any time delay , this function
//  routes the message to the correct agent (if no delay) or stores
//  in the message queue to be dispatched at the correct time
//------------------------------------------------------------------------
void MessageDispatcher::DispatchMsg(
	int    sender,
	int    receiver,
	int    msg,
	void*  ExtraInfo)
{
	//get pointers to the sender and receiver
	BaseGameEntity* pSender = EntityMgr->GetEntityFromID(sender);
	BaseGameEntity* pReceiver = EntityMgr->GetEntityFromID(receiver);

	//make sure the receiver is valid
	if (pReceiver == NULL)
	{
		cout << "\nWarning! No Receiver with ID of " << receiver << " found";

		return;
	}

	//create the telegram
	Telegram telegram(0, sender, receiver, msg, ExtraInfo);

	//send the telegram to the recipient
	Discharge(pReceiver, telegram);

}