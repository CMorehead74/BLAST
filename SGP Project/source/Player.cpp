/*
File:				Player.cpp
Course:				SGP
Author:				Vortex Studios
Last Modified By:	Naveen Anand Gunalan
Last Edited:		10/5/2012
Purpose:			Contain / manage the game code
*/

#include "Player.h"
#include "VoiceReco.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "../SGD Wrappers/CSGD_DirectInput.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Game.h"

#include "AnimationManager.h"
#include "Ally.h"

#include "EventManager.h"
#include "MessageSystem.h"
#include "CreateBulletMessage.h"
#include "CreateShotgunBullet.h"
#include "DestroyPickUpMessage.h"
#include "Camera.h"
#include "BitmapFont.h"
#include "Bullet.h"
#include "PickUp.h"
#include "TileManager.h"
#include "ParticleManager.h"

#include "Enemy.h"
#include "StoreHouse.h"
#include "Mine.h"
#include "Bunker.h"

using namespace std;

#define PLAYER_ROTATION_RATE	10
#define MG_FIRE_RATE			0.05f
#define SHOTGUN_FIRE_RATE		1.5f
#define SNIPER_FIRE_RATE		1.75f
#define ROCKET_FIRE_RATE		2.5f

CPlayer::CPlayer(void)
{
	m_eType = OBJ_PLAYER;

	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	m_nTempSfxID				= pXA->SFXLoadSound(_T("resource/sounds/SFX_PISTOL.wav"));
	m_nDeath					= pXA->SFXLoadSound(_T("resource/sounds/SFX_DEATH.wav"));
	m_nCommandConfirmedSFXID	= pXA->SFXLoadSound(_T("resource/sounds/SFX_CONFIRMED.wav"));
	m_nDryFireSFXID				= pXA->SFXLoadSound(_T("resource/sounds/SFX_EMPTYFIRE.wav"));
	m_nUseItemSFXID				= pXA->SFXLoadSound(_T("resource/sounds/SFX_ITEMUSED.wav"));
	m_nKillSound1SFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_KILLSOUND1.wav"));
	m_nKillSound2SFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_KILLSOUND2.wav"));
	m_nKillSound3SFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_KILLSOUND3.wav"));
	m_bKillSound1				= false;
	m_bKillSound2				= false;
	m_bKillSound3				= false;
	m_nItemPickupSFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_ITEMPICKUP.wav"));
	m_nWeaponPickupSFXID		= pXA->SFXLoadSound(_T("resource/sounds/SFX_WEAPONPICKUP.wav"));
	m_nFireMGSFXID				= pXA->SFXLoadSound(_T("resource/sounds/SFX_MACHINE_GUNNER.wav"));
	m_nFireRocketSFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_ROCKET.wav"));
	m_nFireSniperSFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_SNIPER.wav"));
	m_nFireShotgunSFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_SHOTGUN.wav"));
	m_nHealedSFXID				= pXA->SFXLoadSound(_T("resource/sounds/SFX_MEDICHEAL.wav"));



	m_nDeath     = pXA->SFXLoadSound(_T("resource/sounds/SFX_DEATH.wav"));
	m_nCommandConfirmedSFXID = pXA->SFXLoadSound(_T("resource/sounds/SFX_CONFIRMED.wav"));
	m_nDryFireSFXID = pXA->SFXLoadSound(_T("resource/sounds/SFX_EMPTYFIRE.wav"));
	m_nUseItemSFXID	= pXA->SFXLoadSound(_T("resource/sounds/SFX_ITEMUSED.wav"));
	m_nKillSound1SFXID= pXA->SFXLoadSound(_T("resource/sounds/SFX_KILLSOUND1.wav"));
	m_nKillSound2SFXID= pXA->SFXLoadSound(_T("resource/sounds/SFX_KILLSOUND2.wav"));
	m_nKillSound3SFXID= pXA->SFXLoadSound(_T("resource/sounds/SFX_KILLSOUND3.wav"));
	m_bKillSound1=false;
	m_bKillSound2=false;
	m_bKillSound3=false;
	m_nItemPickupSFXID		= pXA->SFXLoadSound(_T("resource/sounds/SFX_ITEMPICKUP.wav"));
	m_nWeaponPickupSFXID	= pXA->SFXLoadSound(_T("resource/sounds/SFX_WEAPONPICKUP.wav"));
	m_nFireMGSFXID			= pXA->SFXLoadSound(_T("resource/sounds/SFX_MACHINE_GUNNER.wav"));
	m_nFireRocketSFXID		= pXA->SFXLoadSound(_T("resource/sounds/SFX_ROCKET.wav"));
	m_nFireSniperSFXID		= pXA->SFXLoadSound(_T("resource/sounds/SFX_SNIPER.wav"));
	m_nFireShotgunSFXID		= pXA->SFXLoadSound(_T("resource/sounds/SFX_SHOTGUN.wav"));



	CEventManager::GetInstance()->RegisterClient( "shoot", this );
	CEventManager::GetInstance()->RegisterClient("get_kills", this);
	CEventManager::GetInstance()->RegisterClient("rocket_splash", this);
	CEventManager::GetInstance()->RegisterClient( "comfirmed", this );
	m_nCursorImgID = CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor.png"));
	CEventManager::GetInstance()->RegisterClient("confirmed", this);
	CEventManager::GetInstance()->RegisterClient("healed", this);

	m_nCursorImgID		= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/Blast_Mouse_Cursor.png"));
	m_nHealthID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HealthBarFill.jpg"));
	m_nHealthPackImgID	= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/first_aid_kit.png"), D3DCOLOR_XRGB(255, 255, 255));
	m_nFoodImgID		= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/food.png"));
	m_nOreImgID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/ore.png"), D3DCOLOR_XRGB(255,255,255));
	m_nHUDImgID			= CSGD_TextureManager::GetInstance()->LoadTexture(_T("resource/graphics/HUD_assets.png"));

	m_nMaxHealth = 200;
	SetHealth(m_nMaxHealth);
	m_nCash = 6000;
	m_nAmmo = 100;
	m_nWeaponType = 0;
	m_nKills	= 0;

	m_fFireTimer = 0.25f;
	CurrAnimation = new CAnimationInfo();

	m_fPistolRate = 0.25f;

	CurrAnimation->m_nCurrentAnimation = 0;
	CurrAnimation->m_fTimeWaited = 0.17f;
	CurrAnimation->m_nCurrentFrame = 0;
	CurrAnimation->m_bIsLooping = false;
	CurrAnimation->m_bIsPlaying = true;


	m_fTextTimer = 0.0f;
	m_fProductionTimer = 0.0f;
	m_fPickUpTimer = 0.0f;

	m_nPlayerpistolImgID			= CSGD_TextureManager::GetInstance()->LoadTexture( _T("resource/graphics/sprites_player_001.png" ) );
	m_nPlayersniperImgID			= CSGD_TextureManager::GetInstance()->LoadTexture( _T("resource/graphics/sprites_player_002.png" ) );
	m_nPlayerMachineGunnerImgID		= CSGD_TextureManager::GetInstance()->LoadTexture( _T("resource/graphics/sprites_player_003.png" ) );

	//m_pMuzzleFlashEffect = CParticleManager::GetInstance()->CreateEmitter("muzzleFlash");	//cjm - 10/08/12

	//Particles
	//m_pHealingParticleEffect = CParticleManager::GetInstance()->CreateEmitter("Player_Healing_Effect");
	//m_pBloodParticleEffect = CParticleManager::GetInstance()->CreateEmitter("blood");	//cjm - 10/04/12

	m_nDamage = 10;
	m_nArmor = 0;
	m_fMoveSpeed = 200;

	m_nOre = 200;
	m_fFood = 200;
	m_nOreRate = 0;
	m_nFoodRate = 0;

	SetArmor( m_nArmor );
	//SetFireRate(m_fFireTimer);
	m_fFireRate = m_fPistolRate;

	SetDamage(m_nDamage);
	//m_fFireTimer = GetFireRate();

	m_nPopulation	= 0;
	m_nMedics		= 0;
	m_nMGs			= 0;
	m_nSnipers		= 0;
	m_nRockets		= 0;

	m_nBunkers		= 0;
	m_nStoreHouses	= 0;
	m_nMines		= 0;

	m_bIsText	  = false;
	m_bIsAmmoText = false;

	m_fAmmoTimer = 0.0f;

	for(int i = 0; i<8; i++ )
	{
		m_bSurroundingTilesOccupied[i] = false;
	}
	m_bShootParticle = false;
	m_bBlood = false;			//cjm - 10/05/12
	m_bFlash = false;			//cjm - 10/08/12
}

