/*
File:				Enemy.h
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Michael Yatckoske
Last Edited:		9/12/2012
Purpose:			Class for the enemy units in the game.
-- Will have numerous types
*/

#include "Enemy.h"
#include "Game.h"

// Wrapper Includes
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "EventManager.h"

// Message System Includes
#include "MessageSystem.h"
#include "CreateBulletMessage.h"
#include "CreatePickUpMessage.h"
#include "DestroyEnemyMessage.h"

#include "Bullet.h"
#include "Camera.h"

#include "StoreHouse.h"
#include "Mine.h"
#include "Bunker.h"

#include "AttackState.h"
#include "DefenseState.h"
#include "Ally.h"
#include "Player.h"
#include "AnimationManager.h"

// Various Shot Delays
#define	GUNNER_SHOT_DELAY	0.25
#define SNIPER_SHOT_DELAY	1.5
#define MEDIC_SHOT_DELAY	0.75
#define ROCKET_SHOT_DELAY	2.0

CEnemy::CEnemy(void)
{
	m_eType = OBJ_ENEMY;
	m_eUnitType		= eGUNNER;

	m_fFireTimer	= 0.0f;
	m_fPatrolTimer  = 0.0f;

	m_bPatrolStatus = true;

	m_pCurrAIState	= nullptr;
	m_pTarget		= nullptr;

	m_pXA  = CSGD_XAudio2::GetInstance();

	m_nDamage = 10;

	m_nSniper		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SNIPER.wav"));
	m_nMachineGun	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_MACHINE_GUNNER.wav"));
	m_nShotgun		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_SHOTGUN.wav"));
	m_nRocket		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_ROCKET.wav"));


	m_nHealthID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HealthBarFill.jpg"));


	CEventManager::GetInstance()->RegisterClient( "rocket_splash", this);
	CEventManager::GetInstance()->RegisterClient( "Helicopter_attacked", this);
	CEventManager::GetInstance()->RegisterClient( "Helicopter_flying", this);


	m_fLifeTimer	= 0.0f;

	SetVelY(0);

	m_nDamage = GetDamage();
	m_pCurrAIState = new CAttackState();

	CurrAnimation = new CAnimationInfo();

	SetAnimationID(0);


	CurrAnimation->m_nCurrentAnimation;
	CurrAnimation->m_fTimeWaited = 0.17f;
	CurrAnimation->m_nCurrentFrame = 0;
	CurrAnimation->m_bIsLooping = true;
	CurrAnimation->m_bIsPlaying = true;

	//Health
	m_nMaxHealth = 100;
	SetHealth( m_nMaxHealth );

	m_bBlood = false;			//cjm - 10/06/12

	// Spreed out when dropped by helicopter
	m_nMovementXDirection = 1;
	m_nMovementYDirection = 0;
}

CEnemy::~CEnemy(void)
{
	m_pTarget = nullptr;
	delete m_pCurrAIState;
	m_pCurrAIState = nullptr;

	delete CurrAnimation;
	CurrAnimation = nullptr;

	//if(m_nSniper != -1)
	//{
	//	m_pXA->SFXStopSound(m_nSniper);
	//	m_pXA->SFXUnloadSound(m_nSniper);
	//	m_nSniper = -1;
	//}

	//if(m_nMachineGun != -1)
	//{
	//	m_pXA->SFXStopSound(m_nMachineGun);
	//	m_pXA->SFXUnloadSound(m_nMachineGun);
	//	m_nMachineGun = -1;
	//}


	//if(m_nShotgun != -1)
	//{
	//	m_pXA->SFXStopSound(m_nShotgun);
	//	m_pXA->SFXUnloadSound(m_nShotgun);
	//	m_nShotgun = -1;
	//}

	//if(m_nRocket != -1)
	//{
	//	m_pXA->SFXStopSound(m_nRocket);
	//	m_pXA->SFXUnloadSound(m_nRocket);
	//	m_nRocket = -1;
	//}

	CEventManager::GetInstance()->UnregisterClient( "rocket_splash", this);
	CEventManager::GetInstance()->UnregisterClient( "Helicopter_attacked", this);
	CEventManager::GetInstance()->UnregisterClient( "Helicopter_flying", this);

}

