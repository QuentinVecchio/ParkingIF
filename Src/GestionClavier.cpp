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

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques



void GestionClavier(int *_idBalEntrees, int _idBalSortie)
// Algorithme :
{
	for(unsigned int i=0;i<NB_PLACES;i++) 
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
			if(msgsnd(idBalEntrees[0],&req,sizeof(StructRequete), 0) < 0)
				Afficher(TypeZone::MESSAGE, "Erreur commande");		
		} else {
			req.typeBarriere = ENTREE_GASTON_BERGER;
			if(msgsnd(idBalEntrees[2],&req,sizeof(StructRequete), 0) < 0)
				Afficher(TypeZone::MESSAGE, "Erreur commande");		
		}
		numeroPlaque = (numeroPlaque+1) % 999;
	} else if(code == 'A') {
		StructRequete req;
		req.voiture.numeroPlaque = numeroPlaque;
		req.voiture.usager = AUTRE;
		req.voiture.heureArrivee = time(NULL);
		if(valeur == 1) {
			req.typeBarriere = AUTRE_BLAISE_PASCAL;
			if(msgsnd(idBalEntrees[1],&req,sizeof(StructRequete), 0) < 0)
				Afficher(TypeZone::MESSAGE, "Erreur commande");		
		} else {
			req.typeBarriere = ENTREE_GASTON_BERGER;
			if(msgsnd(idBalEntrees[2],&req,sizeof(StructRequete), 0) < 0)
				Afficher(TypeZone::MESSAGE, "Erreur commande");		
		}
		numeroPlaque = (numeroPlaque+1) % 999;
	} else if(code == 'S') {
		StructDemandeSortie demande;
		demande.place = valeur;
		if(msgsnd(idBalSortie,&demande,sizeof(StructDemandeSortie), 0) < 0)
			Afficher(TypeZone::MESSAGE, "Erreur commande");		
	}
} //----- fin de Commande
