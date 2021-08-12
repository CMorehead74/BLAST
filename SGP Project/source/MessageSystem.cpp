/*
	File:				MessageSystem.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/11/2012
	Purpose:			Handles sending and processing messages sent between the classes.
						--	Mostly will handle the creation and destruction of entities.
*/

#include "MessageSystem.h"

// Singleton
CMessageSystem* CMessageSystem::GetInstance(void)
{
	static CMessageSystem s_Instance;
	return &s_Instance;
}

void CMessageSystem::Initialize(MESSAGEPROC pfnMsgProc)
{
	if(pfnMsgProc == nullptr)
		return;

	m_pfnMsgProc = pfnMsgProc;
}

void CMessageSystem::SendMsg(IMessage* pMsg)
{
	if(pMsg == nullptr)
		return;

	m_MessageQueue.push(pMsg);
}

void CMessageSystem::ProcessMessages(void)
{
	if(m_pfnMsgProc == nullptr)
		return;

	while(m_MessageQueue.empty() == false)
	{
		m_pfnMsgProc(m_MessageQueue.front());
		delete m_MessageQueue.front();
		m_MessageQueue.pop();
	}
}

void CMessageSystem::Clear(void)
{
	while(m_MessageQueue.empty() == false)
	{
		delete m_MessageQueue.front();
		m_MessageQueue.pop();
	}
}

void CMessageSystem::Shutdown(void)
{
	Clear();
	m_pfnMsgProc = nullptr;
}