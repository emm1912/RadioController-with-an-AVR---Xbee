# Xbee controller with Atmega8 with PCB and schematic (Using C)
Has been several times when I have a new or old project that could make use of a radio controller and I've been neglecting the opportunity and the privilege of making one.
So I started this journey with the goal of been able to reuse some old game controller and also make everything fit inside (pcb, battery, radio).
To achive this the PCB will have to have all integrated this means.... passive components and voltage regulator, the mcu (an avr atmega8), the Xbee S1 (with a base to remove it 
if necessary), and several I/O pads exposed to used them, all in a small factor so it fits inside a psx controller.
The code (the magic C code that makes all this work) is a simple implementation that let's us send all the data feched by the AVR through the Xbee S1.

## Description
The software I used to design the PCB is Altium Designer, the code of the avr was written using "Atmel studio 6" IDE (you can find the .c file inside the folder
/ATM-8-XBEE-RC/ATM-8-XBEERCAuto31/ATM-8-XBEERCAuto31.c in case you want to use another IDE) 

The structure of the project:

<code>
├── ATM-8-XBEE-RC
│   ├── ATM-8-XBEERCAuto31
│   │   ├── ATM-8-XBEERCAuto31.c
│   │   ├── ATM-8-XBEERCAuto31.cproj
│   │   └── Debug
│   │       ├── ATM-8-XBEERCAuto31.d
│   │       ├── ATM-8-XBEERCAuto31.eep
│   │       ├── ATM-8-XBEERCAuto31.elf
│   │       ├── ATM-8-XBEERCAuto31.hex
│   │       ├── ATM-8-XBEERCAuto31.lss
│   │       ├── ATM-8-XBEERCAuto31.map
│   │       ├── ATM-8-XBEERCAuto31.o
│   │       ├── ATM-8-XBEERCAuto31.srec
│   │       ├── makedep.mk
│   │       └── Makefile
│   ├── ATM-8-XBEERCAuto31.atsln
│   └── ATM-8-XBEERCAuto31.atsuo
├── RCAuto Altium
│   ├── IMPRESIONES USADAS
│   │   ├── B.xps
│   │   └── T.xps
│   ├── __Previews
│   │   ├── RCAuto.PcbDocPreview
│   │   └── RCAuto.SchDocPreview
│   ├── Project Logs for RCAuto
│   ├── RCAuto.PcbDoc
│   ├── RCAuto.PcbDoc.htm
│   ├── RCAuto.PrjPCB
│   ├── RCAuto.PrjPCBStructure
│   └── RCAuto.SchDoc
└── README.md
</code>


### Folder description

#### ATM-8-XBEE-RC:
Inside this folder you will find the project where the code for the AVR was written, keep in mind as I said already I used "Atmel Studio 6" IDE.

#### RCAuto Altium: 
The project created in Altium designer, here is the schematic and the pcb project files and inside this folder you will find the folder:

* IMPRESIONES USADAS		This are the images that I printed to transfer the PCB print onto the Copper Plate

![Screenshot](/Images/Top-Bottom.png)

I have already given you an introduction about what was the goal, let me show you the schematic that I used to design the PCB.

![Screenshot](/Images/Schematic.jpg)

As you can see starting in the upper left side we have the input of the battery following a connector for a switch then a voltage regulator a 3.3V is required due to the
voltage requirements of the Xbee S1, then we find a capacitor for filtering and a conector output to give voltage to the controller, the avr is configured with pull up
ressitors so with my controller I wired it up with all the buttons to negative and the joysticks need plus and minus, this is why we need the las connector to supple some
voltage.

###### Remeber that none of the original circuitry of the controller was used, only identify the vias that are common to all the buttons and connect them to negative and the other side direclty with an input of the avr (make notes).

Next is the avr and on the upper right hand you can see a connector, this connector is used to programm the avr, on the lower left hand is the Xbee S1.
The Xbee S1 doesn't need any extra configuration if you don't want to mess with extra difficulty just know that you need 2 and whatever you send to it's tx and rx pins 
will be transmitted or received on the other (this only works if there are no other Xbees near, in this case you will need to dive deeper into Xbee configuration).

This is how the finished PCB looks like

![Screenshot](/Images/PCB.jpg)

As you might see we avoided making 90° angles with the traces to avoid interference problems, the four corner points are used for us the DIY fellas that will have to 
align this transfer print onto the copper. Beyond this is pretty much straight forward.

Components list:

* (1) 2mm SMD 102 resistor
* (1) 3mm Electrolytic capacitor 2.2µF
* (1) 4mm Electrolytic capacitor 2.2µF
* (1) LF33CV voltage regulator
* (1) MLF Atmega 8
* (1) Xbee S1
* Headers
* Cable
* Regular equipment for soldering and DIY PCB making
* Multimeter (not required but recommended)


### Software description (how the logic works) 

As I have already said the IDE I used was "Atmel Studio 6", making use of C language and the 4 ADC channels that the Atmega 8 has, also I used a watchdog to reset the
controller if we loose connection with the target for safety. 

The logic flow of the program is the following:
Note. All the data send and received is a character (char).

**Controller on:**

1. Controller sends --> "9"		This would be to tell the other avr that we will start sending data
2. Target respond   -->"10"		This would tell us that the other avr (or whatever may be) received the data and is responding to us with the expected data "10"

* We enter the for loop, this has a sequence that the variable "x" is monitoring (at the beginning is zero)



* x = 0
3. Controller sends --> "1"		Indicating that we will send Data1 (channel O of the adc)
4. Target respond   -->"11"		Indicating that we are waiting for Data1

5. Controller sends --> Data1		This would send the values of the adc channel zero
6. Target respond   -->"12"		Indicating that we fetch Data1 (x = 1)



* x = 1
7. Controller sends --> "2"		Indicating that we will send Data2 (channel 1 of the adc)
8. Target respond   -->"13"		Indicating that we are waiting for Data2

9. Controller sends --> Data2		This would send the values of the adc channel one
10. Target respond   -->"14"		Indicating that we fetch Data2 (x = 2)



* x = 2
11. Controller sends --> "3"		Indicating that we will send Data3 (channel 2 of the adc)
12. Target respond   -->"15"		Indicating that we are waiting for Data3

13. Controller sends --> Data3		This would send the values of the adc channel two
14. Target respond   -->"16"		Indicating that we fetch Data3 (x = 3)



* x = 3
15. Controller sends --> "4"		Indicating that we will send Data4 (channel 3 of the adc)
16. Target respond   -->"17"		Indicating that we are waiting for Data4

17. Controller sends --> Data4		This would send the values of the adc channel three
18. Target respond   -->"18"		Indicating that we fetch Data4 (x = 4)



* x = 4
19. Controller sends --> "5"		Indicating that we will send Data5 (Port B)
20. Target respond   -->"19"		Indicating that we are waiting for Data5

21. Controller sends --> Data5		This would send the values of the Port B
22. Target respond   -->"20"		Indicating that we fetch Data5 (x = 0)
* x = 0

##### At this point you have all the pieces you need, now you can make the pcb, programm the avr and decide how to wire your particular controller.

---------------------------------

Here my finished PCB with already some cables attached to it

![Screenshot](/Images/finished2.jpg)  ![Screenshot](/Images/finished1.jpg)
