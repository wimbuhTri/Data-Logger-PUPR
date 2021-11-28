from time import time
from random import randint
#import paho.mqtt.client as paho
from paho.mqtt import client as paho
import multiprocessing
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS



TOKEN = "K_w-a8zb3ontSnIdNpH7gnQlgjlrXJbX8KJ04wcF1p-N0u5lafF7GEkrhznCUVxwzyKVR6awcuDT5mbMJRvBhg=="
INFLUX_URL = "https://us-east-1-1.aws.cloud2.influxdata.com"
ORG = "delapanbelasa@gmail.com"
BUCKET = "test_python"
InfluxClient = InfluxDBClient(url=INFLUX_URL, token=TOKEN)
write_api = InfluxClient.write_api(write_options=SYNCHRONOUS)


def send_influx(**kwargs):
		"""sending influx data using class method"""		
		value = kwargs["value"]
		dataToInflux = f'movement,host={"DistriBox"} activity_meter={value}'
		start = time()
		try :
			write_api.write(BUCKET, ORG, dataToInflux)							
			print(f'DB Rspd: {time()-start:.2f}s')

		except Exception:
			print("some problem in sending DB")
			print(Exception.with_traceback())
			pass
prev = time()
index = 1
is_purging = False
def on_message(client, userdata, message):
	start = time()  
	print(time()-prev)
	string_telemetri = str(message.payload.decode("utf-8"))
	print(string_telemetri)
	
	#Querying_Job = multiprocessing.Process(target=send_influx, kwargs={"value":string_telemetri})
	#Querying_Job.start()
 

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
	#client.loop_start() #start loop to process received messages
	print(10*'\n',) #whitespace   
	client.loop_forever() #stop loop
	#client.disconnect() #disconnect
    

if __name__ == '__main__':
	try :
		mqtt_end_point()
	except KeyboardInterrupt:
		print(3*"\n","KeyboardInterrupt catched, exiting")        
		#quit()