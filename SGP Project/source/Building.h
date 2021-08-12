/*
	File:				Building.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Michael Yatckoske
	Last Edited:		9/14/2012
	Purpose:			Concrete base class for the different building types
*/

#pragma once
#include "Entity.h"
class CBuilding : public CEntity
{

public:
	CBuilding(void);
	virtual ~CBuilding(void);

	// IEntity Interface functions
	virtual void Update( float fElapsedTime );
	virtual void Render(void);

	virtual bool CheckCollision( IEntity *pOther);

	RECT GetRect() const;

	// Accessors
	int GetBuildingType()	const	{ return m_eType; }

	void SetType( int nType )	  { CEntity::m_eType = (ObjectType)nType; }	

private:
};