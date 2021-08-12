/*
File:						VoiceReco.cpp
Course:					SGP
Author:					Vortex Studios
Last Modified By:	Naveen Anand Gunalan
Last Edited:			9/24/2012
Purpose:				Voice Recognition
*/

#include "VoiceReco.h"
#include "EventManager.h"

CVoiceReco* CVoiceReco::GetInstance()
{
	static CVoiceReco s_Instance;
	return &s_Instance;
}

CVoiceReco::CVoiceReco(void)
{
	pPhrase = nullptr;
	pParts = nullptr;
	pwszText = nullptr;

	m_bDone = true;
}


CVoiceReco::~CVoiceReco(void)
{
}

void CVoiceReco::Init()
{
	//initializing comm
	 	CoInitialize(0);
	 
	 	//using shared recognizer uses the the microphone stated in the control panel
		//hr = cpEngine.CoCreateInstance(CLSID_SpSharedRecognizer);
	 	hr = cpEngine.CoCreateInstance(CLSID_SpSharedRecognizer);
	 	hr = cpEngine->CreateRecoContext(&cpRecoCtx);
	 
	 	//asks sapi to use win32 event handle to notify us of events;
	 	hr = cpRecoCtx->SetNotifyWin32Event();
	 
	 	//waiting on the event DONT TRY TO SET THE EVENT sapi sets the event
	 	hEvent = cpRecoCtx->GetNotifyEventHandle();
	 
	 	//positive and negative test cases
	 	ullEvents = SPFEI(SPEI_RECOGNITION) | SPFEI(SPEI_FALSE_RECOGNITION);
	 	hr = cpRecoCtx->SetInterest(ullEvents, ullEvents);
	 
	 	//creating a grammar object
	 	//hr = cpRecoCtx->CreateGrammar(1, &cpGram);
	 	hr = cpRecoCtx->CreateGrammar(NULL, &cpGram);
	 
	 	//loading a grammar from a file stated in the command line
	 	//hr = cpGram->LoadCmdFromFile(L"C:/Users/Schadrac Francois/Documents/Work/Ideas/Sound API test/formation.cfg", SPLO_STATIC);
		 //hr = cpGram->LoadCmdFromFile(L"resource/formation.cfg", SPLO_STATIC);
	 	hr = cpGram->LoadDictation(NULL, SPLO_STATIC);
	 
	 	//setting the rules state which sapi will check against
	 	//hr = cpGram->SetRuleState(0, 0, SPRS_ACTIVE);
	 
	 	// activate dictation
	 	hr = cpGram->SetDictationState(SPRS_ACTIVE);
	 
	 	//cpGram->SetGrammarState(SPGRAMMARSTATE::SPGS_ENABLED);

}

bool CVoiceReco::Listen()
{

	if( m_bDone == false )
	{
		 WaitForSingleObject(hEvent, 100);

		while ( evt.GetFrom(cpRecoCtx) == S_OK )
		{
			//if( evt.GetFrom(cpRecoCtx) == SPEI_FALSE_RECOGNITION || evt.GetFrom(cpRecoCtx) == SPEI_UNDEFINED )
			if ( evt.eEventId == SPEI_FALSE_RECOGNITION )
			{
				return false;
			}

			//if the recognizer understands then we will catch it
			else
			{
				//getting the phrase and storing it
				pPhrase = evt.RecoResult();
				hr = pPhrase->GetPhrase(&pParts);

				//hr = cpRecoResult->GetPhrase(&pParts);


				//you can specify which part of the phrase you want to store
				//if SP_GETWHOLEPHRASE is used the first parameter then it means to start at the beginning of the phrase
				//if it is used as the second parameter then it means to store the entire phrase


				hr = pPhrase->GetText(SP_GETWHOLEPHRASE, SP_GETWHOLEPHRASE, FALSE, &pwszText, 0);

				//hr = cpRecoResult->GetText(pParts->Rule.ulFirstElement, 1, FALSE, &pwszText, NULL);

				wstring follow = L"follow";
				wstring attack = L"attack";
				wstring defend = L"defend";
				wstring wholestring = pwszText;

				size_t found = wholestring.find( attack );
				if (found != string::npos )
				{
					//sending an event to attack
					CEventManager::GetInstance()->sendEvent("attack");
					CEventManager::GetInstance()->sendEvent("confirmed");
				}

				size_t found2 = wholestring.find( defend );
				if (found2 != string::npos )
				{
					//sending an event to defend the surrounding area
					CEventManager::GetInstance()->sendEvent("defend");
					CEventManager::GetInstance()->sendEvent("confirmed");
				}

				size_t found3 = wholestring.find( follow );
				if ( found3 != string::npos )
				{
					//sending an event to follow the player
					CEventManager::GetInstance()->sendEvent("follow");
					CEventManager::GetInstance()->sendEvent("confirmed");
				}

				//releasing what SAPI allocated for us
				CoTaskMemFree(pParts);
				CoTaskMemFree(pwszText);
			}
		}
	}

	return true;
}

void CVoiceReco::ShutDown()
{
	//releasing all the objects
	cpGram.Release();
	cpRecoCtx->SetNotifySink(NULL);
	cpRecoCtx.Release();
	cpEngine.Release();

	CoUninitialize();  
}
