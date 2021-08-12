/*
	File:				CreateAllyMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:		9/12/2012
	Purpose:			Sends a Create Ally Message to the Message System
*/

#pragma once
#include "Message.h"

class CCreateAllyMessage : public CMessage
{
	int						m_nType;
public:
	CCreateAllyMessage(int nType);
	virtual ~CCreateAllyMessage(void);

	int GetType() const			{ return m_nType; }
};

