/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"

/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"
#include "semphr.h"
#include "queue.h"
/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )

	
/*g_status_Button return status of button*/
pinState_t g_status_Button_Current_1=0;
pinState_t g_status_Button_previous_1=0;
pinState_t g_status_Button_Current_2=0;
pinState_t g_status_Button_previous_2=0;

/*Tasks Handler*/
TaskHandle_t Button_Task_Handdler = NULL;
TaskHandle_t Led_Task_Handdler = NULL;
TaskHandle_t Led_Task_2_Handdler = NULL;
TaskHandle_t Periodic_Task_Handdler = NULL;
TaskHandle_t Control_Task_Handdler = NULL;


/*Queue Handler*/
QueueHandle_t xQueue1;


/*String send to uart*/
char rising[10] = "Rising";
char falling[10] = "falling";
char HI[10] = "Mariam";
char Arr[10];

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

/*Create Queue*/
void Setup_Queue(){
	xQueue1 = xQueueCreate( 10, sizeof( Arr) );
	
}

/*check button state*/
void Button_Task( void * pvParameters )
{
 

    for( ;; )
    {
		  g_status_Button_Current_1 = GPIO_read (PORT_0 , PIN0) ; /*read button_1 status every 1ms*/
			g_status_Button_Current_2 = GPIO_read (PORT_0 , PIN1) ; /*read button_2 status every 1ms*/
			vTaskDelay(1);
    }
}
	
/* if press button_0 : send rising with rising edge
     unpress button_0: send falling with falling edge
*/
		
void Led_Task( void * pvParameters )
{
	for( ;; )
    {
			/*just send string at edges*/
			if(g_status_Button_Current_1 != g_status_Button_previous_1){
				/*Check Button High*/
			if(g_status_Button_Current_1==PIN_IS_HIGH){
			 strcpy(Arr,rising);//send Rising at rising edge
				if( xQueue1 != 0 ){
					/* Send String .  Wait for 10 ticks for space to become
        available if necessary. */
					if( xQueueSend( xQueue1,( void * ) &Arr,( TickType_t ) 10 ) != pdPASS ){
				
				}
				}
			}
			/*Check unpress button*/
			else if(g_status_Button_Current_1==PIN_IS_LOW ){
				strcpy(Arr,falling);//send falling at rising edge
				if( xQueue1 != 0 ){
					/* Send String .  Wait for 10 ticks for space to become
        available if necessary. */
					if( xQueueSend( xQueue1, ( void * ) &Arr,( TickType_t ) 10 ) != pdPASS ){
				
				 }
				}
			}
			g_status_Button_previous_1 = g_status_Button_Current_1;
		  }
			vTaskDelay(1);
			}
			
    }
	
/* if press button_1 : send rising with rising edge
     unpress button_1: send falling with falling edge
*/
void Led_Task_2( void * pvParameters )
{
 

    for( ;; )
    {
			/*just send string at edges*/
			if(g_status_Button_Current_2 != g_status_Button_previous_2){
				/*Check Button High*/
			if(g_status_Button_Current_2==PIN_IS_HIGH){
				 strcpy(Arr,rising);//send Rising at rising edge
				if( xQueue1 != 0 ){
					/* Send String .  Wait for 10 ticks for space to become
        available if necessary. */
					if( xQueueSend( xQueue1,( void * ) &Arr,( TickType_t ) 10 ) != pdPASS ){
				
				}
				}
				
			}
			/*Check unpress button*/
			else if(g_status_Button_Current_2==PIN_IS_LOW ){
				strcpy(Arr,falling);//send falling at rising edge
				if( xQueue1 != 0 ){
					/* Send String .  Wait for 10 ticks for space to become
        available if necessary. */
					if( xQueueSend( xQueue1,( void * ) &Arr,( TickType_t ) 10 ) != pdPASS ){
				
				}
				}
			}
			g_status_Button_previous_2 = g_status_Button_Current_2;
			}
			vTaskDelay(1);
		  }
			
}
			
    
/*Task send string Periodicly to uart every 100ms*/
void Periodic_Task( void * pvParameters )
{
    for( ;; )
    {
	     strcpy(Arr,HI);
			/* Send String .  Wait for 10 ticks for space to become
        available if necessary. */
			if( xQueue1 != 0 ){
					if( xQueueSend( xQueue1,( void * ) &Arr,( TickType_t ) 10 ) != pdPASS ){
				
				                                                                         }
				                }
			vTaskDelay(100);
		}
			
}

/*control task that write in uart*/
void Control_Task( void * pvParameters )
{
  for( ;; )
    {
	   if( xQueue1 != 0 ){
			  /* Receive a message from the created queue to hold string
      .  Block for 10 ticks if a message is not immediately available.
      The value is read into string variable, so after calling
      xQueueReceive() xRxedStructure will hold a copy of string. */
			 if( xQueueReceive( xQueue1,&( Arr ),( TickType_t ) 10 ) == pdPASS )
      {
				
        vSerialPutString(Arr,10);
      }
		                    }
	vTaskDelay(1);
		}
			
 }		


int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();
	Setup_Queue();
 
    /* Create Tasks here */
	
	/*Create Button_Task*/
	
	   xTaskCreate(
                    Button_Task,       /* Function that implements the task. */
                    "Button Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                     1,/* Priority at which the task is created. */
 										&Button_Task_Handdler );      /* Used to pass out the created task's handle. */
		/*Create Led_Task*/
		 xTaskCreate(
                    Led_Task,       /* Function that implements the task. */
                    "Led Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                     1,/* Priority at which the task is created. */
                    &Led_Task_Handdler );      /* Used to pass out the created task's handle. */
		/*Create Led_Task_2*/								
		xTaskCreate(
                    Led_Task_2,       /* Function that implements the task. */
                   "Led Task_2",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                     1,/* Priority at which the task is created. */
                    &Led_Task_2_Handdler );      /* Used to pass out the created task's handle. */
			/*Create Periodic_Task*/		
			xTaskCreate(
                    Periodic_Task,       /* Function that implements the task. */
                    "Periodic Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                     1,/* Priority at which the task is created. */
                    &Periodic_Task_Handdler );      /* Used to pass out the created task's handle. */
				/*Create Control_Task*/
			xTaskCreate(
                    Control_Task,       /* Function that implements the task. */
                    "Control Task",          /* Text name for the task. */
                    100,      /* Stack size in words, not bytes. */
                    ( void * ) 0,    /* Parameter passed into the task. */
                     1,/* Priority at which the task is created. */
                    &Control_Task_Handdler );      /* Used to pass out the created task's handle. */

					
					
		

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


