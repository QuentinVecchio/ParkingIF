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
static Voiture pidVoituriers[NB_PLACES];
static int idMPRequetes;
static int idMPContenuParking;
static int idMPNbPlaces;
static int idBal;
static int idSemaphoreSynchro[NB_BARRIERES_ENTREE];
static int idSemaphoreContenuParking;
static int idSemaphoreRequete;
static int idSemaphoreNbPlaces;
static StructTabRequetes *requetes;
static StructParking *parking;
static unsigned int *nbPlaces;

//------------------------------------------------------ Fonctions privées
static int getVoiturier() 
// Algorithme :
//
{
	for(unsigned int i=0;i<NB_PLACES;i++) {
		if(pidVoituriers[i].numeroPlace == -1) 
		{	return i;
		}
	}
	return -1;
} //----- fin de getVoiturier

static bool voitureExiste(unsigned int numeroPlace)
// Algorithme :
//
{
	Voiture v;
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	while(semop(idMPContenuParking,&op,1) == -1 && errno == EINTR);
	v = parking->voitures[numeroPlace];
	op.sem_op = 1;
	semop(idMPContenuParking, &op, 1);
	return !(v.usager == TypeUsager::AUCUN);
} //----- fin de restePlace

static void GereRequete() 
// Algorithme :
//
{
	int bestChoice = -1;
	struct sembuf op;
	op.sem_num = 0;
	op.sem_op = -1;
	op.sem_flg = 0;
	while(semop(idSemaphoreRequete,&op,1) == -1 && errno == EINTR);
	Voiture v1 = requetes->requetes[0].voiture;
	Voiture v2 = requetes->requetes[1].voiture;
	Voiture v3 = requetes->requetes[2].voiture;
	if(v1.usager == PROF)
	{
		if(v3.usager == PROF && v3.heureArrivee < v1.heureArrivee)
		{	bestChoice = 2;
		}
		else
		{	bestChoice = 0;
		}
	}
	else if(v3.usager == PROF)
	{	bestChoice = 2;
	}
	else if(v2.usager == AUTRE)
	{	if(v3.usager == AUTRE && v3.heureArrivee < v2.heureArrivee)
		{	bestChoice = 2;
		}
		else
		{	bestChoice = 1;
		}
	}
	else if(v3.usager != AUCUN)
	{	bestChoice = 2;
	}
	if(bestChoice != -1) 
	{	Effacer((TypeZone)(TypeZone::REQUETE_R1 + bestChoice));
		struct sembuf op;
		op.sem_num = 0;
		op.sem_op = 1;
		op.sem_flg = 0;
		requetes->requetes[bestChoice].typeBarriere = TypeBarriere::AUCUNE;
		requetes->requetes[bestChoice].voiture = {0,0,TypeUsager::AUCUN,0};
		semop(idSemaphoreSynchro[bestChoice], &op, 1);
	}
	op.sem_op = 1;
	semop(idSemaphoreRequete, &op, 1);
	op.sem_op = -1;
	while(semop(idSemaphoreNbPlaces,&op,1) == -1 && errno == EINTR);
	*nbPlaces = *nbPlaces + 1;
	op.sem_op = 1;
	semop(idSemaphoreNbPlaces, &op, 1);
} //----- fin de GereRequete

static void SignalVoitureSortie ( int noSignal )
// Algorithme :
//
{
	int pid;
	int numeroPlace = -1;
	int statut;
	if ((pid = wait(&statut)) == -1)
	{ 	Afficher(TypeZone::MESSAGE, "Erreur wait pid" );
	}
	numeroPlace = WEXITSTATUS(statut);
	for(unsigned int i=0;i<NB_PLACES;i++) {
		if(pidVoituriers[i].numeroPlace == pid) 
		{	struct sembuf op;
			op.sem_num = 0;
			op.sem_op = -1;
			op.sem_flg = 0;
			while(semop(idSemaphoreContenuParking,&op,1) == -1 && errno == EINTR);
			Effacer((TypeZone)numeroPlace);
			AfficherSortie(parking->voitures[numeroPlace].usager, parking->voitures[numeroPlace].numeroPlaque, parking->voitures[numeroPlace].heureArrivee, time(NULL));
			parking->voitures[numeroPlace] = {0,0,TypeUsager::AUCUN,0};
			op.sem_op = 1;
			semop(idSemaphoreContenuParking, &op, 1);			
			pidVoituriers[i].numeroPlace = -1;
			GereRequete();
		}
	}
} //----- fin de SignalVoitureGaree

static void Destruction( int noSignal )
// Algorithme :
{
	for(unsigned int i=0;i<NB_PLACES;i++) {
		if(pidVoituriers[i].numeroPlace != -1) 
		{	kill(pidVoituriers[i].numeroPlace, SIGUSR2);
			waitpid(pidVoituriers[i].numeroPlace, NULL, 0);
		}
	}
	shmdt(nbPlaces);
	shmdt(requetes);
	shmdt(parking);
	exit(0);
} //Fin de Destruction

static void Initialisation(int _idBal, int* _idSemaphoreSynchro, int _idSemaphoreContenuParking, int _idSemaphoreRequete, int _idSemaphoreNbPlaces , int _idMPContenuParking, int _idMPRequete, int _idMPNbPlaces)
// Algorithme :
{
	struct sigaction action;
	action.sa_handler = Destruction;
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
	idMPNbPlaces = _idMPNbPlaces;
	idBal = _idBal;
	idSemaphoreContenuParking = _idSemaphoreContenuParking;
 	idSemaphoreRequete = _idSemaphoreRequete;
 	parking = (StructParking*) shmat(idMPContenuParking,NULL,0);
    requetes = (StructTabRequetes*) shmat(idMPRequetes,NULL,0);
    nbPlaces = (unsigned int *) shmat(idMPNbPlaces,NULL,0);
 	for(unsigned int i=0;i<NB_PLACES;i++) {
		pidVoituriers[i].numeroPlace = -1;
	}
	for(unsigned int i=0;i<NB_BARRIERES_ENTREE;i++)
	{	idSemaphoreSynchro[i] = _idSemaphoreSynchro[i];
	}
} //Fin de Initialisation


static void Moteur()
// Algorithme :
{
	int index = 0;
 	for(;;)
 	{	StructDemandeSortie demande;
 		if(msgrcv(idBal, &demande, sizeof(StructDemandeSortie), 1, MSG_NOERROR) > 0)
 		{	index = getVoiturier();
	 		if(index == -1)
	 		{	Afficher(TypeZone::MESSAGE, "Attention plus de voiturier" );
	 		}
	 		else 
	 		{	if(voitureExiste(demande.place)) 
	 			{	if((pidVoituriers[index].numeroPlace = SortirVoiture(demande.place)) < 0) 
			 		{	Afficher(TypeZone::MESSAGE, "Erreur : Impossible de sortir une voiture" );
			 		}
			 	}
			 	else
			 	{	Afficher(TypeZone::MESSAGE, "La voiture n'existe pas" );
			 	}
	 		}
 		}
 	}
} //Fin de Moteur

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void Sortie(int _idBal, int* _idSemaphoreSynchro, int _idSemaphoreContenuParking, int _idSemaphoreRequete, int _idSemaphoreNbPlaces , int _idMPContenuParking, int _idMPRequete, int _idMPNbPlaces)
// Algorithme :
//
{
	Initialisation(_idBal, _idSemaphoreSynchro, _idSemaphoreContenuParking, _idSemaphoreRequete, _idSemaphoreNbPlaces , _idMPContenuParking, _idMPRequete, _idMPNbPlaces);
 	Moteur();
} //----- fin de Sortie
