/*
	File:						Storehouse.cpp
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/24/2012
	Purpose:				Contain / manage the game code
	*/

#include "StoreHouse.h"
#include "Game.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "TileManager.h"
#include "Player.h"
#include <list>
#include "Bullet.h"
#include "MessageSystem.h"
#include "DestroyStoreHouseMessage.h"
#include "DestroyBulletMessage.h"
#include "EventManager.h"
using namespace std;

CStoreHouse::CStoreHouse(void)
{
	m_eType = OBJ_STOREHOUSE;
	m_fProductionTimer = 0.0f;
	m_nFoodTiles = 0;

	m_nMaxHealth = 100;
//	m_nHealth = 100;

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	m_nStoreHouseID = pTM->LoadTexture( _T("resource/graphics/Storehouse.png"), D3DCOLOR_XRGB(255,255,255)) ;


	m_nHealthID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HealthBarFill.jpg"));


	//getting the player from the object manager
	list<IEntity*> objList = ObjectManager::GetInstance()->GetObjectList();

	for(list<IEntity*>::iterator iter = objList.begin();
		iter != objList.end(); ++iter)
	{
		m_pPlayer = nullptr;
		m_pPlayer = dynamic_cast<CPlayer*>(*iter);
		if(m_pPlayer != nullptr)
			break;
	}

	m_bCollect = true;
	m_nNumFoodCollected = 0;
}


CStoreHouse::~CStoreHouse(void)
{
}

void CStoreHouse::Update( float fElapsedTime )
{
	if( m_bCollect == true )
		CollectFood();

	if( GetHealth() <= 0 )
	{
		CMessageSystem* pMsg = CMessageSystem::GetInstance();

		//decreasing the amount of food from the player
		m_pPlayer->DecreaseFoodRate( m_nNumFoodCollected );

		//destroying the storehouse
		CDestroyStoreHouseMessage* pStoreHouseMsg = new CDestroyStoreHouseMessage(this);
		pMsg->SendMsg( pStoreHouseMsg );
		pStoreHouseMsg = nullptr;
	}
}

void CStoreHouse::Render( void )
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	RECT rSource = { 0, 0, 64, 64 };

	pTM->Draw( m_nStoreHouseID,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1.0f, 1.0f, &rSource );

	float hp = (float)GetHealth()/(float)GetMaxHealth();

	if(hp > 0)
	{
		RECT rHealthBar = { 0, 0, (LONG)(256 * hp), 32 };
		if(hp > 0 && hp <= 0.15f)
			pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
		else if(hp > 0.15f && hp < 0.3f)
			pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0));
		else if(hp > 0.3f && hp <= 1.f)
			pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	}

}

void CStoreHouse::MiniMapRender()
{

	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSource = {0, 0, 64, 64};


	pTm->Draw(m_nStoreHouseID, (int)(GetPosX() * 0.0365f) +606, (int)(GetPosY()  * 0.0365f) +406, 0.08f, 0.08f, &rSource, 0,0,0, D3DCOLOR_RGBA(255,255,255,155));

}


bool CStoreHouse::CheckCollision( IEntity *pOther )
{
	CMessageSystem* pMS = CMessageSystem::GetInstance();

	if(CEntity::CheckCollision(pOther) == false)
		return false;

	switch(pOther->GetType())
	{
	case OBJ_BULLET:
		{

			CBullet* pBullet = dynamic_cast<CBullet*>(pOther);
				CEventManager*	pEM	=CEventManager::GetInstance();

			if( pBullet->GetOwner()->GetType() == OBJ_ENEMY )
			{
				if(pBullet->GetBulletType() == ROCKET)
				pEM->sendEvent("collide_Rocket");
				else
				pEM->sendEvent("collide_Bullet");

				SetHealth( GetHealth()-pBullet->GetOwner()->GetDamage());


				CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(pBullet);
				pMS->SendMsg(pMsg);
				pMsg = nullptr;

			}
		}
		break;
	}
	return true;
}

void CStoreHouse::CollectFood()
{
	CTileManager* TileM = CTileManager::GetInstance();
	RECT rSource = { 0, 0, 64, 64 };
	//rect from which we will collect food
	RECT rFoodCollect = { (LONG)(GetPosX() - TileM->GetTileWidth()), (LONG)(GetPosY() - TileM->GetTileHeight()),
						  (LONG)(GetPosX() + GetWidth() + TileM->GetTileWidth()), (LONG)(GetPosY() + GetHeight() + TileM->GetTileHeight()) };

	//m_pPlayer->DecreaseFoodRate(m_pPlayer->GetFoodRate());

	//storing all of the tiles in layer 2 of the tile manager
	std::vector<CTileManager::Tile_L2> vTiles = TileM->GetLayer2();
	int size  = vTiles.size();

	for(unsigned int i = 0; i < vTiles.size(); i++ )
	{
		if( vTiles[i].m_nTileType == 1 )
		{
			RECT tempRect;
			RECT currTileRect = {vTiles[i].m_nX, vTiles[i].m_nY, vTiles[i].m_nX + TileM->GetTileWidth(),  vTiles[i].m_nY + TileM->GetTileHeight() };
			if( IntersectRect( &tempRect, &rFoodCollect, &currTileRect ) )
			{
				m_pPlayer->IncreaseFoodRate(1);
				m_nNumFoodCollected++;
			}
		}
	}

	m_bCollect = false;
}
