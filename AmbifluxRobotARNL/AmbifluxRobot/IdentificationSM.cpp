#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "IdentificationSM.h"

//Constructeurs

/*IdentificationSM::IdentificationSM():
mySrma(NULL),
myNewState(true),
cptTentativeIdentification(3),
myRunning(false),
cptEchec(3),
myResult(RES_UNKNOWN)
{
	//start();
}*/

IdentificationSM::IdentificationSM(SRMA & pSrma, IhmCommunicationThread & ihm):
myIhm(ihm),
mySrma(pSrma),
myNewState(true),
cptTentativeIdentification(3),
cptEchec(3),
myRunning(false),
myResult(RES_UNKNOWN)
{
	ArLog::log(ArLog::Normal, "Appel constructeur IdentificationSM");
}

IdentificationSM::~IdentificationSM()
{
	ArLog::log(ArLog::Normal, "Appel destructeur IdentificationSM");
}
	
	
//Accesseurs
void IdentificationSM::setState(IdentificationSM::State pState)
{
	myState=pState;
	myNewState = true; 
	myStateStartTime.setToNow();
}

//Démarrage de la StateMachine
void IdentificationSM::start()
{
	setState(STATE_DEM_AFFICHAGE_IDENTIFICATION);
	myRunning = true;
	//SM loop
	while(myRunning == true)
	{
		handler();
		ArUtil::sleep(100);
	}
}


