/*
	File:						Ally.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:		Schadrac Francois
	Last Edited:			10/9/2012
	Purpose:				Handles the allied units
*/

#pragma once
#include "Movingobject.h"
#include "IBaseAIState.h"
#include "../SGD Wrappers/IListener.h"
#include "IEvent.h"
#include "ObjectManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Emitter.h"
//particle manager
#include "ParticleManager.h"

#include "AnimationInfo.h"
#include "Player.h"
class CFollowState;
class IdleState;
class DefenseState;
class PatrolState;
class AttackState;

class CStoreHouse;
class CMine;
class CBunker;
//typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CAlly :public CMovingObject, public IListener
{
private:
	int							m_nAllyType;
	int							m_nConsumptionRate;
	float						m_fFireTimer;
	
	int							m_nDamage;
	int							m_nArmor;

	int							m_nBunkerID;

	// Damage and hp
	int							m_nMaxHealth;

	int							m_nHealthID;

	bool						m_bIsBunkered;
	int							m_nFormationIndex;
	float						m_fHealTimer;

	//Particle effect
	CParticleManager*			m_pPM;
	CEmitter*					m_pShootingEffect;
	bool						m_bShootingParticle;
	bool						m_bBlood;				//cjm 10/05/12

	IEntity*							m_pTarget;
	IBaseAIState*				m_pCurrAIState;
	ObjectManager*				m_pOM;

	//Animations
	CAnimationInfo*				CurrAnimation;
	CAnimationInfo*				CurrAnimationShotgun;

				

	int							m_nAnimationID;
	
	//Sounds

	//player sending the commands
	CPlayer*						m_pPlayer;

	float								m_fFoodTimer;

public:
	CAlly(void);
	~CAlly(void);


	
	//Animation Gets and Sets
	//void SetAnimation(int nAnimationID)				{ m_nAnimationID = nAnimationID; }
	//int GetAnimation()								{return m_nAnimationID;}

	// Friendly buffs
	void IncreaseDamage( int nDamage )					{ m_nDamage += nDamage;  SetDamage(m_nDamage);}
	void IncreaseArmor( int nArmor )					{ m_nArmor  += nArmor; }


	//void SetDamage( int nDamage )		{ m_nDamage = nDamage; }
	void SetArmor( int nArmor )			{ m_nArmor = nArmor; }
	//int GetDamage()							     {return m_nDamage;}
	void SetBunkerID(int bunkerID)		{ m_nBunkerID = bunkerID; }

	void SetFormationIndex(int index)		{ m_nFormationIndex = index; }

	//Accessors
	int GetType()							{return m_nAllyType;}
	int GetConsumtionRate()				{return m_nConsumptionRate;}
	int GetBunkerID()					{return m_nBunkerID;}
	bool GetBunkeredState()				{ return m_bIsBunkered; }

	int GetFormationIndex(void)			{ return m_nFormationIndex; }

	virtual void SetAnimationID(int nAnimationID)				{CurrAnimation->m_nCurrentAnimation = nAnimationID; }
	virtual int  GetAnimationID()								{ return CurrAnimation->m_nCurrentAnimation; }


	
	void EnterBunker()					{ m_bIsBunkered = true; }
	void LeaveBunker()					{ m_bIsBunkered = false; }


	IEntity* GetTarget()				{ return m_pTarget;}
	IBaseAIState* GetCurrState()	{ return m_pCurrAIState;}

	//mutators
	void SetType( int nType )							{ m_nAllyType = nType; }
	void SetFireTimer( float fFireTimer)				{ m_fFireTimer = fFireTimer; }
	void SetTarget( IEntity* pTarget)					{ m_pTarget = pTarget;}
	void SetState( IBaseAIState* pCurrState)			{ delete m_pCurrAIState; m_pCurrAIState = nullptr; m_pCurrAIState = pCurrState; }

	// Damage
	int GetDamage()							     {return m_nDamage;}
	int GetArmor()								 {return m_nArmor;}

	void TakeDamage( int nDamage );

	int GetMaxHealth()						{ return m_nMaxHealth; }

	void MiniMapRender();

	//functions that need to be overridden by the child class
	void Update( float fElapsedTime );
	void Render();

	bool CheckCollision( IEntity* pOther );

	void HandleEvent(IEvent* pEvent);

	bool CheckTileCollision();

};

