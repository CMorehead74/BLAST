/*
	File:					MovingObject.cpp
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:		Michael Yatckoske
	Last Edited:			9/14/2012
	Purpose:				Concrete base class for the Moving Objects in the game
*/

#include "MovingObject.h"

CMovingObject::CMovingObject(void)
{
	m_fVelX				= 0;
	m_fVelY				= 0;
	m_eType				= OBJ_BASE;
	m_fRotation			= 0.0f;
	m_bIsTargetInLineOfSight = false;
}

CMovingObject::~CMovingObject(void)
{
}

void CMovingObject::Update( float fElapsedTime )
{
	//modifying the position based on the velocity per second
	SetPosX(GetPosX() + m_fVelX * fElapsedTime);
	SetPosY(GetPosY() + m_fVelY * fElapsedTime);
}

void CMovingObject::Render()
{
}

bool CMovingObject::CheckCollision( IEntity* pOther )
{
	return false;
}