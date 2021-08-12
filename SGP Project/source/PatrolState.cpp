#include "PatrolState.h"

#include "AttackState.h"

#include "Enemy.h"
#include "Player.h"
#include "Ally.h"

#include "Camera.h"
#include "HELPER.h"

#include "../SGD Wrappers/SGD_Math.h"

CPatrolState::CPatrolState(void)
{
}


CPatrolState::~CPatrolState(void)
{
}

void CPatrolState::Update(float fElapsedTime, CMovingObject* pObject)
{
	m_pOM = ObjectManager::GetInstance();
	SetObjectList(m_pOM->GetObjectList());

	ProcessPatrolState(fElapsedTime, pObject);
}

void CPatrolState::ProcessPatrolState(float fElapsedTime, CMovingObject* pObject)
{
	CEnemy* pEnemy = dynamic_cast<CEnemy*>(pObject);

	//pEnemy->SetPatrolTimer(fElapsedTime);

	float newPosX = 0.0f;
	float newPosY = 0.0f;
	if(pEnemy->GetPatrolTimer() > 4.0f)
	{		
		pEnemy->SetPatrolTimer( 0.0f );
		pEnemy->TogglePatrolStatus();		
	}
	else
		pEnemy->SetPatrolTimer( pEnemy->GetPatrolTimer() + fElapsedTime );
	
	if(pEnemy->GetPatrolStatus())
	{
		pEnemy->SetPosX(pObject->GetPosX() + pEnemy->GetMovementXDirection()*4);
		pEnemy->SetPosY(pObject->GetPosY() + pEnemy->GetMovementYDirection()*4);		
	}
	else
	{
		pEnemy->SetPosX(pObject->GetPosX() - pEnemy->GetMovementXDirection()*4);
		pEnemy->SetPosY(pObject->GetPosY() - pEnemy->GetMovementYDirection()*4);
		
	}	
	
	//float distance = sqrt( pow(abs(pObject->GetPosX() - newPosX),2) + pow(abs(pObject->GetPosY() - newPosY),2) );
	//
	//if( distance > 0.f )
	//{
	//	pObject->SetPosX(pObject->GetPosX() + fElapsedTime);
	//	pObject->SetPosY(pObject->GetPosY() - fElapsedTime);
	//}
	//else
	//{
	//	pObject->SetVelX(-newPosX);
	//	pObject->SetVelY(-newPosY);
	//}
	
	CCamera* pCamera =  CCamera::GetInstance();
	float startX = pObject->GetPosX()-pCamera->GetX();
	float startY = pObject->GetPosY()-pCamera->GetY();
	float endX = pObject->GetPosX() + 1000 - pCamera->GetX();
	float endY = pObject->GetPosY()-pCamera->GetY();
	
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

	if(SearchForTarget(pEnemy))
		pEnemy->SetState(new CAttackState());
	pEnemy->SetState(new CAttackState);
}

bool CPatrolState::SearchForTarget(CEnemy* pEnemy)
{	
	if(pEnemy == nullptr)
		return false;

	list<IEntity*> objectsList = m_pOM->GetObjectList();


	for(std::list<IEntity*>::iterator iter = objectsList.begin(); iter != objectsList.end(); ++iter)
	{
		
		if( (*iter)->GetType() != CEntity::OBJ_ENEMY && (*iter)->GetType() != CEntity::OBJ_HELICOPTER )
		{
			switch((*iter)->GetType())
			{
			case CEntity::OBJ_ALLY:
				{
					CAlly* ally = dynamic_cast<CAlly*>((*iter));
					float distance = sqrt( pow(abs(ally->GetPosX() - pEnemy->GetPosX()),2) + pow(abs(ally->GetPosY() - pEnemy->GetPosY()),2) );

					if(distance < ENEMY_VISION_RANGE)
					{
						return true;
					}
					pEnemy->SetTargetInLineOfSight(false);
					break;
				}
			case CEntity::OBJ_PLAYER:
				{
					CPlayer* player = dynamic_cast<CPlayer*>((*iter));

					float distance = sqrt( pow(abs(player->GetPosX() - pEnemy->GetPosX()),2) + pow(abs(player->GetPosY() - pEnemy->GetPosY()),2) );

					if(distance < ENEMY_VISION_RANGE)
					{
						return true;
					}
					pEnemy->SetTargetInLineOfSight(false);
					break;
				}			

			}
			
		}
	}
	return false;	
}

