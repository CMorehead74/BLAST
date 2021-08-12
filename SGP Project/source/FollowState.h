/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand
	Last Edited:		9/30/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
#include "ibaseaistate.h"
#include "ObjectManager.h"
#include "Entity.h"
#include <list>
#include "IEntity.h"

#include "../SGD Wrappers/SGD_String.h"

#define ALLY_VISION_RANGE 400

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CAlly;
class CPlayer;
class CEnemy;
class CHelicopter;

class CFollowState :
	public IBaseAIState
{
private:
	ObjectManager* m_pOM;

	CEnemy*			m_pEnemy;
	CHelicopter*	m_pHelicopter;

public:
	CFollowState(void);
	virtual ~CFollowState(void);

	virtual void Update(float fElapsedTime, CMovingObject* pObject);
	void ProcessFollowMeState(float fElapsedTime, CMovingObject* pObject);
	bool SearchForTarget(CMovingObject* pObject);

	bool CheckIfHelicopterInRange	(CMovingObject* pObject);
	bool CheckIfEnemyInRange		(CMovingObject* pObject);

	bool IsTargetInRange			(CMovingObject* pObject, CEntity* pObjectTarget);
};

