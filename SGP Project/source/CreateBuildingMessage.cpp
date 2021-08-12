/*
	File:				CreateBuildingMessage.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Create Building Message to the Message System
*/

#include "CreateBuildingMessage.h"

CCreateBuildingMessage::CCreateBuildingMessage(int nType)
	: CMessage(MSG_CREATE_BUILDING)
{
	m_nType = nType;
}

CCreateBuildingMessage::~CCreateBuildingMessage(void)
{
}