void CEnemy::Update( float fElapsedTime )
{
	if( m_pCurrAIState != nullptr )
		m_pCurrAIState->Update(fElapsedTime,this);

	CGame* pGame = CGame::GetInstance();
	// Uncomment the collapsed code when bullets are being created.
	m_fFireTimer += fElapsedTime;
	m_fLifeTimer += fElapsedTime;

	CAnimationManager::GetInstance()->Update(CurrAnimation, fElapsedTime);


	//if(GetPosY() + GetHeight() > pGame->GetHeight())
	//{
	//	SetVelY(-GetVelY());
	//}

	//if(m_fLifeTimer >= 5.0f)
	//{
	//	SetHealth(0);
	//	m_fLifeTimer = 0.0f;
	//}

	//if(GetHealth() <= 0)
	//{
	//	CCreatePickUpMessage* pMsg1 = new CCreatePickUpMessage(this);
	//	CMessageSystem::GetInstance()->SendMsg(pMsg1);
	//	pMsg1 = nullptr;

	//	CDestroyEnemyMessage* pMsg2 = new CDestroyEnemyMessage(this);
	//	CMessageSystem::GetInstance()->SendMsg(pMsg2);
	//	pMsg2 = nullptr;
	//}

	CMovingObject::Update(fElapsedTime);

	switch(m_eUnitType)
	{
	case eGUNNER:
		{
			if(m_fFireTimer > GUNNER_SHOT_DELAY && GetTargetInLineofSight())
			{
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				//m_pXA->SFXPlaySound(m_nMachineGun);


				m_fFireTimer = 0.0f;
			}
		}
		break;

	case eSNIPER:
		{
			if(m_fFireTimer > SNIPER_SHOT_DELAY && GetTargetInLineofSight())
			{
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				//m_pXA->SFXPlaySound(m_nSniper);

				m_fFireTimer = 0.0f;
			}
		}
		break;

	case eMEDIC:
		{
			if(m_fFireTimer > MEDIC_SHOT_DELAY && GetTargetInLineofSight())
			{
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				m_pXA->SFXPlaySound(m_nShotgun);

				m_fFireTimer = 0.0f;
			}
		}
		break;

	case eROCKET:
		{
			if(m_fFireTimer > ROCKET_SHOT_DELAY && GetTargetInLineofSight())
			{
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;


				//m_pXA->SFXPlaySound(m_nRocket);


				m_fFireTimer = 0.0f;
			}
		}
		break;
	}

}