int CPlayer::GetDamage() const
{
	int DmgIncrease = 0;
	switch(GetWeaponType())
	{
	case 0:		// Pistol Damage
		DmgIncrease = 0;
		break;

	case 1:		// Machine Gun
		DmgIncrease = 5;
		break;

	case 2:		// Shotgun
		DmgIncrease = 15;
		break;

	case 3:		// Sniper
		DmgIncrease = 25;
		break;

	case 4:		// Rocket
		DmgIncrease = 40;
		break;
	};

	return m_nDamage + DmgIncrease;
}

CPlayer::~CPlayer(void)
{
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	if(m_nTempSfxID != -1)
	{
		pXA->SFXStopSound(m_nTempSfxID);
		pXA->SFXUnloadSound(m_nTempSfxID);
		m_nTempSfxID = -1;
	}

	//if(m_nDeath != -1)
	//{
	//	pXA->SFXStopSound(m_nDeath);
	//	pXA->SFXUnloadSound(m_nDeath);
	//	m_nDeath = -1;
	//}
	if(m_nCommandConfirmedSFXID != -1)
	{
		pXA->SFXStopSound(m_nCommandConfirmedSFXID);
		pXA->SFXUnloadSound(m_nCommandConfirmedSFXID);
		m_nCommandConfirmedSFXID = -1;
	}	
	if(m_nDryFireSFXID != -1)
	{
		pXA->SFXStopSound(m_nDryFireSFXID);
		pXA->SFXUnloadSound(m_nDryFireSFXID);
		m_nDryFireSFXID = -1;
	}
	if(m_nUseItemSFXID != -1)
	{
		pXA->SFXStopSound(m_nUseItemSFXID);
		pXA->SFXUnloadSound(m_nUseItemSFXID);
		m_nUseItemSFXID = -1;
	}
	if(m_nKillSound1SFXID != -1)
	{
		pXA->SFXStopSound(m_nKillSound1SFXID);
		pXA->SFXUnloadSound(m_nKillSound1SFXID);
		m_nKillSound1SFXID = -1;
	}
	if(m_nKillSound2SFXID != -1)
	{
		pXA->SFXStopSound(m_nKillSound2SFXID);
		pXA->SFXUnloadSound(m_nKillSound2SFXID);
		m_nKillSound2SFXID = -1;
	}
	if(m_nKillSound3SFXID != -1)
	{
		pXA->SFXStopSound(m_nKillSound3SFXID);
		pXA->SFXUnloadSound(m_nKillSound3SFXID);
		m_nKillSound3SFXID = -1;
	}
	if(m_nItemPickupSFXID != -1)
	{
		pXA->SFXStopSound(m_nItemPickupSFXID);
		pXA->SFXUnloadSound(m_nItemPickupSFXID);
		m_nItemPickupSFXID = -1;
	}
	if(m_nWeaponPickupSFXID != -1)
	{
		pXA->SFXStopSound(m_nWeaponPickupSFXID);
		pXA->SFXUnloadSound(m_nWeaponPickupSFXID);
		m_nWeaponPickupSFXID = -1;
	}
	if(m_nFireMGSFXID != -1)
	{
		pXA->SFXStopSound(m_nFireMGSFXID);
		pXA->SFXUnloadSound(m_nFireMGSFXID);
		m_nFireMGSFXID = -1;
	}
	if(m_nFireRocketSFXID != -1)
	{
		pXA->SFXStopSound(m_nFireRocketSFXID);
		pXA->SFXUnloadSound(m_nFireRocketSFXID);
		m_nFireRocketSFXID = -1;
	}
	if(m_nFireShotgunSFXID != -1)
	{
		pXA->SFXStopSound(m_nFireShotgunSFXID);
		pXA->SFXUnloadSound(m_nFireShotgunSFXID);
		m_nFireShotgunSFXID = -1;
	}
	if(m_nFireSniperSFXID != -1)
	{
		pXA->SFXStopSound(m_nFireSniperSFXID);
		pXA->SFXUnloadSound(m_nFireSniperSFXID);
		m_nFireSniperSFXID = -1;
	}
	if(m_nCursorImgID != -1)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorImgID);
		m_nCursorImgID = -1;
	}
	if(m_nCommandConfirmedSFXID != -1)
	{
		pXA->SFXStopSound(m_nCommandConfirmedSFXID);
		pXA->SFXUnloadSound(m_nCommandConfirmedSFXID);
		m_nCommandConfirmedSFXID = -1;
	}
		if(m_nHealedSFXID != -1)
	{
		pXA->SFXStopSound(m_nHealedSFXID);
		pXA->SFXUnloadSound(m_nHealedSFXID);
		m_nHealedSFXID = -1;
	}
	CEventManager::GetInstance()->UnregisterClient("shoot", this );
	CEventManager::GetInstance()->UnregisterClient("get_kills", this);
	CEventManager::GetInstance()->UnregisterClient("rocket_splash", this);
	CEventManager::GetInstance()->UnregisterClient("confirmed", this);
	CEventManager::GetInstance()->UnregisterClient("healed", this);

	delete CurrAnimation;
	CurrAnimation = nullptr;
}

