/*
	File:				Camera.h
	Course:				SGP
	Author:				Vortex Studios
	Last Modified By:	Roberto Alvarado
	Last Edited:		9/8/2012
	Purpose:			Contain / manage the Camera
*/
#pragma once
#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
//using namespace std;
class CCamera
{
public:
	
	//SINGLETON
	static CCamera* GetInstance();


	//Accessors

	float	GetX()		{return m_nX;}
	float	GetY()		{return m_nY;}
	float	GetWidth()	{return m_nWidth;}
	float	GetHeight()	{return m_nHeight;}
	float	GetRight()	{return m_nX+m_nWidth;}
	float	GetBottom()	{return m_nY+m_nHeight;}
	RECT	GetCameraRect();
	

	//Mutators

	void SetX		(float nX)	{m_nX =			nX;}
	void SetY		(float nY)	{m_nY =			nY;}
	void SetWidth	(float nW)	{m_nWidth =		nW;}
	void SetHeight	(float nH)	{m_nHeight =	nH;}

	//Initializes camera values (x,y,Width,Height)
	void InitCamera (float nX, float nY, float nWindowWidth, float nWindowHeight);

	//Takes in players X and Y then sets that to the cameras center point
	void Update(float nX, float nY, int nWorldWidth, int nWorldHeight);


private:
	float		m_nX;
	float		m_nY;
	float		m_nWidth;
	float		m_nHeight;

	CCamera(void);
	~CCamera(void);
	CCamera(const CCamera&);					//Copy Constructor
	CCamera& operator=(const CCamera&);			//Assignment Operator
};

