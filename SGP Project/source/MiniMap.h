#pragma once
#include "entity.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"

#include "Ally.h"
#include "Player.h"
#include "Enemy.h"
#include "Helicopter.h"
#include "TileManager.h"
#include "Camera.h"
#include "Game.h"
#include "ObjectManager.h"
#include "Bunker.h"
#include "Mine.h"
#include "StoreHouse.h"
#include "Shop.h"

#include <list>
#include <cmath>

#include "../SGD Wrappers/SGD_String.h"

//typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CMiniMap : public CEntity
{
public:
	CMiniMap(void);
	virtual ~CMiniMap(void);

	//functions that need to be overridden by the child class
	void Update( float fElapsedTime );
	void Render();

	ObjectManager* m_pOM;

	//Tile Manager
	CTileManager*				m_pTileManager;

	// Gameplay objects
	CEntity*				m_pPlayer;
	CEntity*				m_pHelicopter;
	CEntity*				m_pAlly;
	CEntity*				m_pEnemy;
	CEntity*				m_pBunker;
	CEntity*				m_pMine;
	CEntity*				m_pFarm;
	CEntity*				m_pShop;


	// Data Member:
	int m_nBackgroundImgID;

	// SGD Wrappers:
	CSGD_Direct3D*			m_pD3D;
	CSGD_TextureManager*	m_pTM;
};