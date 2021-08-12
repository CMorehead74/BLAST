#include "AttackState.h"
#include "MovingObject.h"

#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "StoreHouse.h"
#include "Mine.h"
#include "Bunker.h"
#include "Helicopter.h"

#include "PatrolState.h"

#include "../SGD Wrappers/SGD_Math.h"
#include "Camera.h"
#include "HELPER.h"

CAttackState::CAttackState(void)
{
	m_pPlayer		= nullptr;
	m_pEnemy		= nullptr;
	m_pStoreHouse	= nullptr;
	m_pMine			= nullptr;
	m_pBunker		= nullptr;
	m_pAlly			= nullptr;
}


CAttackState::~CAttackState(void)
{
}

void CAttackState::Update(float fElapsedTime, CMovingObject* pObject)
{
	if(pObject == nullptr)
		return;

	m_pOM = ObjectManager::GetInstance();

	SetObjectList(m_pOM->GetObjectList());
	ProcessAttackState(fElapsedTime,pObject);
}

void CAttackState::ProcessAttackState(float fElapsedTime, CMovingObject* pObject)
{
	
	std::list<IEntity*> objList = GetObjectList();
	switch(pObject->GetType())
	{
		case CEntity::OBJ_ENEMY:
			{
				CEnemy* enemy = dynamic_cast<CEnemy*>(pObject);		

				enemy->SetTarget(nullptr);
				if(enemy->GetTarget() == nullptr)
				{
					//Search for Target
					if(!SearchForTarget(pObject))
					{
						//enemy->SetTarget(nullptr);
						return;
					}
				}

				switch(enemy->GetTarget()->GetType())
				{
				case CEntity::OBJ_PLAYER:
					{
						if(IsTargetInRange(pObject,m_pPlayer))
						{
							//enemy->SetTarget(m_pPlayer);
							//m_pPlayer = dynamic_cast<CPlayer*>(enemy->GetTarget());
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pPlayer->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pPlayer->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pPlayer->GetPosX() - newPosX),2) + pow(abs(m_pPlayer->GetPosY() - newPosY),2) );

							if( distance > 300 )
							{
								pObject->SetPosX(newPosX);
								pObject->SetPosY(newPosY);
							}
							else
							{
								pObject->SetVelX(0);
								pObject->SetVelY(0);
							}

							CCamera* pCamera =  CCamera::GetInstance();
							float startX = pObject->GetPosX()-pCamera->GetX();
							float startY = pObject->GetPosY()-pCamera->GetY();
							float endX = m_pPlayer->GetPosX()-pCamera->GetX();
							float endY = m_pPlayer->GetPosY()-pCamera->GetY();

							tVector2D vToTarget; 
							vToTarget.fX = endX - startX;
							vToTarget.fY = endY - startY;			

							tVector2D vOrientation = { 0, -1 };
							vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

							pObject->SetRotation(pObject->GetRotation() + fAngle);	

							//if(fAngle > -0.5f && fAngle < 0.5f)
							pObject->SetTargetInLineOfSight(true);
						}
						break;
					}
				case CEntity::OBJ_ALLY:
					{
						if(IsTargetInRange(pObject,m_pAlly))
						{
							//m_pAlly = dynamic_cast<CPlayer*>(enemy->GetTarget());
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pAlly->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pAlly->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pAlly->GetPosX() - newPosX),2) + pow(abs(m_pAlly->GetPosY() - newPosY),2) );

							if( distance > 300 )
							{
								pObject->SetPosX(newPosX);
								pObject->SetPosY(newPosY);
							}
							else
							{
								pObject->SetVelX(0);
								pObject->SetVelY(0);
							}

							CCamera* pCamera =  CCamera::GetInstance();
							float startX = pObject->GetPosX()-pCamera->GetX();
							float startY = pObject->GetPosY()-pCamera->GetY();
							float endX = m_pAlly->GetPosX()-pCamera->GetX();
							float endY = m_pAlly->GetPosY()-pCamera->GetY();

							tVector2D vToTarget; 
							vToTarget.fX = endX - startX;
							vToTarget.fY = endY - startY;			

							tVector2D vOrientation = { 0, -1 };
							vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

							pObject->SetRotation(pObject->GetRotation() + fAngle);	

							//if(fAngle > -0.5f && fAngle < 0.5f)
							pObject->SetTargetInLineOfSight(true);
						}						
					}
					break;
				case CEntity::OBJ_STOREHOUSE:
					{
						if(IsTargetInRange(pObject,m_pStoreHouse))
						{
							//m_pStoreHouse = dynamic_cast<CPlayer*>(enemy->GetTarget());
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pStoreHouse->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pStoreHouse->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pStoreHouse->GetPosX() - newPosX),2) + pow(abs(m_pStoreHouse->GetPosY() - newPosY),2) );

							if( distance > 300 )
							{
								pObject->SetPosX(newPosX);
								pObject->SetPosY(newPosY);
							}
							else
							{
								pObject->SetVelX(0);
								pObject->SetVelY(0);
							}

							CCamera* pCamera =  CCamera::GetInstance();
							float startX = pObject->GetPosX()-pCamera->GetX();
							float startY = pObject->GetPosY()-pCamera->GetY();
							float endX = m_pStoreHouse->GetPosX()-pCamera->GetX();
							float endY = m_pStoreHouse->GetPosY()-pCamera->GetY();

							tVector2D vToTarget; 
							vToTarget.fX = endX - startX;
							vToTarget.fY = endY - startY;			

							tVector2D vOrientation = { 0, -1 };
							vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

							pObject->SetRotation(pObject->GetRotation() + fAngle);	

							//if(fAngle > -0.5f && fAngle < 0.5f)
							pObject->SetTargetInLineOfSight(true);
						}	
					}
					break;
				case CEntity::OBJ_MINE:
					{
						if(IsTargetInRange(pObject,m_pMine))
						{
							//m_pMine = dynamic_cast<CPlayer*>(enemy->GetTarget());
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pMine->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pMine->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pMine->GetPosX() - newPosX),2) + pow(abs(m_pMine->GetPosY() - newPosY),2) );

							if( distance > 300 )
							{
								pObject->SetPosX(newPosX);
								pObject->SetPosY(newPosY);
							}
							else
							{
								pObject->SetVelX(0);
								pObject->SetVelY(0);
							}

							CCamera* pCamera =  CCamera::GetInstance();
							float startX = pObject->GetPosX()-pCamera->GetX();
							float startY = pObject->GetPosY()-pCamera->GetY();
							float endX = m_pMine->GetPosX()-pCamera->GetX();
							float endY = m_pMine->GetPosY()-pCamera->GetY();

							tVector2D vToTarget; 
							vToTarget.fX = endX - startX;
							vToTarget.fY = endY - startY;			

							tVector2D vOrientation = { 0, -1 };
							vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

							pObject->SetRotation(pObject->GetRotation() + fAngle);	

							//if(fAngle > -0.5f && fAngle < 0.5f)
							pObject->SetTargetInLineOfSight(true);
						}
					}
					break;
				case CEntity::OBJ_BUNKER:
					{
						if(IsTargetInRange(pObject,m_pBunker))
						{
							//m_pBunker = dynamic_cast<CPlayer*>(enemy->GetTarget());
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pBunker->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pBunker->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pBunker->GetPosX() - newPosX),2) + pow(abs(m_pBunker->GetPosY() - newPosY),2) );

							if( distance > 300 )
							{
								pObject->SetPosX(newPosX);
								pObject->SetPosY(newPosY);
							}
							else
							{
								pObject->SetVelX(0);
								pObject->SetVelY(0);
							}

							CCamera* pCamera =  CCamera::GetInstance();
							float startX = pObject->GetPosX()-pCamera->GetX();
							float startY = pObject->GetPosY()-pCamera->GetY();
							float endX = m_pBunker->GetPosX()-pCamera->GetX();
							float endY = m_pBunker->GetPosY()-pCamera->GetY();

							tVector2D vToTarget; 
							vToTarget.fX = endX - startX;
							vToTarget.fY = endY - startY;			

							tVector2D vOrientation = { 0, -1 };
							vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

							pObject->SetRotation(pObject->GetRotation() + fAngle);	

							//if(fAngle > -0.5f && fAngle < 0.5f)
							pObject->SetTargetInLineOfSight(true);
						}
					break;	
					}
				}
				break;
			}
		case CEntity::OBJ_ALLY:
			{
				CAlly* ally = dynamic_cast<CAlly*>(pObject);
				
				ally->SetTarget(nullptr);						

				if(FindNearestEnemy())
				{				
					//SearchForTarget(pObject);				
					ally->SetTarget(m_pEnemy);					
					

					switch(ally->GetTarget()->GetType())
					{
					case CEntity::OBJ_ENEMY:
						{							
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pEnemy->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pEnemy->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pEnemy->GetPosX() - newPosX),2) + pow(abs(m_pEnemy->GetPosY() - newPosY),2) );

							if( distance > 300 )
							{
								pObject->SetPosX(newPosX);
								pObject->SetPosY(newPosY);
							}
							else
							{
								pObject->SetVelX(0);
								pObject->SetVelY(0);
							}

							CCamera* pCamera =  CCamera::GetInstance();
							float startX = pObject->GetPosX()-pCamera->GetX();
							float startY = pObject->GetPosY()-pCamera->GetY();
							float endX = m_pEnemy->GetPosX()-pCamera->GetX();
							float endY = m_pEnemy->GetPosY()-pCamera->GetY();

							tVector2D vToTarget; 
							vToTarget.fX = endX - startX;
							vToTarget.fY = endY - startY;			

							tVector2D vOrientation = { 0, -1 };
							vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

							pObject->SetRotation(pObject->GetRotation() + fAngle);	

							//if(fAngle > -0.5f && fAngle < 0.5f)
							pObject->SetTargetInLineOfSight(true);


							break;
						}
					case CEntity::OBJ_HELICOPTER:
						{
							if(IsTargetInRange(pObject,m_pHelicopter) && m_pHelicopter->GetIsSpawning())
							{
								//m_pHelicopter = dynamic_cast<CPlayer*>(enemy->GetTarget());
								float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pHelicopter->GetPosX() - pObject->GetPosX());
								float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pHelicopter->GetPosY() - pObject->GetPosY());

								float distance = sqrt( pow(abs(m_pHelicopter->GetPosX() - newPosX),2) + pow(abs(m_pHelicopter->GetPosY() - newPosY),2) );

								if( distance > 300 )
								{
									pObject->SetPosX(newPosX);
									pObject->SetPosY(newPosY);
								}
								else
								{
									pObject->SetVelX(0);
									pObject->SetVelY(0);
								}

								CCamera* pCamera =  CCamera::GetInstance();
								float startX = pObject->GetPosX()-pCamera->GetX();
								float startY = pObject->GetPosY()-pCamera->GetY();
								float endX = m_pHelicopter->GetPosX()-pCamera->GetX();
								float endY = m_pHelicopter->GetPosY()-pCamera->GetY();

								tVector2D vToTarget; 
								vToTarget.fX = endX - startX;
								vToTarget.fY = endY - startY;			

								tVector2D vOrientation = { 0, -1 };
								vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );

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

								pObject->SetRotation(pObject->GetRotation() + fAngle);	

								//if(fAngle > -0.5f && fAngle < 0.5f)
								pObject->SetTargetInLineOfSight(true);
							}
							break;
						}
					}
					break;
				}
				else
					pObject->SetTargetInLineOfSight(false);
			}
	}
	/*player = nullptr;
	if( player != nullptr )
	{
	break;
	}*/



}

