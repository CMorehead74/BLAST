/*
	File:				DestroyBuildingMessage.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Sends a Destroy Building Message to the Message System
*/

#include "DestroyBuildingMessage.h"
#include "Building.h"
#include <cassert>

CDestroyBuildingMessage::CDestroyBuildingMessage(CBuilding* pBuilding)
	: CMessage(MSG_DESTROY_BUILDING)
{
	assert(pBuilding != nullptr && "CDestroyBuildingMessage ctor -- Cannot take a null pointer.");
	m_pBuilding = pBuilding;
	//m_pBuilding->AddRef();
}

CDestroyBuildingMessage::~CDestroyBuildingMessage(void)
{
	//m_pBuilding->Release();
	m_pBuilding = nullptr;
}