bool CEnemy::CheckCollision( IEntity* pOther )
{
	CMessageSystem* pMS = CMessageSystem::GetInstance();

	if(CEntity::CheckCollision(pOther) == false)
		return false;


	switch(pOther->GetType())
	{
	case OBJ_BASE:
		return false;	
	case OBJ_STOREHOUSE:
		{		
			CStoreHouse* pStoreHouse = dynamic_cast<CStoreHouse*>(pOther);

			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &pStoreHouse->GetRect()))
			{
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pStoreHouse->GetRect().left<= GetRect().right && pStoreHouse->GetRect().right>GetRect().right && pStoreHouse->GetRect().bottom>GetRect().top && pStoreHouse->GetRect().top<GetRect().bottom)
					SetPosX((float)(pStoreHouse->GetRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pStoreHouse->GetRect().right>=GetRect().left && pStoreHouse->GetRect().left<GetRect().right && pStoreHouse->GetRect().bottom>GetRect().top && pStoreHouse->GetRect().top<GetRect().bottom)
					SetPosX((float)(pStoreHouse->GetRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pStoreHouse->GetRect().top<=GetRect().bottom && pStoreHouse->GetRect().bottom>GetRect().bottom && pStoreHouse->GetRect().right>GetRect().left && pStoreHouse->GetRect().left<GetRect().right)
					SetPosY((float)(pStoreHouse->GetRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pStoreHouse->GetRect().bottom>=GetRect().top && pStoreHouse->GetRect().top<GetRect().top && pStoreHouse->GetRect().right>GetRect().left && pStoreHouse->GetRect().left<GetRect().right)
					SetPosY((float)(pStoreHouse->GetRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));		
			}

			break;
		}
	case OBJ_MINE:
		{
			CMine* pMine = dynamic_cast<CMine*>(pOther);
			
			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &pMine->GetRect()))
			{
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pMine->GetRect().left<= GetRect().right && pMine->GetRect().right>GetRect().right && pMine->GetRect().bottom>GetRect().top && pMine->GetRect().top<GetRect().bottom)
					SetPosX((float)(pMine->GetRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pMine->GetRect().right>=GetRect().left && pMine->GetRect().left<GetRect().right && pMine->GetRect().bottom>GetRect().top && pMine->GetRect().top<GetRect().bottom)
					SetPosX((float)(pMine->GetRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pMine->GetRect().top<=GetRect().bottom && pMine->GetRect().bottom>GetRect().bottom && pMine->GetRect().right>GetRect().left && pMine->GetRect().left<GetRect().right)
					SetPosY((float)(pMine->GetRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pMine->GetRect().bottom>=GetRect().top && pMine->GetRect().top<GetRect().top && pMine->GetRect().right>GetRect().left && pMine->GetRect().left<GetRect().right)
					SetPosY((float)(pMine->GetRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));		
			}

			return false;
		}
	case OBJ_BUNKER:
		{
			CBunker* pBunker = dynamic_cast<CBunker*>(pOther);

			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &pBunker->GetRect()))
			{
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pBunker->GetRect().left<= GetRect().right && pBunker->GetRect().right>GetRect().right && pBunker->GetRect().bottom>GetRect().top && pBunker->GetRect().top<GetRect().bottom)
					SetPosX((float)(pBunker->GetRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pBunker->GetRect().right>=GetRect().left && pBunker->GetRect().left<GetRect().right && pBunker->GetRect().bottom>GetRect().top && pBunker->GetRect().top<GetRect().bottom)
					SetPosX((float)(pBunker->GetRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pBunker->GetRect().top<=GetRect().bottom && pBunker->GetRect().bottom>GetRect().bottom && pBunker->GetRect().right>GetRect().left && pBunker->GetRect().left<GetRect().right)
					SetPosY((float)(pBunker->GetRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pBunker->GetRect().bottom>=GetRect().top && pBunker->GetRect().top<GetRect().top && pBunker->GetRect().right>GetRect().left && pBunker->GetRect().left<GetRect().right)
					SetPosY((float)(pBunker->GetRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));		
			}		

			return false;
		}
	case OBJ_ENEMY:
		{		

			CEnemy* pObj = dynamic_cast<CEnemy*>(pOther);

			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &pObj->GetRect()))
			{
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pObj->GetRect().left<= GetRect().right && pObj->GetRect().right>GetRect().right && pObj->GetRect().bottom>GetRect().top && pObj->GetRect().top<GetRect().bottom)
					SetPosX((float)(pObj->GetRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pObj->GetRect().right>=GetRect().left && pObj->GetRect().left<GetRect().right && pObj->GetRect().bottom>GetRect().top && pObj->GetRect().top<GetRect().bottom)
					SetPosX((float)(pObj->GetRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pObj->GetRect().top<=GetRect().bottom && pObj->GetRect().bottom>GetRect().bottom && pObj->GetRect().right>GetRect().left && pObj->GetRect().left<GetRect().right)
					SetPosY((float)(pObj->GetRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pObj->GetRect().bottom>=GetRect().top && pObj->GetRect().top<GetRect().top && pObj->GetRect().right>GetRect().left && pObj->GetRect().left<GetRect().right)
					SetPosY((float)(pObj->GetRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));	
			}

			/*pObj->SetPosX(GetPosX() + 48);
			pObj->SetPosY(GetPosY() );

			SetPosX(GetPosX() - 48);
			SetPosY(GetPosY());*/

			break;
		}

	case OBJ_BULLET:
		{
			CBullet* pBullet = dynamic_cast<CBullet*>(pOther);

			for(unsigned int i = 0; i < pBullet->m_vSniperHits.size(); ++i)
			{
				if(pBullet->m_vSniperHits[i] == this)
				{
					return false;
				}
			}

			int nType = pBullet->GetOwner()->GetType();
			if(nType != OBJ_ENEMY)
			{
				SetHealth(GetHealth() - pBullet->GetOwner()->GetDamage());
				m_bBlood = true;												//cjm - 10/08/12

				if(GetHealth()  <= 0)
				{
					CCreatePickUpMessage* pMsg1 = new CCreatePickUpMessage(this);
					CMessageSystem::GetInstance()->SendMsg(pMsg1);
					pMsg1 = nullptr;

					CDestroyEnemyMessage* pMsg = new CDestroyEnemyMessage(this);
					pMS->SendMsg(pMsg);
					pMsg = nullptr;					
				}
			}
		}
		break;

	case OBJ_BUILDING:
		return false;

	case OBJ_PICKUP:
		return false;


	};

	return true;
}

