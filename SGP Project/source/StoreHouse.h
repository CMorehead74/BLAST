/*
	File:						Storehouse.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/24/2012
	Purpose:				Contain / manage the game code
	*/

#pragma once
#include "Building.h"

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

//typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CStoreHouse :public CBuilding
{
private:
	//the number of surrounding food tiles
	int						m_nFoodTiles;

	//the amount of time it takes to increment the food
	float					m_fProductionTimer;

	//Health of the storehouse
	int						m_nHealth;
	int						m_nMaxHealth;


	// StoreHouse ID's
	int						m_nStoreHouseID;
	int						m_nHealthID;

	//player we'll be passing information to.
	CPlayer*			m_pPlayer;

	//bool that says whether we should collect food or not
	bool					m_bCollect;

	//amount of food being collected
	int						m_nNumFoodCollected;

public:
	CStoreHouse(void);
	virtual ~CStoreHouse(void);
	

	//IDs
	int GetMineID()					{return m_nStoreHouseID;}
	void SetMineID(int nStoreHouseID)		{ m_nStoreHouseID = nStoreHouseID; }



	// Health 
	int GetMaxHealth()					{ return m_nMaxHealth; }
	void SetMaxHealth(int nhealth)		{ m_nMaxHealth+= nhealth; }

	//get the amount of food being collected at the moment
	int GetNumFood()					{ return m_nNumFoodCollected; }
	
	// Render on the mini map
	void MiniMapRender();



	// IEntity Interface functions
	virtual void Update( float fElapsedTime );
	virtual void Render(void);

	virtual bool CheckCollision( IEntity *pOther);

	void CollectFood();
};

