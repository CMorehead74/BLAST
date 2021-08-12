/*
File:				Helicopter.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Matthew Berry
Last Edited:		10/9/2012
Purpose:			A game object class for the helicopter enemy in the game
--	Will be spawning enemies
--	Will have a STATIC POSITION in this current version
*/

#include "Helicopter.h"

#include "EventManager.h"
#include "MessageSystem.h"
#include "CreateEnemyMessage.h"

// Wrapper Includes
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "TileManager.h"

#include "AnimationManager.h"

//needed classes
#include "Camera.h"
#include "Player.h"
#include "Bullet.h"
#include "DestroyBulletMessage.h"
#include "ParticleManager.h"

#define SPAWN_RATE 1.0f

CHelicopter::CHelicopter(void)
{
	m_eType = OBJ_HELICOPTER;

	m_fSpawnTimer		= 0.0f;
	m_fOnGroundTimer	= 5.0f;
	m_fDropRate			= 0.0f;
	m_fFlightTimer		= 0.0f;
	m_fScaleImage		= 1.0f;

	m_fMoveSpeed		= 100.0f;

	int m_fTimeWaited = 0;

	m_bOnTheGround		= true;
	m_bIsFlying			= false;
	m_bIsLanding		= false;
	m_bIsSpawning		= true;

	m_pPlayer = nullptr;

	m_pOM = ObjectManager::GetInstance();

	m_ptTarget.x = -10;
	m_ptTarget.y = -10;

	PlayerFound = false;

	//Health
	m_nMaxHealth = 1000;
	SetHealth( m_nMaxHealth );

	CurrAnimation = new CAnimationInfo();

	CurrAnimation->m_nCurrentAnimation = 5;
	CurrAnimation->m_fTimeWaited = 0.17f;
	CurrAnimation->m_nCurrentFrame = 1;
	CurrAnimation->m_bIsLooping = true;
	CurrAnimation->m_bIsPlaying = true;

	m_nHelicopterImgID   = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/Helicopter.png"));

	m_pXA  = CSGD_XAudio2::GetInstance();

	m_nHelicopter		= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_HELICOPTER.wav"));
	m_nHealthID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HealthBarFill.jpg"));

	m_nBulletHit	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_BULLETCOLLIDE_COPTER.wav"));
	m_nRocketHit	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_HELICOPTEREXPLODE.wav"));
	//m_nEnemySpawn	= m_pXA->SFXLoadSound(_T("resource/sounds/SFX_ENEMYSPAWN.wav"));

//	m_pSmokeParticleEffect = CParticleManager::GetInstance()->CreateEmitter("smoke");	//cjm - 10/04/12
}

void CHelicopter::Update( float fElapsedTime )
{
	CAnimationManager::GetInstance()->Update(CurrAnimation, fElapsedTime);	

	if(m_pXA->SFXIsSoundPlaying(m_nHelicopter))
	{

	}
	else
	{
		m_pXA->SFXPlaySound(m_nHelicopter);
	}

	if(m_bOnTheGround == true)
	{
		m_fOnGroundTimer += fElapsedTime;
		if(m_fOnGroundTimer >= 5.0f)
			m_bIsSpawning = true;
		if(m_fOnGroundTimer >= 15.0f)
		{
			m_fOnGroundTimer = 0.0f;
			m_bIsSpawning = false;
			m_bOnTheGround = false;
			m_bIsFlying = true;
			m_ptTarget.x = (LONG)GetPosX();
			m_ptTarget.y = -200;
			PlayerFound = false;
		}
	}

	if(m_bIsSpawning == true)
	{
		m_fSpawnTimer += fElapsedTime;
		if(m_fSpawnTimer > SPAWN_RATE)
		{
		//	m_pXA->SFXPlaySound(m_nEnemySpawn);
			SpawnEnemies();
			m_fSpawnTimer = 0.0f;
		}
	}

	if(m_bIsFlying == true)
	{
		//m_fFlightTimer += fElapsedTime;
		/*SetVelX(m_fMoveSpeed);
		SetVelY(m_fMoveSpeed);*/

		if(GetPosY() <= -150)
		{
			if(m_pPlayer == nullptr)
			{
				list<IEntity*> objList = m_pOM->GetObjectList();

				for(std::list<IEntity*>::iterator iter = objList.begin();
					iter != objList.end(); ++iter)
				{
					if((*iter)->GetType() == OBJ_PLAYER)
					{
						m_pPlayer = dynamic_cast<CPlayer*>(*iter);
						break;
					}
				}
			}

			m_ptTarget.x = (LONG)m_pPlayer->GetPosX();
			m_ptTarget.y = (LONG)m_pPlayer->GetPosY();
		}

		float newPosX = GetPosX() + fElapsedTime * 0.5f * (m_ptTarget.x - GetPosX());
		float newPosY = GetPosY() + fElapsedTime * 0.5f * (m_ptTarget.y - GetPosY());

		float distance = sqrt(pow(abs(m_ptTarget.x - newPosX),2) + pow(abs(m_ptTarget.y - newPosY), 2));

		if(PlayerFound == true)
		{
			if(distance > 10)
			{
				SetPosX(newPosX);
				SetPosY(newPosY);
			}
			else
			{
				m_bIsFlying = false;
				m_bOnTheGround = true;
				m_fFlightTimer = 0.0f;
				SetVelX(0);
				SetVelY(0);
				PlayerFound = false;
			}
		}
		else
		{
			if(distance > 10)
			{
				SetPosX(newPosX);
				SetPosY(newPosY);
			}
			else
			{
				if(GetPosY() <= m_pPlayer->GetPosY() + 10)
				{
					PlayerFound = true;

					int helipad = rand() % 4;
					if(helipad == 0)
					{
						m_ptTarget.x = 2568;
						m_ptTarget.y = 771;
					}
					else if(helipad == 1)
					{
						m_ptTarget.x = 321;
						m_ptTarget.y = 3848;
					}
					else if(helipad == 2)
					{
						m_ptTarget.x = 2661;
						m_ptTarget.y = 2808;
					}
					else if(helipad == 3)
					{
						m_ptTarget.x = 624;
						m_ptTarget.y = 1682;
					}
				}
			}
		}

		tVector2D vToTarget; 
		vToTarget.fX = m_ptTarget.x - GetPosX();
		vToTarget.fY = m_ptTarget.y - GetPosY();			

		tVector2D vOrientation = { 0, -1 };
		vOrientation = Vector2DRotate( vOrientation, GetRotation() );

		vToTarget = Vector2DNormalize(vToTarget);

		// Calculate the angle between the vectors
		//	NOTE:	Always in radians between [ 0, pi ]
		float fAngle = AngleBetweenVectors( vOrientation, vToTarget );

		// Rotate slowly:
		if( fAngle > (SGD_PI * fElapsedTime) )
			fAngle = (SGD_PI * fElapsedTime);


		// Which direction to turn? Clockwise or counter-clockwise
		if( Steering( vOrientation, vToTarget ) < 0 )
			fAngle = -fAngle;

		SetRotation(GetRotation() + fAngle);	
	}
}