void CPlayer::Update( float fElapsedTime )
{
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();

	CAnimationManager::GetInstance()->Update(CurrAnimation, (fElapsedTime * 5));

	if(m_fHurtTimer != 0.0f)					//cjm - 10/06/12
	{											//cjm - 10/06/12
		m_fHurtTimer -= fElapsedTime;			//cjm - 10/06/12
		if(m_fHurtTimer <= 0.0f)				//cjm - 10/06/12
			m_fHurtTimer = 0.0f;				//cjm - 10/06/12
	}											//cjm - 10/06/12

	//Update Particle
	//CParticleManager::GetInstance()->Update(fElapsedTime);
	//
	//tVector2D vMouseTarget = { 0, -1 };
	//
	//vMouseTarget.fX = (float)pDI->MouseGetPosX();
	//vMouseTarget.fY = (float)pDI->MouseGetPosY();
	//
	////Orientation vector
	////starting orientation for the player
	//tVector2D vOrientation = { 0, -1 };
	//
	////Rotate the vector
	////vOrientation = Vector2DRotate( vOrientation, GetRotation() );
	//
	//tVector2D vMouseTpPlayerTarget;
	//vMouseTpPlayerTarget.fX = vMouseTarget.fX - (GetPosX() - CCamera::GetInstance()->GetX());
	//vMouseTpPlayerTarget.fY = vMouseTarget.fY - (GetPosY() - CCamera::GetInstance()->GetY());
	//
	//vMouseTpPlayerTarget = Vector2DNormalize(vMouseTpPlayerTarget);
	//// Calculate the angle between the vectors (0 -> pi)
	//float angle = AngleBetweenVectors( vOrientation, vMouseTpPlayerTarget );
	//float a = 96;//GetWidth() * 0.5f;
	//float b = 96;//GetHeight() * 0.5f;
	////angle  -= 90*3.14f/180;
	//if( Steering( vOrientation, vMouseTpPlayerTarget ) < 0.0f )
	//	angle = -angle;
	//float x = (a * b) / sqrt(b * b + a * a * tan(angle) * tan(angle));
	//float y = (a * b * tan(angle)) / sqrt(b * b + a * a * tan(angle) * tan(angle));
	//angle = angle*180/3.14f;	

	// starting the muzzle splash at the right angle
	//double angle2 = GetRotation()*57.27f;
	//if(angle2 >= 0 && angle2 < 180)
	//{
	//
	//}
	//else
	//{
	//	angle2 = -angle2;
	//}


	//if( angle > 0 )
	//{
	//	m_pMuzzleFlashEffect->SetEmitterPosX((int)(GetPosX() + x));// - ccamera::getinstance()->getx());
	//	m_pMuzzleFlashEffect->SetEmitterPosY((int)(GetPosY() + y));// - ccamera::getinstance()->gety());
	//}
	//else
	//{
	//	m_pMuzzleFlashEffect->SetEmitterPosX((int)(GetPosX() - x));// - ccamera::getinstance()->getx());
	//	m_pMuzzleFlashEffect->SetEmitterPosY((int)(GetPosY() - y));// - ccamera::getinstance()->gety());
	//}
	//m_pMuzzleFlashEffect->Update(fElapsedTime);

	//CParticleManager::GetInstance()->Update(fElapsedTime);

	//m_pMuzzleFlashEffect->SetEmitterPosX((int)vMouseTpPlayerTarget.fX +50);	//cjm - 10/08/12
	//m_pMuzzleFlashEffect->SetEmitterPosY((int)vMouseTpPlayerTarget.fY +5);	//cjm - 10/08/12
	//m_pMuzzleFlashEffect->Update(fElapsedTime);								//cjm - 10/08/12

	if(this->GetKills()==20&&m_bKillSound1==false)
	{
		m_bKillSound1=true;
		if(pXA->SFXIsSoundPlaying(m_nKillSound1SFXID)==false)
			pXA->SFXPlaySound(m_nKillSound1SFXID);
	}
	if(this->GetKills()==40&&m_bKillSound2==false)
	{
		m_bKillSound2=true;
		if(pXA->SFXIsSoundPlaying(m_nKillSound2SFXID)==false)
			pXA->SFXPlaySound(m_nKillSound2SFXID);
	}
	if(this->GetKills()==60&&m_bKillSound3==false)
	{
		m_bKillSound3=true;
		if(pXA->SFXIsSoundPlaying(m_nKillSound3SFXID)==false)
			pXA->SFXPlaySound(m_nKillSound3SFXID);
	}

	m_fProductionTimer += fElapsedTime;
	if(m_fProductionTimer >= 10.0f)
	{
		IncreaseOre(m_nOreRate);
		IncreaseFood((float)m_nFoodRate);
		m_fProductionTimer = 0.0f;
	}

	if(GetHealth() >= 0)
	{

		//m_fFireRate = GetFireRate();		
		if(m_nDamage > 10 )
		{
			m_nDamage = GetDamage();
		}

		m_fMoveSpeed = GetMoveSpeed();

		m_fFireTimer += fElapsedTime;

		CGame* pGame = CGame::GetInstance();

		if(m_nHealth <= 0 )
		{

			if(pXA->SFXIsSoundPlaying(m_nDeath))
			{

			}
			else
			{
			//	pXA->SFXPlaySound(m_nDeath);
			}
		}




		tVector2D vMouseTarget = { 0, -1 };

		vMouseTarget.fX = (float)pDI->MouseGetPosX();// + GetPosX();// - CCamera::GetInstance()->GetX()*0.5f;
		vMouseTarget.fY = (float)pDI->MouseGetPosY();// + GetPosY();// - CCamera::GetInstance()->GetY()*0.5f;

		//Orientation vector
		//starting orientation for the player
		tVector2D vOrientation = { 0, -1 };

		//Rotate the vector
		vOrientation = Vector2DRotate( vOrientation, GetRotation() );

		tVector2D vMouseTpPlayerTarget;
		vMouseTpPlayerTarget.fX = vMouseTarget.fX - (GetPosX() - CCamera::GetInstance()->GetX());
		vMouseTpPlayerTarget.fY = vMouseTarget.fY - (GetPosY() - CCamera::GetInstance()->GetY());

		vMouseTpPlayerTarget = Vector2DNormalize(vMouseTpPlayerTarget);
		// Calculate the angle between the vectors (0 -> pi)
		float fAngle = AngleBetweenVectors( vOrientation, vMouseTpPlayerTarget );

		if( fAngle > PLAYER_ROTATION_RATE * fElapsedTime )
			fAngle = PLAYER_ROTATION_RATE * fElapsedTime;

		// Which side is it on: right (+) or left (-)
		if( Steering( vOrientation, vMouseTpPlayerTarget ) < 0.0f )
			fAngle = -fAngle;

		SetRotation( GetRotation() + fAngle );

		//float temp = (GetRotation() + fAngle);

		if(GetWeaponType() != 0)
		{
			m_fPickUpTimer += fElapsedTime;
			if(m_fPickUpTimer >= 30.0f)
			{
				SetWeaponType(0);
				m_fFireRate = m_fPistolRate;
				CurrAnimation->m_nCurrentAnimation = 0;
				m_fPickUpTimer = 0.0f;
			}
		}		



		//m_pMuzzleFlashEffect->SetEmitterPosX((int)vMouseTpPlayerTarget.fX +50);	//cjm - 10/08/12
		//m_pMuzzleFlashEffect->SetEmitterPosY((int)vMouseTpPlayerTarget.fY +5);	//cjm - 10/08/12
		//m_pMuzzleFlashEffect->Update(fElapsedTime);								//cjm - 10/08/12
		//m_pMuzzleFlashEffect->SetEmitterRotation(temp);


		Input();
		CMovingObject::Update(fElapsedTime);

		//out of bounds check
		if( GetPosX() + GetWidth() > CTileManager::GetInstance()->GetWorldWidth() )
		{
			SetPosX( (float)(CTileManager::GetInstance()->GetWorldWidth()  - GetWidth()) );
		}

		if( GetPosX() /*- (GetWidth())*/ < 0 )
		{
			SetPosX( 0 /*+ GetWidth()*/ );
		}

		if( GetPosY() + GetHeight() > CTileManager::GetInstance()->GetWorldHeight() )
		{
			SetPosY( (float)(CTileManager::GetInstance()->GetWorldHeight() - GetHeight()) );
		}

		if(GetPosY() /*- (GetHeight())*/ < 0 )
		{
			SetPosY( 0/*GetHeight()*/ );
		}

//   		CVoiceReco* pVR = CVoiceReco::GetInstance();
//   		pVR->StartListening();
//   		pVR->Listen();

		SetVelY(0);
		SetVelX(0);

		if(m_bIsText == true)
		{
			m_fTextTimer += fElapsedTime;

			if( m_fTextTimer > 0.5f)
			{

				m_bIsText = false;
				m_fTextTimer = 0.0f;
			}
		}


			if(m_bIsAmmoText == true)
		{
			m_fAmmoTimer += fElapsedTime;

			if( m_fAmmoTimer > 0.5f)
			{

				m_bIsAmmoText = false;
				m_fAmmoTimer = 0.0f;
			}
			

		}
	}

	//if(GetWeaponType() == 1)
	//{
	//	if (m_bShootParticle)
	//	{
	//		m_bShootParticle = false;
	//	}
	//}

}

