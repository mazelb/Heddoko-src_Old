/*
 * Ser.h
 *
 * Created: 9/9/2015 2:55:35 PM
 *  Author: Hriday Mehta
 */ 


#ifndef SER_H_
#define SER_H_

extern void SerialInit   			(void);
extern int  SerialGetCharUart0   	(void);
extern int  SerialPutCharUart0   	(int input_char);
extern int  SerialGetCharUart1   	(void);
extern int  SerialPutCharUart1   	(int input_char);
extern int  SerialGetCharUsart1		(void);
extern int  SerialPutCharUsart1		(int input_char);
extern int  SerialGetCharUsart0		(void);
extern int  SerialPutCharUsart0		(int input_char);
extern int  SerialGetCharUsart1nb	(void);
extern int  SerialGetCharUart1nb 	(void);
extern int  SerialGetCharUsart0nb	(void);
extern int  SerialGetCharUart0nb 	(void);
extern int 	SerialPrintUsart1 		(char str[20]);
extern int  SerialPrint				(int c, char str[20]);		//print string
extern int  SerialPrint_p			(int c, char *str);				//print string pointer
extern int  StringLength			(char *);
extern int  QnAck 					(int c);
extern int  QnScanAck				(int c);
extern int  QnConAck				(int c);

#endif /* SER_H_ */