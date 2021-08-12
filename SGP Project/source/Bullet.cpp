/*
	File:						Bullet.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/12/2012
	Purpose:				Contain / manage the game code
*/

#include "Bullet.h"
#include <assert.h>
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "DestroyBulletMessage.h"
#include "EventManager.h"
#include "Game.h"
#include "MessageSystem.h"
#include "Camera.h"
#include "TileManager.h"
#include "Player.h"
#include "Ally.h"
#include "Enemy.h"
#include "Helicopter.h"

#define LIFESPAN 1.0f

CBullet::CBullet(void)
{
	m_eType = OBJ_BULLET;

	CEventManager::GetInstance()->RegisterClient( "shoot", this );
	m_pOwner = nullptr;
	imgID = -1;
	count = false;

	m_pOM = ObjectManager::GetInstance();
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	m_pTrailParticleEffect = CParticleManager::GetInstance()->CreateEmitter("fire");

	m_fLifeTimer = 0.0f;

	//m_pMuzzleFlashEffect = CParticleManager::GetInstance()->CreateEmitter("muzzleFlash");		//cjm - 10/10/12
	m_bFlash = true;		//cjm - 10/08/12
}

CBullet::~CBullet(void)
{
	CEventManager::GetInstance()->UnregisterClient( "shoot", this );
	m_pOwner->Release();
	for(unsigned int i = 0; i < m_vSniperHits.size(); ++i)
		m_vSniperHits[i]->Release();
	m_vSniperHits.clear();
}

void CBullet::SetOwner( CMovingObject* pOwner )
{
	if( m_pOwner != nullptr )
		m_pOwner->Release();

	m_pOwner = pOwner;

	if( m_pOwner != nullptr )
		m_pOwner->AddRef();

	switch(m_pOwner->GetType())
	{
	case OBJ_ALLY:
		{
			CAlly* pAlly = dynamic_cast<CAlly*>(m_pOwner);
			switch(pAlly->GetType())
			{
			case 0:
				m_eBullet = MG;
				break;

			case 1:
				m_eBullet = SHOTGUN;
				break;

			case 2:
				m_eBullet = ROCKET;
				break;

			case 3:
				m_eBullet = SNIPER;
				break;
			};
		}
		break;

	case OBJ_ENEMY:
		{
			CEnemy* pEnemy = dynamic_cast<CEnemy*>(m_pOwner);
			switch(pEnemy->GetUnitType())
			{
			case 0:
				m_eBullet = MG;
				break;

			case 1:
				m_eBullet = SNIPER;
				break;

			case 2:
				m_eBullet = SHOTGUN;
				break;

			case 3:
				m_eBullet = ROCKET;
				break;
			};
		}
		break;

	case OBJ_PLAYER:
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pOwner);
			switch(pPlayer->GetWeaponType())
			{
			case 0:
				m_eBullet = PISTOL;
				break;

			case 1:
				m_eBullet = MG;
				break;

			case 2:
				m_eBullet = SHOTGUN;
				break;

			case 3:
				m_eBullet = SNIPER;
				break;

			case 4:
				m_eBullet = ROCKET;
				break;
			};
		}
		break;
	};
}