void CPlayer::MiniMapRender()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();


	RECT rSource = { 0, 0, 64, 64 };
	pTm->Draw(GetImageID(), ((int)(GetPosX() * 0.0365f) +606), ((int)(GetPosY() * 0.0365f ) +406), 0.09f, 0.09f, &rSource,(rSource.left + rSource.right) * 0.5f,  (rSource.top + rSource.bottom) * 0.5f, GetRotation(), D3DCOLOR_RGBA(255,255,255,225));
}


void CPlayer::Render()
{
	CSGD_TextureManager* pTm = CSGD_TextureManager::GetInstance();
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	CBitmapFont* pFont = CBitmapFont::GetInstance();

	//if (m_bFlash)																								//cjm - 10/08/12
	//{																											//cjm - 10/08/12
	//	CParticleManager::GetInstance()->Render("muzzleFlash",((int)GetPosX() + 50), ((int)GetPosY() + 50), GetRotation());		//cjm - 10/08/12

	//	/*rotate then translate.
	//		need to translate to the players x, y.*/

	//	m_pMuzzleFlashEffect->Render();																						//cjm - 10/08/12
	//	m_bFlash = false;																										//cjm - 10/08/12
	//}

	//printing the angle
	//CSGD_Direct3D::GetInstance()->GetSprite()->Flush();

	//std::stringstream ss;//create a stringstream
	//ss <<GetRotation();//add number to the stream
	//
	//CBitmapFont::GetInstance()->PrintString(ss.str().c_str(), 100, 140, 1.0f, 255, 0, 0, 0);

	RECT rsource = { 100, 0, 180, 100 };

	if(CurrAnimation->m_nCurrentAnimation == 0)
	{
		CAnimationManager::GetInstance()->Render(CurrAnimation,m_nPlayerpistolImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1, 1, 0,0,GetRotation(), D3DCOLOR_RGBA(255,255,255,255));
	}
	else if( CurrAnimation->m_nCurrentAnimation == 1 )
	{
		CAnimationManager::GetInstance()->Render(CurrAnimation,m_nPlayerpistolImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1, 1, 0,0,GetRotation(), D3DCOLOR_RGBA(255,255,255,255));
	}
	else if( CurrAnimation->m_nCurrentAnimation == 2 )
	{
		CAnimationManager::GetInstance()->Render(CurrAnimation,m_nPlayersniperImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1, 1, 0,0,GetRotation(), D3DCOLOR_RGBA(255,255,255,255));
	}
	else if( CurrAnimation->m_nCurrentAnimation == 3 )
	{
		CAnimationManager::GetInstance()->Render(CurrAnimation,m_nPlayersniperImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1, 1, 0,0,GetRotation(),D3DCOLOR_RGBA(255,255,255,255));
	}
	else if( CurrAnimation->m_nCurrentAnimation == 4)
	{
		CAnimationManager::GetInstance()->Render(CurrAnimation,m_nPlayerMachineGunnerImgID ,(int)(GetPosX() - CCamera::GetInstance()->GetX()), (int)(GetPosY() - CCamera::GetInstance()->GetY()), 1, 1, 0,0,GetRotation(), D3DCOLOR_RGBA(255,255,255,255));
	}


	//if (m_bShootParticle == true )
	//{
	//	CParticleManager* pPm = CParticleManager::GetInstance();	
	//	m_pHealingParticleEffect->Render();
	//	m_bShootParticle = false;
	//}

	RECT Rsource = { 636,272, 656,308};
	//	pTm->Draw(m_nPlayerpistolImgID, (int)GetPosX() - CCamera::GetInstance()->GetX() + 10, (int)GetPosY() - CCamera::GetInstance()->GetY() - 70, 1, 1, &Rsource, - 10, 70, GetRotation());




//pTm->Draw(m_nCursorImgID, pDI->MouseGetPosX() + 16, pDI->MouseGetPosY() + 16);


	if(m_bIsText == true)
	{
		ostringstream output;
		output << "$200";
		pFont->PrintString(output.str().c_str(), (int)GetPosX() - (int)CCamera::GetInstance()->GetX(), (int)GetPosY() - (int)CCamera::GetInstance()->GetY(), 1.0f, 255, 0, 255, 0);
		output.str("");

	}
	if(m_bIsAmmoText == true)
	{
		ostringstream output;
		output << "+25 Ammo";
		pFont->PrintString(output.str().c_str(), (int)GetPosX() - (int)CCamera::GetInstance()->GetX() - 50, (int)GetPosY() - (int)CCamera::GetInstance()->GetY()+ 60, 1.0f, 255, 0, 0, 255);
		output.str("");
	}

}
// Commit push

