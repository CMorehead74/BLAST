/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once

#include <list>
#include "IEntity.h"

class CMovingObject;
//class CEnemy;
//class CAlly;
//class CMine;
//class CStoreHouse;

class IBaseAIState
{
public:

	IBaseAIState(void)
	{
	}

	virtual ~IBaseAIState(void) = 0
	{
	}

	virtual void Update(float fElapsedTime, CMovingObject* pObject) = 0;
	
	//std::vector<CEnemy*> GetEnemyList(void){return m_vEnemyList;}
	//std::vector<CEnemy*> GetEnemyList(void){return m_vEnemyList;}
	//std::vector<CEnemy*> GetEnemyList(void){return m_vEnemyList;}
	//std::vector<CEnemy*> GetEnemyList(void){return m_vEnemyList;}
	//std::vector<CEnemy*> GetEnemyList(void){return m_vEnemyList;}

	//Accessor
	std::list<IEntity*> GetObjectList(void){return m_lObjectsList;}
	//Mutator
	void SetObjectList(std::list<IEntity*> objectList){ m_lObjectsList = objectList; }

private:
	//std::vector<CEnemy*>		m_vEnemyList;
	//std::vector<CAlly*>			m_vAllyList;
	//std::vector<CStoreHOuse*>	m_vStoreHouseList;
	//std::vector<CMin*>			m_vMineList;
	//std::vector<CBunker*>		m_vBunkerList;

	std::list<IEntity*> m_lObjectsList;
};

