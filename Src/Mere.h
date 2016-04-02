/*************************************************************************
                           Mere  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Interface de la tâche <Mere> (fichier Mere.h) ---------
#if ! defined ( MERE_H )
#define MERE_H
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h>
#include <cstdlib>
#include "GestionClavier.h"
#include "Heure.h"
#include "Outils.h"
#include "Donnees.h"
#include "Entree.h"
#include "Sortie.h"    

using namespace std;
//------------------------------------------------------------------------
// Rôle de la tâche <Mere>
//	
//
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées

//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
int main();
// Mode d'emploi :
//
// Contrat :
//

void Initialisation();
// Mode d'emploi :
//
// Contrat :
//

void Moteur();
// Mode d'emploi :
//
// Contrat :
//

void Destruction();
// Mode d'emploi :
//
// Contrat :
//

#endif // MERE_H

