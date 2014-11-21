#include "TraiterWorkOrderTask.h"
#include "Srma.h"

TraiterWorkOrderTask::TraiterWorkOrderTask():p_WorkOrder(NULL),p_Srma(NULL)
{
}

TraiterWorkOrderTask::TraiterWorkOrderTask(SRMA *pSrma, WorkOrder *pWorkOrder):
p_WorkOrder(pWorkOrder),
p_Srma(pSrma)
{
	p_Robot=pSrma->getRobot();
	p_Tablette=pSrma->getTablette();
}

void TraiterWorkOrderTask::Start()
{
	ArASyncTask::runAsync();
	printf("Thread TraiterWorkOrderTask started\n");
}

void TraiterWorkOrderTask::Start(SRMA *pSrma, WorkOrder *pWorkOrder)
{
	p_WorkOrder=pWorkOrder;
	p_Srma=pSrma;
	p_Robot=pSrma->getRobot();
	p_Tablette=pSrma->getTablette();
	ArASyncTask::runAsync();
	printf("Thread TraiterWorkOrderTask started\n");
}

void TraiterWorkOrderTask::Stop()
{
	ArASyncTask::stopRunning();
	printf("Thread TraiterWorkOrderTask stopped\n");
}


void* TraiterWorkOrderTask::runThread(void*) 
  {
	//On set l'attribut currentWorkOrder de SRMA
	p_Srma->setCurrentWorkOrder(p_WorkOrder);
	
	char bufETCPServerEvt[256];

	//initialisation du démarrage du traitement
	//Le status du WorkOrder passe à InProgress
	p_WorkOrder->start();
	DAL::updateWorkOrder(*p_WorkOrder);
	//Mise à jour du state du SRMA
	p_Srma->setStateId(SRMA::BUSY);
	//stateId=BUSY;
	//On enleve l autodock
	p_Srma->getRobot()->DisableAutodock();
	int i=0;

	//Traitement des p_WorkOrderRoutings dans l'ordre de séquence
	while(i<int(p_WorkOrder->getWorkOrderRoutingList().size()))
	{	
		p_WorkOrderRouting=p_WorkOrder->getWorkOrderRoutingList()[i];
		
		//On ne traite que les p_WorkOrder au status NEW
		if(p_WorkOrderRouting->getStatusId()==WorkOrderRouting::NEW)
		{	
			printf("Debut traitement WorkOrderRoutingNo %s\n",p_WorkOrderRouting->getWorkOrderRoutingNo());
			p_Srma->setCurrentWorkOrderRouting(p_WorkOrderRouting);
			//Transport vers une destination
			if(strcmp(p_WorkOrderRouting->getType(),"T")==0)
			{
				WorkOrderRoutingTask wor(WorkOrderRoutingTask::TRANSPORT,p_Srma,p_WorkOrderRouting);
				wor.Start();
				//Attente fin de la tache
				do{
					ArUtil::sleep(250);
				} while(wor.getRunning());
	
				
				//traiterWorkOrderRoutingTransport(*p_WorkOrderRouting);
				//si le Transport a été annulé
				//On annule le WorkOrder
				//Attention le p_Robot peut être chargé!!!!! A traiter

				if(p_WorkOrderRouting->isCancelled())
				{
					p_WorkOrder->cancel();
					DAL::updateWorkOrder(*p_WorkOrder);
					//Prevenir la p_Tablette
					sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",p_WorkOrder->getWorkOrderNo(),(int)WorkOrder::CANCELLED);
					p_Tablette->SendMessage(bufETCPServerEvt);
					//Mise à jour du state du SRMA
					
					if(p_Srma->IsLoaded())
						p_Srma->setStateId(SRMA::FAULT);
						//stateId=FAULT;
					else
					{
						p_Srma->setStateId(SRMA::READY);
						//stateId=READY;
						//On remet l autodock
						//p_Robot.EnableAutodock();
					}
					//Notification de la fin de tache
					evtTaskEnded.notify(0);
					return NULL;
				}				
			}
			//Chargement
			else if(strcmp(p_WorkOrderRouting->getType(),"C")==0)
			{
				WorkOrderRoutingTask wor(WorkOrderRoutingTask::CHARGEMENT,p_Srma,p_WorkOrderRouting);
				wor.Start();
				do{
					ArUtil::sleep(250);
				} while(wor.getRunning());
				
		
				//p_WorkOrderRouting=*wor;
				//traiterWorkOrderRoutingChargement(*p_WorkOrderRouting);
				//si le Chargement a été annulé
				//On annule le WorkOrder
				//Le p_Robot est libéré
				//il faudra prévenir le client
				if(p_WorkOrderRouting->isCancelled())
				{
					p_WorkOrder->cancel();
					DAL::updateWorkOrder(*p_WorkOrder);
					//Prevenir la p_Tablette
					sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",p_WorkOrder->getWorkOrderNo(),(int)WorkOrder::CANCELLED);	
					p_Tablette->SendMessage(bufETCPServerEvt);	
					//Mise à jour du state du SRMA
					p_Srma->setStateId(SRMA::READY);
					//stateId=READY;
					//On remet l autodock
					p_Srma->getRobot()->EnableAutodock();
					//Notification de la fin de tache
					evtTaskEnded.notify(0);
					return NULL;
				}
			}
			else if(strcmp(p_WorkOrderRouting->getType(),"A")==0)
			{
				WorkOrderRoutingTask wor(WorkOrderRoutingTask::TRANSFERTA,p_Srma,p_WorkOrderRouting);
				wor.Start();
				do{
					ArUtil::sleep(250);
				} while(wor.getRunning());
				
		
				//p_WorkOrderRouting=*wor;
				//traiterWorkOrderRoutingChargement(*p_WorkOrderRouting);
				//si le Chargement a été annulé
				//On annule le WorkOrder
				//Le p_Robot est libéré
				//il faudra prévenir le client
				if(p_WorkOrderRouting->isCancelled())
				{
					p_WorkOrder->cancel();
					DAL::updateWorkOrder(*p_WorkOrder);
					//Prevenir la p_Tablette
					sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",p_WorkOrder->getWorkOrderNo(),(int)WorkOrder::CANCELLED);	
					p_Tablette->SendMessage(bufETCPServerEvt);	
					//Mise à jour du state du SRMA
					p_Srma->setStateId(SRMA::READY);
					//stateId=READY;
					//On remet l autodock
					p_Srma->getRobot()->EnableAutodock();
					//Notification de la fin de tache
					evtTaskEnded.notify(0);
					return NULL;
				}
			}

			else if(strcmp(p_WorkOrderRouting->getType(),"B")==0)
			{
				WorkOrderRoutingTask wor(WorkOrderRoutingTask::TRANSFERTB,p_Srma,p_WorkOrderRouting);
				wor.Start();
				do{
					ArUtil::sleep(250);
				} while(wor.getRunning());
				
		
				//p_WorkOrderRouting=*wor;
				//traiterWorkOrderRoutingChargement(*p_WorkOrderRouting);
				//si le Chargement a été annulé
				//On annule le WorkOrder
				//Le p_Robot est libéré
				//il faudra prévenir le client
				if(p_WorkOrderRouting->isCancelled())
				{
					p_WorkOrder->cancel();
					DAL::updateWorkOrder(*p_WorkOrder);
					//Prevenir la p_Tablette
					sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",p_WorkOrder->getWorkOrderNo(),(int)WorkOrder::CANCELLED);	
					p_Tablette->SendMessage(bufETCPServerEvt);	
					//Mise à jour du state du SRMA
					p_Srma->setStateId(SRMA::READY);
					//stateId=READY;
					//On remet l autodock
					p_Srma->getRobot()->EnableAutodock();
					//Notification de la fin de tache
					evtTaskEnded.notify(0);
					return NULL;
				}
			}

		//Livraison
			else if(strcmp(p_WorkOrderRouting->getType(),"L")==0)
			{
				WorkOrderRoutingTask wor(WorkOrderRoutingTask::LIVRAISON,p_Srma,p_WorkOrderRouting);
				wor.Start();

			//Attente fin de la tache
				do{
					ArUtil::sleep(250);
				} while(wor.getRunning());

			
				//p_WorkOrderRouting=*wor;
				//traiterWorkOrderRoutingLivraison(*p_WorkOrderRouting);	
				//si la livraison a été annulée
				//On annule le WorkOrder
				//Attention le p_Robot doit etre déchargé!!!!! A traiter
				//il faudra prévenir le client
				if(p_WorkOrderRouting->isCancelled())
				{
					char p_WorkOrderNo[256];
					WorkOrderRouting worRetourArticles= WorkOrderRouting();
					//worRetourArticles->setStatusId(WorkOrderRouting::NEW);
					//Attention!!! Le p_Robot va retourner à la Location du 1er WorkOrderRouting du WOrkOrderCourant
					worRetourArticles.setLocation(p_WorkOrder->getWorkOrderRoutingList()[0]->getLocation());
					worRetourArticles.setOperationSequence(p_WorkOrderRouting->getOperationSequence()+10);
					worRetourArticles.setType("T");
					worRetourArticles.setWorkOrder(p_WorkOrder);
					
					sprintf(p_WorkOrderNo,"%s-%d",p_WorkOrder->getWorkOrderNo(),worRetourArticles.getOperationSequence());
					worRetourArticles.setWorkOrderRoutingNo(p_WorkOrderNo);
					
					WorkOrderRoutingTask worRetourArticlesTask(WorkOrderRoutingTask::TRANSPORT,p_Srma,&worRetourArticles);
					worRetourArticlesTask.Start();

					
					//Attente fin de la tache
					do{
						ArUtil::sleep(250);
					} while(worRetourArticlesTask.getRunning());


					//traiterWorkOrderRoutingTransport(worRetourArticles);

					//Peut aussi mal se passer!!
					
					if(worRetourArticles.isClosed())
					{
						WorkOrderRouting worDechargement;
						//worDechargement->setStatusId(WorkOrderRouting::NEW);
						worDechargement.setLocation(p_WorkOrder->getWorkOrderRoutingList()[0]->getLocation());
						worDechargement.setOperationSequence(worRetourArticles.getOperationSequence()+10);
						worDechargement.setType("D");
						worDechargement.setWorkOrder(p_WorkOrder);
						
						sprintf(p_WorkOrderNo,"%s-%d",p_WorkOrder->getWorkOrderNo(),worDechargement.getOperationSequence());
						worDechargement.setWorkOrderRoutingNo(p_WorkOrderNo);

						//Déchargement
						WorkOrderRoutingTask worDechargementTask(WorkOrderRoutingTask::TRANSPORT,p_Srma,&worDechargement);
						worDechargementTask.Start();

						//Attente fin de la tache
						do{
							ArUtil::sleep(250);
						} while(worDechargementTask.getRunning());
						//traiterWorkOrderRoutingDechargement(worDechargement);
						// OK
						if(worDechargement.getStatusId()==WorkOrderRouting::CLOSE)
						{
							//Mise à jour du state du SRMA
							p_Srma->setStateId(SRMA::READY);
							//stateId=READY;
							//On remet l autodock
							p_Srma->getRobot()->EnableAutodock();
							//Notification de la fin de tache
							evtTaskEnded.notify(0);
							return NULL;
						}
						//Déchargement impossible
						else
						{
							if(worDechargement.isCancelled())
							{
								//Le SRMA passe FAULT (on ne peut pas continuer car il est chargé) et on sort				
								p_Srma->setStateId(SRMA::FAULT);
								//Notification de la fin de tache
								evtTaskEnded.notify(0);
								return NULL;
							}
						}
					}
					//Annulation du WorkOrder
					p_WorkOrder->cancel();
					DAL::updateWorkOrder(*p_WorkOrder);
					//Prevenir la p_Tablette
					sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",p_WorkOrder->getWorkOrderNo(),(int)WorkOrder::CANCELLED);	
					p_Tablette->SendMessage(bufETCPServerEvt);	
					//Mise à jour du state du SRMA
					p_Srma->setStateId(SRMA::READY);
					//On remet l autodock
					p_Srma->getRobot()->EnableAutodock();
					//Notification de la fin de tache
					evtTaskEnded.notify(0);
					return NULL;
				}
			}
			printf("Fin traitement normal WorkOrderRoutingNo %s\n",p_WorkOrderRouting->getWorkOrderRoutingNo());
		}
		i++;
	}
	//Tous les WorkOrderRoutings ont été réalisés

	//RAZ p_WorkOrderRouting et currentWorkOrder
	p_WorkOrder->close();
	
	//On reset l attribut currentWorkOrder de SRMA
	p_Srma->setCurrentWorkOrder(NULL);
	//currentWorkOrder=NULL;
	p_Srma->setCurrentWorkOrderRouting(NULL);
	//p_WorkOrderRouting=NULL;
	DAL::updateWorkOrder(*p_WorkOrder);
	sprintf(bufETCPServerEvt,"Object WorkOrder WorkOrderNo %s StatusId %d\n",p_WorkOrder->getWorkOrderNo(),(int)WorkOrder::CLOSE);	
	p_Tablette->SendMessage(bufETCPServerEvt);					
	//On remet l autodock
	p_Srma->getRobot()->EnableAutodock();
	//Mise à jour du state du SRMA
	p_Srma->setStateId(SRMA::READY);
	//Notification de la fin de tache
	evtTaskEnded.notify(0);
	return NULL;

}