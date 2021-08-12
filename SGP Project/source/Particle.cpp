/*
	File:				Particle.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Naveen Anand Gunalan
	Last Edited:		9/21/2012
	Purpose:			takes care if individual Particles
*/
#include "Particle.h"
#include "Bullet.h"
#include "Player.h"


CParticle::CParticle(void)
{
	pTM = CSGD_TextureManager::GetInstance();
}

CParticle::CParticle(float angleOfEmission,
					 float outerRingRadius, 
					 RECT emitterDimension, 
					 int emitterType, 
					 int X, 
					 int Y, 
					 float wind, 
					 float velocityX, 
					 float velocityY, 
					 int currentScaleX, 
					 int currentScaleY, 
					 int endScaleX,
					 int endScaleY,
					 RECT startColor, 
					 RECT endColor, 
					 float startRotation, 
					 float endRotation, 
					 float currentLife, 
					 float myLife, 
					 float scaleRate, 
					 float colorRate, 
					 float rotationRate, 
					 float velXRate, 
					 float velYRate, 
					 int particleImageID)
{
	
	pTM = CSGD_TextureManager::GetInstance();

	m_fOuterRingRadius = outerRingRadius;

	m_EmitterDimensions = emitterDimension;

	m_angleOfEmission = (int)angleOfEmission;

	m_ParentEmitterType = emitterType;
	//centering it
	m_nParticlePosX = X;// -(int)(width * 0.5f);
	m_nParticlePosY = Y;// -(int)(height * 0.5f);

	m_fCurrentVelX = velocityX;
	m_fCurrentVelY = velocityY;

	//m_fwind = wind;

	m_fCurrentScaleX = (float)currentScaleX;
	m_fCurrentScaleY = (float)currentScaleY;

	m_fEndScaleX = (float)endScaleX;
	m_fEndScaleY = (float)endScaleY;

	m_fCurrenRotation = startRotation;
	m_fEndRotation = endRotation;

	m_CurrentColor = startColor;
	m_fStartColor = startColor;
	m_fEndColor = endColor;

	m_fCurrentLife = currentLife;            

	m_fEndRotation = endRotation;
	m_fMyLife = myLife;

	m_scaleRate = scaleRate;
	m_rotationRate = rotationRate;
	m_colorRate = colorRate;
	m_velXRate = velXRate;
	m_velYRate = velYRate;

	m_particleImgID = particleImageID;
}

CParticle::~CParticle(void)
{
}

