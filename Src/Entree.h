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

//------------------------------------------------------------------------
// Rôle de la tâche <Entree>
//	La tâche entrée gère des voitures à sa barrière et les fait entrer selon des
//	contraintes de disponibilité et de privilège
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
void Entree(int numero, int idBal, int idSemaphoreSynchro, int idSemaphoreContenuParking, int idSemaphoreRequete, int idSemaphoreNbPlaces , int idMPContenuParking, int idMPRequete, int idMPNbPlaces);
// Mode d'emploi :
//
// Contrat :
//

void LanceRequete();
// Mode d'emploi :
//
// Contrat :
//

#endif // Entree_H

