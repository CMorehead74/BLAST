/*
	File:				MessageSystem.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Handles sending and processing messages sent between the classes.
						--	Mostly will handle the creation and destruction of entities.
*/
#pragma once

#include "IMessage.h"
#include <queue>

// Typedef for a Function Pointer
typedef void (*MESSAGEPROC)(IMessage*);

class CMessageSystem
{
public:
	// Singleton
	static CMessageSystem* GetInstance(void);

	int GetNumMessages(void)	{ return (int)m_MessageQueue.size(); }

	void Initialize(MESSAGEPROC pfnMsgProc);

	void SendMsg(IMessage* pMsg);

	void ProcessMessages(void);

	void Clear(void);

	void Shutdown(void);

private:
	std::queue<IMessage*>		m_MessageQueue;		// Push the messages into here
	MESSAGEPROC					m_pfnMsgProc;		// Pointer to the GamePlayState's Message Proc to handle the messages

	CMessageSystem() { m_pfnMsgProc = nullptr; }
	CMessageSystem(const CMessageSystem&);
	CMessageSystem& operator=(const CMessageSystem&);
	~CMessageSystem() { }
};