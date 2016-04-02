/*************************************************************************
                           Sortie  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C)	Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Réalisation de la tâche <Sortie> (fichier Sortie.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "Sortie.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static Voiture pidVoituriers[NB_VOITURIERS];
static int idMPRequetes;
static int idMPContenuParking;
static int idBal;
static int idSemaphoreSynchro[NB_BARRIERES_ENTREE];
static int idSemaphoreContenuParking;
static int idSemaphoreRequete;
static StructTabRequetes *requetes;
static StructParking *parking;

//------------------------------------------------------ Fonctions privées
static int getVoiturier() 
// Algorithme :
//
{
	for(int i=0;i<NB_VOITURIERS;i++) {
		if(pidVoituriers[i].numeroPlace == -1) 
		{	return i;
		}
	}
	return -1;
} //----- fin de getVoiturier

static void SignalDestruction ( int noSignal )
// Algorithme :
//
{
	for(int i=0;i<NB_VOITURIERS;i++) {
		if(pidVoituriers[i].numeroPlace != -1) 
		{	kill(pidVoituriers[i].numeroPlace, SIGUSR2);
			waitpid(pidVoituriers[i].numeroPlace, NULL, 0);
		}
	}
	exit(0);
} //----- fin de SignalDestruction
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void SignalVoitureSortie ( int noSignal )
// Algorithme :
//
{
	int pid;
	int numeroPlace = -1;
	int statut;
	if ((pid = wait(&statut)) == -1)
	{ cout << "Erreur wait pid" << endl;
	}
	numeroPlace = WEXITSTATUS(statut);
	for(unsigned int i=0;i<NB_VOITURIERS;i++) {
		if(pidVoituriers[i].numeroPlace == pid) 
		{	struct sembuf op;
			op.sem_num = 0;
			op.sem_op = -1;
			op.sem_flg = 0;
			while(semop(idMPContenuParking,&op,1) == -1 && errno == EINTR);
			Effacer((TypeZone)numeroPlace);
			AfficherSortie(parking->voitures[numeroPlace].usager, parking->voitures[numeroPlace].numeroPlaque, parking->voitures[numeroPlace].heureArrivee, time(NULL));
			parking->voitures[numeroPlace] = {0,0,TypeUsager::AUCUN,0};;
			op.sem_op = 1;
			semop(idMPContenuParking, &op, 1);
			pidVoituriers[i].numeroPlace = -1;
			GereRequete();
		}
	}
} //----- fin de SignalVoitureGaree

void GereRequete() 
// Algorithme :
//
{
	
} //----- fin de GereRequete

void Sortie(int _idBal, int* _idSemaphoreSynchro, int _idSemaphoreContenuParking, int _idSemaphoreRequete , int _idMPContenuParking, int _idMPRequete)
// Algorithme :
//
{
	struct sigaction action;
	action.sa_handler = SignalDestruction;
	sigemptyset ( &action.sa_mask );
	action.sa_flags = 0;
	sigaction( SIGUSR2, &action, NULL );
	struct sigaction actionCHLD;
	actionCHLD.sa_handler = SignalVoitureSortie;
	sigemptyset ( &actionCHLD.sa_mask );
	actionCHLD.sa_flags = 0;
	sigaction( SIGCHLD, &actionCHLD, NULL );
 	idMPRequetes = _idMPRequete;
	idMPContenuParking = _idMPContenuParking;
	idBal = _idBal;
	idSemaphoreContenuParking = _idSemaphoreContenuParking;
 	idSemaphoreRequete = _idSemaphoreRequete;
 	parking = (StructParking*) shmat(idMPContenuParking,NULL,0);
    requetes = (StructTabRequetes*) shmat(idMPRequetes,NULL,0);
 	for(unsigned int i=0;i<NB_VOITURIERS;i++) {
		pidVoituriers[i].numeroPlace = -1;
	}
	for(unsigned int i=0;i<NB_BARRIERES_ENTREE;i++)
	{	idSemaphoreSynchro[i] = _idSemaphoreSynchro[i];
	}
 	int index = 0;
 	for(;;)
 	{	StructDemandeSortie demande;
 		if(msgrcv(idBal, &demande, sizeof(StructDemandeSortie), 1, MSG_NOERROR) > 0)
 		{	index = getVoiturier();
	 		if(index == -1)
	 		{	cout << "Attention plus de voiturier" << endl;
	 		}
	 		else 
	 		{	if((pidVoituriers[index].numeroPlace = SortirVoiture(demande.place)) >= 0) 
		 		{		sleep(1);
		 		}
	 		}
 		}
 	}
} //----- fin de Sortie
