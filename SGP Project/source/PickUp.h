/*
	File:				PickUp.h
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

#pragma once
#include "Entity.h"

class CPickUp : public CEntity
{
protected:
	enum PickUpType	{ OBJ_AMMO, OBJ_CASH, OBJ_MACHINEGUN, OBJ_SHOTGUN, OBJ_SNIPER, OBJ_ROCKET };
	PickUpType m_ePickUpType;

public:
	CPickUp(void);
	virtual ~CPickUp(void);

	int GetPickUpType(void) const { return m_ePickUpType; }

	void Update( float fElapsedTime );
	void Render(void);
	bool CheckCollision( IEntity *pOther);
	RECT GetRect(void) const;

private:
	float			m_fLifeTimer;
};