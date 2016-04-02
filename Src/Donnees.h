#if ! defined DONNEES_H
#define DONNEES_H
#include "Outils.h"

#define NB_VOITURIERS 8

struct StructDemandeSortie {
	long type = 1; 
	int place;
};
typedef struct StructDemandeSortie StructDemandeSortie;

struct Voiture {
	unsigned int numeroPlaque;
	int numeroPlace;
	TypeUsager usager;
	time_t heureArrivee;
};
typedef struct Voiture voiture;

struct StructRequete {
	long type = 1;
	Voiture voiture;
	TypeBarriere typeBarriere; 
};
typedef struct StructRequete StructRequete;

struct StructTabRequetes {
	long type = 1;
	StructRequete requetes[3];
};
typedef struct StructTabRequetes StructTabRequetes;

struct StructParking {
	long type = 1;
	Voiture voitures[NB_VOITURIERS];
};
typedef struct StructParking StructParking;
#endif
