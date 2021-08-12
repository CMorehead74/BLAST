/*
	File:				Camera.cpp
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Roberto Alvarado
	Last Edited:		9/15/2012
	Purpose:			Contain / manage the Camera
*/

#include "Camera.h"

/*static*/ CCamera* CCamera::GetInstance()
{
	static CCamera pCamera;
	return &pCamera;
}


CCamera::CCamera(void)
{
	InitCamera(0,0,0,0);
}
CCamera::~CCamera(void)
{
}

void CCamera::Update(float nX, float nY, int nWorldWidth, int nWorldHeight)
{
	m_nX = nX -(m_nWidth* 0.5f);
	m_nY = nY -(m_nHeight*0.5f);

	if(m_nX < 0)	//Check if the camera has moved past the left side of the World
		m_nX=0;
	if(m_nY < 0)	//Check if the camera has moved past the top side of the World
		m_nY=0;

	if(m_nX+ m_nWidth > nWorldWidth)	//Check if the camera has moved past the right side of the World
		m_nX= nWorldWidth-m_nWidth;
	if(m_nY+m_nHeight > nWorldHeight)	//Check if the camera has moved past the bottom side of the World
		m_nY= nWorldHeight-m_nHeight;
}
void CCamera::InitCamera (float nX, float nY, float nWindowWidth, float nWindowHeight)
{
	m_nX		= nX;
	m_nY		= nY;
	m_nWidth	= nWindowWidth;
	m_nHeight	= nWindowHeight;
}

RECT CCamera::GetCameraRect()
{
	RECT rCam = {(long)m_nX, (long)m_nY, (long)GetRight(), (long)GetBottom()};
	return rCam;
}
