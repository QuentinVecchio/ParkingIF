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
static int idBal;
static int idSemaphoreSynchro;
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
			while(semop(idMPContenuParking,&op,1) == -1 && errno == EINTR);
			parking->voitures[numeroPlace] = pidVoituriers[i];
			op.sem_op = 1;
			semop(idMPContenuParking, &op, 1);
			AfficherPlace(numeroPlace, pidVoituriers[i].usager, pidVoituriers[i].numeroPlaque,pidVoituriers[i].heureArrivee);
			pidVoituriers[i].numeroPlace = -1;
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
	semop(idMPRequetes, &op, 1);
	//Ecriture requete dans la mémoire partagé
	op.sem_op = 1;
	semop(idMPRequetes, &op, 1);
	op.sem_op = -1;
	//Attente pour garer
	semop(idSemaphoreSynchro, &op, 1);
	int index = getVoiturier();
	if(index == -1)
	{	LanceRequete(req);
	}
	else 
	{	//On se sert de numéro place de Voiturier pour stocker temporairement le pid du voiturier
		if((pidVoituriers[index].numeroPlace = GarerVoiture(req.typeBarriere)) < 0) 
		{	LanceRequete(req);
		}
		else 
		{	
			sleep(1);
		}
	}
	op.sem_op = 1;
	semop(idSemaphoreSynchro, &op, 1);
	
} //----- fin de LanceRequete

void Entree ( int _numero, int _idBal, int _idSemaphoreSynchro, int _idSemaphoreContenuParking, int _idSemaphoreRequete , int _idMPContenuParking, int _idMPRequete )
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
	idBal = _idBal;
	idSemaphoreSynchro = _idSemaphoreSynchro;
	idSemaphoreContenuParking = _idSemaphoreContenuParking;
 	idSemaphoreRequete = _idSemaphoreRequete;
  	parking = (StructParking*) shmat(idMPContenuParking,NULL,0);
    requetes = (StructTabRequetes*) shmat(idMPRequetes,NULL,0);

 	for(int i=0;i<NB_VOITURIERS;i++) {
		pidVoituriers[i].numeroPlace = -1;
	}
 	int index = 0;
 	for(;;)
 	{	StructRequete req;
 		if(msgrcv(idBal, &req, sizeof(StructRequete), 1, MSG_NOERROR) > 0)
 		{	index = getVoiturier();
 			if(index == -1)
	 		{	cout << "Erreur, aucun voiturier" << endl;
	 		}
	 		else 
	 		{	pidVoituriers[index] = req.voiture;
	 			if((pidVoituriers[index].numeroPlace = GarerVoiture(req.typeBarriere)) < 0) 
		 		{	cout << "Erreur, aucun voiturier !!!!!!!!!!!!!!!!!" << endl;
		 			LanceRequete(req);
		 		}
		 		else 
		 		{	sleep(1);
		 		}
	 		}
 		}
 	}
} //----- fin de Entree
