/*
	File:						Mine.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Mine that will collect ore
*/

#pragma once
#include "building.h"

//forward declaration of the player
class CPlayer;

#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/IListener.h"
#include "MessageSystem.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "Entity.h"

class CMine :public CBuilding
{
private:
	//the number of surrounding food tiles
	int						m_nOreTiles;

	//the amount of time it takes to increment the food
	float					m_fProductionTimer;

	//Health of the storehouse
	int						m_nMaxHealth;


	//Mine Ids
	int						m_nMineID;
	int						m_nHealthID;

	//player we'll be passing information to.
	CPlayer*			m_pPlayer;

	//bool that lets us that it is ok to collect ore
	bool					m_bCollect;

	//the number of ore currently being collected
	int						m_nNumOreCollected;

public:
	CMine(void);
	virtual ~CMine(void);

	// IEntity Interface functions
	virtual void Update( float fElapsedTime );
	virtual void Render(void);

	virtual bool CheckCollision( IEntity *pOther);


	int GetMineID()							{return m_nMineID;}
	void SetMineID(int nMineID)		{ m_nMineID = nMineID; }


	int GetMaxHealth()						{ return m_nMaxHealth; }
	void SetMaxHealth(int nhealth)	{ m_nMaxHealth+= nhealth; }

	//Getting the amount of ore currently being generated
	int GetNumOre()						{ return m_nNumOreCollected; }

	// Render on the mini map
	void MiniMapRender();

	void CollectOre();
};