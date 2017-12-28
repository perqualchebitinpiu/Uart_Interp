# Author: Alfonso Breglia
# Email:  perqualchebitinpiu@gmail.com
# Date:   28/12/2018
# Description: Generatore di comandi per l'interprete uart
#

import serial
import time 

COM_NAME = "COM4"


def send_hello(com):
	com.flushInput()
	com.write("HELLO\n".encode())
	line = com.readline()
	if(line  == "ERR"):
		print("ERROR IN HELLO")
	
	
def send_ledon(com,stat):
	com.flushInput()
	com.write("LEDON,{:d}\n".format(stat).encode())
	line = com.readline()
	if(line  == "ERR"):
		print("ERROR IN LEDON")

def send_aread(com):
	com.flushInput()
	com.write("AREAD\n".encode())
	line = com.readline()	
	val  = -1
	if(line  == "ERR"):
		print("ERROR IN AREAD")		
	else: 
		val = int(line,16)
		
	return val
	
	
if __name__ == "__main__":
	
	# crea la seriale e setta DTR falso 
	# prima di aprire la seriale per non resettare 
	# la board
	
	com = serial.Serial()
	com.port = COM_NAME
	com.baudrate = 9600
	com.timeout = 1
	com.setDTR(False)
	com.open()
	
	
	time.sleep(0.1)
	com.flushInput()
	time.sleep(0.1)
	send_hello(com)
	time.sleep(0.1)
	send_ledon(com,1)
	time.sleep(0.1)
	send_ledon(com,0)
	time.sleep(0.1)
	send_ledon(com,1)
	time.sleep(0.1)	
	val = send_aread(com)
	print("Value: {:d}".format(val))
	time.sleep(0.1)	
	send_ledon(com,0)
	time.sleep(0.1)	


	




