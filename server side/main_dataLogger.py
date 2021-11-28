from time import time
from random import randint
#import paho.mqtt.client as paho
from paho.mqtt import client as paho
import SpreadsheedAPI as SpreadApi
import multiprocessing
def on_message(client, userdata, message):
	start = time()  
	string_telemetri = str(message.payload.decode("utf-8"))
	print("spdsh try")
	#print(string_telemetri)
	SpreadApi.postSpreadsheet(str(string_telemetri))
	print("spdsh ok")
	print(string_telemetri)
 

def mqtt_end_point():
	"""Build session and init MQTT instance"""
	decission = input("Choose your warior :: \n[L]ocal \n[R]aspi \n>>")
	#create client object client1.on_publish = on_publish #assign function to callback client1.connect(broker,port) #establish connection client1.publish("house/bulb1","on")
	NamaClient = f'client-ingest{randint(0,9999)}'
	print(f'login as{NamaClient}')
	client = paho.Client(NamaClient)
	if decission.lower() == "r":            
		client.connect("10.33.109.231",1883)
		print("Connected to raspi MQTT broker")            
	else:   
		client.connect("localhost",1883)
		print("Connected to this PC MQTT broker")         

	client.on_message=on_message    #Bind function to callback
	#client.connect("localhost",1883)
	client.subscribe("/status")#subscribe
	client.subscribe("/process")
	client.subscribe("/room1")
	print(10*'\n',) #whitespace   
	client.loop_forever() #stop loop
	
    

if True :#__name__ == '__main__':
	try :
		mqtt_end_point()
	except KeyboardInterrupt:
		print(3*"\n","KeyboardInterrupt catched, exiting")        
		#quit()