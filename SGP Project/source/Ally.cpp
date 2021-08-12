/*
	File:						Ally.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:		Schadrac Francois
	Last Edited:			10/9/2012
	Purpose:				Handles the allied units
*/

#include "Ally.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "EventManager.h"
#include "CreateBulletMessage.h"
#include "DestroyAllyMessage.h"
#include "MessageSystem.h"
#include "Camera.h"
#include "Bullet.h"

#include "StoreHouse.h"
#include "Mine.h"
#include "Bunker.h"

//Animation
#include "AnimationManager.h"

//AI States Header Includes
#include "AttackState.h"
#include "DefenseState.h"
#include "FollowState.h"
#include "IdleState.h"
#include "PatrolState.h"



#include "Enemy.h"

#define	GUNNER_SHOT_DELAY	0.25
#define SNIPER_SHOT_DELAY	1.5
#define MEDIC_SHOT_DELAY	0.75
#define ROCKET_SHOT_DELAY	2.0
#define FOOD_TiMER 3.0

CAlly::CAlly(void)
{
	m_eType = OBJ_ALLY;
	m_pPM = CParticleManager::GetInstance();
//	m_pPM->CreateEmitter("fire");
	//m_bShootingParticle = false;
	//m_pShootingEffect = nullptr;

	CEventManager::GetInstance()->RegisterClient( "attack", this );
	CEventManager::GetInstance()->RegisterClient( "defend", this );
	CEventManager::GetInstance()->RegisterClient( "follow", this );
	CEventManager::GetInstance()->RegisterClient( "right", this );
	CEventManager::GetInstance()->RegisterClient( "rocket_splash", this);
	CEventManager::GetInstance()->RegisterClient( "medic_heal", this);
	m_pCurrAIState = new CIdleState();
	
	m_fHealTimer = 0.0f;

	m_nConsumptionRate = 3;
	m_fFireTimer = 0.0f;
	m_nDamage = 10;
	m_nArmor = 0;
	m_fFoodTimer = 0.0f;

	m_nFormationIndex = -1;

	SetArmor( m_nArmor );
	SetDamage( m_nDamage );

	m_nHealthID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HealthBarFill.jpg"));

	//CAnimationManager::GetInstance()->LoadAnimationFile("sprite_medic.xml");		//cjm - 10/04/12 


	m_bBlood = false;
	m_nBunkerID = -1;

	m_pOM = ObjectManager::GetInstance();

	//Health
	m_nMaxHealth = 100;
	SetHealth( m_nMaxHealth );
	SetTarget(nullptr);

	//is the ally in the bunker
	m_bIsBunkered = false;


	// Animations
	CurrAnimation = new CAnimationInfo();
	
	SetAnimationID(0);

	CurrAnimation->m_nCurrentAnimation;
	CurrAnimation->m_fTimeWaited = 0.17f;
	CurrAnimation->m_nCurrentFrame = 0;
	CurrAnimation->m_bIsLooping = true;
	CurrAnimation->m_bIsPlaying = true;


	//CurrAnimationShotgun = new CAnimationInfo();

	//CurrAnimationShotgun->m_nCurrentAnimation = 6;
	//CurrAnimationShotgun->m_fTimeWaited = 0.17f;
	//CurrAnimationShotgun->m_nCurrentFrame = 0;
	//CurrAnimationShotgun->m_bIsLooping = true;
	//CurrAnimationShotgun->m_bIsPlaying = true;


	list<IEntity*> objList = ObjectManager::GetInstance()->GetObjectList();
	for(std::list<IEntity*>::iterator iter = objList.begin();
		iter != objList.end(); ++iter)
	{
		if((*iter)->GetType() == CEntity::OBJ_PLAYER)
			m_pPlayer = dynamic_cast<CPlayer*>(*iter);		
	}
}


CAlly::~CAlly(void)
{
	CEventManager::GetInstance()->UnregisterClient( "attack", this );
	CEventManager::GetInstance()->UnregisterClient( "defend", this );
	CEventManager::GetInstance()->UnregisterClient( "follow", this );
	CEventManager::GetInstance()->UnregisterClient( "right", this );
	CEventManager::GetInstance()->UnregisterClient( "rocket_splash", this);
	CEventManager::GetInstance()->UnregisterClient( "medic_heal", this);

	delete m_pCurrAIState;

	//delete CurrAnimationShotgun;
	//CurrAnimationShotgun = nullptr;
	//
	delete CurrAnimation;
	CurrAnimation = nullptr;
}

