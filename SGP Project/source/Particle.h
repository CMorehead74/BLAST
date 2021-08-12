/*
	File:				Particle.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/21/2012
	Purpose:			takes care if individual Particles
*/

#pragma once

#include <Windows.h>
#include "../SGD Wrappers/CSGD_Direct3D.h"
#include "../SGD Wrappers/CSGD_TextureManager.h"
#include "Camera.h"

#include "HELPER.h"

class CParticle
{
private:

	CSGD_TextureManager* pTM;
	int m_ParentEmitterType;
	int m_angleOfEmission;
	//Particle Position
	int m_nParticlePosX;
	int m_nParticlePosY;

	//Particle Current Stats
	float m_fCurrentScaleX;
	float m_fCurrentScaleY;

	RECT m_CurrentColor; 

	float m_fCurrenRotation;

	float m_fCurrentVelX;
	float m_fCurrentVelY;

	float m_fCurrentLife;

	float m_fMyLife;

	float m_fEndScaleX;
	float m_fEndScaleY;
	float m_fEndRotation;

	RECT m_fEndColor;
	RECT m_fStartColor;

	float m_fOuterRingRadius;
	float m_fAngleOfEmission;

	float m_scaleRate;
    float m_rotationRate;
    float m_colorRate;
    float m_velXRate;
    float m_velYRate; 

	int m_particleImgID;

	RECT m_EmitterDimensions;

public:
	CParticle(void);
	CParticle(float angleOfEmission,float outerRingRadius, RECT emitterDimension, int emitterType, int X, int Y, float wind, float velocityX, float velocityY, int currentScaleX, int currentScaleY, int endScaleX, int endScaleY, RECT startColor, RECT endColor, float startRotation, float endRotation, float currentLife, float myLife, float scaleRate, float colorRate, float rotationRate, float velXRate, float velYRate, int particleImageID);
	
	~CParticle(void);

	//Accessors
	int GetParticlePosX			(void)		{ return m_nParticlePosX; }
	int GetParticlePosY			(void)		{ return m_nParticlePosY; }
	float GetMyLife				(void)		{ return m_fMyLife; }

	float GetCurrentLife		(void)		{ return m_fCurrentLife; }

	void SetParticlePosX   ( int   value ) { m_nParticlePosX	= value; }
	void SetParticlePosY   ( int   value ) { m_nParticlePosY	= value; }
	void SetCurrentScaleX   ( float value ) { m_fCurrentScaleX	= value; }
	void SetCurrentScaleY   ( float value ) { m_fCurrentScaleY	= value; }
	void SetCurrentRotation( float value ) { m_fCurrenRotation	= value; }
	void SetCurrentVelX    ( float value ) { m_fCurrentVelX	    = value; }
	void SetCurrentVelY    ( float value ) { m_fCurrentVelY	    = value; }
	void SetCurrentLife    ( float value ) { m_fCurrentLife	    = value; }
				           									 
	void SetMyLife         ( float value ) { m_fMyLife			= value; }
	
				           									 
	void SetEndScaleX       ( float value ) { m_fEndScaleX		= value; }
	void SetEndScaleY       ( float value ) { m_fEndScaleY		= value; }
	void SetEndRotation    ( float value ) { m_fEndRotation		= value; }
	void SetEndColor       ( RECT value ) { m_fEndColor		= value; }

	void Update(float fElapsedTime);
	void Render(void);
	void RenderWithRotation(float rotation);

	//void RenderRotation(int ImgID, int PosX, int PosY,  float fScaleX, float fScaleY, float fRotateX, float fRotateY, float fCurrRotate, DWORD color);
};

