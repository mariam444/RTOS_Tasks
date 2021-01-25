/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"
#include "semphr.h"

/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

	
	/*g_status_Button return status of button*/


/*Tasks Handler*/
TaskHandle_t First_Task_Handdler = NULL;
TaskHandle_t Second_Task_Handdler = NULL;

SemaphoreHandle_t xSemaphore = NULL;
signed char arr[] = "HI" ;
signed char arr2[] = "bye" ;
unsigned short s = 3 ;
unsigned short c = 4 ;
/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/


/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */

int i = 0 ;
long x ;
char d = 'd' ;



void Create_Semaphore() {

  if ( xSemaphore == NULL )  
  {
     xSemaphore = xSemaphoreCreateBinary();  
    if ( ( xSemaphore ) != NULL )
      xSemaphoreGive( ( xSemaphore ) );  
  }
}
void First_Task( void * pvParameters )
{

	for( ;; )
    {
			
				 if( xSemaphore != NULL ){
    
		  if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE ){
			for(i=0 ; i<10 ; i++){
				vSerialPutString(arr,s);
				vTaskDelay(2);
			}
			 if( xSemaphoreGive( xSemaphore ) != pdTRUE ){
            
			xSemaphoreGive( xSemaphore );
			 }
		}
	}
		 vTaskDelay(100);
    }
	}

void Second_Task( void * pvParameters )
{
	for( ;; )
    {
			 if( xSemaphore != NULL ){
    
			if( xSemaphoreTake( xSemaphore, ( TickType_t ) 10 ) == pdTRUE ){ 
			for(i=0 ; i<10 ; i++){
			vSerialPutString(arr2,c);
				vTaskDelay(2);
					
				}
			if( xSemaphoreGive( xSemaphore ) != pdTRUE ){
            
			xSemaphoreGive( xSemaphore );
			 }
			}
		}
		
			for(i=0 ; i<100000;i++){
				
			}
		
		 vTaskDelay(500);
    }
		
}



	
		



int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
  Create_Semaphore();
    /* Create Tasks here */
	
	/*Create First_Task*/
	
	   xTaskCreate(
                    First_Task,       /* Function that implements the task. */
                    "First Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                     2,/* Priority at which the task is created. */
                    &First_Task_Handdler );      /* Used to pass out the created task's handle. */
		 xTaskCreate(
                    Second_Task,       /* Function that implements the task. */
                    "Second Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                    1,   /* Priority at which the task is created. */
                    &Second_Task_Handdler );      /* Used to pass out the created task's handle. */
	
										

					
					
		

	/* Now all the tasks have been started - start the scheduler.

	NOTE : Tasks run in system mode and the scheduler runs in Supervisor mode.
	The processor MUST be in supervisor mode when vTaskStartScheduler is 
	called.  The demo applications included in the FreeRTOS.org download switch
	to supervisor mode prior to main being called.  If you are not using one of
	these demo application projects then ensure Supervisor mode is used here. */
	
	

	
	
	
	
	vTaskStartScheduler();
	
	

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);

	/* Configure GPIO */
	GPIO_init();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/


