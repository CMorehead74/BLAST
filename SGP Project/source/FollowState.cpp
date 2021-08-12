#include "FollowState.h"
#include "HELPER.h"

#include "MovingObject.h"
#include "Player.h"
#include "Ally.h"
#include "Enemy.h"
#include "Helicopter.h"
#include "../SGD Wrappers/SGD_Math.h"

CFollowState::CFollowState(void)
{
	m_pOM			= nullptr;
	
	m_pEnemy		= nullptr;
	m_pHelicopter	= nullptr;
}

CFollowState::~CFollowState(void)
{
}

void CFollowState::Update(float fElapsedTime, CMovingObject* pObject)
{
	ProcessFollowMeState(fElapsedTime, pObject);
}

void CFollowState::ProcessFollowMeState(float fElapsedTime, CMovingObject* pObject)
{
	CPlayer* player;
	m_pOM = ObjectManager::GetInstance();

	list<IEntity*> objectsList = m_pOM->GetObjectList();

	CAlly* ally = dynamic_cast<CAlly*>(pObject);

	if( ally->GetBunkeredState() == false )
	{
		if(!SearchForTarget(pObject))
		{
			for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
			{
				if((*iter)->GetType() == CEntity::OBJ_PLAYER)
				{
					player = dynamic_cast<CPlayer*>(*iter);
					break;
				}
			}

			//put in formation
			int trys = 0;
			float newPosX = 0.0f;
			float newPosY = 0.0f;

			if(ally->GetFormationIndex() == -1)
			{
				int randValue = rand()%8;
				while(player->GetSurroundingTiles(randValue) && trys < 10)
				{
					randValue = rand()%8;
					trys++;
				}				

				ally->SetFormationIndex(randValue);
				player->SetSurroundingTiles(randValue);
			}
			
			if( trys < 8 && trys >= 0)
			{
				
				switch(ally->GetFormationIndex())
				{
				case 0:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() - 128 - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() - 128 - pObject->GetPosY()));
					break;
				case 1:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() - 128 - pObject->GetPosY()));
					break;
				case 2:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() + 128 - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() - 128 - pObject->GetPosY()));
					break;
				case 3:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() - 128 - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() - pObject->GetPosY()));
					break;
				case 4:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() + 128 - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() - pObject->GetPosY()));
					break;
				case 5:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() - 128 - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() + 128 - pObject->GetPosY()));
					break;
				case 6:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() + 128 - pObject->GetPosY()));
					break;
				case 7:
					newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() + 128 - pObject->GetPosX()));
					newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() + 128 - pObject->GetPosY()));
					break;			
				}
			}
			else
			{
				newPosX = (float)(pObject->GetPosX() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosX() - pObject->GetPosX()));
				newPosY = (float)(pObject->GetPosY() + fElapsedTime * MOVE_SPEED_ALLY_FOLLOWSTATE * (player->GetPosY() - 64 - 64 - pObject->GetPosY()));		
			}
			pObject->SetPosX(newPosX);
			pObject->SetPosY(newPosY);

			pObject->SetRotation(player->GetRotation());// pObject->GetRotation() + fAngle );
			
			//Stop shooting
			pObject->SetTargetInLineOfSight(false);
		}
		else
		{
			switch(ally->GetTarget()->GetType())
			{
			case CEntity::OBJ_ENEMY:
				{
					if(IsTargetInRange(pObject,m_pEnemy))
					{
						//m_pEnemy = dynamic_cast<CPlayer*>(enemy->GetTarget());
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
					}
					else
						pObject->SetTargetInLineOfSight(false);
					break;
				}
			case CEntity::OBJ_HELICOPTER:
				{
					if(IsTargetInRange(pObject,m_pHelicopter) && m_pHelicopter->GetIsSpawning())
					{
						//m_pHelicopter = dynamic_cast<CPlayer*>(enemy->GetTarget());						

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
					else
						pObject->SetTargetInLineOfSight(false);
					break;
				}				
			}
		}
		
	}
	else
	{
		if(CheckIfEnemyInRange(pObject))
		{	
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
		}
		else if(CheckIfHelicopterInRange(pObject))
		{
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
		else
			pObject->SetTargetInLineOfSight(false);

	}
}

bool CFollowState::CheckIfEnemyInRange(CMovingObject* pObject)
{
	list<IEntity*> objectsList = m_pOM->GetObjectList();

	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{		
		if( (*iter)->GetType() == CEntity::OBJ_ENEMY )
		{
			m_pEnemy = dynamic_cast<CEnemy*>(*iter);			

			float distance = sqrt( pow(abs(m_pEnemy->GetPosX() - pObject->GetPosX()),2) + pow(abs(m_pEnemy->GetPosY() - pObject->GetPosY()),2) );

			if(distance < ALLY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}

bool CFollowState::CheckIfHelicopterInRange	(CMovingObject* pObject)
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

			if(distance < ALLY_VISION_RANGE)
			{
				return true;
			}
			pObject->SetTargetInLineOfSight(false);
		}
	}
	return false;
}
bool CFollowState::SearchForTarget(CMovingObject* pObject)
{	
	CAlly* pAlly;
	
	if(pObject->GetType() == CEntity::OBJ_ALLY)
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
		else
		{
			pAlly->SetTarget(nullptr);
			return false;
		}
	}
	return false;
}

bool CFollowState::IsTargetInRange(CMovingObject* pObject, CEntity* pObjectTarget)
{
	float distance = sqrt( pow(abs(pObjectTarget->GetPosX() - pObject->GetPosX()),2) + pow(abs(pObjectTarget->GetPosY() - pObject->GetPosY()),2) );

	if(distance < ALLY_VISION_RANGE)
	{
		return true;
	}	
	return false;
}