void CBullet::Render()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	RECT rSourceRect  = { 642,150,650, 166 };
	RECT rRocketRect  = { 0,0,64,64 };

	//Rotating the bullet about the player center
	D3DXMATRIX rotation;
	D3DXMATRIX translate;
	D3DXMATRIX combined;

	// Initialize the Combined matrix.
	D3DXMatrixIdentity(&combined);

	// Translate the sprite
	D3DXMatrixTranslation(&translate, 10.f, -50.f, 0.0f);
	combined *= translate;

	D3DXMatrixRotationZ(&rotation, GetOwner()->GetRotation());
	combined *= rotation;

	static int count = 0;
	if (m_bFlash)		
	{	
		
		if(m_eBullet == PISTOL)																
		{	
			//count++;						
			CParticleManager::GetInstance()->Render("muzzleFlash",(int)(GetOwner()->GetPosX() + combined._41), (int)(GetOwner()->GetPosY() + combined._42), GetOwner()->GetRotation()-1.7f);
			//if(count == 2)
			//	m_bFlash = false;			
		}
		//if(m_eBullet == SNIPER)															
		//{
		//	CParticleManager::GetInstance()->Render("muzzleFlash",(int)(GetOwner()->GetPosX() + combined._41), (int)(GetOwner()->GetPosY() + combined._42), GetOwner()->GetRotation()-1.7f);
		//
		//	//CParticleManager::GetInstance()->Render("muzzleFlash",(int)(GetOwner()->GetPosX() + 80), (int)(GetOwner()->GetPosY() + 5), GetRotation() );
		//	//m_bFlash = false;
		//}
		//if (m_eBullet == SHOTGUN || m_eBullet == MG)
		//{
		//	CParticleManager::GetInstance()->Render("muzzleFlash",(int)(GetOwner()->GetPosX() + combined._41), (int)(GetOwner()->GetPosY() + combined._42), GetOwner()->GetRotation()-1.7f);
		//
		//	//CParticleManager::GetInstance()->Render("muzzleFlash",(int)(GetOwner()->GetPosX() + 60), (int)(GetOwner()->GetPosY() + 5), GetRotation() );
		//	//m_bFlash = false;
		//}
	}

	//pTm->Draw( GetImageID(), (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()) , 1.0, 1.0, &rSourceRect, 0,  0, GetRotation() );

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------	
	if (m_eBullet == ROCKET)
	{
		//pTm->Draw( GetImageID(), (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1.0, 1.0, &rRocketRect, 0,  0, GetRotation() );
		m_pTrailParticleEffect->Render();
	}
	if (m_eBullet == MG || m_eBullet == SNIPER || m_eBullet == PISTOL || m_eBullet == SHOTGUN)
		pTm->Draw( GetImageID(), (int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()) , 1.0, 1.0, &rSourceRect, 0,  0, GetRotation() );
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------	

}

void CBullet::Update( float fElapsedTime )
{
	//if( (GetPosX() < 0 || GetPosX() > CTileManager::GetInstance()->GetWorldWidth()) ||
	//	(GetPosY() < 0 || GetPosY() > CTileManager::GetInstance()->GetWorldHeight() ))
//------------------------------------------------------------------------------------------------------------------------------------ //cjm - 10/09/12

	if(m_eBullet == ROCKET)					//cjm - 10/09/12
	{										//cjm - 10/09/12
		m_pTrailParticleEffect->SetEmitterPosX((int)GetPosX());// - CCamera::GetInstance()->GetX());
		m_pTrailParticleEffect->SetEmitterPosY((int)GetPosY());// - CCamera::GetInstance()->GetY());

		m_pTrailParticleEffect->Update(fElapsedTime);
	}		

	if(m_eBullet == PISTOL && m_bFlash)													
	{	
		CParticleManager::GetInstance()->Update(fElapsedTime);
	}																									

	//if(m_eBullet == SNIPER && m_bFlash)																	
	//{	
	//	m_pMuzzleFlashEffect->SetEmitterPosX((int)(GetPosX()  + 50));				
	//	m_pMuzzleFlashEffect->SetEmitterPosY((int)GetPosY()  + 10 );				
	//	m_pMuzzleFlashEffect->SetEmitterRotation(GetOwner()->GetRotation()-1.7f);
	//
	//	m_pMuzzleFlashEffect->Update(fElapsedTime);	
	//}
	//
	//if (m_eBullet == SHOTGUN || m_eBullet == MG && !m_bFlash)
	//{
	//	m_pMuzzleFlashEffect->SetEmitterPosX((int)GetPosX()  + 60);					
	//	m_pMuzzleFlashEffect->SetEmitterPosY((int)GetPosY()  + 10 );					
	//	m_pMuzzleFlashEffect->SetEmitterRotation(GetOwner()->GetRotation()-1.7f);
	//
	//
	//	m_pMuzzleFlashEffect->Update(fElapsedTime);	
	//}
//------------------------------------------------------------------------------------------------------------------------------------	//cjm - 10/10/12
	m_fLifeTimer += fElapsedTime;
	if( m_fLifeTimer > LIFESPAN )
	{
		CDestroyBulletMessage* pMsg = new CDestroyBulletMessage( this );
		CMessageSystem::GetInstance()->SendMsg( pMsg );
		pMsg = nullptr;
		m_fLifeTimer = 0.0f;
	}

	CMovingObject::Update(fElapsedTime);
}

