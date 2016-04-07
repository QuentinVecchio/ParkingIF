/*************************************************************************
                           Entree  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C)	Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Réalisation de la tâche <Entree> (fichier Entree.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "Entree.h"
///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static Voiture pidVoituriers[NB_VOITURIERS];
static int numero;
static int idMPRequetes;
static int idMPContenuParking;
static int idMPNbPlaces;
static int idBal;
static int idSemaphoreSynchro;
static int idSemaphoreContenuParking;
static int idSemaphoreRequete;
static int idSemaphoreNbPlace;
static StructTabRequetes *requetes;
static StructParking *parking;
static unsigned int *nbPlaces;
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

static bool restePlace()
// Algorithme :
//
{
	int nb = 0;
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	while(semop(idSemaphoreNbPlace,&op,1) == -1 && errno == EINTR);
	nb = *nbPlaces;
	op.sem_op = 1;
	semop(idSemaphoreNbPlace, &op, 1);
	return (nb > 0);
} //----- fin de restePlace

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
	shmdt(nbPlaces);
	shmdt(requetes);
	shmdt(parking);
	exit(0);
} //----- fin de SignalDestruction
//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void SignalVoitureGaree ( int noSignal )
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
		{	pidVoituriers[i].numeroPlace = numeroPlace;
			struct sembuf op;
			op.sem_num = 0;
			op.sem_op = -1;
			op.sem_flg = 0;
			while(semop(idSemaphoreNbPlace,&op,1) == -1 && errno == EINTR);
			*nbPlaces = *nbPlaces - 1;
			op.sem_op = 1;
			semop(idSemaphoreNbPlace, &op, 1);
			op.sem_op = -1;
			while(semop(idSemaphoreContenuParking,&op,1) == -1 && errno == EINTR);
			parking->voitures[numeroPlace] = pidVoituriers[i];
			op.sem_op = 1;
			semop(idSemaphoreContenuParking, &op, 1);
			AfficherPlace(numeroPlace, pidVoituriers[i].usager, pidVoituriers[i].numeroPlaque,pidVoituriers[i].heureArrivee);
			pidVoituriers[i] = {0,-1,TypeUsager::AUCUN,0};

		}
	}
} //----- fin de SignalVoitureGaree

void LanceRequete(StructRequete req) 
// Algorithme :
//
{
	AfficherRequete(req.typeBarriere, req.voiture.usager, req.voiture.heureArrivee);
    DessinerVoitureBarriere(req.typeBarriere,req.voiture.usager);
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	while(semop(idSemaphoreRequete,&op,1) == -1 && errno == EINTR);
	requetes->requetes[numero] = req;
	op.sem_op = 1;
	semop(idSemaphoreRequete, &op, 1);
	op.sem_op = -1;
	//Attente pour garer
	while(semop(idSemaphoreSynchro,&op,1) == -1 && errno == EINTR);
	int index = getVoiturier();
	if(index == -1)
	{	LanceRequete(req);
	}
	else 
	{	//On se sert de numéro place de Voiturier pour stocker temporairement le pid du voiturier
		pidVoituriers[index] = req.voiture;
		if((pidVoituriers[index].numeroPlace = GarerVoiture(req.typeBarriere)) < 0) 
		{	LanceRequete(req);
		}
		else 
		{	sleep(2);
		}
	}
	op.sem_op = 1;
	semop(idSemaphoreSynchro, &op, 1);
	
} //----- fin de LanceRequete

void Entree ( int _numero, int _idBal, int _idSemaphoreSynchro, int _idSemaphoreContenuParking, int _idSemaphoreRequete, int _idSemaphoreNbPlaces , int _idMPContenuParking, int _idMPRequete, int _idMPNbPlaces )
// Algorithme :
//
{
	struct sigaction action;
	action.sa_handler = SignalDestruction;
	sigemptyset ( &action.sa_mask );
	action.sa_flags = 0;
	sigaction( SIGUSR2, &action, NULL );
	struct sigaction actionCHLD;
	actionCHLD.sa_handler = SignalVoitureGaree;
	sigemptyset ( &actionCHLD.sa_mask );
	actionCHLD.sa_flags = 0;
	sigaction( SIGCHLD, &actionCHLD, NULL );
	numero = _numero;
 	idMPRequetes = _idMPRequete;
	idMPContenuParking = _idMPContenuParking;
	idMPNbPlaces = _idMPNbPlaces;
	idBal = _idBal;
	idSemaphoreSynchro = _idSemaphoreSynchro;
	idSemaphoreContenuParking = _idSemaphoreContenuParking;
 	idSemaphoreRequete = _idSemaphoreRequete;
  	parking = (StructParking*) shmat(idMPContenuParking,NULL,0);
    requetes = (StructTabRequetes*) shmat(idMPRequetes,NULL,0);
    nbPlaces = (unsigned int *) shmat(idMPNbPlaces,NULL,0);
 	for(int i=0;i<NB_VOITURIERS;i++) {
		pidVoituriers[i] = {0,-1,TypeUsager::AUCUN,0};
	}
 	int index = 0;
 	for(;;)
 	{	StructRequete req;
 		while(msgrcv(idBal,&req,sizeof(StructRequete),0,0) == -1 && errno == EINTR);
 		index = getVoiturier();
 		if(index == -1)
	 	{	cout << "Erreur, aucun voiturier" << endl;
	 	}
	 	else 
	 	{	//On se sert de numéro place de Voiturier pour stocker temporairement le pid du voiturier
	 		if(restePlace()) 
	 		{	pidVoituriers[index] = req.voiture;
	 			if((pidVoituriers[index].numeroPlace = GarerVoiture(req.typeBarriere)) < 0) 
	 			{	LanceRequete(req);
	 			}
	 		}
	 		else
	 		{	LanceRequete(req);
	 		}
 		}
 		sleep(2);
 	}
} //----- fin de Entree