void CPlayer::Input()
{
	CSGD_DirectInput* pDI = CSGD_DirectInput::GetInstance();
	CVoiceReco* pVR = CVoiceReco::GetInstance();
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	//if( pDI->KeyPressed( DIK_Y))
	//{
	//	CurrAnimation->m_nCurrentAnimation = 0;
	//}
	//if( pDI->KeyPressed( DIK_U))
	//{
	//	CurrAnimation->m_nCurrentAnimation = 2;
	//}
	//if( pDI->KeyPressed( DIK_I))
	//{
	//	CurrAnimation->m_nCurrentAnimation = 3;
	//}

	if( pDI->KeyDown( DIK_W ) || pDI->JoystickGetLStickDirDown(DIR_UP))
	{
		SetVelY( -m_fMoveSpeed  );
		//CurrAnimation->m_bIsLooping = true;
		//CurrAnimation->m_bIsPlaying = true;
	}
	//else
	//{
	//	CurrAnimation->m_bIsPlaying = false;
	//	CurrAnimation->m_bIsLooping = false;

	//}
	if( pDI->KeyDown( DIK_S ) || pDI->JoystickGetLStickDirDown(DIR_DOWN) )
	{
		SetVelY( m_fMoveSpeed );
		//CurrAnimation->m_bIsLooping = true;
		//CurrAnimation->m_bIsPlaying = true;
	}
	//else
	//{
	//	CurrAnimation->m_bIsPlaying = false;
	//	CurrAnimation->m_bIsLooping = false;

	//}
	if( pDI->KeyDown( DIK_A ) || pDI->JoystickGetLStickDirDown(DIR_LEFT) )
	{
		SetVelX( -m_fMoveSpeed );
		/*	CurrAnimation->m_bIsLooping = true;
		CurrAnimation->m_bIsPlaying = true;*/
	}
	//else
	//{
	//	//CurrAnimation->m_bIsPlaying = false;
	//	//CurrAnimation->m_bIsLooping = false;

	//}
	if( pDI->KeyDown( DIK_D ) || pDI->JoystickGetLStickDirDown(DIR_RIGHT) )
	{
		SetVelX( m_fMoveSpeed );		
	}
	//else
	//{ 
	//	CurrAnimation->m_bIsPlaying = false;
	//	CurrAnimation->m_bIsLooping = false;

	//}

	if( pDI->KeyDown( DIK_W ) || pDI->KeyDown( DIK_S ) || pDI->KeyDown( DIK_A ) || pDI->KeyDown( DIK_D )
		|| pDI->JoystickGetLStickDirDown(DIR_UP) || pDI->JoystickGetLStickDirDown(DIR_DOWN) || pDI->JoystickGetLStickDirDown(DIR_LEFT) || pDI->JoystickGetLStickDirDown(DIR_RIGHT))
	{
		CurrAnimation->m_bIsLooping = true;
		CurrAnimation->m_bIsPlaying = true;
	}
	else
	{
		CurrAnimation->m_bIsLooping = false;
		CurrAnimation->m_bIsPlaying = false;
		CurrAnimation->m_nCurrentFrame = 2;
	}


	//CurrAnimation->m_bIsPlaying = false;


	// Use a Healthpack!
	if(pDI->KeyPressed(DIK_F) || pDI->JoystickButtonPressed(4))
	{
		if(GetHealth() != GetMaxHealth())
		{
			if(GetHealthPacks() > 0)
			{
				pXA->SFXPlaySound(m_nUseItemSFXID);
				DecreaseHealthPacks(1);
				if(GetHealth() + 100 > GetMaxHealth())
					SetHealth(GetMaxHealth());
				else
					SetHealth(GetHealth() + 100);
			}
		}
	}

	if((pDI->MouseButtonDown( (char)DIMOUSE_XAXISAB ) && m_fFireTimer >= m_fFireRate) || (pDI->JoystickButtonDown(5) && m_fFireTimer >= m_fFireRate))
	{
		if(GetAmmo() > 0 || GetWeaponType() != 0)
		{
			m_fFireTimer = 0.0f;

			if(GetWeaponType() == 0)	// Pistol Shooting
			{
				pXA->SFXPlaySound(m_nTempSfxID);
				DecreaseAmmo(1);
			}

			switch(GetWeaponType())
			{
			case 1:		// Machine Gun Shooting
				pXA->SFXPlaySound(m_nFireMGSFXID);
				break;

			case 2:		// Shotgun Firing
					pXA->SFXPlaySound(m_nFireShotgunSFXID);		
				break;

			case 3:		// Sniper Firing
				pXA->SFXPlaySound(m_nFireSniperSFXID);
				break;

			case 4:		// Rocket Firing
				pXA->SFXPlaySound(m_nFireShotgunSFXID);
				break;			
			};

			m_bFlash = true;		//cjm - 10/08/12


			if(GetWeaponType() == 2)
			{
				CCreateShotgunBullet* pSBullet = new CCreateShotgunBullet( this );
				CMessageSystem::GetInstance()->SendMsg( pSBullet );
				pSBullet = nullptr;

				//CCreateBulletMessage* pBullet = new CCreateBulletMessage( this );
				//CMessageSystem::GetInstance()->SendMsg( pBullet );
				//pBullet = nullptr;
			}
			else
			{
			CCreateBulletMessage* pBullets = new CCreateBulletMessage( this );
			CMessageSystem::GetInstance()->SendMsg( pBullets );
			pBullets = nullptr;
			}

		}
		else if(GetAmmo() <= 0)
		{
			if(pXA->SFXIsSoundPlaying(m_nDryFireSFXID)==false)
				pXA->SFXPlaySound(m_nDryFireSFXID);
		}

		//	//if(GetAmmo() > 0 || GetWeaponType() != 0) ///??? WHat is this logic ? Just add ammo to the initial if check
		////	{

		//		m_fFireTimer = 0.0f;

		//		if(GetWeaponType() == 0)
		//		{
		//	//		if(pXA->SFXIsSoundPlaying(m_nTempSfxID)==false)
		//				pXA->SFXPlaySound(m_nTempSfxID);
		//			DecreaseAmmo(1);
		//			m_bFlash = true;		//cjm - 10/08/12
		//		}
		//		else if(GetWeaponType() == 1)
		//		{
		//		//	if(pXA->SFXIsSoundPlaying(m_nFireMGSFXID)==false)
		//				pXA->SFXPlaySound(m_nFireMGSFXID);
		//			DecreaseAmmo(0);
		//		}
		//		else if(GetWeaponType() == 2)
		//		{
		//		//	if(pXA->SFXIsSoundPlaying(m_nFireShotgunSFXID)==false)
		//				pXA->SFXPlaySound(m_nFireShotgunSFXID);
		//			DecreaseAmmo(0);
		//			m_bFlash = true;	
		//		}
		//		else if(GetWeaponType() == 3)
		//		{
		//			//if(pXA->SFXIsSoundPlaying(m_nFireSniperSFXID)==false)
		//				pXA->SFXPlaySound(m_nFireSniperSFXID);
		//			DecreaseAmmo(0);
		//			m_bFlash = true;		
		//		}
		//		else if(GetWeaponType() == 4)
		//		{
		//			//if(pXA->SFXIsSoundPlaying(m_nFireShotgunSFXID)==false)
		//			pXA->SFXPlaySound(m_nFireShotgunSFXID);
		//			DecreaseAmmo(0);
		//			m_bFlash = true;		
		//		}
		//		//sending an event to shoot
		//		//CEventManager::GetInstance()->sendEvent("shoot");

		//	CCreateBulletMessage* pBullet = new CCreateBulletMessage( this );
		//	CMessageSystem::GetInstance()->SendMsg( pBullet );
		//	pBullet = nullptr;
		////}
		//else if(GetAmmo() <= 0)
		//{
		//	pXA->SFXPlaySound(m_nDryFireSFXID);
		//}
	}
	//if(pDI->MouseButtonDown( (char)DIMOUSE_XAXISAB ) && m_fFireTimer >= GetFireRate() && GetAmmo() <= 0)
	//{
	//	//;
	//	m_fFireTimer = 0.0f;
	//	if(pXA->SFXIsSoundPlaying(m_nDryFireSFXID)==false)
	//	
	//}

	//attack input
	if(pDI->KeyPressed(DIK_1) || pDI->JoystickButtonDown(0) )
	{
	
	//	pXA->SFXPlaySound(m_nCommandConfirmedSFXID);
		//sending an event to attack
		CEventManager::GetInstance()->sendEvent("attack");
	}
	if(pDI->KeyPressed(DIK_2) || pDI->JoystickButtonDown(1))
	{

//		pXA->SFXPlaySound(m_nCommandConfirmedSFXID);
		//sending an event to defend the surrounding area
		CEventManager::GetInstance()->sendEvent("defend");
	}
	if(pDI->KeyPressed(DIK_3) || pDI->JoystickButtonDown(2))
	{

	//	pXA->SFXPlaySound(m_nCommandConfirmedSFXID);
		//sending an event to follow the player
		CEventManager::GetInstance()->sendEvent("follow");
	}

}

