/*
	File:						Player.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/11/2012
	Purpose:				Contain / manage the game code
*/

#pragma once
#include "movingobject.h"
#include "Animation.h"
#include "AnimationInfo.h"
#include "../SGD Wrappers/IListener.h"
#include "IEvent.h"
#include "TileManager.h"

//Particles System
//#include "ParticleManager.h"
//Emitter
#include "Emitter.h"

class CPlayer : public CMovingObject, public IListener
{
private:

	//data members to be manipulated during gameplay
													//                                     NOTES                                                        //
	float				m_fPickUpTimer;				//only needs an accessor because we dont want this variable to be mutated.
	float				m_fFood;							
	float				m_fMoveSpeed;
	float				m_fFireRate;
	float				m_fPistolRate;
	float				m_fFireTimer;				//only needs an accessor because we dont want this variable to be mutated.
	float				m_fProductionTimer;

	int					m_nOreRate;
	int					m_nFoodRate;

	int					m_nDamage;
	int					m_nAmmo;
	int					m_nCash;
	int					m_nOre;
	int					m_nWeaponType;
	int					m_nHealthPacks;
	int					m_nMaxHealthPacks;
	int					m_nHealth;
	int					m_nMaxHealth;
	int					m_nArmor;
	int					m_nPopulation;
	int					m_nKills;

	int					m_nMedics;
	int					m_nMGs;
	int					m_nSnipers;
	int					m_nRockets;

	int					m_nBunkers;
	int					m_nStoreHouses;
	int					m_nMines;

	// Image ID:
	int					m_nCursorImgID;
	int					m_nHealthID;
	int					m_nHealthPackImgID;
	int					m_nFoodImgID;
	int					m_nOreImgID;
	int					m_nHUDImgID;

	// Audio ID:
	int					m_nTempSfxID;
	int					m_nDeath;
	int					m_nCommandConfirmedSFXID;
	int					m_nDryFireSFXID;
	int					m_nUseItemSFXID;

	int					m_nItemPickupSFXID;
	int					m_nWeaponPickupSFXID;
	int					m_nFireMGSFXID;
	int					m_nFireRocketSFXID;
	int					m_nFireSniperSFXID;
	int					m_nFireShotgunSFXID;
	int					m_nHealedSFXID;

	int					m_nKillSound1SFXID;
	int					m_nKillSound2SFXID;
	int					m_nKillSound3SFXID;
	bool				m_bKillSound1;
	bool				m_bKillSound2;
	bool				m_bKillSound3;

	bool				m_bSurroundingTilesOccupied[8];

	bool				m_bShootParticle;


	bool				m_bIsText;
	float				m_fTextTimer;

	bool				m_bIsAmmoText;
	float				m_fAmmoTimer;

	// Animation Variables:

	//CAnimation* currAnimation;

	CAnimationInfo* CurrAnimation;

	int					m_nPlayerpistolImgID;
	int					m_nPlayershotgunImgID;
	int					m_nPlayersniperImgID;
	int					m_nPlayerRocketImgID;
	int					m_nPlayerMachineGunnerImgID;


	CEmitter*			m_pHealingParticleEffect;
	bool				m_bBlood;					//cjm - 10/05/12
	float				m_fHurtTimer;				//cjm - 10/06/12
	bool				m_bFlash;					//cjm - 10/08/12
	CEmitter*			m_pMuzzleFlashEffect;		//cjm - 10/08/12

	


public:
	CPlayer(void);
	virtual ~CPlayer(void);

	//instead of setting these functions we will increment or decrement them

	//increase
	void IncreaseFood( float fFood )							{ m_fFood += fFood; }
	void IncreaseCash( int nCash )							{ m_nCash += nCash; }
	void IncreaseOre( int nOre )								{ m_nOre += nOre; }
	void IncreaseHealthPacks( int nHealthPacks )		{ m_nHealthPacks += nHealthPacks; }
	void IncreaseHealth( int nHealth )						{ m_nHealth += nHealth; }
	void IncreaseFoodRate( int nRate )						{ m_nFoodRate += nRate; }
	void IncreaseOreRate( int nRate )						{ m_nOreRate += nRate; }
	void IncreasePopulation( int nPopulation )			{ m_nPopulation += nPopulation; }
	void IncreaseMedics(int nMedic)							{ m_nMedics += nMedic; }
	void IncreaseMGs(int nMG)									{ m_nMGs += nMG; }
	void IncreaseSnipers(int nSniper)						{ m_nSnipers += nSniper; }
	void IncreaseRockets(int nRockets)						{ m_nRockets += nRockets; }

	void IncreaseBunkers(int nBunkers)					{ m_nBunkers += nBunkers; }
	void IncreaseStoreHouses(int nSH)					{ m_nStoreHouses += nSH; }
	void IncreaseMines(int nMines)						{ m_nMines += nMines; }

	void DecreaseBunkers(int nBunkers);
	void DecreaseStoreHouses(int nSH);
	void DecreaseMines(int nMines);


	//Upgrades
	void IncreaseMaxHealth( int nMaxhealth )				{ m_nMaxHealth += nMaxhealth; }
	void IncreaseArmor( int nArmor )							{ m_nArmor  += nArmor; }
	void IncreaseMovementSpeed( float fMoveSpeed )    { m_fMoveSpeed += fMoveSpeed; }
	void IncreaseDamage( int nDamage )					{ m_nDamage += nDamage;}
	void IncreaseRateOfFire( float fRateOfFire )				{ m_fPistolRate += fRateOfFire; }

