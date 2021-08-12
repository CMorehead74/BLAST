/*
	File:				Emitter.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/21/2012
	Purpose:			Managing of all the Emitters happen here
*/
#pragma once

#include <vector>
#include "Particle.h"
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include <cmath>
#include "HELPER.h"

class CEmitter
{
public:
	CEmitter(void);
	~CEmitter(void);

	CEmitter(int noOfParticles,EMITTERTYPE type);

	void CreateParticle();
	void Update(float fElapsedTime);
	void Render(void);

	//Accessors
	//std::vector<CParticle> GetActiveParticleList(void)		{ return m_vActiveParticles; }

	int GetEmitterPosX			(void)		{ return m_nEmitterPosX; }
	int GetEmitterPosY			(void)		{ return m_nEmitterPosY; }
	int GetParticleImgID			(void)		{ return m_nParticleID; }
	std::string GetParticleImageName (void)		{ return m_szParticleImage; }
	std::string GetEmitterBgImageName (void)		{ return m_szEmitterBgImage; }
	std::string GetEmitterName (void)		{ return m_szEmitterName; }
	//Mutators

	void SetEmitterName			( std::string value){ m_szEmitterName		= value; }		
	void SetParticleImageName	( std::string value){ m_szParticleImage		= value; }		
	void SetEmitterBgImageName	( std::string value){ m_szEmitterBgImage	= value; }	

	void SetParticleImgID		( int value)		{ m_nParticleID			= value; }

	void SetEmitterType			( int value)		{ m_nEmitterType		= value; }
	void SetNoOfParticles		( int value )		{ m_nNoOfParticles		= value; }
	void SetIsLooping			( bool value)		{ m_bIsLooping			= value; }
	void SetIsBlending			( bool value)		{ m_bIsBlending			= value; }
	void SetMinLife				( float value)		{ m_fMinLife			= value; }
	void SetMaxLife				( float value)		{ m_fMaxLife			= value; }
	void SetMinScaleX			( int value)		{ m_nMinScaleX			= value; }
	void SetMaxScaleX			( int value)		{ m_nMaxScaleX			= value; }
	void SetMinScaleY			( int value)		{ m_nMinScaleY			= value; }
	void SetMaxScaleY			( int value)		{ m_nMaxScaleY			= value; }
	void SetStartMinRotation	( float value)		{ m_fStartMinRotation	= value; }
	void SetStartMaxRotation	( float value)		{ m_fStartMaxRotation	= value; }
	void SetEndMinRotation		( float value)		{ m_fEndMinRotation		= value; }
	void SetEndMaxRotation		( float value)		{ m_fEndMaxRotation		= value; }
	void SetEmitterRotation		( float value)		{ m_fEmitterRotation	= value; }

	void SetEmitterPosX			( int value)		{ m_nEmitterPosX		= value; }
	void SetEmitterPosY			( int value)		{ m_nEmitterPosY		= value; }
	void SetEmitterWidth		( int value)		{ m_nWidth				= value; }
	void SetEmitterHeight		( int value)		{ m_nHeight				= value; }
	void SetOuterRingRadius		( int value)		{ m_nOuterRingRadius	= value; }
	
	void SetMinVelX				( float value)		{ m_nMinVelX			= value; }
	void SetMaxVelX				( float value)		{ m_nMaxVelX			= value; }
	void SetMinVelY				( float value)		{ m_nMinVelY			= value; }
	void SetMaxVelY				( float value)		{ m_nMaxVelY			= value; }
	void SetGravityX			( float value)		{ m_fGravityX			= value; }
	void SetGravityY			( float value)		{ m_fGravityY			= value; }
	void SetWind				( float value)		{ m_fWind				= value; }

	void SetScaleRate			( float value )		{ m_fScaleRate			= value; }
	void SetEmissionRate		( float value )		{ m_fEmissionRate		= value; }
	void SetColorRate			( float value )		{ m_fColorRate			= value; }
	void SetRotationRate		( float value )		{ m_fRotationRate		= value; }
	void SetVelXRate			( float value )		{ m_fVelXRate			= value; }
	void SetVelYRate			( float value )		{ m_fVelYRate			= value; }

	//void SetMinColor			( D3DCOLOR_ARGB value );
	void SetMinA				( int value)		{ m_nMinA				= value; }
	void SetMinR				( int value)		{ m_nMinR				= value; }
	void SetMinG				( int value)		{ m_nMinG				= value; }
	void SetMinB				( int value)		{ m_nMinB				= value; }
	void SetMaxA				( int value)		{ m_nMaxA				= value; }
	void SetMaxR				( int value)		{ m_nMaxR				= value; }
	void SetMaxG				( int value)		{ m_nMaxG				= value; }
	void SetMaxB				( int value)		{ m_nMaxB				= value; }

	void SetSourceBlend			( int value)		{ m_nSourceBlend		= value; }
	void SetDestBlend			( int value)		{ m_nDestBlend			= value; }

	void AddParticle(void);

private:
	//EmitterName
	std::string	m_szEmitterName;

	//SGD wrappers
	CSGD_Direct3D*		m_pD3D;
	//EmitterType
	int		m_nEmitterType;

	//Max number of particles
	int		m_nNoOfParticles;

	//Is Particles Looping
	bool	m_bIsLooping;
	bool	m_bIsBlending;

	//Life
	float	m_fMinLife;
	float	m_fMaxLife;

	//Scale
	int	m_nMinScaleX;
	int	m_nMaxScaleX;
	int	m_nMinScaleY;
	int	m_nMaxScaleY;

	//Rate
	float m_fEmissionRate;
	float m_fScaleRate;
	float m_fColorRate;
	float m_fRotationRate;
	float m_fVelXRate;
	float m_fVelYRate;

	//Rotation
	float	m_fStartMinRotation;
	float	m_fStartMaxRotation;
	float	m_fEndMinRotation;
	float	m_fEndMaxRotation;

	//Emitter Rotation
	float	m_fEmitterRotation;

	//Position
	int		m_nEmitterPosX;
	int		m_nEmitterPosY;
	int		m_nWidth;
	int		m_nHeight;

	//Circulat Emitter
	int		m_nOuterRingRadius;
	//Wind
	float   m_fWind;

	//Gravity
	float	m_fGravityX;
	float	m_fGravityY;

	//Velocity
	float	m_nMinVelX;
	float	m_nMaxVelX;
	float	m_nMinVelY;
	float	m_nMaxVelY;

	//Color
	//D3DCOLOR_ARGB m_MinColor;
	//D3DCOLOR_ARGB m_MaxColor;
	int   m_nMinA;
	int   m_nMinR;
	int   m_nMinG;
	int   m_nMinB;
	int   m_nMaxA;
	int   m_nMaxR;
	int   m_nMaxG;
	int   m_nMaxB;

	//RenderType
	int		m_nSourceBlend;
	int		m_nDestBlend;

	//Images
	int		m_nParticleID;	
	int		m_nBlendID;		

	std::string		m_szParticleImage;
	std::string		m_szEmitterBgImage;

	std::vector<CParticle> m_vDeadParticles;

	bool	m_bFinished;
	int	m_nTotalParticlesCreated;

	std::vector<CParticle> m_vActiveParticles;

};