void CPlayer::HandleEvent( IEvent* pEvent )
{
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();

	if( pEvent->GetEventID() == "shoot" )
	{
		pXA->SFXPlaySound(m_nTempSfxID);
		if(GetWeaponType() == ROCKET)	//cjm - 10/09/12
		{								//cjm - 10/09/12
			m_bShootParticle = true;
		}								//cjm - 10/09/12

	}

	if(pEvent->GetEventID() == "get_kills")
	{
		m_nKills++;
	}

	if(pEvent->GetEventID() == "confirmed")
	{
		if(pXA->SFXIsSoundPlaying(m_nCommandConfirmedSFXID)==false)
		pXA->SFXPlaySound(m_nCommandConfirmedSFXID);
	}
	if(pEvent->GetEventID() == "healed")
	{
		if(pXA->SFXIsSoundPlaying(m_nHealedSFXID)==false)
			pXA->SFXPlaySound(m_nHealedSFXID);
	}
	/*
	if(pEvent->GetEventID() == "Rocketcollided")
	{
	pXA->SFXPlaySound(m_nCommandConfirmedSFXID);
	}
	*/

}

#pragma region Decrease Functions

void CPlayer::DecreaseFood( float fFood )
{
	if( m_fFood > 0 )
		m_fFood -= fFood;
}

void CPlayer::DecreaseCash( int nCash )
{
	if( m_nCash > 0 )
		m_nCash -= nCash;
	if( m_nCash < 0 )
		m_nCash = 0;
}

void CPlayer::DecreaseOre( int nOre )
{
	if( m_nOre > 0 )
		m_nOre -= nOre;
	if( m_nOre < 0 )
		m_nOre = 0;
}

void CPlayer::DecreaseHealthPacks( int nHealthPacks )
{
	if( m_nHealthPacks > 0 )
		m_nHealthPacks -= nHealthPacks;
	if( m_nHealthPacks < 0 )
		m_nHealthPacks = 0;
}

void CPlayer::TakeDamage( int nDamage )
{
	if( m_nHealth > 0)
		m_nHealth -= nDamage;
	if( m_nHealth < 0 )
		m_nHealth = 0;
}

void CPlayer::DecreaseArmor( int nDamage )
{
	if( m_nArmor > 0 )
		m_nArmor -= nDamage;
	if( m_nArmor < 0 )
		m_nArmor = 0;
}

void CPlayer::DecreasePopulation( int nPopulation )
{
	if( m_nPopulation > 0 )
		m_nPopulation -= nPopulation;
	if( m_nPopulation < 0 )
		m_nPopulation = 0;
}

void CPlayer::DecreaseMedics(int nMedic)
{
	m_nMedics -= nMedic;
	if( m_nMedics < 0 )
		m_nMedics = 0;
}

void CPlayer::DecreaseMGs(int nMG)
{
	m_nMGs -= nMG;
	if( m_nMGs < 0 )
		m_nMGs = 0;
}

void CPlayer::DecreaseSnipers(int nSniper)
{
	m_nSnipers -= nSniper;
	if( m_nSnipers < 0 )
		m_nSnipers = 0;
}

void CPlayer::DecreaseRockets(int nRockets)
{
	m_nRockets -= nRockets;
	if( m_nRockets < 0 )
		m_nRockets = 0;
}

void CPlayer::DecreaseBunkers(int nBunkers)
{
	m_nBunkers -= nBunkers;
	if( m_nBunkers < 0 )
		m_nBunkers = 0;
}

void CPlayer::DecreaseStoreHouses(int nSH)
{
	m_nStoreHouses -= nSH;
	if( m_nStoreHouses < 0 )
		m_nStoreHouses = 0;
}

void CPlayer::DecreaseMines(int nMines)
{
	m_nMines -= nMines;
	if( m_nMines < 0 )
		m_nMines = 0;
}

void CPlayer::DecreaseAmmo( int nAmmo )
{
	if( m_nAmmo > 0 )
		m_nAmmo -= nAmmo;
	if( m_nAmmo < 0 )
		m_nAmmo = 0;
}

#pragma endregion 