void CEnemy::Render()
{
	//RECT rSource = { GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight() };
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rsource = { 100, 0, 180, 100 };
	CAnimationManager::GetInstance()->Render(CurrAnimation, GetImageID() ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1.0f, 1.0f, (rsource.left + rsource.right) *0.5f,(rsource.top + rsource.bottom) *0.5f, GetRotation(), D3DCOLOR_RGBA(255,255,255,255));
	
	if (m_bBlood)																							//cjm - 10/06/12
	{																										//cjm - 10/06/12
	CParticleManager::GetInstance()->Render("blood_nav",(int)(GetPosX()-32), (int)(GetPosY()-32), 0.0f);				
	//cjm - 10/06/12
	m_bBlood = false;																						//cjm - 10/06/12
	}																										//cjm - 10/06/12
	
	/*switch(m_eUnitType)
	{
	case GUNNER:
		pTm->Draw(GetImageID(), (int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY(),1.0f,1.0f,nullptr,GetWidth()*0.5f,GetHeight()*0.5f,GetRotation());
		break;

	case SNIPER:
		pTm->Draw(GetImageID(), (int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY(),1.0f,1.0f,nullptr,GetWidth()*0.5f,GetHeight()*0.5f,GetRotation());
		break;

	case MEDIC:
		pTm->Draw(GetImageID(), (int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY(),1.0f,1.0f,nullptr,GetWidth()*0.5f,GetHeight()*0.5f,GetRotation());
		break;

	case ROCKET:
		pTm->Draw(GetImageID(), (int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY(),1.0f,1.0f,nullptr,GetWidth()*0.5f,GetHeight()*0.5f,GetRotation());
		break;
	};*/

	// Draw Health Bar
	float hp = (float)GetHealth()/(float)GetMaxHealth();


	if(hp > 0)
	{
		RECT rHealthBar = { 0, 0, (LONG)(256 * hp), 32 };
		if(hp > 0 && hp <= 0.15f)
			pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY() ), 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
		else if(hp > 0.15f && hp < 0.3f)
			pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY() ), 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0));
		else if(hp > 0.3f && hp <= 1.f)
			pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY() ), 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	}
}

void CEnemy::HandleEvent(IEvent* pEvent)
{
	if(pEvent->GetEventID() == "rocket_splash")
	{
		CEntity* owner = (CEntity*)pEvent->GetVariable();
		switch(owner->GetType())
		{
		case OBJ_ENEMY:
			{



			}
			break;

		case OBJ_ALLY:
			{
				CAlly* pAlly = dynamic_cast<CAlly*>(owner);
				TakeDamage(pAlly->GetDamage());
			}
			break;

		case OBJ_PLAYER:
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(owner);
				TakeDamage(pPlayer->GetDamage());
			}
			break;
		};
	}

	if(pEvent->GetEventID() == "Helicopter_attacked")
	{
		delete m_pCurrAIState;
		m_pCurrAIState = nullptr;
		m_pCurrAIState = new CDefenseState();		
	}

	if(pEvent->GetEventID() == "Helicopter_flying")
	{
		delete m_pCurrAIState;
		m_pCurrAIState = nullptr;
		m_pCurrAIState = new CAttackState();		
	}
}
void CEnemy::MiniMapRender()
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	//RECT rSource = { GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight() };
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSource = {0, 0, 64, 64};

	pTm->Draw(GetImageID(), (int)((GetPosX() *0.0365f ) + 606), (int)((GetPosY() * 0.0365f) + 406), 0.09f, 0.09f, &rSource ,GetWidth()*0.5f,GetHeight()*0.5f,GetRotation(), D3DCOLOR_RGBA(255,0,0,255));

	 
}



void CEnemy::TakeDamage( int nDamage )
{
	if( m_nHealth > 0)
		m_nHealth -= nDamage;
	if( m_nHealth < 0 )
		m_nHealth = 0;
}

bool CEnemy::CheckTileCollision()
{
	CTileManager* pTileManager = CTileManager::GetInstance();


	std::vector<CTileManager::Tile_L1> vTile = pTileManager->GetLayer1();

	for(unsigned int i = 0; i < vTile.size(); i++)
	{
		if(vTile[i].m_bIsCollidable)
		{
			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &vTile[i].GetCollisionRect()))
			{			
			
				//CEventManager::GetInstance()->sendEvent(vTile[i].m_szEventName);
				
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && vTile[i].GetCollisionRect().left<= GetRect().right && vTile[i].GetCollisionRect().right>GetRect().right && vTile[i].GetCollisionRect().bottom>GetRect().top && vTile[i].GetCollisionRect().top<GetRect().bottom)
				{
					SetPosX((float)(vTile[i].GetCollisionRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				}
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && vTile[i].GetCollisionRect().right>=GetRect().left && vTile[i].GetCollisionRect().left<GetRect().right && vTile[i].GetCollisionRect().bottom>GetRect().top && vTile[i].GetCollisionRect().top<GetRect().bottom)
				{
					SetPosX((float)(vTile[i].GetCollisionRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				}
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && vTile[i].GetCollisionRect().top<=GetRect().bottom && vTile[i].GetCollisionRect().bottom>GetRect().bottom && vTile[i].GetCollisionRect().right>GetRect().left && vTile[i].GetCollisionRect().left<GetRect().right)
				{	
					SetPosY((float)(vTile[i].GetCollisionRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				}
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && vTile[i].GetCollisionRect().bottom>=GetRect().top && vTile[i].GetCollisionRect().top<GetRect().top && vTile[i].GetCollisionRect().right>GetRect().left && vTile[i].GetCollisionRect().left<GetRect().right)
				{
					SetPosY((float)(vTile[i].GetCollisionRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));	
				}		

				return true;
			}
		}
	}
	return false;

}