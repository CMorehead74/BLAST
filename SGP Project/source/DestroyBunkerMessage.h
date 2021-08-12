/*
	File:						DestroyBunkerMessage.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/26/2012
	Purpose:				Sends a Create Building Message to the Message System
*/

#pragma once
#include "message.h"

class CBunker;

class CDestroyBunkerMessage :
	public CMessage
{
private:
	CBunker*			m_pBunker;
public:
	CDestroyBunkerMessage(CBunker* pBunker);
	virtual ~CDestroyBunkerMessage(void);

	CBunker* GetBunker() const			{ return m_pBunker; }
};

