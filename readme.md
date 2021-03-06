# Matrix Clock
Documentation of electronic clock which matrix of LED's controlling by microcontroller ATmega 328P. Communication between clock and user realized by bluetooth. Room brightness determines matrix brightness.
# Processor
ATmega 328P 20MHz  
Period: 0,05 us = 50 ns  
Connected to:
* five shift registers to control the matrix (four to control X axes, one to control Y axis) 
* relay to generate acustic signals (quaters and full hours)
* DS3231 timer by I2C
* HC-06 bluetooth module by RS232
* photoresistor to brightness measurement

# Timers Math calculations

## Main timer (matrix refresh)
8bit Timer0  
Mode: CTC  
Refresh freq: 100 Hz  
Y levels of matrix: 8  
Number of brightness level: 20  
uC period of overflow: 20 000 000 / 100 / 8 / 20 =  1250  
Prescaler: 8  
OCR: 156  

## Logic PWM Timer (PWM matrix brightness)
16bit Timer1
Mode: CTC  
Refresh freq: 100 Hz  
Y levels of matrix: 8  
Number of brightness level: 20  
Number of PWM brightness levels: 3  
uC period of overflow: 20 000 000 / 100 / 8 / 20 / 3 =  417  
Prescaler: 8  
OCR: 52  

## Internal time timer (between DS3231 1Hz overflows)
8bit Timer2  
Mode: CTC  
Freq: 1kHz  
Period: 1ms  
uC period to overflow: 20 000 000 / 1 000 = 20 000  
Prescaler: 256    
OCR: 78  

## Power consumption
Basic mode: 20mA  
High brightness: 66mA  

Basic Mode + Bluetooth: 41mA  
High brightness + Bluetooth: 82mA  