void CAlly::Update( float fElapsedTime )
{
	m_fFireTimer += fElapsedTime;
	m_fFoodTimer += fElapsedTime;
	m_fHealTimer += fElapsedTime;

	//Animation update
	CAnimationManager::GetInstance()->Update(CurrAnimation, fElapsedTime);
	//CAnimationManager::GetInstance()->Update(CurrAnimationShotgun, fElapsedTime);

	//CurrAnimation->m_nCurrentAnimation = GetAnimationID();
	//updating the current AI state
	if(m_pCurrAIState != nullptr)
		m_pCurrAIState->Update(fElapsedTime,this);
	
	//decreasing the amount of food needed to sustain this ally
	if ( m_fFoodTimer >= FOOD_TiMER )
	{
		if( m_pPlayer->GetFood() > m_nConsumptionRate )
		{
			m_pPlayer->DecreaseFood( (float)m_nConsumptionRate );
			m_fFoodTimer = 0.0f;
		}
		else
		{
			SetHealth( GetHealth() - 25 );
			m_fFoodTimer = 0.0f;
		}

	}


	if( GetHealth() > 0 )
	{
		m_nDamage = GetDamage();
		m_nArmor = GetArmor();
	}

	switch( m_nAllyType )
	{
	case ALLY_GUNNER:
		{
			if(m_fFireTimer > GUNNER_SHOT_DELAY && GetTargetInLineofSight())
			{
			//m_bShootingParticle = true;
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				m_fFireTimer = 0.0f;
			}
			 
		}
		break;
	case ALLY_SNIPER:
		{
			if(m_fFireTimer >SNIPER_SHOT_DELAY && GetTargetInLineofSight())
			{
				//m_bShootingParticle = true;
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				m_fFireTimer = 0.0f;
			}
		}
		break;
	case ALLY_ROCKET:
		{
			if(m_fFireTimer > ROCKET_SHOT_DELAY && GetTargetInLineofSight())
			{
				//m_bShootingParticle = true;
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				m_fFireTimer = 0.0f;
			}
		}
		break;
	case ALLY_MEDIC:
		{
			if( m_fFireTimer >MEDIC_SHOT_DELAY && GetTargetInLineofSight())
			{
				//m_bShootingParticle = true;

				//CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				//CMessageSystem::GetInstance()->SendMsg(pMsg);
				CCreateBulletMessage* pMsg = new CCreateBulletMessage(this);
				CMessageSystem::GetInstance()->SendMsg(pMsg);
				pMsg = nullptr;

				m_fFireTimer = 0.0f;
			}


			RECT heal = {}, rOverlap = {};
			heal.left	= (LONG)(GetPosX() - 48) - (LONG)CCamera::GetInstance()->GetX();
			heal.top	= (LONG)(GetPosY() - 48) - (LONG)CCamera::GetInstance()->GetY();
			heal.right	= (LONG)(GetPosX() + 48) - (LONG)CCamera::GetInstance()->GetX();
			heal.bottom	= (LONG)(GetPosY() + 48) - (LONG)CCamera::GetInstance()->GetY();

			list<IEntity*> objList = m_pOM->GetObjectList();

			for(std::list<IEntity*>::iterator iter = objList.begin();
				iter != objList.end(); ++iter)
			{
				if((*iter)->GetType() == OBJ_ALLY || (*iter)->GetType() == OBJ_PLAYER)
				{
					if(IntersectRect(&rOverlap, &heal, &(*iter)->GetRect()))
					{
						if(m_fHealTimer >= 3.0f)
						{
							// If heal rect hits something not an enemy, send an event
							CEntity* hit = dynamic_cast<CEntity*>((*iter));
							if((*iter)->GetType() == OBJ_PLAYER)
							{
								CPlayer* obj = dynamic_cast<CPlayer*>((*iter));
								if(obj->GetHealth() >= obj->GetMaxHealth())
									break;
								hit->SetHealth(hit->GetHealth() + 30);
								m_fHealTimer = 0.0f;
								CEventManager::GetInstance()->sendEvent("healed");
								if(obj->GetHealth() > obj->GetMaxHealth())
									obj->SetHealth(obj->GetMaxHealth());
							}
							else if((*iter)->GetType() == OBJ_ALLY)
							{
								CAlly* obj = dynamic_cast<CAlly*>((*iter));
								if(obj->GetHealth() >= obj->GetMaxHealth())
									break;
								hit->SetHealth(hit->GetHealth() + 30);
								m_fHealTimer = 0.0f;
								if(obj->GetHealth() > obj->GetMaxHealth())
									obj->SetHealth(obj->GetMaxHealth());
							}
						}
					}
				}
			}


		}
		break;
	}

	if(GetHealth()<= 0)
	{
		CMessageSystem* pMS = CMessageSystem::GetInstance();
		CDestroyAllyMessage* pMsg = new CDestroyAllyMessage(this);
		pMS->SendMsg(pMsg);
		pMsg = nullptr;
	}

	CMovingObject::Update(fElapsedTime);
}



