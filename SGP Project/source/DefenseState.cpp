#include "DefenseState.h"

#include "MovingObject.h"

#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "StoreHouse.h"
#include "Mine.h"
#include "Bunker.h"
#include "Helicopter.h"

#include "AttackState.h"

#include "../SGD Wrappers/SGD_Math.h"
#include "Camera.h"


CDefenseState::CDefenseState(void)
{
}


CDefenseState::~CDefenseState(void)
{
}

void CDefenseState::Update(float fElapsedTime, CMovingObject* pObject)
{
	if(pObject == nullptr)
		return;

	m_pOM = ObjectManager::GetInstance();

	SetObjectList(m_pOM->GetObjectList());
	ProcessDefenseState(fElapsedTime,pObject);
}

void CDefenseState::ProcessDefenseState(float fElapsedTime, CMovingObject* pObject)
{
	std::list<IEntity*> objList = GetObjectList();
	switch(pObject->GetType())
	{
	case CEntity::OBJ_ENEMY:
		{
			CEnemy* enemy = dynamic_cast<CEnemy*>(pObject);		
			//CHelicopter* pHelicopter = nullptr;
			//pHelicopter = 
						
			
			//Must check if the helicoter is null TODO Naveen
			if( FindHelicopter() )
			{
				if(m_pHelicopter->GetIsSpawning())
				{
					float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pHelicopter->GetPosX() - pObject->GetPosX());
					float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pHelicopter->GetPosY() - pObject->GetPosY());

					float distance = sqrt( pow(abs(m_pHelicopter->GetPosX() - newPosX),2) + pow(abs(m_pHelicopter->GetPosY() - newPosY),2) );

					if( distance > 100 )
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
				}
				else
					enemy->SetState(new CAttackState());
			}
			//enemy->SetTarget(nullptr);
			//if(enemy->GetTarget() == nullptr)
			//{
			//	//Search for Target
			//	if(!SearchForTarget(pObject))
			//	{
			//		enemy->SetTarget(nullptr);
			//		return;
			//	}
			//}
			//
			//switch(enemy->GetTarget()->GetType())
			//{
			//case CEntity::OBJ_PLAYER:
			//	{
			//		if(IsTargetInRange(pObject,m_pPlayer))
			//		{
			//			//enemy->SetTarget(m_pPlayer);
			//			//m_pPlayer = dynamic_cast<CPlayer*>(enemy->GetTarget());
			//			float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pPlayer->GetPosX() - pObject->GetPosX());
			//			float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pPlayer->GetPosY() - pObject->GetPosY());
			//
			//			float distance = sqrt( pow(abs(m_pPlayer->GetPosX() - newPosX),2) + pow(abs(m_pPlayer->GetPosY() - newPosY),2) );
			//
			//			if( distance > 300 )
			//			{
			//				pObject->SetPosX(newPosX);
			//				pObject->SetPosY(newPosY);
			//			}
			//			else
			//			{
			//				pObject->SetVelX(0);
			//				pObject->SetVelY(0);
			//			}
			//
			//			CCamera* pCamera =  CCamera::GetInstance();
			//			float startX = pObject->GetPosX()-pCamera->GetX();
			//			float startY = pObject->GetPosY()-pCamera->GetY();
			//			float endX = m_pPlayer->GetPosX()-pCamera->GetX();
			//			float endY = m_pPlayer->GetPosY()-pCamera->GetY();
			//
			//			tVector2D vToTarget; 
			//			vToTarget.fX = endX - startX;
			//			vToTarget.fY = endY - startY;			
			//
			//			tVector2D vOrientation = { 0, -1 };
			//			vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );
			//
			//			vToTarget = Vector2DNormalize(vToTarget);
			//
			//			// Calculate the angle between the vectors
			//			//	NOTE:	Always in radians between [ 0, pi ]
			//			float fAngle = AngleBetweenVectors( vOrientation, vToTarget );
			//
			//			// Rotate slowly:
			//			if( fAngle > (SGD_PI * fElapsedTime) )
			//				fAngle = (SGD_PI * fElapsedTime);
			//
			//
			//			// Which direction to turn? Clockwise or counter-clockwise
			//			if( Steering( vOrientation, vToTarget ) < 0 )
			//				fAngle = -fAngle;
			//
			//			pObject->SetRotation(pObject->GetRotation() + fAngle);	
			//
			//			//if(fAngle > -0.5f && fAngle < 0.5f)
			//			pObject->SetTargetInLineOfSight(true);
			//		}
			//		break;
			//	}
			//case CEntity::OBJ_ALLY:
			//	{
			//		if(IsTargetInRange(pObject,m_pAlly))
			//		{
			//			//m_pAlly = dynamic_cast<CPlayer*>(enemy->GetTarget());
			//			float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pAlly->GetPosX() - pObject->GetPosX());
			//			float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pAlly->GetPosY() - pObject->GetPosY());
			//
			//			float distance = sqrt( pow(abs(m_pAlly->GetPosX() - newPosX),2) + pow(abs(m_pAlly->GetPosY() - newPosY),2) );
			//
			//			if( distance > 300 )
			//			{
			//				pObject->SetPosX(newPosX);
			//				pObject->SetPosY(newPosY);
			//			}
			//			else
			//			{
			//				pObject->SetVelX(0);
			//				pObject->SetVelY(0);
			//			}
			//
			//			CCamera* pCamera =  CCamera::GetInstance();
			//			float startX = pObject->GetPosX()-pCamera->GetX();
			//			float startY = pObject->GetPosY()-pCamera->GetY();
			//			float endX = m_pAlly->GetPosX()-pCamera->GetX();
			//			float endY = m_pAlly->GetPosY()-pCamera->GetY();
			//
			//			tVector2D vToTarget; 
			//			vToTarget.fX = endX - startX;
			//			vToTarget.fY = endY - startY;			
			//
			//			tVector2D vOrientation = { 0, -1 };
			//			vOrientation = Vector2DRotate( vOrientation, pObject->GetRotation() );
			//
			//			vToTarget = Vector2DNormalize(vToTarget);
			//
			//			// Calculate the angle between the vectors
			//			//	NOTE:	Always in radians between [ 0, pi ]
			//			float fAngle = AngleBetweenVectors( vOrientation, vToTarget );
			//
			//			// Rotate slowly:
			//			if( fAngle > (SGD_PI * fElapsedTime) )
			//				fAngle = (SGD_PI * fElapsedTime);
			//
			//
			//			// Which direction to turn? Clockwise or counter-clockwise
			//			if( Steering( vOrientation, vToTarget ) < 0 )
			//				fAngle = -fAngle;
			//
			//			pObject->SetRotation(pObject->GetRotation() + fAngle);	
			//
			//			//if(fAngle > -0.5f && fAngle < 0.5f)
			//			pObject->SetTargetInLineOfSight(true);
			//		}
			//	break;
			//	}
			break;
		}		
	case CEntity::OBJ_ALLY:
		{
			CAlly* pAlly = dynamic_cast<CAlly*>(pObject);
			m_pBunker = nullptr;

			if(pAlly->GetBunkeredState())
			{
				if(SearchForTarget(pObject))
				{
					switch(pAlly->GetTarget()->GetType())
					{
					case CEntity::OBJ_ENEMY:
						{							
							float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pEnemy->GetPosX() - pObject->GetPosX());
							float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pEnemy->GetPosY() - pObject->GetPosY());

							float distance = sqrt( pow(abs(m_pEnemy->GetPosX() - newPosX),2) + pow(abs(m_pEnemy->GetPosY() - newPosY),2) );

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
					}
				}
				else
					pObject->SetTargetInLineOfSight(false);
			}
			else
			{
				FindNearestBunker(pObject);
				if(pAlly->GetBunkerID()!=-1)
				{
					if(m_pBunker == nullptr)
					{
						pAlly->SetBunkerID(-1);
						break;
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

					float newPosX = pObject->GetPosX() + fElapsedTime * 0.5f * (m_pBunker->GetPosX() - pObject->GetPosX());
					float newPosY = pObject->GetPosY() + fElapsedTime * 0.5f * (m_pBunker->GetPosY() - pObject->GetPosY());

					float distance = sqrt( pow(abs(m_pBunker->GetPosX() - newPosX),2) + pow(abs(m_pBunker->GetPosY() - newPosY),2) );

					if( distance > 100 )
					{
						pObject->SetPosX(newPosX);
						pObject->SetPosY(newPosY);
					}
					else
					{
						pObject->SetVelX(0);
						pObject->SetVelY(0);
						if(m_pBunker->GetNumAllies() < 4)
						{
							pAlly->EnterBunker();
							m_pBunker->IncreaseAllyCount();
							pAlly->SetPosX( m_pBunker->GetPosX() +  m_pBunker->GetWidth()*0.5f);
							pAlly->SetPosY( m_pBunker->GetPosY() +  m_pBunker->GetHeight()*0.5f);
						}
						else
						{							
							pAlly->SetBunkerID(-1);
							break;
						}
					}					

					//if(fAngle > -0.5f && fAngle < 0.5f)
					//pObject->SetTargetInLineOfSight(true);
				}
				else
					pAlly->SetState(new CAttackState());
			}
		}
		break;
		
	}
}

