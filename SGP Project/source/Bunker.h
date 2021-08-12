/*
	File:						Bunker.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/26/2012
	Purpose:				Contain / manage the game code
*/

#pragma once
#include "building.h"
#include <list>
#include "ObjectManager.h"
#include "Entity.h"

#include "../SGD Wrappers/SGD_String.h"

#include "../SGD Wrappers/CSGD_TextureManager.h"

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CAlly;

class CBunker :public CBuilding
{
private:
	//number of units currently inside of the bunker
	int				m_nNumAllies;

	//img id
	int				m_nBunkerImgID;
	int				m_nHealthID;

	//Bunker ids
	int				m_nBunkerID;

	//Bunker Health	
	int				m_nMaxHealth;	

	//Wrappers
	CSGD_TextureManager*				m_pTM;
	ObjectManager*						m_pOM;

public:
	CBunker(void);
	virtual ~CBunker(void);

	//IEntity interface functions
	virtual void Update( float fElapsedTime );
	virtual void Render();

	virtual bool CheckCollision( IEntity* pOther );

	void Evacuate();

	//accessors
	int GetBunkerID()					{return m_nBunkerID;}
	int GetMaxHealth()					{ return m_nMaxHealth; }
	int GetNumAllies()	const			{ return m_nNumAllies; }
	//Mutators
	void SetBunkerID(int bunkerID)		{ m_nBunkerID = bunkerID; }
	void SetMaxHealth(int health)		{ m_nMaxHealth+=health; }
	void IncreaseAllyCount()			{ m_nNumAllies++;}

	void MiniMapRender();
};

