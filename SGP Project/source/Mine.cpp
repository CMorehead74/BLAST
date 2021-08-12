/*
	File:						Mine.cpp
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/27/2012
	Purpose:				Mine that will collect ore
*/

#include "Mine.h"
#include "StoreHouse.h"
#include "Game.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "TileManager.h"
#include "Player.h"
#include <list>
#include "Bullet.h"
#include "MessageSystem.h"
#include "DestroyMineMessage.h"
#include "DestroyBulletMessage.h"
using namespace std;

CMine::CMine(void)
{
	m_eType = OBJ_MINE;
	m_fProductionTimer = 0.0f;
	m_nOreTiles = 0;

	m_nMaxHealth = 100;
	SetHealth( 200 );
	//m_nHealth = 100;

	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	m_nMineID = pTM->LoadTexture( _T("resource/graphics/Mine.png"), D3DCOLOR_XRGB(255,255,255)) ;
	
	
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
	m_nNumOreCollected = 0;
}


CMine::~CMine(void)
{
}

void CMine::Update( float fElapsedTime )
{
	if( m_bCollect == true )
		CollectOre();

	if( GetHealth() <= 0 )
	{
		CMessageSystem* pMsg = CMessageSystem::GetInstance();

		//decreasing the amount of ore from the player
		m_pPlayer->DecreaseOreRate( m_nNumOreCollected );

		//destroying the mine
		CDestroyMineMessage* pMineMsg = new CDestroyMineMessage(this);
		pMsg->SendMsg( pMineMsg );
		pMineMsg = nullptr;
	}
}

void CMine::Render( void )
{
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	RECT rSource = { 0, 0, 64, 32 };

	pTM->Draw( GetMineID(), (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1.0f, 1.0f, &rSource );

	float hp = (float)GetHealth()/(float)GetMaxHealth();

	if(hp > 0)
	{
		RECT rHealthBar = { 0, 0, (LONG)(256 * hp), 32 };
		if(hp > 0 && hp <= 0.15f)
			pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
		else if(hp > 0.15f && hp < 0.3f)
			pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0));
		else if(hp > 0.3f && hp <= 1.f)
			pTM->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	}


}

void CMine::MiniMapRender()
{

	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSource = {0, 0, 64, 64};


	pTm->Draw(m_nMineID, (int)((GetPosX() * 0.0365f) +606), (int)((GetPosY()  * 0.0365f) +406), 0.08f, 0.08f, &rSource, 0,0,0, D3DCOLOR_RGBA(255,255,255,155));


}

bool CMine::CheckCollision( IEntity *pOther )
{
	CMessageSystem* pMS = CMessageSystem::GetInstance();

	if(CEntity::CheckCollision(pOther) == false)
		return false;

	switch(pOther->GetType())
	{
	case OBJ_BULLET:
		CBullet* pBullet = dynamic_cast<CBullet*>(pOther);

		if( pBullet->GetOwner()->GetType() == OBJ_ENEMY )
		{
			CEventManager*	pEM	= CEventManager::GetInstance();

			if(pBullet->GetAnimationID() == ROCKET)
				pEM->sendEvent("collide_Rocket");
			else
				pEM->sendEvent("collide_Bullet");

			SetHealth( GetHealth()-pBullet->GetOwner()->GetDamage());

			CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(pBullet);
			pMS->SendMsg(pMsg);
			pMsg = nullptr;

		}
		break;
	};

	return true;
}

void CMine::CollectOre()
{
	CTileManager* TileM = CTileManager::GetInstance();
	RECT rSource = { 0, 0, 64, 64 };
	//rect from which we will collect food
	RECT rOreCollect= { (LONG)(GetPosX() - TileM->GetTileWidth()), (LONG)(GetPosY() - TileM->GetTileHeight()),
		(LONG)(GetPosX() + GetWidth() + TileM->GetTileWidth()), (LONG)(GetPosY() + GetHeight() + TileM->GetTileHeight()) };

	//m_pPlayer->DecreaseOreRate(m_pPlayer->GetOreRate());

	//storing all of the tiles in layer 2 of the tile manager
	std::vector<CTileManager::Tile_L2> vTiles = TileM->GetLayer2();
	int size  = vTiles.size();

	for(unsigned int i = 0; i < vTiles.size(); i++ )
	{
		if( vTiles[i].m_nTileType == 2 )
		{
			RECT tempRect;
			RECT currTileRect = {vTiles[i].m_nX, vTiles[i].m_nY, vTiles[i].m_nX + TileM->GetTileWidth(),  vTiles[i].m_nY + TileM->GetTileHeight() };
			if( IntersectRect( &tempRect, &rOreCollect, &currTileRect ) )
			{
				m_pPlayer->IncreaseOreRate(1);
				m_nNumOreCollected++;
			}
		}
	}

	m_bCollect = false;
}