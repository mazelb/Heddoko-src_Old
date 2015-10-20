/*
 * common.h
 *
 * Created: 9/21/2015 8:34:24 AM
 *  Author: sean
 * @brief: Includes all the generic values used by all files
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define VERSION "V0.9"

#define ALL_INTERRUPT_MASK  0xffffffff
#define TRUE 1
#define FALSE 0
typedef enum 
{
	STATUS_PASS = 0,
	STATUS_FAIL = 1,
	STATUS_EOF = 2 //end of file, used in getChar	
}status_t;

//TASK Dependent defines
//Higher priority means higher priority
#define TASK_MAIN_STACK_SIZE				   (4096/sizeof(portSTACK_TYPE))
#define TASK_MAIN_STACK_PRIORITY               (tskIDLE_PRIORITY + 2)
#define TASK_STATE_MACHINE_STACK_SIZE          (3000/sizeof(portSTACK_TYPE))
#define TASK_STATE_MACHINE_PRIORITY            (tskIDLE_PRIORITY + 3)
#define TASK_SD_CARD_WRITE_STACK_SIZE          (2048/sizeof(portSTACK_TYPE))
#define TASK_SD_CARD_WRITE_PRIORITY            (tskIDLE_PRIORITY + 4)
#define TASK_FABSENSE_STACK_SIZE               (1024/sizeof(portSTACK_TYPE))
#define TASK_FABSENSE_PRIORITY				   (tskIDLE_PRIORITY + 5)
#define TASK_QUINTIC_STACK_SIZE                (3000/sizeof(portSTACK_TYPE))
#define TASK_QUINTIC_PRIORITY				   (tskIDLE_PRIORITY + 5)
#define TASK_SERIAL_RECEIVE_STACK_SIZE         (2500/sizeof(portSTACK_TYPE))
#define TASK_SERIAL_RECEIVE_PRIORITY		   (tskIDLE_PRIORITY + 6)
#define TASK_DATA_HANDLER_STACK_SIZE           (3000/sizeof(portSTACK_TYPE))
#define TASK_DATA_HANDLER_PRIORITY             (tskIDLE_PRIORITY + 7)

#define TASK_IMU_INIT_STACK_SIZE			   (3000/sizeof(portSTACK_TYPE))
#define TASK_IMU_INIT_PRIORITY				   (tskIDLE_PRIORITY + 7)

#define WAKEUP_DELAY	1000


#endif /* COMMON_H_ */