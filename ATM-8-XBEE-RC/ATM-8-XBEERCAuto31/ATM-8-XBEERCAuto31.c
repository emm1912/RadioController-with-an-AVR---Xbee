/*
 * ATM_8_XBEERCAuto31.c
 *
 * Created: 03/04/2023 08:49:48 p.m.
 * Author: Cordova
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL // or whatever may be your frequency
#endif

#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#include <avr/io.h>
#include <avr/wdt.h>
char ReceivedByte;

int read_adc(int channel){
	ADMUX&=0b11110000;              //Clear the older channel that was read
	ADMUX|=channel;                 //Defines the new ADC channel to be read
	ADCSRA|=(1<<ADSC);              //Starts a new conversion
	while(ADCSRA&(1<<ADSC));        //Wait until the conversion is done
	return ADCH;                    //Returns the ADC value of the chosen channel
}

int main(void)
{
	    wdt_disable();
	    DDRB=0b00000000;                                             //Configure port as IN
		PORTB=0b11111111;                                            //Configure pullup for the controller buttons (Up down left right circle triangle square and X)
		DDRD=0b10000000;                                             //PD0/PD1 Serial - PD2-PD6 Free - PD7 OUT for indicating led
		DDRC=0b00000000;                                             //PCO-PC3 used for ADC (Dualshock joysticks) - PC4 free - PC5 start button
		PORTC=0b00100000;                                            //Pullup PC5 for start button
		//Puerto C solo para ADC
	    //Configuracion ADC
	    ADMUX=_BV(ADLAR);                                            //Left
	    ADCSRA=_BV(ADEN)|_BV(ADSC)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0); //act ADC,act start convertion,Bits preescala
	    //Inicializar USART
	    UCSRB=_BV(RXEN)|_BV(TXEN);                                   //Enables RX y TX
	    UCSRC=_BV(URSEL)|_BV(UCSZ1)|_BV(UCSZ0);                      //Selects between accessing the UCSRC or the UBRRH Register -- 8 data Bits
	    UBRRH=(BAUD_PRESCALE>>8);                                    //Baud rate value
	    UBRRL=BAUD_PRESCALE;                                         //Baud rate value
		int x;
		x=0;
		PORTD=0b10000000;                                            //PD7 Indicating led is turned on to indicate we are on and we haven't recived confiration/response
		wdt_enable(WDTO_1S);                                         //Enabled watchdog if we spend to much time waiting for a response we reset (for security reasons)
		 
						 //I'm on and ready
					     do{                                                        
			                while((UCSRA&(1<<UDRE))==0){};           // Do nothing until UDR is ready for more data to be written to it
			                UDR=9;                                   // Enviar valor de "9" se prendio control
		                    while((UCSRA&(1<<RXC))==0){};            // Do nothing until data have been received and is ready to be read from UDR
			                ReceivedByte=UDR;                        // Fetch the received byte value into the variable " ByteReceived "
		                    }while(ReceivedByte!=10);                                            
		PORTD=0b00000000;                                            // PD7 Indicating led is turned off after we received confirmation/response to save battery
		
    for (;;)
    {
		wdt_reset();
 		   while(x==0){
				
				       //Intent of sending Data 1
					     do{                                                        
			                while((UCSRA&(1<<UDRE))==0){};                       // Do nothing until UDR is ready for more data to be written to it
			                UDR=1;                                               // We send value of "1" indicating that we will send data1
		                    while((UCSRA&(1<<RXC))==0){};                        // Do nothing until data have been received and is ready to be read from UDR
			                ReceivedByte=UDR;                                    // Fetch the received byte value into the variable " ByteReceived "
		                    }while(ReceivedByte!=11);	
							
						//Actually sending Data 1 
				         do{	                                                    				
					        while((UCSRA&(1<<UDRE))==0){};     				  
			                UDR=read_adc(x);                   
					        while((UCSRA&(1<<RXC))==0){};  			
					        ReceivedByte=UDR;			
							}while(ReceivedByte!=12);
						x=1;
						wdt_reset();
					   }			           
			   	
 		   while(x==1){
				
				       //Intent of sending Data 2
					     do{                                                        
			                while((UCSRA&(1<<UDRE))==0){};                     
			                UDR=2;                                             
		                    while((UCSRA&(1<<RXC))==0){};                       
			                ReceivedByte=UDR;                              
		                    }while(ReceivedByte!=13);	
							
						//Actually sending Data 2
				         do{	                                                    				
					        while((UCSRA&(1<<UDRE))==0){};     				  
			                UDR=read_adc(x);                   
					        while((UCSRA&(1<<RXC))==0){};  			
					        ReceivedByte=UDR;			
							}while(ReceivedByte!=14);
						x=2;
						wdt_reset();
					   }	
					   
 		   while(x==2){
				
				       //Intent of sending Data 3
					     do{                                                        
			                while((UCSRA&(1<<UDRE))==0){};                     
			                UDR=3;                                             
		                    while((UCSRA&(1<<RXC))==0){};                       
			                ReceivedByte=UDR;                              
		                    }while(ReceivedByte!=15);	
							
						//Actually sending Data 3
				         do{	                                                    				
					        while((UCSRA&(1<<UDRE))==0){};     				  
			                UDR=read_adc(x);                   
					        while((UCSRA&(1<<RXC))==0){};  			
					        ReceivedByte=UDR;			
							}while(ReceivedByte!=16);
						x=3;
						wdt_reset();
					   }					   			   		   
       
 		   while(x==3){
				
				       //Intent of sending Data 4
					     do{                                                        
			                while((UCSRA&(1<<UDRE))==0){};                     
			                UDR=4;                                             
		                    while((UCSRA&(1<<RXC))==0){};                       
			                ReceivedByte=UDR;                              
		                    }while(ReceivedByte!=17);	
							
						//Actually sending Data 4
				         do{	                                                    				
					        while((UCSRA&(1<<UDRE))==0){};     				  
			                UDR=read_adc(x);                   
					        while((UCSRA&(1<<RXC))==0){};  			
					        ReceivedByte=UDR;			
							}while(ReceivedByte!=18);
						x=4;
						wdt_reset();
					   }
					   
 		   while(x==4){
				
				       //Intent of sending Data 5
					     do{                                                        
			                while((UCSRA&(1<<UDRE))==0){};                     
			                UDR=5;                                             
		                    while((UCSRA&(1<<RXC))==0){};                       
			                ReceivedByte=UDR;                              
		                    }while(ReceivedByte!=19);	
							
						//Actually sending Data 5
				         do{	                                                    				
					        while((UCSRA&(1<<UDRE))==0){};     				  
			                UDR=PINB;                   
					        while((UCSRA&(1<<RXC))==0){};  			
					        ReceivedByte=UDR;			
							}while(ReceivedByte!=20);
						x=0;
						wdt_reset();
					   }					   	   
    }
}
