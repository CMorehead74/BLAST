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
#include "Entity.h"
#include <list>
#include "IEntity.h"

#include "../SGD Wrappers/SGD_String.h"

#define ALLY_VISION_RANGE 400

typedef CObjectManager<TSTRING, CEntity> ObjectManager;

class CEnemy;
class CAlly;
class CPlayer;

class CPatrolState :
	public IBaseAIState
{
private:
	ObjectManager* m_pOM;

	CEnemy*			m_pEnemy;

public:
	CPatrolState(void);
	virtual ~CPatrolState(void);

	virtual void Update(float fElapsedTime, CMovingObject* pObject);
	void ProcessPatrolState(float fElapsedTime, CMovingObject* pObject);
	bool SearchForTarget(CEnemy* pEnemy);

};

