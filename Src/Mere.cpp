/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Quentin Vecchio
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

///////////////////////////////////////////////////////////////// 	 PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
/*
	Entree 1 : Entree Prof
	Entree 2 : Entree etudiant
	Entree 3 : Entree communue
*/
static pid_t pidEntrees[3];
static pid_t pidSortie;
static pid_t pidHeure;
static pid_t pidGestionClavier;

static int BalEntreeId[3];
static int BalSortieId;

static int MPRequeteId;
static int MPContenuParkingId;
static int MPNbPlaceId;
static StructParking *MPParking;
static StructTabRequetes *MPRequetes;
static unsigned int * MPNbPlace;

static int SemaphoreContenuParkingId;
static int SemaphoreRequeteId;
static int SemaphoreNbPlaceId;
static int SemaphoreSynchroEntreesSortie[3];

//------------------------------------------------------ Fonctions privées

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

int main() 
// Algorithme :
{
	Initialisation();
	Moteur();
	return 0;
} //Fin de main

void Initialisation()
// Algorithme :
{
	//Création indice de la clé
	int indiceCle = 0;
	//Initialisation de l'application
	InitialiserApplication(XTERM);
	//Création processus heure
	pidHeure = ActiverHeure();
	//Création des BàLs
	for(unsigned int i =0;i<NB_BARRIERES_ENTREE;i++)
	{	if((BalEntreeId[i] = msgget(ftok("./Parking",indiceCle++), IPC_CREAT | 0660)) < 0)
		{	cout << "Erreur création BAL " << i << endl;
		}
	}	
	if((BalSortieId = msgget(ftok("./Parking",indiceCle++), IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur création BAL Sortie" << endl;
	}
	//Création des MP
	if((MPContenuParkingId = shmget(ftok("./Parking",indiceCle++), sizeof(StructParking), IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur : Impossible de créer la mémoire partagé ContenuParking" << endl;
	}
	else
	{	MPParking = (StructParking*) shmat(MPContenuParkingId,NULL,0);
		for(unsigned int i=0; i < NB_PLACES ; i++)
   	 	{	MPParking->voitures[i] = {0,0,TypeUsager::AUCUN,0};
    	}
    	shmdt(MPParking);
	}
	if((MPRequeteId = shmget(ftok("./Parking",indiceCle++), sizeof(StructTabRequetes),IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur : Impossible de créer la mémoire partagée requetes" << endl;
	}
	else
	{	MPRequetes = (StructTabRequetes*) shmat(MPRequeteId,NULL,0);
		for(unsigned int i=0; i < NB_BARRIERES_ENTREE ; i++)
   	 	{	MPRequetes->requetes[i].typeBarriere = TypeBarriere::AUCUNE;
   	 		MPRequetes->requetes[i].voiture = {0,0,TypeUsager::AUCUN,0};
    	}
    	shmdt(MPRequetes);
	}
	if((MPNbPlaceId = shmget(ftok("./Parking",indiceCle++), sizeof(unsigned int),IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur : Impossible de créer la mémoire partagée nb place" << endl;
	}
	else
	{	MPNbPlace = (unsigned int*) shmat(MPNbPlaceId,NULL,0);
		*MPNbPlace = NB_PLACES;
    	shmdt(MPNbPlace);
	}
	//Création sémaphores
	if((SemaphoreNbPlaceId = semget(ftok("./Parking",indiceCle++), 1, IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if(semctl(SemaphoreNbPlaceId, 0, SETVAL, 1) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if((SemaphoreContenuParkingId = semget(ftok("./Parking",indiceCle++), 1, IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if(semctl(SemaphoreContenuParkingId, 0, SETVAL, 1) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if((SemaphoreRequeteId = semget(ftok("./Parking",indiceCle++), 1, IPC_CREAT | 0660)) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	if(semctl(SemaphoreRequeteId, 0, SETVAL, 1) < 0)
	{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
	}
	for(unsigned int i=0;i<NB_BARRIERES_ENTREE;i++)
	{	if((SemaphoreSynchroEntreesSortie[i] = semget(ftok("./Parking",indiceCle++), 1, IPC_CREAT | 0660)) < 0)
		{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
		}
		if(semctl(SemaphoreSynchroEntreesSortie[i], 0, SETVAL, 1) < 0)
		{	cout << "Erreur : Impossible de créer la sémaphore" << endl;
		}
		struct sembuf op;
		op.sem_num = 0;
		op.sem_op = -1;
		op.sem_flg = 0;
		semop(SemaphoreSynchroEntreesSortie[i], &op, 1);
	}
	//Création Processus
	if((pidGestionClavier = fork()) < 0) 
	{	cout << "Erreur : impossible de faire un fork" << endl;	
	}
	else if (pidGestionClavier == 0)
	{	GestionClavier(BalEntreeId, BalSortieId);
	}
	else
	{	if((pidSortie = fork()) < 0) 
		{	cout << "Erreur : impossible de faire un fork" << endl;	
		} 
		else if (pidSortie == 0)
		{	Sortie(BalSortieId, SemaphoreSynchroEntreesSortie, SemaphoreContenuParkingId, SemaphoreRequeteId ,SemaphoreNbPlaceId, MPContenuParkingId, MPRequeteId, MPNbPlaceId);
		}
		else {
			for(unsigned int i=0;i<NB_BARRIERES_ENTREE;i++) 
			{	 if((pidEntrees[i] = fork()) < 0) 
				{	cout << "Erreur : impossible de faire un fork " << i << endl;	
				} 
				else if (pidEntrees[i] == 0)
				{	Entree(i, BalEntreeId[i], SemaphoreSynchroEntreesSortie[i], SemaphoreContenuParkingId, SemaphoreRequeteId, SemaphoreNbPlaceId, MPContenuParkingId, MPRequeteId, MPNbPlaceId);
				}
			}
		}
	}
} //Fin de Initialisation


void Moteur()
// Algorithme :
{
	waitpid(pidGestionClavier, NULL, 0);
	Destruction();
} //Fin de Moteur


void Destruction()
// Algorithme :
{
	//Destruction processus entrées
	for(int i=0;i<3;i++)
	{	kill(pidEntrees[i], SIGUSR2);
		waitpid(pidEntrees[i], NULL, 0);
	}	
	//Destruction sortie
	kill(pidSortie, SIGUSR2);
	waitpid(pidSortie, NULL, 0);
	//Destruction mémoire partagé
	if(shmctl(MPRequeteId, IPC_RMID, 0)!=0)
	{	cout << "Erreur : Impossible de détruire la MP requete" << endl;
	}
	if(shmctl(MPContenuParkingId, IPC_RMID, 0)!=0)
	{	cout << "Erreur : Impossible de détruire la MP ContenuParking" << endl;
	}
	if(shmctl(MPNbPlaceId, IPC_RMID, 0)!=0)
	{	cout << "Erreur : Impossible de détruire la MP nb place" << endl;
	}
	//Destruction Bals
	for(int i=0; i< 3;i++)
	{	if(msgctl(BalEntreeId[i], IPC_RMID, 0)!=0)
		{	cout << "Erreur : Impossible de détruire la Bal Entree " << i << endl;
		}
	}
	if(msgctl(BalSortieId, IPC_RMID, 0)!=0)
	{	cout << "Erreur : Impossible de détruire la Bal Sortie" << endl;
	}
	//Destruction sémaphore
	if(semctl(SemaphoreNbPlaceId, 0, IPC_RMID, 0) != 0)
	{	cout << "Erreur : Impossible de détruire la sémaphore Compteur place " << endl;
	}
	if(semctl(SemaphoreContenuParkingId, 0, IPC_RMID, 0) != 0)
	{	cout << "Erreur : Impossible de détruire la sémaphore Contenu parking " << endl;
	}
	if(semctl(SemaphoreRequeteId, 0, IPC_RMID, 0) != 0)
	{	cout << "Erreur : Impossible de détruire la sémaphore requete" << endl;
	}
	for(unsigned int i=0;i<NB_BARRIERES_ENTREE;i++) 
	{	if(semctl(SemaphoreSynchroEntreesSortie[i], 0, IPC_RMID, 0) != 0)
		{	cout << "Erreur : Impossible de détruire la sémaphore entree sortie " << i << endl;
		}
	}
	//Destruction processus heure
	kill(pidHeure, SIGUSR2);
	waitpid(pidHeure, NULL, 0);
	//Destruction de la vue
	TerminerApplication(false);
} //Fin de Destruction

