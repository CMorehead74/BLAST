/*
	File:				CreateAllyMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:		9/12/2012
	Purpose:			Sends a Create Ally Message to the Message System
*/

#include "CreateAllyMessage.h"

CCreateAllyMessage::CCreateAllyMessage(int nType)
	: CMessage(MSG_CREATE_ALLY)
{
	m_nType = nType;
}

CCreateAllyMessage::~CCreateAllyMessage(void)
{
}