void CHelicopter::MiniMapUpdate( float fElapsedTime )
{
	if(m_pXA->SFXIsSoundPlaying(m_nHelicopter))
	{

	}
	else
	{
		//if(m_bOnTheGround == true)
		m_pXA->SFXPlaySound(m_nHelicopter);
	}

	if(m_bOnTheGround == true)
	{
		m_fOnGroundTimer += fElapsedTime;
		if(m_fOnGroundTimer >= 5.0f)
			m_bIsSpawning = true;
		if(m_fOnGroundTimer >= 15.0f)
		{
			//Sending an Event to Change enemies back to attack state when the helicopter takes off
			CEventManager::GetInstance()->sendEvent("Helicopter_flying");

			m_fOnGroundTimer = 0.0f;
			m_bIsSpawning = false;
			m_bOnTheGround = false;
			m_bIsFlying = true;
		}
	}

	//if(m_bIsSpawning == true)
	//{
	//	m_fSpawnTimer += fElapsedTime;
	//	if(m_fSpawnTimer > SPAWN_RATE)
	//	{
	//		SpawnEnemies();
	//		m_fSpawnTimer = 0.0f;
	//	}
	//}

	if(m_bIsFlying == true)
	{
		m_fFlightTimer += fElapsedTime;
		SetVelX(m_fMoveSpeed);
		SetVelY(m_fMoveSpeed);

		CMovingObject::Update(fElapsedTime);

		if(GetPosX() - GetWidth() > CTileManager::GetInstance()->GetWorldWidth())
			m_fMoveSpeed = -m_fMoveSpeed;

		if(GetPosX() + GetWidth() < 0)
			m_fMoveSpeed = -m_fMoveSpeed;

		if(GetPosY() - GetHeight() > CTileManager::GetInstance()->GetWorldHeight())
			m_fMoveSpeed = -m_fMoveSpeed;

		if(GetPosY() + GetHeight() < 0)
			m_fMoveSpeed = -m_fMoveSpeed;

		if(m_fFlightTimer >= 10.0f)
		{
			m_bIsFlying = false;
			m_bOnTheGround = false;
			m_fFlightTimer = 0.0f;
			SetVelX(0);
			SetVelY(0);
		}
	}


}