bool CPlayer::CheckCollision(IEntity* pOther)
{
	CBitmapFont* pFont = CBitmapFont::GetInstance();
	CMessageSystem* pMS = CMessageSystem::GetInstance();
	CSGD_XAudio2* pXA = CSGD_XAudio2::GetInstance();
	if(CEntity::CheckCollision(pOther) == false)
		return false;

	switch(pOther->GetType())
	{
	case OBJ_BASE:
		return false;
	case OBJ_ENEMY:
		{
			CEnemy* pEnemy = dynamic_cast<CEnemy*>(pOther);

			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &pEnemy->GetRect()))
			{
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pEnemy->GetRect().left<= GetRect().right && pEnemy->GetRect().right>GetRect().right && pEnemy->GetRect().bottom>GetRect().top && pEnemy->GetRect().top<GetRect().bottom)
					SetPosX((float)(pEnemy->GetRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pEnemy->GetRect().right>=GetRect().left && pEnemy->GetRect().left<GetRect().right && pEnemy->GetRect().bottom>GetRect().top && pEnemy->GetRect().top<GetRect().bottom)
					SetPosX((float)(pEnemy->GetRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pEnemy->GetRect().top<=GetRect().bottom && pEnemy->GetRect().bottom>GetRect().bottom && pEnemy->GetRect().right>GetRect().left && pEnemy->GetRect().left<GetRect().right)
					SetPosY((float)(pEnemy->GetRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pEnemy->GetRect().bottom>=GetRect().top && pEnemy->GetRect().top<GetRect().top && pEnemy->GetRect().right>GetRect().left && pEnemy->GetRect().left<GetRect().right)
					SetPosY((float)(pEnemy->GetRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));	
			}
			/*tVector2D toTarget = { pEnemy->GetPosX() - GetPosX(), pEnemy->GetPosY() - GetPosY() };
			Vector2DNormalize(toTarget);
			tVector2D normalVector = {0,-1};

			float angle = AngleBetweenVectors(toTarget,normalVector);

			if(Steering(toTarget,normalVector) < 0)
				angle = -angle;

			if( angle > 0 && angle <= 1.57 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle > 1.57 && angle <= 3.14 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle < -1.57 && angle >= -3.14 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()+10);
			}
			else if( angle < 0 && angle >= -1.57 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()-10);
			}*/

			return false;
		}
	case OBJ_ALLY:
		{
			CAlly* pAlly = dynamic_cast<CAlly*>(pOther);

			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &pAlly->GetRect()))
			{
				//entered from left
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pAlly->GetRect().left<= GetRect().right && pAlly->GetRect().right>GetRect().right && pAlly->GetRect().bottom>GetRect().top && pAlly->GetRect().top<GetRect().bottom)
					SetPosX((float)(pAlly->GetRect().left + CCamera::GetInstance()->GetX() - GetWidth()*0.5f));
				//entered from right
				if( (rectTemp.bottom - rectTemp.top) > (rectTemp.right - rectTemp.left) && pAlly->GetRect().right>=GetRect().left && pAlly->GetRect().left<GetRect().right && pAlly->GetRect().bottom>GetRect().top && pAlly->GetRect().top<GetRect().bottom)
					SetPosX((float)(pAlly->GetRect().right + CCamera::GetInstance()->GetX() + GetWidth()*0.5f));
				//entered from top
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pAlly->GetRect().top<=GetRect().bottom && pAlly->GetRect().bottom>GetRect().bottom && pAlly->GetRect().right>GetRect().left && pAlly->GetRect().left<GetRect().right)
					SetPosY((float)(pAlly->GetRect().top + CCamera::GetInstance()->GetY() - GetHeight()*0.5f));
				//entered from bottom
				if( (rectTemp.bottom - rectTemp.top) < (rectTemp.right - rectTemp.left) && pAlly->GetRect().bottom>=GetRect().top && pAlly->GetRect().top<GetRect().top && pAlly->GetRect().right>GetRect().left && pAlly->GetRect().left<GetRect().right)
					SetPosY((float)(pAlly->GetRect().bottom + CCamera::GetInstance()->GetY() + GetHeight()*0.5f));		
			}
			//entered from left
			//if(GetRect().right>=pAlly->GetRect().left && GetRect().top<pAlly->GetRect().bottom && GetRect().bottom>pAlly->GetRect().top && GetRect().left<pAlly->GetRect().left)
			//	SetPosX((float)(pAlly->GetRect().left - GetWidth()*0.5f));
			////entered from right
			//else if(GetRect().left<=pAlly->GetRect().right && GetRect().right>pAlly->GetRect().right && GetRect().top<pAlly->GetRect().bottom && GetRect().bottom>pAlly->GetRect().top )
			//	SetPosX((float)(pAlly->GetRect().right + GetWidth()*0.5f));
			////entered from top
			//else if(GetRect().bottom>=pAlly->GetRect().top && GetRect().left<pAlly->GetRect().right && GetRect().right>pAlly->GetRect().left && GetRect().top<pAlly->GetRect().top)
			//	SetPosY((float)(pAlly->GetRect().top - GetHeight()*0.5f));
			////entered from bottom
			//else if(GetRect().top<=pAlly->GetRect().bottom && GetRect().left<pAlly->GetRect().right && GetRect().right>pAlly->GetRect().left && GetRect().bottom>pAlly->GetRect().bottom)
			//	SetPosY((float)(pAlly->GetRect().bottom + GetHeight()*0.5f));	
			/*tVector2D toTarget = { pAlly->GetPosX() - GetPosX(), pAlly->GetPosY() - GetPosY() };
			Vector2DNormalize(toTarget);
			tVector2D normalVector = {0,-1};

			float angle = AngleBetweenVectors(toTarget,normalVector);

			if(Steering(toTarget,normalVector) < 0)
				angle = -angle;

			if( angle > 0 && angle <= 1.57 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle > 1.57 && angle <= 3.14 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle < -1.57 && angle >= -3.14 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()+10);
			}
			else if( angle < 0 && angle >= -1.57 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()-10);
			}*/

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
					m_fHurtTimer = 1.0f;															//cjm = 10/06/12
					m_bBlood = true;																//cjm - 10/05/12
				}
				else
				{
					SetHealth(GetHealth());
					m_fHurtTimer = 1.0f;															//cjm = 10/06/12
					m_bBlood = true;																//cjm - 10/05/12
				}
			}
		}
		break;

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
			/*tVector2D toTarget = { pStoreHouse->GetPosX() - GetPosX(), pStoreHouse->GetPosY() - GetPosY() };
			Vector2DNormalize(toTarget);
			tVector2D normalVector = {0,-1};

			float angle = AngleBetweenVectors(toTarget,normalVector);

			if(Steering(toTarget,normalVector) < 0)
				angle = -angle;

			if( angle > 0 && angle <= 1.57 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle > 1.57 && angle <= 3.14 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle < -1.57 && angle >= -3.14 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()+10);
			}
			else if( angle < 0 && angle >= -1.57 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()-10);
			}*/

			return false;
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
			/*tVector2D toTarget = { pBuker->GetPosX() - GetPosX(), pBuker->GetPosY() - GetPosY() };
			Vector2DNormalize(toTarget);
			tVector2D normalVector = {0,-1};

			float angle = AngleBetweenVectors(toTarget,normalVector);

			if(Steering(toTarget,normalVector) < 0)
				angle = -angle;

			if( angle > 0 && angle <= 1.57 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle > 1.57 && angle <= 3.14 )
			{
				SetPosX(GetPosX()+10);
				SetPosY(GetPosY()-10);
			}
			else if( angle < -1.57 && angle >= -3.14 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()+10);
			}
			else if( angle < 0 && angle >= -1.57 )
			{
				SetPosX(GetPosX()-10);
				SetPosY(GetPosY()-10);
			}*/

			return false;
		}
	case OBJ_PICKUP:
		{
			CPickUp* pObj = dynamic_cast<CPickUp*>(pOther);
			switch(pObj->GetPickUpType())
			{
			case 0:	// Ammo
				{
					if(pXA->SFXIsSoundPlaying(m_nItemPickupSFXID)==false)
						pXA->SFXPlaySound(m_nItemPickupSFXID);
					IncreaseAmmo(10);

					m_bIsAmmoText = true;
				}
				break;

			case 1:	// Cash
				{
					if(pXA->SFXIsSoundPlaying(m_nItemPickupSFXID)==false)
						pXA->SFXPlaySound(m_nItemPickupSFXID);
					IncreaseCash(200);

					m_bIsText = true;
				}
				break;

			case 2:	// Machine Gun
				{	//if(m_fPickUpTimer == 0.0f	)
					if(pXA->SFXIsSoundPlaying(m_nWeaponPickupSFXID)==false)
						pXA->SFXPlaySound(m_nWeaponPickupSFXID);
					SetWeaponType(1);
					m_fFireTimer = m_fFireRate = MG_FIRE_RATE;
					m_fPickUpTimer = 0.0f;
					CurrAnimation->m_nCurrentAnimation = 4;
				}

				break;

			case 3:	// Shotgun
				{	//if(m_fPickUpTimer == 0.0f)
					if(pXA->SFXIsSoundPlaying(m_nWeaponPickupSFXID)==false)
						pXA->SFXPlaySound(m_nWeaponPickupSFXID);		
					SetWeaponType(2);
					m_fFireTimer = m_fFireRate = SHOTGUN_FIRE_RATE;
					m_fPickUpTimer = 0.0f;
					CurrAnimation->m_nCurrentAnimation = 1;
				}
				break;

			case 4:	// Sniper
				{	//if(m_fPickUpTimer == 0.0f)
					if(pXA->SFXIsSoundPlaying(m_nWeaponPickupSFXID)==false)
						pXA->SFXPlaySound(m_nWeaponPickupSFXID);	
					SetWeaponType(3);
					m_fFireTimer = m_fFireRate = SNIPER_FIRE_RATE;
					m_fPickUpTimer = 0.0f;
					CurrAnimation->m_nCurrentAnimation = 2;
				}
				break;

			case 5:	// Rocket
				{	//if(m_fPickUpTimer == 0.0f)
					if(pXA->SFXIsSoundPlaying(m_nWeaponPickupSFXID)==false)
						pXA->SFXPlaySound(m_nWeaponPickupSFXID);	
					SetWeaponType(4);
					m_fFireTimer = m_fFireRate = ROCKET_FIRE_RATE;
					m_fPickUpTimer = 0.0f;
					CurrAnimation->m_nCurrentAnimation = 3;
				}
				break;

			};

			CDestroyPickUpMessage* pMsg2 = new CDestroyPickUpMessage(pObj);
			CMessageSystem::GetInstance()->SendMsg(pMsg2);
			pMsg2 = nullptr;
		}
		break;
	};

	return true;
}

