/*
	File:						VoiceReco.cpp
	Course:					SGP
	Author:					Vortex Studios
	Last Modified By:	Schadrac Francois
	Last Edited:			9/24/2012
	Purpose:				Voice Recognition
*/

#pragma once
#include <Windows.h>
#include <sphelper.h>

#pragma warning(disable:4995)
#include <iostream>
using namespace std;


class CVoiceReco
{
private:

	//the information received from the recognizer
	LPWSTR pwszText;

	//event sent when the recognizer is done recognizing 
	HANDLE hEvent;

	//hResult
	HRESULT hr;

	//as the recognizer works it fires events and this is how we are going to catch those events
	CSpEvent evt;

	//parts of a phrase
	SPPHRASE *pParts;

	//I stands for interface
	//pointer to the phrase
	ISpPhrase *pPhrase;

	//event we are going to wait on
	ULONGLONG ullEvents;

	//Com - smart pointer that wraps the recognizer object
	//also models the engine
	CComPtr<ISpRecognizer> cpEngine;

	//recognition context for each window that requires commands
	CComPtr<ISpRecoContext> cpRecoCtx;

	//grammar used to dictate what is ok to say
	CComPtr<ISpRecoGrammar> cpGram;

	CComPtr<ISpRecoResult>    cpRecoResult;

	//securing the singleton
	CVoiceReco(void);
	~CVoiceReco(void);
	CVoiceReco( const CVoiceReco& );
	CVoiceReco& operator=( const CVoiceReco& );

public:

	//bool that will start or stop the while loop that takes in input
	bool			m_bDone;

	//singleton
	static CVoiceReco* GetInstance();

	//this is going to initialize the voice recognition 
	void Init();

	//actually going to listen to the user
	bool Listen();

	//shutting everything down
	void ShutDown();

	//start listening
	void StartListening()				{ m_bDone = false; }
	void StopListening()				{ m_bDone = true; }
};

