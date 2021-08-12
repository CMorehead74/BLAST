/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
#include "ibaseaistate.h"

#include "ObjectManager.h"
#include <list>
#include <cmath>
#include "Entity.h"

#include "../SGD Wrappers/SGD_String.h"

#define ENEMY_VISION_RANGE 400
#define ALLY_VISION_RANGE 400

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CEnemy;
class CStoreHouse;
class CMine;
class CAlly;
class CPlayer;
class CBunker;
class CHelicopter;

class CDefenseState :
	public IBaseAIState
{
private:
	ObjectManager* m_pOM;

	CPlayer*		m_pPlayer;
	CEnemy*			m_pEnemy;
	CStoreHouse*	m_pStoreHouse;
	CMine*			m_pMine;
	CBunker*		m_pBunker;
	CAlly*			m_pAlly;
	CHelicopter*	m_pHelicopter;

public:
	CDefenseState(void);
	virtual ~CDefenseState(void);

	void Update(float fElapsedTime, CMovingObject* pObject);
	void ProcessDefenseState(float fElapsedTime, CMovingObject* pObject);

	bool CheckIfAllyInRange			(CMovingObject* pObject);
	bool CheckIfEnemyInRange		(CMovingObject* pObject);
	bool CheckIfBunkerInRange		(CMovingObject* pObject);
	bool CheckIfHelicopterInRange	(CMovingObject* pObject);
	bool CheckIfPlayerInRange		(CMovingObject* pObject);

	bool IsTargetInRange			(CMovingObject* pObject, CEntity* pObjectTarget);
	bool SearchForTarget			(CMovingObject* pObject);

	void FindNearestBunker			(CMovingObject* pObject);
	bool FindHelicopter				(void);

};