void CHelicopter::Render()
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	//RECT rSource = {0, 0, 256, 194};
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();
	//pTm->Draw(GetImageID(), (int)GetPosX() - CCamera::GetInstance()->GetX(), (int)GetPosY() - CCamera::GetInstance()->GetY());

	RECT rsource = { 0, 0, 512, 1024 };
	if(m_bIsFlying == false)
		CAnimationManager::GetInstance()->Render(CurrAnimation, m_nHelicopterImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 2.0f, 2.0f, (rsource.left + rsource.right) *0.5f, (rsource.top + rsource.bottom) *0.5f, GetRotation(), D3DCOLOR_RGBA(255,255,0,255)  );
	else
		CAnimationManager::GetInstance()->Render(CurrAnimation, m_nHelicopterImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 2.0f, 2.0f, (rsource.left + rsource.right) *0.5f, (rsource.top + rsource.bottom) *0.5f, GetRotation(), D3DCOLOR_RGBA(0,0,0,100)  );



	// Draw Health Bar
	float hp = (float)GetHealth()/(float)GetMaxHealth();

	if(hp > 0 && m_bIsFlying == false)
	{
		RECT rHealthBar = { 0, 0, (LONG)(256 * hp), 32 };
		if(hp > 0 && hp <= 0.15f)
			pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()  -10),(int)(GetPosY() - CCamera::GetInstance()->GetY() ), 0.75f, 0.20f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
		else if(hp > 0.15f && hp < 0.3f)										   
			pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()  -10),(int)(GetPosY() - CCamera::GetInstance()->GetY() ), 0.75f, 0.20f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 0));
		else if(hp > 0.3f && hp <= 1.f)											   
			pTm->Draw(m_nHealthID, (int)(GetPosX() - CCamera::GetInstance()->GetX()  -10),(int)(GetPosY() - CCamera::GetInstance()->GetY() ), 0.75f, 0.20f, &rHealthBar, 0.0f, 0.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	}



}

void CHelicopter::MiniMapRender()
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	RECT rSource = {0, 0, 128, 194};
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();

	pTm->Draw(m_nHelicopterImgID, (int)((GetPosX() * 0.0365f) +606), (int)((GetPosY() * 0.0365f ) +406), 0.10f, 0.10f, &rSource, 0,0,0, D3DCOLOR_RGBA(255,255,255,225));

}

void CHelicopter::SpawnEnemies(void)
{
	CCreateEnemyMessage* pEnemy1 = new CCreateEnemyMessage();
	CMessageSystem::GetInstance()->SendMsg(pEnemy1);
	pEnemy1 = nullptr;
}

CHelicopter::~CHelicopter(void)
{
	if(m_nHelicopter != -1)
	{
		m_pXA->SFXStopSound(m_nHelicopter);
		m_pXA->SFXUnloadSound(m_nHelicopter);
		m_nHelicopter = -1;
	}
// 	if(m_nEnemySpawn != -1)
//{
//	m_pXA->SFXStopSound(m_nEnemySpawn);
//	m_pXA->SFXUnloadSound(m_nEnemySpawn);
// 	m_nEnemySpawn = -1;
//}
	if(m_nRocketHit != -1)
	{
		m_pXA->SFXStopSound(m_nRocketHit);
		m_pXA->SFXUnloadSound(m_nRocketHit);
		m_nRocketHit = -1;
	}
	if(m_nBulletHit != -1)
	{
		m_pXA->SFXStopSound(m_nBulletHit);
		m_pXA->SFXUnloadSound(m_nBulletHit);
		m_nBulletHit = -1;
	}

	delete CurrAnimation;
	CurrAnimation = nullptr;
}

bool CHelicopter::CheckCollision( IEntity* pOther )
{
	CMessageSystem* pMsg = CMessageSystem::GetInstance();

	if( CEntity::CheckCollision( pOther ) == false )
		return false;

	if(m_bIsFlying == true)
		return false;

	if( pOther->GetType() == OBJ_BULLET )
	{
		CBullet* pBullet = dynamic_cast<CBullet*>( pOther );

		for(unsigned int i = 0; i < pBullet->m_vSniperHits.size(); ++i)
		{
			if(pBullet->m_vSniperHits[i] == this)
			{
				return false;
			}
		}
		
		if( pBullet->GetOwner()->GetType() != OBJ_ENEMY )
		{			
			if(pBullet->GetOwner()->GetType()==OBJ_PLAYER)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(pBullet->GetOwner());
				if(	pPlayer->GetWeaponType()==ROCKET)
				{
					if(m_pXA->SFXIsSoundPlaying(m_nRocketHit)==false)
						m_pXA->SFXPlaySound(m_nRocketHit);
				}
				else
				{
					if(m_pXA->SFXIsSoundPlaying(m_nBulletHit)==false)
						m_pXA->SFXPlaySound(m_nBulletHit);
				}
			}
			
	
			SetHealth(GetHealth() - pBullet->GetOwner()->GetDamage());			

			CEventManager::GetInstance()->sendEvent("Helicopter_attacked");
			if(GetHealth() <= 0)
			{
				int i = 0;
			}
			CDestroyBulletMessage* pMsg = new CDestroyBulletMessage(pBullet);
			CMessageSystem* pMs = CMessageSystem::GetInstance();
			pMs->SendMsg(pMsg);
			pMsg = nullptr;
		}
	}
	return true;
}