bool CDefenseState::IsTargetInRange(CMovingObject* pObject, CEntity* pObjectTarget)
{
	float distance = sqrt( pow(abs(pObjectTarget->GetPosX() - pObject->GetPosX()),2) + pow(abs(pObjectTarget->GetPosY() - pObject->GetPosY()),2) );

	if(distance < ENEMY_VISION_RANGE)
	{
		return true;
	}	
	return false;
}

bool CDefenseState::CheckIfAllyInRange(CMovingObject* pObject)
{
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

bool CDefenseState::CheckIfEnemyInRange(CMovingObject* pObject)
{
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

bool CDefenseState::CheckIfPlayerInRange(CMovingObject* pObject)
{
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

bool CDefenseState::CheckIfHelicopterInRange	(CMovingObject* pObject)
{
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

bool CDefenseState::CheckIfBunkerInRange		(CMovingObject* pObject)
{
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

bool CDefenseState::SearchForTarget(CMovingObject* pObject)
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
		else
		{
			pEnemy->SetTarget(nullptr);
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
		else if(CheckIfHelicopterInRange(pObject))
		{
			pAlly->SetTarget(m_pHelicopter);
			return true;
		}
		else
		{
			pAlly->SetTarget(nullptr);
			return false;
		}
	}
	return false;
}

void CDefenseState::FindNearestBunker(CMovingObject* pObject)
{
	list<IEntity*> objectsList = m_pOM->GetObjectList();
	float minDistance = 50000.0f;	

	CAlly* pAlly = dynamic_cast<CAlly*>(pObject);
	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		
		if( (*iter)->GetType() == CEntity::OBJ_BUNKER )
		{
			CBunker* pBunker = dynamic_cast<CBunker*>(*iter);

			//float newPosX = pObject->GetPosX() + fElapsedTime * (m_pPlayer->GetPosX() - pObject->GetPosX());
			//float newPosY = pObject->GetPosY() + fElapsedTime * (m_pPlayer->GetPosY() - pObject->GetPosY());

			float distance = sqrt( pow(abs(pBunker->GetPosX() - pObject->GetPosX()),2) + pow(abs(pBunker->GetPosY() - pObject->GetPosY()),2) );

			if(distance < minDistance && pBunker->GetNumAllies() < 4)
			{
				m_pBunker = pBunker;
				minDistance = distance;
				pAlly->SetBunkerID(m_pBunker->GetBunkerID());			
			}
		}
	}
}

bool CDefenseState::FindHelicopter()
{
	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		if( (*iter)->GetType() == CEntity::OBJ_HELICOPTER )
		{
			m_pHelicopter =  dynamic_cast<CHelicopter*>(*iter);	
			return true;
		}
	}	
	return false;
}




