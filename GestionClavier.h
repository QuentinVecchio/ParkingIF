/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Adrien Lepic, Marcin , Quentin Vecchio
    e-mail               : vecchioquentin@hotmail.fr
*************************************************************************/

//---------- Interface de la tâche <GestionClavier> (fichier GestionClavier.h) -------
#if ! defined ( GestionClavier_H )
#define GestionClavier_H

//------------------------------------------------------------------------
// Rôle de la tâche <GestionClavier>
//	La tâche Gestion Clavier va gérer le clavier et envoyer les commandes
//	aux taches entrée et sortie
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////  INCLUDE
//--------------------------------------------------- Interfaces utilisées
#include "Menu.h"
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques
static void SignalDestruction ( int noSignal );
// Mode d'emploi :
//
// Contrat :
//

void GestionClavier();
// Mode d'emploi :
//
// Contrat :
//

void Commande(char code, unsigned int valeur);
// Mode d'emploi :
//
// Contrat :

//

#endif // GestionClavier_H

