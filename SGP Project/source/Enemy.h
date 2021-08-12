/*
	File:				Enemy.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/12/2012
	Purpose:			Class for the enemy units in the game.
						-- Will have numerous types
*/

#pragma once
#include "MovingObject.h"
#include "IBaseAIState.h"
#include "../SGD Wrappers/CSGD_XAudio2.h"
#include "../SGD Wrappers/IListener.h"
#include "IEvent.h"
#include "AnimationInfo.h"

class CAttackState;
class CStoreHouse;
class CMine;
class CBunker;

class CEnemy : public CMovingObject, public IListener
{
protected:
	//enum UnitType { SHOTGUN, GUNNER, MEDIC, SNIPER, ROCKET };		//had to move to helper class - cjm
	//enum UnitType { GUNNER, MEDIC, SNIPER, ROCKET };
	UnitType m_eUnitType;

public:
	CEnemy(void);
	virtual ~CEnemy(void);

	void Update( float fElapsedTime );
	void Render();
	bool CheckCollision(IEntity* pOther);

	int			GetUnitType(void)	const		{ return m_eUnitType;	}
	IEntity*	GetTarget(void)		const		{ return m_pTarget;		}
	IBaseAIState* GetCurrState()				{ return m_pCurrAIState;}

	void		SetUnitType(int nType)			{ m_eUnitType = (UnitType)nType;	}
	void		SetTarget(IEntity* pTarget)		{ m_pTarget = pTarget;				}
	void		SetState( IBaseAIState* pCurrState)	{ delete m_pCurrAIState; m_pCurrAIState = nullptr; m_pCurrAIState = pCurrState; }
	void		SetPatrolTimer(float timer)		{ m_fPatrolTimer = timer; }
	void		TogglePatrolStatus(void)		{ m_bPatrolStatus = !m_bPatrolStatus; }

	void		SetMovementXDirection(int x)	{ m_nMovementXDirection = x; }
	void		SetMovementYDirection(int y)	{ m_nMovementYDirection = y; }

	void MiniMapRender();

	// Damage
	int GetDamage()							    {return m_nDamage;			}
	void TakeDamage( int nDamage );

	// Heath@

	int GetMaxHealth()							{ return m_nMaxHealth;		}
	int GetCurrentHealth()						{ return m_nHealth;			}
	float GetPatrolTimer()						{ return m_fPatrolTimer;	}
	int GetPatrolStatus()						{ return m_bPatrolStatus;	}

	int GetMovementXDirection()					{ return m_nMovementXDirection; }
	int GetMovementYDirection()					{ return m_nMovementYDirection; }

	
	virtual void SetAnimationID(int nAnimationID)				{CurrAnimation->m_nCurrentAnimation = nAnimationID; }
	virtual int  GetAnimationID()								{ return CurrAnimation->m_nCurrentAnimation; }

	bool CheckTileCollision();

private:
	// Data Members
	float			m_fFireTimer;
	float			m_fLifeTimer;

	float			m_fPatrolTimer;

	IBaseAIState*	m_pCurrAIState;
	IEntity*		m_pTarget;

	bool			m_bPatrolStatus;
	// SGD Wrappers:
	CSGD_XAudio2*			m_pXA;

	// Animations
	CAnimationInfo* CurrAnimation;

	// Audio IDs:
	int m_nSniper;
	int m_nMachineGun;
	int m_nShotgun;
	int m_nRocket;

	// Damage and hp
	int			m_nDamage;
	int			m_nHealth;
	int			m_nMaxHealth;

	int			m_nHealthID;
	bool		m_bBlood;					//cjm - 10/06/12

	// Spreed out when dropped by helicopter
	int			m_nMovementXDirection;
	int			m_nMovementYDirection;

	void HandleEvent(IEvent* pEvent);
};