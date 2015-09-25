/*
 * common.h
 *
 * Created: 9/21/2015 8:34:24 AM
 *  Author: sean
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define ALL_INTERRUPT_MASK  0xffffffff
#define TRUE 1
#define FALSE 0
typedef enum 
{
	STATUS_PASS = 0,
	STATUS_FAIL = 1,
	STATUS_EOF = 2 //end of file, used in getChar	
}status_t;



#endif /* COMMON_H_ */