void CParticle::Update(float fElapsedTime)
{
	m_fCurrentLife++;

	if (m_ParentEmitterType == RECTANGLE)
	{
		//m_nParticlePosX += (int)m_fCurrentVelX;m_angleOfEmission
		//m_nParticlePosY += (int)m_fCurrentVelY;
	}
	else if(m_ParentEmitterType == CIRCLE)
	{
		float a = (m_EmitterDimensions.right + m_fOuterRingRadius) * 0.5f;
		float b = (m_EmitterDimensions.bottom + m_fOuterRingRadius) * 0.5f;
		int endposX = (int)((a * b) / sqrt(b * b + a * a * tan((double)m_angleOfEmission) * tan((double)m_angleOfEmission)));
		int endposY = (int)((a * b * tan((double)m_angleOfEmission)) / sqrt(b * b + a * a * tan((double)m_angleOfEmission) * tan((double)m_angleOfEmission)));
		
		a = m_EmitterDimensions.right * 0.5f;
		b = m_EmitterDimensions.bottom * 0.5f;
		
		int startposX = (int)((a * b) / sqrt(b * b + a * a * tan((double)m_angleOfEmission) * tan((double)m_angleOfEmission)));
		int startposY = (int)((a * b * tan((double)m_angleOfEmission)) / sqrt(b * b + a * a * tan((double)m_angleOfEmission) * tan((double)m_angleOfEmission)));
					
		if (m_angleOfEmission > 90 && m_angleOfEmission < 270)
		{
			m_nParticlePosX = m_EmitterDimensions.left + startposX + (int)(rand()%101 / 10) * (endposX - startposX);
			m_nParticlePosY = m_EmitterDimensions.top + startposY + (int)(rand()%101 / 10) * (endposY - startposY);
		}
		else
		{
			m_nParticlePosX = m_EmitterDimensions.left - startposX - (int)(rand()%101 / 10) * (endposX - startposX);
			m_nParticlePosY = m_EmitterDimensions.top  - startposY - (int)(rand()%101 / 10) * (endposY - startposY);
		}


	}
	m_nParticlePosX += (int)m_fCurrentVelX;
	m_nParticlePosY += (int)m_fCurrentVelY;

	
	//Calculate interpolation
	float fAmount = m_fCurrentLife / m_fMyLife;

	//Color
	float A = m_fStartColor.left  + fAmount * (m_fEndColor.left   - m_fStartColor.left  );
	float R = m_fStartColor.top   + fAmount * (m_fEndColor.top    - m_fStartColor.top   );
	float G = m_fStartColor.right + fAmount * (m_fEndColor.right  - m_fStartColor.right );
	float B = m_fStartColor.bottom+ fAmount * (m_fEndColor.bottom - m_fStartColor.bottom);
	
	if (A < 0) { A = 0; }
	else if (A > 255) { A = 255; }
	if (R > 255) { R = 255; }
	else if (R < 0) { R = 0; }
	if (G > 255) { G = 255; }
	else if (G < 0) { G = 0; }
	if (B > 255) { B = 255; }
	else if (B < 0) { B = 0; }

	/*float A = 0.0f;
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	if(m_CurrentColor.left < m_fEndColor.left)
		A = m_CurrentColor.left + m_colorRate;
	if (m_CurrentColor.top < m_fEndColor.top)
		R = m_CurrentColor.top + m_colorRate;
	if (m_CurrentColor.right < m_fEndColor.right)
		G = m_CurrentColor.right + m_colorRate;
	if (m_CurrentColor.bottom < m_fEndColor.bottom)
		B = m_CurrentColor.bottom + m_colorRate;*/
	SetRect(&m_CurrentColor,(int)A, (int)R, (int)G, (int)B);

	//Rotation
	if(m_fCurrenRotation < m_fEndRotation)
		m_fCurrenRotation += m_rotationRate; //m_startRotation + fAmount * (m_endRotation - m_startRotation);
	else if(m_fCurrenRotation > m_fEndRotation)
		m_fCurrenRotation -= m_rotationRate;
	//Scale
	if (m_fCurrentScaleX < m_fEndScaleX)
		m_fCurrentScaleX += m_scaleRate;
	if (m_fCurrentScaleY < m_fEndScaleY)
		m_fCurrentScaleY += m_scaleRate;
}

void CParticle::Render(void)
{
	RECT rSource = {0,0,pTM->GetTextureWidth(m_particleImgID),pTM->GetTextureHeight(m_particleImgID)};

	pTM->Draw(m_particleImgID, (int)(m_nParticlePosX - CCamera::GetInstance()->GetX()), (int)(m_nParticlePosY - CCamera::GetInstance()->GetY()),
		m_fCurrentScaleX, m_fCurrentScaleY, &rSource, pTM->GetTextureWidth(m_particleImgID)*0.5f, pTM->GetTextureHeight(m_particleImgID)*0.5f, 
		(float)(m_fCurrenRotation*3.141592654f/180.0), D3DCOLOR_ARGB(m_CurrentColor.left,m_CurrentColor.top,m_CurrentColor.right,m_CurrentColor.bottom));
}

void CParticle::RenderWithRotation(float rotation)
{

	RECT rSource = {0,0,pTM->GetTextureWidth(m_particleImgID),pTM->GetTextureHeight(m_particleImgID)};

	pTM->Draw(m_particleImgID, (int)(m_nParticlePosX - CCamera::GetInstance()->GetX()), (int)(m_nParticlePosY - CCamera::GetInstance()->GetY()), m_fCurrentScaleX, m_fCurrentScaleY, &rSource,
		0, 0,  rotation, D3DCOLOR_ARGB(m_CurrentColor.left,m_CurrentColor.top,m_CurrentColor.right,m_CurrentColor.bottom));
}

//void CParticle::RenderRotation(int ImgID, int PosX, int PosY,  float fScaleX, float fScaleY, float fRotateX, float fRotateY, float fCurrRotate, DWORD color)
//{
//	RECT rSource = {0,0,pTM->GetTextureWidth(m_particleImgID),pTM->GetTextureHeight(m_particleImgID)};
//
//	pTM->Draw(m_particleImgID, PosX, PosY, fScaleX, fScaleY, &rSource, fRotateX, fRotateY, fCurrRotate, D3DCOLOR_ARGB(m_CurrentColor.left,m_CurrentColor.top,m_CurrentColor.right,m_CurrentColor.bottom));
//}