/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Quentin Vecchio
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
#include "Donnees.h"
#include "Outils.h"
#include <sys/msg.h>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

using namespace std;
//------------------------------------------------------------- Constantes

//------------------------------------------------------------------ Types

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void GestionClavier(int * BalEntreesId, int BalSortieId);
//	Paramètres : 
//		BalEntreesId : tableau de taille NB_BARRIERES_ENTREE contenant les id des BàL d'entrée
//		BalSortieId : contient l'id de la BàL de sortie
// Mode d'emploi :
//	Permet de lancer la tâche GestionClavier
// Contrat :
//

void Commande(char code, unsigned int valeur);
//	Paramètres : 
//		code : type de la requete
//		valeur : Numéro de la porte
// Mode d'emploi :
//	Fonction qui permet de mettre dans la bonne Bal la requete, ou permet de lancer la fin de l'application
// Contrat :
//

#endif // GestionClavier_H

