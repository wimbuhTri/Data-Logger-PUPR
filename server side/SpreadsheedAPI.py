import netifaces
import datetime
import socket
import time
import gspread
from oauth2client.service_account import ServiceAccountCredentials

#init gSpread API
scope = ["https://spreadsheets.google.com/feeds",'https://www.googleapis.com/auth/spreadsheets',"https://www.googleapis.com/auth/drive.file","https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name("Spreadsheed automation-b4874085c9fd.json", scope)
client = gspread.authorize(creds)
sheet = client.open("python spreadsheed automation").sheet1  # Open the spreadhseet

def postSpreadsheet(theRow):
    global sheet
    print("inside send")
    sheet.insert_row(formatter(theRow), 2)
    print("send ok")

def post_multi_Spreadsheet(theList):
    global sheet
    print("inside send")
    sheet.insert_rows(theList, 2)
    print("send ok")


def formatter(data):
    print("try splitted")
    add_timestamp = data.split(",")
    print("splitted")
    x = datetime.datetime.now()
    add_timestamp.append(x.strftime("%X"))
    add_timestamp.append(x.strftime("%x"))
    return add_timestamp

