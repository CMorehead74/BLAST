/*
	File:				PickUp.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/14/2012
	Purpose:			Class to store the item drops (pick ups) from the enemies
						for the player to use.

						These picks ups include:
						--	Ammo
						--	Cash
						--	Weapons
							--	Machine Gun
							--	Shotgun
							--	Sniper Rifle
							--	Rocket Launcher
*/

#include "PickUp.h"

#include "Game.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "MessageSystem.h"
#include "DestroyPickUpMessage.h"
#include "Camera.h"

CPickUp::CPickUp(void)
{
	m_eType = OBJ_PICKUP;

	CGame* pGame = CGame::GetInstance();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	SetPosX((float)(rand()%pGame->GetWidth()));
	SetPosY((float)(rand()%pGame->GetHeight()));

	m_fLifeTimer = 0.0f;

	int type = rand() % 15;
	
	switch(type)
			{
			case 0:
			case 14:
			case 6:
			case 7:
			case 8:
				{
					m_ePickUpType = OBJ_AMMO;
				}
				break;

			case 1:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
				{
					m_ePickUpType = OBJ_CASH;
				}
				break;

			case 2:
				{
					m_ePickUpType = OBJ_MACHINEGUN;
				}
				break;

			case 3:
				{
					m_ePickUpType = OBJ_SHOTGUN;
				}
				break;

			case 4:
				{
					m_ePickUpType = OBJ_SNIPER;
				}
				break;

			case 5:
				{
					m_ePickUpType = OBJ_ROCKET;
				}
				break;
			};

}


CPickUp::~CPickUp(void)
{
}

void CPickUp::Update(float fElapsedTime)
{
	m_fLifeTimer += fElapsedTime;
	if(m_fLifeTimer >= 10.0f)
	{
		CDestroyPickUpMessage* pMsg2 = new CDestroyPickUpMessage(this);
		CMessageSystem::GetInstance()->SendMsg(pMsg2);
		pMsg2 = nullptr;
	}
}

void CPickUp::Render()
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	pTM->Draw(GetImageID(),(int)(GetPosX() - GetWidth()*0.5f - CCamera::GetInstance()->GetX()), (int)(GetPosY() - GetHeight()*0.5f - CCamera::GetInstance()->GetY()));
}

bool CPickUp::CheckCollision(IEntity* pOther)
{

	return false;
}

RECT CPickUp::GetRect() const
{
	RECT rTemp;

	rTemp.left	= (LONG)(GetPosX()) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.top	= (LONG)(GetPosY()) - (LONG)CCamera::GetInstance()->GetY();
	rTemp.right	= (LONG)(GetPosX() + GetWidth() ) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.bottom= (LONG)(GetPosY() + GetHeight()) - (LONG)CCamera::GetInstance()->GetY();

	/*if(this->GetType() == OBJ_HELICOPTER)
	{
		rTemp.left	= (LONG)GetPosX();
		rTemp.top	= (LONG)(GetPosY()	+ 79);
		rTemp.right	= (LONG)(rTemp.left	+ 224);
		rTemp.bottom= (LONG)(rTemp.top	+ 35);
	}*/

	return rTemp;
}