/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Réalisation de la tâche <GestionClavier> (fichier GestionClavier.cpp) ---

/////////////////////////////////////////////////////////////////  INCLUDE
//-------------------------------------------------------- Include système

//------------------------------------------------------ Include personnel
#include "GestionClavier.h"

///////////////////////////////////////////////////////////////////  PRIVE
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//---------------------------------------------------- Variables statiques
static int idBalEntrees[3]; 
static int idBalSortie;
static unsigned int numeroPlaque;
//------------------------------------------------------ Fonctions privées
static void SignalDestruction ( int noSignal )
// Algorithme :
{
	exit(0);
} //----- fin de SignalDestruction

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques



void GestionClavier(int *_idBalEntrees, int _idBalSortie)
// Algorithme :
{
	//Création du handler et assignation
	struct sigaction action;
	action.sa_handler = SignalDestruction;
	sigemptyset ( &action.sa_mask );
	action.sa_flags = 0;
	sigaction( SIGUSR2, &action, NULL );
	for(int i=0;i<3;i++) 
	{	idBalEntrees[i] = _idBalEntrees[i];
	}
	numeroPlaque = 0;
	idBalSortie = _idBalSortie;
	//Attente infini
	for( ; ; ) 
	{	Menu();
	}
} //----- fin de GestionClavier

void Commande(char code, unsigned int valeur)
// Algorithme :
{
	if(code == 'E' && valeur == 0) {
		exit(0);
	} else if(code == 'P') {
		StructRequete req;
		req.voiture.numeroPlaque = numeroPlaque;
		req.voiture.usager = PROF;
		req.voiture.heureArrivee = time(NULL);
		if(valeur == 1) {
			req.typeBarriere = PROF_BLAISE_PASCAL;			
		} else {
			req.typeBarriere = ENTREE_GASTON_BERGER;
		}
		if(msgsnd(idBalEntrees[valeur - 1],&req,sizeof(StructRequete), 0) < 0)
			cout << "Error clavier" << endl;
		numeroPlaque = (numeroPlaque+1) % 999;
	} else if(code == 'A') {
		StructRequete req;
		req.voiture.numeroPlaque = numeroPlaque;
		req.voiture.usager = AUTRE;
		req.voiture.heureArrivee = time(NULL);
		if(valeur == 1) {
			req.typeBarriere = AUTRE_BLAISE_PASCAL;
		} else {
			req.typeBarriere = ENTREE_GASTON_BERGER;
		}
		if(msgsnd(idBalEntrees[valeur - 1],&req,sizeof(StructRequete), 0) < 0)
			cout << "Error clavier" << endl;
		numeroPlaque = (numeroPlaque+1) % 999;
	} else if(code == 'S') {
		StructDemandeSortie demande;
		demande.place = valeur;
		if(msgsnd(idBalSortie,&demande,sizeof(StructDemandeSortie), 0) < 0)
			cout << "Error clavier" << endl;
	}
} //----- fin de Commande
