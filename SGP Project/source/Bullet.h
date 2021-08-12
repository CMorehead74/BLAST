/*
	File:						Bullet.h
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/12/2012
	Purpose:				Contain / manage the game code
*/

#pragma once
#include "movingobject.h"
#include "../SGD Wrappers/IListener.h"
#include "ObjectManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "Emitter.h"


typedef CObjectManager<TSTRING, CEntity> ObjectManager;


class CBullet : public CMovingObject, public IListener
{
public:
	enum BulletType { SNIPER, MG, PISTOL, SHOTGUN, ROCKET };
	CBullet(void);
	virtual ~CBullet(void);

	//accessors
	CMovingObject* GetOwner()					{return m_pOwner;}
	float GetLifeTimer()						{return m_fLifeTimer;}

	//mutators
	void SetOwner( CMovingObject* pOwner );
	void SetLifeTimer( float fLifeTimer )			{m_fLifeTimer = fLifeTimer;}

	//functions from the parent class that have to be overrriden
	void Render();
	void Update( float fElapsedTime );
	bool CheckCollision(IEntity* pOther);
	void SetFlash(bool f) { m_bFlash = f; }		//cjm - 10/10/12
	bool GetFlash(void) { return m_bFlash; }						//cjm - 10/10/12

	virtual RECT GetRect() const;

	void HandleEvent(IEvent* pEvent);

	int		GetBulletType(void)	const	{ return m_eBullet; }
	std::vector<IEntity*>			m_vSniperHits;
private:
	CMovingObject*					m_pOwner;
	float							m_fLifeTimer;
	
	BulletType						m_eBullet;
	ObjectManager*					m_pOM;
	bool							m_bFlash;					//cjm - 10/10/12

	//delete this when it is no longer needed
	bool count;
	int imgID;
	
	CEmitter*				m_pTrailParticleEffect;
	CEmitter*				m_pMuzzleFlashEffect;			//cjm - 10/10/12
};