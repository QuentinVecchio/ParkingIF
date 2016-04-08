/*************************************************************************
                           Sortie  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C)  Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Interface de la tâche <Sortie> (fichier Sortie.h) -------
#if ! defined ( Sortie_H )
#define Sortie_H

//------------------------------------------------------------------------
// Rôle de la tâche <Sortie>
//	La tâche sortie gère la sortie des voitures du parking
//	La tâche gère également les requêtes des entrées
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include <iostream>
#include "Outils.h"
#include "Donnees.h"
#include <sys/msg.h>
#include <sys/sem.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/shm.h>
#include <sys/wait.h>
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Sortie(int idBal, int* idSemaphoreSynchro, int idSemaphoreContenuParking, int idSemaphoreRequete, int idSemaphoreNbPlaces , int idMPContenuParking, int idMPRequete, int idMPNbPlaces);
// Paramètres :
//		idBal : id de la BaL de sortie 
//	 	idSemaphoreSynchro : tableau de semaphore de synchro avec l'entree
//		idSemaphoreContenuParking : id de la semaphore correspondant à la semaphore d'accès à la MP Parking
//		idSemaphoreRequete : id de la semaphore correspondant à la semaphore d'accès à la MP Requete
//		idSemaphoreNbPlaces : id de la semaphore correspondant à la semaphore d'accès à la MP NbPlace
//	 	idMPContenuParking : id de la MP Parking
//		idMPRequete : id de la MP Requete
//		idMPNbPlace : id de la MP Nb Place
// Mode d'emploi :
//	Lancer la tâche Sortie
// Contrat :
//	Toutes les semaphores, MP et Bal doivent avoir été créé avant l'appel de cette fonction

#endif // Sortie_H