bool CPlayer::CheckTileCollision()
{
	CTileManager* pTileManager = CTileManager::GetInstance();

	//unsigned int TileCol = (GetPosX() - CCamera::GetInstance()->GetX()) / pTileManager->GetMapWidth()  - CCamera::GetInstance()->GetX();
	//unsigned int TileRow = (GetPosY() - CCamera::GetInstance()->GetY()) / pTileManager->GetMapHeight() - CCamera::GetInstance()->GetY();
	//QQQQ
	//int Tile = pTileManager->GetLayer[0].m_vTiles[TileRow * pTileManager->GetMapWidth() + TileCol -1 ].;

	std::vector<CTileManager::Tile_L1> vTile = pTileManager->GetLayer1();

	for(unsigned int i = 0; i < vTile.size(); i++)
	{
		if(vTile[i].m_bIsCollidable)
		{
			RECT rectTemp;
			if(IntersectRect(&rectTemp, &GetRect(), &vTile[i].GetCollisionRect()))
			{
				CEventManager::GetInstance()->sendEvent(vTile[i].m_szEventName);
				
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
				
				/*tVector2D toTarget = { vTile[i].GetCollisionRect().left + 32 - GetPosX(), vTile[i].GetCollisionRect().top + 32 - GetPosY() };
				Vector2DNormalize(toTarget);
				tVector2D normalVector = {0,-1};

				float angle = AngleBetweenVectors(toTarget,normalVector);

				if(Steering(toTarget,normalVector) < 0)
					angle = -angle;

				if( angle > 0 && angle <= 1.57 )
				{
					SetPosX(GetPosX()+10);
				}
				else if( angle > 1.57 && angle <= 3.14 )
				{
					SetPosX(GetPosX()+10);
				}
				else if( angle < -1.57 && angle >= -3.14 )
				{
					SetPosY(GetPosY()+10);
				}
				else if( angle < 0 && angle >= -1.57 )
				{
					SetPosY(GetPosY()-10);
				}*/

				////entered from left
				//if(GetRect().right>=vTile[i].GetCollisionRect().left && GetRect().top<vTile[i].GetCollisionRect().bottom && GetRect().bottom>vTile[i].GetCollisionRect().top && GetRect().left<vTile[i].GetCollisionRect().left)
				//	SetPosX((float)(vTile[i].GetCollisionRect().left - GetWidth()*0.5f));
				////entered from right
				//else if(GetRect().left<=vTile[i].GetCollisionRect().right && GetRect().top<vTile[i].GetCollisionRect().bottom && GetRect().bottom>vTile[i].GetCollisionRect().top && GetRect().right>vTile[i].GetCollisionRect().right)
				//	SetPosX((float)(vTile[i].GetCollisionRect().right + GetWidth()*0.5f));
				////entered from top
				//else if(GetRect().bottom>=vTile[i].GetCollisionRect().top && GetRect().left<vTile[i].GetCollisionRect().right && GetRect().right>vTile[i].GetCollisionRect().left && GetRect().top<vTile[i].GetCollisionRect().top)
				//	SetPosY((float)(vTile[i].GetCollisionRect().top - GetHeight()*0.5f));
				////entered from bottom
				//else if(GetRect().top<=vTile[i].GetCollisionRect().bottom && GetRect().left<vTile[i].GetCollisionRect().right && GetRect().right>vTile[i].GetCollisionRect().left && GetRect().bottom>vTile[i].GetCollisionRect().bottom)
				//	SetPosY((float)(vTile[i].GetCollisionRect().bottom + GetHeight()*0.5f));				

				return true;
			}
		}
	}

	//CTileManager::Tile_L1 tTile2 = pTileManager->GetTile(0, (TileRow -1) * pTileManager->GetMapWidth() + TileCol);
	//CTileManager::Tile_L1 tTile3 = pTileManager->GetTile(0, (TileRow-1) * pTileManager->GetMapWidth() + TileCol + 1);
	//CTileManager::Tile_L1 tTile4 = pTileManager->GetTile(0, TileRow * pTileManager->GetMapWidth() + TileCol -1);
	//CTileManager::Tile_L1 tTile5 = pTileManager->GetTile(0, TileRow * pTileManager->GetMapWidth() + TileCol +1);
	//CTileManager::Tile_L1 tTile6 = pTileManager->GetTile(0, (TileRow + 1) * pTileManager->GetMapWidth() + TileCol -1);
	//CTileManager::Tile_L1 tTile7 = pTileManager->GetTile(0, (TileRow + 1) * pTileManager->GetMapWidth() + TileCol);
	//CTileManager::Tile_L1 tTile8 = pTileManager->GetTile(0, (TileRow + 1) * pTileManager->GetMapWidth() + TileCol +1);

	//if( tTile1.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile1.m_szEventName);
	//	return true;
	//}
	//if( tTile2.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile2.m_szEventName);
	//	return true;
	//}
	//if( tTile3.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile3.m_szEventName);
	//	return true;
	//}
	//if( tTile4.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile4.m_szEventName);
	//	return true;
	//}
	//if( tTile5.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile5.m_szEventName);
	//	return true;
	//}
	//if( tTile6.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile6.m_szEventName);
	//	return true;
	//}
	//if( tTile7.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile7.m_szEventName);
	//	return true;
	//}
	//if( tTile8.m_bIsCollidable == true )
	//{
	//	CEventManager::GetInstance()->sendEvent(tTile8.m_szEventName);
	//	return true;
	//}


	return false;

}
