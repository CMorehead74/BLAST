/*
	File:				Building.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/14/2012
	Purpose:			Concrete base class for the different building types
*/

#include "Building.h"
#include "Camera.h"

CBuilding::CBuilding(void)
{
	m_eType = OBJ_BUILDBASE;
}

CBuilding::~CBuilding(void)
{
}

RECT CBuilding::GetRect() const
{
	RECT rTemp;

	rTemp.left	= (LONG)(GetPosX() ) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.top	= (LONG)(GetPosY() ) - (LONG)CCamera::GetInstance()->GetY();
	rTemp.right	= (LONG)(GetPosX() + GetWidth()  ) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.bottom= (LONG)(GetPosY() + GetHeight() ) - (LONG)CCamera::GetInstance()->GetY();

	return rTemp;
}

void CBuilding::Update(float fElapsedTime)
{

}

void CBuilding::Render()
{

}

bool CBuilding::CheckCollision(IEntity* pOther)
{
	return false;
}