	///in addition to having a mutator for the ammo we will also have an increment and decrement function
	//used for when we pick up ammo for a gun we have already have
	void IncreaseAmmo( int nAmmo )						{ m_nAmmo += nAmmo; }

	//Decrease
	void DecreaseFood( float fFood );
	void DecreaseCash( int nCash ); 
	void DecreaseOre( int nOre );
	void DecreaseHealthPacks( int nHealthPacks );
	void TakeDamage( int nDamage );
	void DecreaseArmor( int nDamage );

	void DecreaseFoodRate( int nRate )					{ m_nFoodRate -= nRate; }
	void DecreaseOreRate( int nRate )					{ m_nOreRate -= nRate; }

	void DecreasePopulation( int nPopulation );
	void DecreaseMedics(int nMedic);
	void DecreaseMGs(int nMG);
	void DecreaseSnipers(int nSniper);
	void DecreaseRockets(int nRockets);

	void SetKills(int nKills)	{ m_nKills = nKills; }

	///in addition to having a mutator for the ammo we will also have an increment and decrement function
	//used for when we pick up ammo for a gun we have already have
	void DecreaseAmmo( int nAmmo );


	//Mutators
	void SetMoveSpeed( float fMoveSpeed )				{ m_fMoveSpeed = fMoveSpeed; }
	void SetFireRate( float fFireRate )					{ m_fPistolRate = fFireRate; }
	void SetLoadFireRate( float fRate)					{ m_fFireRate = fRate; }
	//used when the player picks up a weapon that is different from his current weapon
	void SetAmmo( int nAmmo )							{ m_nAmmo = nAmmo; }
	void SetWeaponType( int nWeapon )					{ m_nWeaponType = nWeapon; }
	void SetMaxHealthPacks( int nMaxHealthPacks )		{ m_nMaxHealthPacks = nMaxHealthPacks; }
	void SetHealthPacks(int nHealthPacks)				{ m_nHealthPacks = nHealthPacks; }
	void SetArmor( int nArmor )							{ m_nArmor = nArmor; }
	void SetHealingParticleEffect(CEmitter* emitter)	{ m_pHealingParticleEffect = emitter;}

	void SetFood(float fFood)	{ m_fFood = fFood; }
	void SetOreRate(int nRate)	{ m_nOreRate = nRate; }
	void SetFoodRate(int nRate)	{ m_nFoodRate = nRate; }
	void SetCash(int nCash)		{ m_nCash = nCash; }
	void SetOre(int nOre)		{ m_nOre = nOre; }
	void SetMaxHealth(int nMax)	{ m_nMaxHealth = nMax; }


	//follow state
	void SetSurroundingTiles(int index){ m_bSurroundingTilesOccupied[index] = true; }

	//accessors
	float GetPickupTimer()		const	{ return m_fPickUpTimer; }
	void  SetPickUpTimer(float nTimer)	{ m_fPickUpTimer = nTimer; }
	float GetFood()				const	{ return m_fFood; }
	
	int GetAmmo()				const	{ return m_nAmmo; }
	int GetCash()				const	{ return m_nCash; }
	int GetOre()				const	{ return m_nOre; }
	int GetWeaponType()			const	{ return m_nWeaponType; }
	int GetHealthPacks()		const	{ return m_nHealthPacks; }
	int GetMaxHealthPacks()		const	{ return m_nMaxHealthPacks; }
	int GetCurrentHealth()		const	{ return m_nHealth; }

	int GetPopulation()			const	{ return m_nPopulation; }
	int GetMedics()				const	{ return m_nMedics; }
	int GetMGs()				const	{ return m_nMGs; }
	int GetSnipers()			const	{ return m_nSnipers; }
	int GetRockets()			const	{ return m_nRockets; }

	int GetBunkers()			const	{ return m_nBunkers; }
	int GetStoreHouses()		const	{ return m_nStoreHouses; }
	int GetMines()				const	{ return m_nMines; }

	int GetHUDImgID()			const	{ return m_nHUDImgID; }
	int GetOreRate()			const	{ return m_nOreRate; }
	int GetFoodRate()			const	{ return m_nFoodRate; }

	float GetHurtTimer(void) { return m_fHurtTimer; }				//cjm - 10/06/12

	virtual void SetAnimationID(int nAnimationID)				{CurrAnimation->m_nCurrentAnimation = nAnimationID; }
	virtual int  GetAnimationID()								{ return CurrAnimation->m_nCurrentAnimation; }

	// Upgrades
	float GetMoveSpeed()						{ return m_fMoveSpeed;	}
	float GetFireRate()							{ return m_fPistolRate;	}
	int GetMaxHealth()							{ return m_nMaxHealth;	}
	int GetArmor()								{ return m_nArmor;		}
	virtual int GetDamage() const;
	int GetBaseDamage() const					{ return m_nDamage; }
	int GetKills()								{ return m_nKills;		}
	
	bool GetSurroundingTiles(int index){ return m_bSurroundingTilesOccupied[index]; }

	// Get Emitter details
	CEmitter* GetHealingParticleEffect()	{return m_pHealingParticleEffect;}
	
	//virtual functions that need to be overridden
	virtual void Update( float fElapsedTime );
	virtual void Render();
	virtual bool CheckCollision(IEntity* pOther/*, CTile* pTile = nullptr*/);


	void MiniMapRender();

	bool CheckTileCollision();

	//Input function that will control the player
	void Input();

	void HandleEvent(IEvent* pEvent);
};