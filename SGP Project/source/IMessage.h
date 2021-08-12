/*
	File:				IMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Abstract Base Class for the Message System
*/

#pragma once
class IMessage
{
public:
	IMessage(void) {}

	virtual ~IMessage(void) = 0 {}
	virtual int GetMessageID(void) = 0 {}

private:
	IMessage(const IMessage&);
	IMessage& operator=(const IMessage&);
};