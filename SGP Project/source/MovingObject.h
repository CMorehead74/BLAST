/*
	File:				MovingObject.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/25/2012
	Purpose:			Concrete base class for the Moving Objects in the game
*/

#pragma once
#include "Entity.h"
#include "HELPER.h"

//simulated after CEntity from SGD
class CMovingObject : public CEntity
{
protected:
	//enum MovingObjectType { OBJ_MOVEBASE, OBJ_PLAYER, OBJ_ALLY, OBJ_ENEMY, OBJ_HELICOPTER, OBJ_BULLET };
	//MovingObjectType m_eMoveObjType;

private:

	//how fast we are going to move
	float			m_fVelX;
	float			m_fVelY;
	float			m_fRotation;
	int				m_nDamage;
	bool			m_bIsTargetInLineOfSight;
public:
	CMovingObject(void);
	virtual ~CMovingObject(void);

	//mutators
	void SetVelX( float fX )						{ m_fVelX = fX; }
	void SetVelY( float fY )						{ m_fVelY = fY; }
	void SetRotation( float fRotation )		{ m_fRotation = fRotation; }
	virtual void SetDamage(int nDamage)     { m_nDamage = nDamage; }
	void SetTargetInLineOfSight(bool bIsTargetInLineofSight){m_bIsTargetInLineOfSight = bIsTargetInLineofSight;}
	// Accessors
	float GetVelX()		const	{ return m_fVelX; }
	float GetVelY()		const	{ return m_fVelY; }
	float GetRotation()			{ return m_fRotation; }
	virtual int GetDamage()     const   { return m_nDamage; }
	bool GetTargetInLineofSight() const {return m_bIsTargetInLineOfSight;}

	//int GetMoveObjType()	const		{ return m_eMoveObjType; }

	//pure virtual functions from the parent class
	virtual void Update( float fElapsedTime );
	virtual void Render();
	virtual bool CheckCollision( IEntity* pOther );
	//virtual RECT GetRect() const;
};