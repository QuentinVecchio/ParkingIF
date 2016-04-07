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
// Rôle de la tâche <Sortie>
//	La tâche sortie gère la sortie des voitures du parking
//	La tâche gère également les requêtes des entrées
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
// Mode d'emploi :
//
// Contrat :
//

void GereRequete();
// Mode d'emploi :
//
// Contrat :
//

#endif // Sortie_H

