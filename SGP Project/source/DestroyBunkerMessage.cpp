/*
	File:						DestroyBunkerMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/26/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#include "DestroyBunkerMessage.h"
#include "Bunker.h"
#include <cassert>

CDestroyBunkerMessage::CDestroyBunkerMessage(CBunker* pBunker )
	:CMessage( MSG_DESTROY_BUNKER )
{
	assert(pBunker != nullptr && "CDestroyBunkerMessage ctor - Cannot take in a null pointer.");
	m_pBunker = pBunker;
	m_pBunker->AddRef();
}


CDestroyBunkerMessage::~CDestroyBunkerMessage(void)
{
	m_pBunker->Release();
	m_pBunker = nullptr;
}