void IdentificationSM::handler()
{
	bool cond(false);
	int retCode;

	switch(myState)
	{
		case STATE_DEM_AFFICHAGE_IDENTIFICATION:
			if(myNewState)
			{
				printf("Entering IDENTFICATIONSM:STATE_DEM_AFFICHAGE_IDENTIFICATION\n");
				myNewState = false;
				printf("Sending : Open IdentificationVC\n");
				if(myIhm.sendRequest(bufRequestIhm) == 0){
					setState(STATE_ATTENTE_CARTE);
					break;
				}
			}

			//5sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if(--cptEchec <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_FIN);
				}
				else
					setState(STATE_DEM_AFFICHAGE_IDENTIFICATION);
			}

			break;

		
		case STATE_ATTENTE_CARTE:
			//Premier coup
			if(myNewState)
			{
				printf("Entering IDENTFICATIONSM:STATE_ATTENTE_CARTE\n");
				myNewState = false;
				//Todo remettre
				//Activation du lecteur carte
				//lecteurCarte.open();
				//lecteurCarte.startPooling();
				break;
			}

			//On attend la détection d'une carte
			for(int i=0;i<4;i++)
				//Todo remettre
				bufCardId[i]=lecteurCarte.getCardId()[i];
			cond=!(bufCardId[0]==0x00 && bufCardId[1]==0x00 && bufCardId[2]==0x00 && bufCardId[3]==0x00);
			//Le lecteur a signalé la presence d'une carte
			if(cond)
			{
				printf("Carte lue : %02X:%02X:%02X:%02X\n", bufCardId[3], bufCardId[2], bufCardId[1], bufCardId[0]);
				sprintf(strCardId,"%02X%02X%02X%02X\0",bufCardId[3], bufCardId[2], bufCardId[1], bufCardId[0]);
				//Identifier utilisateur
				setState(STATE_IDENTIFICATION);
				break;
			}
			
			//Rien recu ou timeout depassé
			if(myStateStartTime.mSecSince() > 60000)
			{
				myResult=RES_TIMEOUT;
				setState(STATE_DEM_FERMETURE_IDENTIFICATION);	
			}
			break;		

		case STATE_IDENTIFICATION:	
			if(myNewState)
			{
				printf("Entering IDENTFICATIONSM:STATE_IDENTIFICATION\n");	
				myNewState = false;
				//Todo remettre
				//lecteurCarte.stopPooling();
				//lecteurCarte.close();
			}
			printf("Tentative Identification No : %d\n",cptTentativeIdentification);
			//Recuperer infos de l utilisateur
			//Todo : DALRest
			//retCode=DAL::getResourceByCardId(&operateur,strCardId);
			//Identification failed
			if(retCode!=0)
			{
				//3 tentatives
				if((cptTentativeIdentification--)<=0)
				{
					myResult=RES_IDENTIFICATION_FAILED;
					setState(STATE_DEM_FERMETURE_IDENTIFICATION);	
				}
				else
					setState(STATE_DEM_AFFICHAGE_IDENTIFICATIONFAILED);	
				break;
			}
			//Identification OK
			myResult=RES_IDENTIFICATION_OK;
			setState(STATE_FIN);
			//setState(STATE_DEM_FERMETURE_IDENTIFICATION);
				
			break;

		
		//Demande affichage popup identification failed
		// et attente acquittement
		case STATE_DEM_AFFICHAGE_IDENTIFICATIONFAILED:
			if (myNewState)
			{
				printf("Entering IDENTFICATIONSM:STATE_DEM_AFFICHAGE_IDENTIFICATIONFAILED\n");
				myNewState=false;
				sprintf(bufRequestIhm,"Open IdentificationFailedVC\n");
				if(myIhm.sendRequest(bufRequestIhm) == 0){
					setState(STATE_ATTENTE_ACQUITTEMENT);
					break;
				}
				break;
			}

			//2sec pour que la tablette ait ouvert le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_DEM_FERMETURE_IDENTIFICATIONFAILED);
				}
				else
					setState(STATE_DEM_AFFICHAGE_IDENTIFICATIONFAILED);
				
				break;
			}
			break;

		case STATE_ATTENTE_ACQUITTEMENT:
			//Fomulaire not ouvert donc acquittement recu
			//Todo : à changer sur réception msg tablette
			/*if(!tabletteSM->IsIdentificationFailedVCOpened())
			{
				setState(STATE_ATTENTE_CARTE);
				break;
			}*/

			//Timeout depassé, on demabde la fermeture du form
			if(myStateStartTime.mSecSince() > 60000)
			{
				myResult=RES_TIMEOUT;
				setState(STATE_DEM_FERMETURE_IDENTIFICATIONFAILED);	
			}
			break;	

		//Demande de fermeture
		case STATE_DEM_FERMETURE_IDENTIFICATIONFAILED:
			if(myNewState)
			{
				printf("Entering IDENTFICATIONSM:STATE_DEM_FERMETURE_IDENTIFICATIONFAILED\n");
				myNewState=false;
				sprintf(bufRequestIhm,"Close IdentificationFailedVC\n");
				if(myIhm.sendRequest(bufRequestIhm) == 0){
					setState(STATE_DEM_AFFICHAGE_IDENTIFICATION);
					break;
				}
				break;
			}
			

			//5sec pour que la tablette ait fermé le form
			if(myStateStartTime.mSecSince() > 5000 )
			{
				if((--cptEchec) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_IDENTIFICATIONFAILED);
				
				break;
			}


			break;
			

		case STATE_DEM_FERMETURE_IDENTIFICATION:
			if(myNewState)
			{
				myNewState=false;
				printf("Entering IDENTFICATIONSM:STATE_DEM_FERMETURE_IDENTIFICATION\n");
				sprintf(bufRequestIhm,"Close IdentificationVC\n");
				if(myIhm.sendRequest(bufRequestIhm) == 0){
					setState(STATE_FIN);
					break;
				}
				
			}
			

			//Rien recu ou timeout depassé
			if(myStateStartTime.mSecSince() > 5000)
			{
				if((cptEchec--) <= 0)
				{
					myResult=RES_ERROR;
					setState(STATE_FIN);
				}
				else
					setState(STATE_DEM_FERMETURE_IDENTIFICATION);
				
				//sprintf(bufRequestIhm,"Close IdentificationdVC\n");
				//tabletteSM->SendMessage(bufRequestIhm);
			}
			
			break;

		case STATE_FIN:
			if(myNewState)
			{
				printf("Entering IDENTFICATIONSM:STATE_FIN\n");
				myNewState=false;
			}
				myRunning = false;
			break;
	}
	
}

IdentificationSM::State IdentificationSM::getState(){return myState;}
IdentificationSM::Resultat IdentificationSM::getResultat(){return myResult;}
//Resource* IdentificationSM::getOperateur(){return operateur;}
Resource* IdentificationSM::getOperateur(){return &operateur;}

