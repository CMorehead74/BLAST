/*
	File:				Helicopter.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Matthew Berry
	Last Edited:		10/9/2012
	Purpose:			A game object class for the helicopter enemy in the game
						--	Will be spawning enemies
						--	Will have a STATIC POSITION in this current version
*/

#pragma once
#include "MovingObject.h"

#include "Animation.h"
#include "AnimationInfo.h"

#include "ObjectManager.h"
#include "../SGD Wrappers/SGD_String.h"

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

#include "../SGD Wrappers/CSGD_XAudio2.h"

class CPlayer;

class CHelicopter : public CMovingObject
{
public:
	CHelicopter(void);
	virtual ~CHelicopter(void);

	virtual void Update( float fElapsedTime );
	virtual void Render();

	void MiniMapUpdate( float fElapsedTime );
	void MiniMapRender();

	POINT GetTargetPosition()				{ return m_ptTarget; }
	
	int GetMaxHealth()						{ return m_nMaxHealth; }
	int GetCurrentHealth()					{ return m_nHealth; }

	bool GetIsSpawning()					{ return m_bIsSpawning;}
	bool GetPlayerFound()	const			{ return PlayerFound; }
	bool GetOnGround()		const			{ return m_bOnTheGround; }
	bool GetFlying()		const			{ return m_bIsFlying; }
	bool GetSpawning()		const			{ return m_bIsSpawning; }
	bool GetLanding()		const			{ return m_bIsLanding; }

	void SetPlayerFound(bool pf)			{ PlayerFound = pf; }
	void SetOnTheGround(bool otg)			{ m_bOnTheGround = otg; }
	void SetIsFlying(bool isfly)			{ m_bIsFlying = isfly; }
	void SetIsSpawning(bool isspawn)		{ m_bIsSpawning = isspawn; }
	void SetIsLanding(bool island)			{ m_bIsLanding = island; }

	void SetPlayerPointer(CPlayer* player)	{ m_pPlayer = player; }

private:
	// Data Members
	float		m_fSpawnTimer;
	float		m_fOnGroundTimer;
	float		m_fDropRate;
	float		m_fFlightTimer;
	float		m_fScaleImage;

	ObjectManager* m_pOM;

	float		m_fMoveSpeed;

	bool		PlayerFound;
	CPlayer*	m_pPlayer;

	POINT		m_ptTarget;

	// Animations
	CAnimationInfo* CurrAnimation;

	int				m_nHelicopterImgID;


	// What State Is the Helicopter In?
	bool		m_bOnTheGround;
	bool		m_bIsSpawning;
	bool		m_bIsFlying;
	bool		m_bIsLanding;

	int			m_nHealthID;

	int			m_nHealth;
	int			m_nMaxHealth;
//	CEmitter*		m_pSmokeParticleEffect;		//cjm - 10/04/12

	// SGD Wrappers:
	CSGD_XAudio2*			m_pXA;

	// Audio IDs:
	int						m_nHelicopter;
	int						m_nBulletHit;
	int						m_nRocketHit;
//	int						m_nEnemySpawn;

	// Private Functions
	void SpawnEnemies(void);

	bool CheckCollision( IEntity* pOther );
};