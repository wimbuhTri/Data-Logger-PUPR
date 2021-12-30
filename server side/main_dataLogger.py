from multiprocessing import Process, Queue
from time import time, sleep
from random import randint
#import paho.mqtt.client as paho
from paho.mqtt import client as paho
import SpreadsheedAPI as SpreadApi
import multiprocessing

def gspread_multirowOBJ(theList):
 	SpreadApi.post_multi_Spreadsheet(theList)


def on_message(client, userdata, message):
	start = time()  
	string_telemetri = str(message.payload.decode("utf-8"))
	#timestamping and formatting incoimng telemety then put them to the queue
	queue.put(SpreadApi.formatter(string_telemetri))
 
def schadule_handler(sleep,gspread_multirowOBJ,queue,send_interval):	
	unpacked = []
	while True:
		sleep(send_interval)
		if not queue.empty() :
			print("awake")
			while not queue.empty():
				unpacked.append(queue.get())
			print("unpackedd")
			unpacked.reverse()
			gspread_multirowOBJ(unpacked)
			print(">>> pushed to Gspread")
		print("sleeping")
		unpacked.clear()


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
	
    

if __name__ == '__main__':
	try :
		"""init shaduler"""
		queue = Queue()
		send_interval = 5 #detik
		schadule_handler_process = Process(target=schadule_handler, args=(sleep,gspread_multirowOBJ,queue,send_interval))
		schadule_handler_process.start()
		"""init mqtt end point"""
		mqtt_end_point()
	except KeyboardInterrupt:
		print(3*"\n","KeyboardInterrupt catched, exiting")        
		#quit()