/*
	File:					IEntity.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:		Schadrac Francois
	Last Edited:			9/8/2012
	Purpose:				Abstract base class for the game objects
*/

//As the interface class I will make it do nothing but abstract functions
// -Schadrac Francois
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class IEntity
{
public:

	//IEntity(void);

	virtual ~IEntity(void)					= 0
	{}

	//functions to be overridden by all classes that inherit from this class
	virtual void Update( float fElapsedTime ) = 0;
	virtual void Render(void) = 0;
	virtual RECT GetRect(void) const = 0;
	virtual int  GetType(void) const = 0;
	virtual bool CheckCollision( IEntity *pOther) = 0;
	virtual void AddRef() = 0;
	virtual void Release() = 0; 
};

