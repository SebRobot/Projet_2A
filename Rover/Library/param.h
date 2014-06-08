



#ifndef PARAM_H
#define PARMA_H

#define I2C_OK
//#define TELNET
#define THREAD_PROP


#define DBG_PROP
#define DBG_CONNECTION_HANDLER

#define DSPL_MSG

#ifdef DSPL_MSG
	#define DSPL_RCV_MSG  // Display received message
	#define DSPL_SND_MSG  // Display sended message
#endif

#define MAX_THRESHOLD_TENS  11.5
#define MIN_THRESHOLD_TENS  10.5
#define SEC_DSPL_TENSION      60 // Display tension of battery every minute
#define T_INFOS            5000 // Périod 500ms for send infos


#endif // PARAM_H
