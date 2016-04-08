/*************************************************************************
                           Donnnees  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

#if ! defined DONNEES_H
#define DONNEES_H
#include "Outils.h"

//Structure pour boite aux lettres sortie
struct StructDemandeSortie {
	long type = 1; 
	int place;
};
typedef struct StructDemandeSortie StructDemandeSortie;

//Structure voiture
struct Voiture {
	unsigned int numeroPlaque;
	int numeroPlace;
	TypeUsager usager;
	time_t heureArrivee;
};
typedef struct Voiture voiture;

//Structure pour boite aux lettres entrée
struct StructRequete {
	long type = 1;
	Voiture voiture;
	TypeBarriere typeBarriere; 
};
typedef struct StructRequete StructRequete;

//Structure pour mémoires partagée Requetes
struct StructTabRequetes {
	long type = 1;
	StructRequete requetes[3];
};
typedef struct StructTabRequetes StructTabRequetes;

//Structure pour mémoires partagée Parking
struct StructParking {
	long type = 1;
	Voiture voitures[NB_PLACES];
};
typedef struct StructParking StructParking;
#endif
