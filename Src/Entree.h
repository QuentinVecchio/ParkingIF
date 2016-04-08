/*************************************************************************
                           Entree  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C)  Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Interface de la tâche <Entree> (fichier Entree.h) -------
#if ! defined ( Entree_H )
#define Entree_H

//------------------------------------------------------------------------
// Rôle de la tâche <Entree>
//	La tâche entrée gère des voitures à sa barrière et les fait entrer selon des
//	contraintes de disponibilité et de privilège
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
void Entree(int numero, int idBal, int idSemaphoreSynchro, int idSemaphoreContenuParking, int idSemaphoreRequete, int idSemaphoreNbPlaces , int idMPContenuParking, int idMPRequete, int idMPNbPlaces);
// Paramtres :
//		numero : Numéro de la porte d'entrée
//		idBal : id de la BaL correspondant à la porte
//		idSemaphoreSynchro : id de la semaphore correspondant à la semaphore de synchro
//		idSemaphoreContenuParking : id de la semaphore correspondant à la semaphore d'accès à la MP Parking
//		idSemaphoreRequete : id de la semaphore correspondant à la semaphore d'accès à la MP Requete
//		idSemaphoreNbPlaces : id de la semaphore correspondant à la semaphore d'accès à la MP NbPlace
//	 	idMPContenuParking : id de la MP Parking
//		idMPRequete : id de la MP Requete
//		idMPNbPlace : id de la MP Nb Place
// Mode d'emploi :
//	Permet de lancer la tâche entrée
// Contrat :
//	Toutes les semaphores, MP et Bal doivent avoir été créé avant l'appel de cette fonction
#endif // Entree_H