void CBullet::HandleEvent( IEvent* pEvent )
{
	if(  pEvent->GetEventID() == "shoot" )
	{
		if( count == true )
			count = false;
		else if( count == false )
			count = true;
	}
}

bool CBullet::CheckCollision(IEntity* pOther)
{
	CMessageSystem* pMS = CMessageSystem::GetInstance();

	

	if(CEntity::CheckCollision(pOther) == false)
		return false;
	
	if(m_eBullet == SNIPER)
	{
		for(unsigned int i = 0; i < m_vSniperHits.size(); ++i)
		{
			if(m_vSniperHits[i] == pOther)
				return false;
		}
	}
	
	switch(pOther->GetType())
	{
	case OBJ_BASE:
	case OBJ_BULLET:
		return false;
	case OBJ_PICKUP:
		return false;
	//case OBJ_BUNKER:
	//	{
	//		CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
	//		pMS->SendMsg(pMsg);
	//		pMsg = nullptr;
	//	}
	case OBJ_PLAYER:
		{
			if(GetOwner()->GetType() == OBJ_ENEMY)
			{
				if(m_eBullet != SNIPER)
				{
					//if( GetArmor() < pBullet->GetOwner()->GetDamage())
					//{
					//	GetOwner()->SetHealth(GetHealth() - (pBullet->GetOwner()->GetDamage() - GetArmor()));
					//	GetOwner()->SetHurtTimer(1.0f);											//cjm = 10/06/12																						//cjm - 10/05/12
					//}
					//else
					//{
					//	SetHealth(GetHealth());
					//	GetOwner()->SetHurtTimer(1.0f);															//cjm = 10/06/12
					//}
					CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
					pMS->SendMsg(pMsg);
					pMsg = nullptr;
				}
				if(m_eBullet == ROCKET)
				{
					RECT splash, rOther = pOther->GetRect(), rOverlap;
					splash.left		= (LONG)(GetPosX() - 48);
					splash.top		= (LONG)(GetPosY() - 48);
					splash.right	= (LONG)(GetPosX() + 48);
					splash.bottom	= (LONG)(GetPosY() + 48);

					list<IEntity*> objList = m_pOM->GetObjectList();

					for(std::list<IEntity*>::iterator iter = objList.begin();
						iter != objList.end(); ++iter)
					{
						if((*iter)->GetType() != OBJ_ENEMY && (*iter)->GetType() != OBJ_BULLET)
						{
							if(IntersectRect(&rOverlap, &splash, &(*iter)->GetRect()))
							{
								// If splash rect hits something not an enemy, send an event
								CEntity* hit = dynamic_cast<CEntity*>((*iter));
								hit->SetHealth(hit->GetHealth() - m_pOwner->GetDamage());
							}
						}
					}
				}
			}
			else
				return false;
		}
		break;

	case OBJ_ALLY:
		{
			if(GetOwner()->GetType() == OBJ_ENEMY)
			{
				if(m_eBullet != SNIPER)
				{
					CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
					pMS->SendMsg(pMsg);
					pMsg = nullptr;
				}
				if(m_eBullet == ROCKET)
				{
					RECT splash, rOther = pOther->GetRect(), rOverlap;
					splash.left		= (LONG)(GetPosX() - 48);
					splash.top		= (LONG)(GetPosY() - 48);
					splash.right	= (LONG)(GetPosX() + 48);
					splash.bottom	= (LONG)(GetPosY() + 48);

					list<IEntity*> objList = m_pOM->GetObjectList();

					for(std::list<IEntity*>::iterator iter = objList.begin();
						iter != objList.end(); ++iter)
					{
						if((*iter)->GetType() != OBJ_ENEMY && (*iter)->GetType() != OBJ_BULLET)
						{
							if(IntersectRect(&rOverlap, &splash, &(*iter)->GetRect()))
							{
								// If splash rect hits something not an enemy, send an event
								CEntity* hit = dynamic_cast<CEntity*>((*iter));
								hit->SetHealth(hit->GetHealth() - m_pOwner->GetDamage());
							}
						}
					}
				}
			}
			else
				return false;
		}
		break;

	case OBJ_ENEMY:
		{
			if(GetOwner()->GetType() != OBJ_ENEMY)
			{
				if(m_eBullet != SNIPER)
				{					
					CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
					pMS->SendMsg(pMsg);
					pMsg = nullptr;
				}
				if(m_eBullet == ROCKET)
				{
					RECT splash, rOther = pOther->GetRect(), rOverlap;
					splash.left		= (LONG)(GetPosX() - 48);
					splash.top		= (LONG)(GetPosY() - 48);
					splash.right	= (LONG)(GetPosX() + 48);
					splash.bottom	= (LONG)(GetPosY() + 48);

					list<IEntity*> objList = m_pOM->GetObjectList();

					for(std::list<IEntity*>::iterator iter = objList.begin();
						iter != objList.end(); ++iter)
					{
						if((*iter)->GetType() == OBJ_ENEMY && (*iter)->GetType() != OBJ_BULLET)
						{
							if(IntersectRect(&rOverlap, &splash, &(*iter)->GetRect()))
							{
								// If splash rect hits something not an enemy, send an event
								CEntity* hit = dynamic_cast<CEntity*>((*iter));
								hit->SetHealth(hit->GetHealth() - m_pOwner->GetDamage());
							}
						}
					}
				}
			}
			else
				return false;
		}
		break;

	case OBJ_HELICOPTER:
		{
			CHelicopter* pObj = dynamic_cast<CHelicopter*>(pOther);
			if(pObj->GetFlying() == true)
				return false;

			if(GetOwner()->GetType() != OBJ_ENEMY)
			{
				if(m_eBullet != SNIPER)
				{					
					CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
					pMS->SendMsg(pMsg);
					pMsg = nullptr;
				}
				if(m_eBullet == ROCKET)
				{
					RECT splash, rOther = pOther->GetRect(), rOverlap;
					splash.left		= (LONG)(GetPosX() - 48);
					splash.top		= (LONG)(GetPosY() - 48);
					splash.right	= (LONG)(GetPosX() + 48);
					splash.bottom	= (LONG)(GetPosY() + 48);

					list<IEntity*> objList = m_pOM->GetObjectList();

					for(std::list<IEntity*>::iterator iter = objList.begin();
						iter != objList.end(); ++iter)
					{
						if((*iter)->GetType() == OBJ_ENEMY && (*iter)->GetType() != OBJ_BULLET)
						{
							if(IntersectRect(&rOverlap, &splash, &(*iter)->GetRect()))
							{
								// If splash rect hits something not an enemy, send an event
								CEntity* hit = dynamic_cast<CEntity*>((*iter));
								hit->SetHealth(hit->GetHealth() - m_pOwner->GetDamage());
							}
						}
					}
				}
			}
			else
				return false;
			//if(m_eBullet != SNIPER)
			//{
			//	 if(m_eBullet==ROCKET)
			//	 {
			//		 //play sound here
			//	 }
			//	CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(this);
			//	pMS->SendMsg(pMsg);
			//	pMsg = nullptr;
			//}
		}
		break;
	};

	if(m_eBullet == SNIPER)
	{
		m_vSniperHits.push_back(pOther);
		pOther->AddRef();
	}

	return true;
}

RECT CBullet::GetRect() const
{
	RECT rTemp;

	rTemp.left	= (LONG)(GetPosX() - GetWidth() ) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.top	= (LONG)(GetPosY() - GetHeight()) - (LONG)CCamera::GetInstance()->GetY();
	rTemp.right	= (LONG)(GetPosX() + GetWidth() ) - (LONG)CCamera::GetInstance()->GetX();
	rTemp.bottom= (LONG)(GetPosY() + GetHeight()) - (LONG)CCamera::GetInstance()->GetY();

	return rTemp;
}