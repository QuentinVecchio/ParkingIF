/*************************************************************************
                           GestionClavier  -  description
                             -------------------
    début                : 18/03/16
    copyright            : (C) Adrien Lepic, Marcin , Quentin Vecchio
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

//------------------------------------------------------ Fonctions privées

//////////////////////////////////////////////////////////////////  PUBLIC
//---------------------------------------------------- Fonctions publiques

void SignalDestruction ( int noSignal )
{
	exit(0);
}

void GestionClavier()
// Algorithme :
{
	//Création du handler et assignation
	struct sigaction action;
	action.sa_handler = SignalDestruction;
	sigemptyset ( &action.sa_mask );
	action.sa_flags = 0;
	sigaction( SIGUSR2, &action, NULL );
	//Lancement du Menu
	Menu();
	//Attente infini
	for( ; ; ) 
	{
	}
} //----- fin de GestionClavier

void Commande(char code, unsigned int valeur)
// Algorithme :
{
	if(code == 'E' && valeur == 0) {
		kill(getppid(), SIGUSR2);
	} else if(code == 'P') {

	} else if(code == 'A') {

	} else if(code == 'S') {

	}
} //----- fin de Commande
