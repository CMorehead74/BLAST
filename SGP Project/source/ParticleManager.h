/*
	File:				IGameState.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand
	Last Edited:		9/5/2012
	Purpose:			Contain / manage the game code
*/

#pragma once
//#include <Windows.h>
#include <vector>
#include "../SGD Wrappers/SGD_Math.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "HELPER.h"

#include "Emitter.h"
//#include "Particle.h"
#include "Game.h"

//class CEmitter;

class CParticleManager
{
private:
	//std::map<EmitterIDType, std::string> m_mpEmitterLoaded;

	std::map<std::string, CEmitter> m_mAllEmitter;

	std::vector< CEmitter >	m_vEmitter;
	
	CSGD_TextureManager*	m_pTM;

	PARTICLETYPE			m_ePType;

	CParticleManager(void);
	CParticleManager(float parentPosX, float parentPosY,float rotation,PARTICLETYPE PType);
	~CParticleManager(void);
	CParticleManager( const CParticleManager& );			
	CParticleManager& operator=( const CParticleManager& );

public:
	// SINGLETON:
	static CParticleManager* GetInstance( void );

	
	void Update(float fElapsedTime);	
	void Render(std::string emitterName, int x, int y, float rotation);

	bool StartParicleEffect(std::string emitterName);

	bool StartMachineGunnerBulletParticle(void);
	bool StartSniperBulletParticle(void);
	bool StartRocketLauncherBulletParticle(void);
	bool StartMedicBulletParticle(void);

	bool StartSelectionParticle(void);
	bool StartLevelUpParticle(void);
	bool StartSnowParticle(void);
	bool StartRainParticle(void);
	bool StartHelicopterParticle(void);
	
	//Load from XML
	bool RegisterEmitter(std::string fileName);

	CEmitter* CreateEmitter(std::string szRegistedEmitterName);
	//Accessors
	//std::vector< tParticle> GetParticleList(void){return m_vParticles;}

	//Mutators
	//void	SetParentPosX( float	posX )	{	m_nParentX	= posX;		}
	//void	SetParentPosY( float	posY )	{	m_nParentY	= posY;		}

};
