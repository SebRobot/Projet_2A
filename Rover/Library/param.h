/*  Projet Rover
Auteurs: Solana Yoann        IENAC 12L1
         
Date: Juin 2014

Description: Fichier param.h
	Macro pour un fonctionnmeent dégradé ou faire des tests
			Activation IMU
			Utilisation de telnet pour tester la communication
			
	Affichage d'information pour le debug sur:
			la propulsion
			la gestion d'un client
			
	Affichage d'information: 
			du type message
	        du type message recue
	        du type message envoyé
	        sur le thread de propulsion
	        
	Macro pour définir des constantes
*/


#ifndef PARAM_H
#define PARMA_H


#define I2C_OK
//#define IMU
//#define TELNET

// Macro pour le DEBUG
#define DBG_PROP
#define DBG_CONNECTION_HANDLER

// Macro pour l'affichage d'information
#define THREAD_PROP // Afficher des information du tread "threadProp"
#define DSPL_MSG          // Display all message
#define DSPL_INFO_MAIN
#ifdef DSPL_MSG
	#define DSPL_RCV_MSG  // Display received message
	#define DSPL_SND_MSG  // Display sended message
#endif

// Macro pour définir des constantes
#define MAX_THRESHOLD_TENS  11.5
#define MIN_THRESHOLD_TENS  10.5
#define SEC_DSPL_TENSION       1 // Display tension of battery every minute
#define T_INFOS             5000 // Périod 500ms for send infos
#define DIST_MIN_SONAR        20 // In centimeter
#define TIME_BLINK_SONAR     500 // In milisecond

#endif // PARAM_H
