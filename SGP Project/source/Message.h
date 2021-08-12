/*
	File:				CMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Concrete Base Class for the message system to allow for polymorphism
*/

#pragma once
#include "IMessage.h"

enum Message_Type		{ MSG_CREATE_ALLY, MSG_CREATE_BUILDING, MSG_CREATE_BULLET, MSG_CREATE_SHOTGUN_BULLET, MSG_CREATE_ENEMY, MSG_CREATE_PICKUP, MSG_CREATE_BUNKER, MSG_CREATE_STOREHOUSE, MSG_CREATE_MINE,
										MSG_DESTROY_ALLY, MSG_DESTROY_BUILDING, MSG_DESTROY_BULLET, MSG_DESTROY_SHOTGUN_BULLET,MSG_DESTROY_ENEMY, MSG_DESTROY_PICKUP, MSG_DESTROY_BUNKER, MSG_DESTROY_STOREHOUSE, MSG_DESTROY_MINE };

class CMessage : public IMessage
{
public:
	CMessage(Message_Type msgID)		{ m_nMessageID = msgID; }
	virtual ~CMessage(void)				{ }
	virtual int GetMessageID(void)		{ return m_nMessageID; }

private:
	int m_nMessageID;
};