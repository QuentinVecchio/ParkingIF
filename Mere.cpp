/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Adrien Lepic, Marcin , Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//------------------------------------------------------------------------
// Rôle de la tâche <Mere>
//
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Mere.h"
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////////////

/*
	Entree 1 : Entree Prof
	Entree 2 : Entree etudiant
	Entree 3 : Entree communue
*/
static pid_t pidEntrees[3];
static pid_t pidSortie;
static pid_t pidHeure;
static pid_t pidGestionClavier;

static int BalEntreeId[0];
static int BalSortieId;

static int MPRequeteId;
static int MPNbPlaceId;
static int MPEtatId;

static int SemaphoreCompteurPlaceId;
static int SemaphoreRequeteId;
static int SemaphoreEtatId;
static int SemaphoreSynchroEntreesSortie[3];

int main() 
{
	Initialisation();
	Moteur();
	return 0;
}

void SignalDestruction ( int noSignal )
{
	Destruction();
	exit(0);
}

void Initialisation()
{
	//Création du handler et assignation
	struct sigaction action;
	action.sa_handler = SignalDestruction;
	sigemptyset ( &action.sa_mask );
	action.sa_flags = 0;
	sigaction( SIGUSR2, &action, NULL );
	//Création de la clé publique
	key_t cle = ftok("parkingIF",1);
	//Création des BàLs
	if((BalEntreeId[0] = msgget(cle, IPC_CREAT)) < 0)
	{	cout << "Erreur création BAL" << endl;
	}
	if((BalEntreeId[1] = msgget(cle, IPC_CREAT)) < 0)
	{	cout << "Erreur création BAL" << endl;
	}
	if((BalEntreeId[2] = msgget(cle, IPC_CREAT)) < 0)
	{	cout << "Erreur création BAL" << endl;
	}
	if((BalSortieId = msgget(cle, IPC_CREAT)) < 0)
	{	cout << "Erreur création BAL" << endl;
	}
	//Création des MP
	if((MPNbPlaceId = shmget(cle, sizeof(unsigned int), IPC_CREAT)) < 0)
	{	cout << "Erreur : Impossible de créer la mémoire partagée" << endl;
	}
	if((MPRequeteId = shmget(cle, sizeof(StructRequete)*999, IPC_CREAT)) < 0)
	{	cout << "Erreur : Impossible de créer la mémoire partagée" << endl;
	}
	if((MPEtatId = shmget(cle, sizeof(bool)*NB_PLACES, IPC_CREAT)) < 0)
	{	cout << "Erreur : Impossible de créer la mémoire partagée" << endl;
	}
	//Création sémaphores
	if((SemaphoreCompteurPlaceId = semget(cle, 1, IPC_CREAT)) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if((SemaphoreRequeteId = semget(cle, 1, IPC_CREAT)) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if((SemaphoreEtatId = semget(cle, 1, IPC_CREAT)) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	for(int i=0;i<3;i++)
	{	if((SemaphoreSynchroEntreesSortie[i] = semget(cle, 1, IPC_CREAT)) < 0)
		{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
		}
	}
	//Création Processus
	if((pidGestionClavier = fork()) < 0) 
	{	cout << "Erreur : impossible de faire un fork" << endl;	
	} 
	else if (pidGestionClavier == 0)
	{	GestionClavier();
		sleep(10);
	}
	else
	{	if((pidSortie = fork()) < 0) 
		{	cout << "Erreur : impossible de faire un fork" << endl;	
		} 
		else if (pidSortie == 0)
		{	//Sortie(BalSortieId);
			sleep(10);
		}
		if((pidEntrees[0] = fork()) < 0) 
		{	cout << "Erreur : impossible de faire un fork" << endl;	
		} 
		else if (pidEntrees[0] == 0)
		{	//Entree(1, BalEntreeId[0]);
			sleep(10);
		}
		if((pidEntrees[1] = fork()) < 0) 
		{	cout << "Erreur : impossible de faire un fork" << endl;	
		} 
		else if (pidEntrees[1] == 0)
		{	//Entree(2, BalEntreeId[1]);
			sleep(10);		
		}
		if((pidEntrees[2] = fork()) < 0) 
		{	cout << "Erreur : impossible de faire un fork" << endl;	
		} 
		else if (pidEntrees[2] == 0)
		{	//Entree(3, BalEntreeId[2]);
			sleep(10);		
		}
	}
	//Initialisation de l'application
	InitialiserApplication(XTERM);
	//Création processus heure
	pidHeure = ActiverHeure();
}

void Moteur()
{
	for( ; ; ) 
	{
	}
}

void Destruction()
{
	//Destruction processus entrées
	for(int i=0;i<3;i++)
	{	kill(pidEntrees[i], SIGUSR2);
		waitpid(pidEntrees[i], NULL, 0);
	}	
	//Destruction sortie
	kill(pidSortie, SIGUSR2);
	waitpid(pidSortie, NULL, 0);
	//Destruction Gestion Clavier
	kill(pidGestionClavier, SIGUSR2);
	waitpid(pidGestionClavier, NULL, 0);
	//Destruction mémoire partagé
	if(shmctl(MPRequeteId, IPC_RMID, NULL)!=0)
	{	cout << "Erreur : Impossible de détruire la Bal" << endl;
	}
	if(shmctl(MPNbPlaceId, IPC_RMID, NULL)!=0)
	{	cout << "Erreur : Impossible de détruire la Bal" << endl;
	}
	if(shmctl(MPEtatId, IPC_RMID, NULL)!=0)
	{	cout << "Erreur : Impossible de détruire la Bal" << endl;
	}
	//Destruction Bals
	for(int i=0; i< 3;i++)
	{	if(msgctl(BalEntreeId[i], IPC_RMID, NULL)!=0)
		{	cout << "Erreur : Impossible de détruire la Bal" << endl;
		}
	}
	if(msgctl(BalSortieId, IPC_RMID, NULL)!=0)
	{	cout << "Erreur : Impossible de détruire la Bal" << endl;
	}
	//Destruction sémaphore
	if(semctl(SemaphoreCompteurPlaceId, 0, IPC_RMID, 0) != 0)
	{	cout << "Erreur : Impossible de détruire la sémaphore" << endl;
	}
	if(semctl(SemaphoreRequeteId, 0, IPC_RMID, 0) != 0)
	{	cout << "Erreur : Impossible de détruire la sémaphore" << endl;
	}
	if(semctl(SemaphoreEtatId, 0, IPC_RMID,0) != 0)
	{	cout << "Erreur : Impossible de détruire la sémaphore" << endl;
	}
	for(int i=0;i<3;i++)
	{	if(semctl(SemaphoreSynchroEntreesSortie[i], 0, IPC_RMID, 0) != 0)
		{	cout << "Erreur : Impossible de détruire la sémaphore" << endl;
		}
	}
	//Destruction processus heure
	kill(pidHeure, SIGUSR2);
	waitpid(pidHeure, NULL, 0);
	//Destruction de la vue
	TerminerApplication();
}