bool CAttackState::CheckIfAllyInRange(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		if( (*iter)->GetType() == CEntity::OBJ_ALLY )
		{
			m_pAlly = dynamic_cast<CAlly*>(*iter);			

			float distance = sqrt( pow(abs(m_pAlly->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pAlly->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::CheckIfStoreHouseInRange(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		if( (*iter)->GetType() == CEntity::OBJ_STOREHOUSE )
		{
			m_pStoreHouse = dynamic_cast<CStoreHouse*>(*iter);			
	
			float distance = sqrt( pow(abs(m_pStoreHouse->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pStoreHouse->GetPosY() - pObject->GetPosY()),2) );
	
			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::CheckIfMineHouseInRange(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		if( (*iter)->GetType() == CEntity::OBJ_MINE )
		{
			m_pMine = dynamic_cast<CMine*>(*iter);			

			float distance = sqrt( pow(abs(m_pMine->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pMine->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::CheckIfEnemyInRange(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		if( (*iter)->GetType() == CEntity::OBJ_ENEMY )
		{
			m_pEnemy = dynamic_cast<CEnemy*>(*iter);			

			float distance = sqrt( pow(abs(m_pEnemy->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pEnemy->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::CheckIfPlayerInRange(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		
		if( (*iter)->GetType() == CEntity::OBJ_PLAYER )
		{
			m_pPlayer = dynamic_cast<CPlayer*>(*iter);

			//float newPosX = pObject->GetPosX() + fElapsedTime * (m_pPlayer->GetPosX() - pObject->GetPosX());
			//float newPosY = pObject->GetPosY() + fElapsedTime * (m_pPlayer->GetPosY() - pObject->GetPosY());

			float distance = sqrt( pow(abs(m_pPlayer->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pPlayer->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::CheckIfHelicopterInRange	(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		
		if( (*iter)->GetType() == CEntity::OBJ_HELICOPTER )
		{
			m_pHelicopter = dynamic_cast<CHelicopter*>(*iter);

			//float newPosX = pObject->GetPosX() + fElapsedTime * (m_pPlayer->GetPosX() - pObject->GetPosX());
			//float newPosY = pObject->GetPosY() + fElapsedTime * (m_pPlayer->GetPosY() - pObject->GetPosY());

			float distance = sqrt( pow(abs(m_pHelicopter->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pHelicopter->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::CheckIfBunkerInRange		(CMovingObject* pObject)
{
	if(pObject == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		
		if( (*iter)->GetType() == CEntity::OBJ_BUNKER )
		{
			m_pBunker = dynamic_cast<CBunker*>(*iter);

			//float newPosX = pObject->GetPosX() + fElapsedTime * (m_pPlayer->GetPosX() - pObject->GetPosX());
			//float newPosY = pObject->GetPosY() + fElapsedTime * (m_pPlayer->GetPosY() - pObject->GetPosY());

			float distance = sqrt( pow(abs(m_pBunker->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pBunker->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ENEMY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CAttackState::SearchForTarget(CMovingObject* pObject)
{
	CEnemy* pEnemy;
	CAlly* pAlly;
	
	if( pObject->GetType() == CEntity::OBJ_ENEMY )
	{
		pEnemy = dynamic_cast<CEnemy*>(pObject);

		if(CheckIfPlayerInRange(pObject))
		{
			pEnemy->SetTarget(m_pPlayer);		
			return true;
		}
		else if(CheckIfAllyInRange(pObject))
		{
			pEnemy->SetTarget(m_pAlly);				
			return true;
		}
		else if(CheckIfBunkerInRange(pObject))
		{
			pEnemy->SetTarget(m_pBunker);	
			return true;
		}
		
		else if(CheckIfStoreHouseInRange(pObject))
		{
			pEnemy->SetTarget(m_pStoreHouse);
			return true;
		}
		else if(CheckIfMineHouseInRange(pObject))
		{
			pEnemy->SetTarget(m_pMine);
			return true;
		}		
		else
		{
			pEnemy->SetTarget(nullptr);
			pEnemy->SetState(new CPatrolState());
			return false;
		}
	}
	else if(pObject->GetType() == CEntity::OBJ_ALLY)
	{
		pAlly = dynamic_cast<CAlly*>(pObject);

		if(CheckIfEnemyInRange(pObject))
		{
			pAlly->SetTarget(m_pEnemy);
			return true;
		}
		else if(CheckIfHelicopterInRange(pObject) && (m_pHelicopter->GetOnGround() || m_pHelicopter->GetOnGround()))
		{
			pAlly->SetTarget(m_pHelicopter);
			return true;
		}		
	}
	return false;
}

bool CAttackState::IsTargetInRange(CMovingObject* pObject, CEntity* pObjectTarget)
{
	float distance = sqrt( pow(abs(pObjectTarget->GetPosX() - pObject->GetPosX()),2) + pow(abs(pObjectTarget->GetPosY() - pObject->GetPosY()),2) );

	if(distance < ENEMY_VISION_RANGE)
	{
		return true;
	}	
	return false;
}


bool CAttackState::FindNearestEnemy(void)
{	
	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		
		if( (*iter)->GetType() == CEntity::OBJ_ENEMY )
		{
			m_pEnemy =  dynamic_cast<CEnemy*>(*iter);
			return true;
		}
	}
	return false;
}
//bool CAttackState::CheckIfAllyInRange(void)
//{
//	return false;
//}
//bool CAttackState::CheckIfStoreHouseInRange(void)
//{
//	return false;
//}
//bool CAttackState::CheckIfMineHouseInRange(void)
//{
//	return false;
//}
//bool CAttackState::CheckIfEnemyInRange(void)
//{
//	return false;
//}
//
//bool CAttackState::CheckIfPlayerInRange(void)
//{
//	return true;
//}
//
//bool CAttackState::CheckIfBunkerInRange(void)
//{
//	return false;
//}