void CAlly::Render()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	if ( m_bIsBunkered == false )
	{

		//pTm->Draw(GetImageID(), (int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY(), 1.0f, 1.0f, nullptr,GetWidth()*0.5f,GetHeight()*0.5f,GetRotation());
		if (m_bBlood)																									//cjm - 10/05/12
		{																												//cjm - 10/05/12
			CParticleManager::GetInstance()->Render("blood_nav", (int)(GetPosX() - 32), (int)(GetPosY() - 32 ), 0.0f);	//cjm - 10/05/12
			m_bBlood = false;																							//cjm - 10/05/12
		}																												//cjm - 10/05/12


		RECT rsource = { 0, 0, 64, 64 };

		//if(CurrAnimation->m_nCurrentAnimation  == 0 || CurrAnimation->m_nCurrentAnimation == 5)



		CAnimationManager::GetInstance()->Render(CurrAnimation,GetImageID() ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1, 1, 0,0,GetRotation(), D3DCOLOR_RGBA(255,255,255,255));


		//CAnimationManager::GetInstance()->Render(CurrAnimationShotgun,GetImageID() ,(int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY(), 1, 1, 0,0,GetRotation(), D3DCOLOR_RGBA(255,255,255,255));


		/*	if (m_bShootingParticle == true && m_pShootingEffect != nullptr )
		{
		m_pShootingEffect->Render();
		m_bShootingParticle = false;
		}*/

		// Draw Health Bar
		float hp = (float)GetHealth()/(float)GetMaxHealth();
		if(hp > 0)
		{
			RECT rHealthBar = { 0, 0, (LONG)(256 * hp), 32 };
			if(hp > 0 && hp <= 0.15f)
				pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
			else if(hp > 0.15f && hp < 0.3f)
				pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0));
			else if(hp > 0.3f && hp <= 1.f)
				pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()),(int)(GetPosY() - CCamera::GetInstance()->GetY()) , 0.10f, 0.10f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
		}
	}

}



void CAlly::MiniMapRender()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSource = {0, 0, 64, 64};


	pTm->Draw(GetImageID(), (int)((GetPosX() * 0.0365f) +606), (int)((GetPosY()  * 0.0365f) +406), 0.09f, 0.09f, &rSource, 0,0,0, D3DCOLOR_RGBA(0,255,0,155));

}

bool CAlly::CheckCollision( IEntity* pOther )
{
	CMessageSystem* pMS = CMessageSystem::GetInstance();

	if(CEntity::CheckCollision(pOther) == false)
		return false;
	
	switch(pOther->GetType())
	{
	case OBJ_ALLY:
		{		

			CAlly* pObj = dynamic_cast<CAlly*>(pOther);

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
			if(nType == OBJ_ENEMY)
			{

				if( GetArmor() < pBullet->GetOwner()->GetDamage())
				{
					SetHealth(GetHealth() - (pBullet->GetOwner()->GetDamage() - GetArmor()));
					m_bBlood = true;																//cjm - 10/05/12
				}
				else
				{
					SetHealth(GetHealth());
					m_bBlood = true;																//cjm - 10/05/12
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

void CAlly::HandleEvent( IEvent* pEvent )
{	

	if( pEvent->GetEventID() == "attack" && m_bIsBunkered == false)
	{
		CEventManager::GetInstance()->sendEvent("confirmed");
		delete m_pCurrAIState;
		m_pCurrAIState = nullptr;
		m_pCurrAIState = new CAttackState();
		//SetVelY( -100 );
	}
	if( pEvent->GetEventID() == "defend" )
	{
		CEventManager::GetInstance()->sendEvent("confirmed");
		delete m_pCurrAIState;
		m_pCurrAIState = nullptr;
		m_pCurrAIState = new CDefenseState();
		//SetVelY( 100 );
	}
	if( pEvent->GetEventID() == "follow" && m_bIsBunkered == false)
	{
		CEventManager::GetInstance()->sendEvent("confirmed");
		delete m_pCurrAIState;
		m_pCurrAIState = nullptr;
		m_pCurrAIState = new CFollowState();
		//SetVelX( - 100 );
	}
	if( pEvent->GetEventID() == "right" )
	{
		SetVelX( 100 );
	}

	if(pEvent->GetEventID() == "medic_heal")
	{
		CEntity* owner = (CEntity*)pEvent->GetVariable();
		switch(owner->GetType())
		{
		case OBJ_ENEMY:
			{
				CEnemy* pEnemy = dynamic_cast<CEnemy*>(owner);
				SetHealth(GetHealth() - pEnemy->GetDamage());
			}
			break;

		case OBJ_ALLY:
			break;

		case OBJ_PLAYER:
			{
				/*CPlayer* pPlayer = dynamic_cast<CPlayer*>(owner);
				TakeDamage(pPlayer->GetDamage());*/
			}
			break;
		};
	}

	if(pEvent->GetEventID() == "rocket_splash")
	{
		CEntity* owner = (CEntity*)pEvent->GetVariable();
		switch(owner->GetType())
		{
		case OBJ_ENEMY:
			{
				CEnemy* pEnemy = dynamic_cast<CEnemy*>(owner);
				SetHealth(GetHealth() - pEnemy->GetDamage());
			}
			break;

		case OBJ_ALLY:
			break;

		case OBJ_PLAYER:
			{
				/*CPlayer* pPlayer = dynamic_cast<CPlayer*>(owner);
				TakeDamage(pPlayer->GetDamage());*/
			}
			break;
		};
	}
}

void CAlly::TakeDamage(int nDamage)
{
	/*if( m_nHealth > 0)
		m_nHealth -= nDamage;
	if( m_nHealth < 0 )
		m_nHealth = 0;*/
}

bool CAlly::CheckTileCollision()
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