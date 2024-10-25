import datetime
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
import imaplib
import smtplib
from threading import Thread
import urllib.request
import serial
from matplotlib import pyplot as plt
import numpy as np
import requests

# Serial communication parameters
PORT = 'COM6'
BAUD_RATE = 9600

# ThingSpeak channel and API keys for data exchange
CHANNEL_ID = '2704092'
API_KEY_WRITE = 'Y49AZNA2XYKBTOQ6'
API_KEY_READ = 'PI11FA38B673BURI'

# URLs for reading and writing operations
BASE_URL = ' https://api.thingspeak.com/'
WRITE_URL = '{}update?api_key={}'.format(BASE_URL, API_KEY_WRITE)
READ_CHANNEL_URL = '{}channels/{}/feeds.json?api_key={}'.format(BASE_URL, CHANNEL_ID, API_KEY_READ)
READ_FIELD1_URL = '{}channels/{}/fields/{}.json?api_key={}&results{}'.format(BASE_URL, CHANNEL_ID, 1, API_KEY_READ, 10)
READ_FIELD2_URL = '{}channels/{}/fields/{}.json?api_key={}&results{}'.format(BASE_URL, CHANNEL_ID, 2, API_KEY_READ, 10)

READ_FIELD3_URL = '{}channels/{}/fields/{}.json?api_key={}&results{}'.format(BASE_URL, CHANNEL_ID, 3, API_KEY_READ, 10)
READ_FIELD4_URL = '{}channels/{}/fields/{}.json?api_key={}&results{}'.format(BASE_URL, CHANNEL_ID, 4, API_KEY_READ, 10)
READ_FIELD5_URL = '{}channels/{}/fields/{}.json?api_key={}&results{}'.format(BASE_URL, CHANNEL_ID, 5, API_KEY_READ, 10)

temperature = requests.get(READ_FIELD1_URL)
illumination = requests.get(READ_FIELD2_URL)
motionCounter = requests.get(READ_FIELD3_URL)
securedStateTimer = requests.get(READ_FIELD4_URL)
autoStateTimer = requests.get(READ_FIELD5_URL)

dataJsonTemperature = temperature.json()
dataJsonIllumination = illumination.json()
dataJsonMotion = motionCounter.json()
dataJsonSecuredStateTimer = securedStateTimer.json()
dataJsonAutoStateTimer = autoStateTimer.json()

#Extract temperature
feeds = dataJsonTemperature["feeds"]
temperatureArray = []
for x in feeds:
    x = float(x["field1"])
    temperatureArray.append(x)

#Extract illumination
feeds_illum = dataJsonIllumination["feeds"]
illuminationArray = []
for x in feeds_illum:
    x = float(x["field2"])
    illuminationArray.append(x)

#Extract motion counter
feeds_motion_counter = dataJsonMotion["feeds"]
motionCounterArray = []
for x in feeds_motion_counter:
    x = float(x["field3"])
    motionCounterArray.append(x)

#Extract secured state timer
feeds_secured_state_timer = dataJsonSecuredStateTimer["feeds"]
securedStateTimerArray = []
for x in feeds_secured_state_timer:
    x = float(x["field4"])
    securedStateTimerArray.append(x)

#Extract auto state timer
feeds_auto_state_timer = dataJsonAutoStateTimer["feeds"]
autoStateTimerArray = []
for x in feeds_auto_state_timer: 
    x = float(x["field5"])
    autoStateTimerArray.append(x)

# Buffer for accumulating serial data
dataBuffer = []

# Process data from Arduino
def processData(dataBuff):
  print(len(dataBuff))
  if len(dataBuff) >= 6:  # Ensure there are enough elements for a "D" packet
    processedData = {
      "temp_value": dataBuff[1],
      "ilum_value": dataBuff[2],
      "motion_counter": dataBuff[3],
      "secured_state_timer": dataBuff[4],
      "auto_state_timer": dataBuff[5]
    }
    print(f"Processed data: {processedData}")
    sendTS(processedData)
    dataBuff.clear()

# Send data to TS
def sendTS(data):
  resp = urllib.request.urlopen("{}&field1={}&field2={}&field3={}&field4={}&field5={}".format(WRITE_URL, data["temp_value"], data["ilum_value"], data["motion_counter"], data["secured_state_timer"], data["auto_state_timer"]))

def receive(serialCom):
  receviedMessage = ""
  while True:
    if serialCom.in_waiting > 0:
      receviedMessage = serialCom.readline().decode('ascii').strip()  # Read line from serial
      print(f"Received message: {receviedMessage}")

      if receviedMessage.startswith("D"):  # Check if message starts with "D"

        # Split the data by commas
        dataBuffer = receviedMessage.split(',')
        processData(dataBuffer)  # Process the "D" data
      elif receviedMessage.startswith("M"):  # Check if message starts with "M"
        sendReportMotion()  # Handle the "M" packet


def checkMail(email, serialCommunication):
   email.select('inbox')
   while True:
      retcode, responseLightOn = email.search(None, '(SUBJECT "LIGHT ON" UNSEEN)')
      retcode, responseLightOff = email.search(None, '(SUBJECT "LIGHT OFF" UNSEEN)')
      retcode, responseLightAuto = email.search(None, '(SUBJECT "LIGHT AUTO" UNSEEN)')
      retcode, responseLightSecure = email.search(None, '(SUBJECT "LIGHT SECURE" UNSEEN)')

      retcode, responseHvacOn = email.search(None, '(SUBJECT "HVAC ON" UNSEEN)')
      retcode, responseHvacOff = email.search(None, '(SUBJECT "HVAC OFF" UNSEEN)')
      retcode, responseCooling = email.search(None, '(SUBJECT "COOLING" UNSEEN)')
      retcode, responseHeating = email.search(None, '(SUBJECT "HEATING" UNSEEN)')


      retcode, responseSE = email.search(None, '(SUBJECT "SEND REPORT" UNSEEN)')

      # Write LIGHT ON in serial
      if len(responseLightOn[0]) > 0:
        text = "LIGHT ON"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseLightOn[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('LIGHT ON')

      # Write LIGHT OFF in serial
      if len(responseLightOff[0]) > 0:
        text = "LIGHT OFF"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseLightOff[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('LIGHT OFF')
      
      # Write LIGHT AUTO in serial
      if len(responseLightAuto[0]) > 0:
        text = "LIGHT AUTO"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseLightAuto[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('LIGHT AUTO')    
      
      # Write LIGHT SECURE in serial
      if len(responseLightSecure[0]) > 0:
        text = "LIGHT SECURE"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseLightSecure[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('LIGHT SECURE')

      # Write HVAC ON in serial
      if len(responseHvacOn[0]) > 0:
        text = "HVAC ON"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseHvacOn[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('HVAC ON')
        
      # Write HVAC OFF in serial
      if len(responseHvacOff[0]) > 0:
        text = "HVAC OFF"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseHvacOff[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('HVAC OFF')

      # Write COOLING in serial
      if len(responseCooling[0]) > 0:
        text = "COOLING"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseCooling[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('COOLING')

      # Write HEATING in serial
      if len(responseHeating[0]) > 0:
        text = "HEATING"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseHeating[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')
        print('HEATING')

      # SEND REPORT 
      if len(responseSE[0]) > 0:
        emailIds = responseSE[0].split()
        for id in emailIds:
          email.store(id, '+FLAGS', '\\Seen')
        sendReport()
        print('REPORT SENT')

# Send report when motion is detected
def sendReportMotion():
  message = MIMEMultipart()
  message["Subject"] = 'Motion Detected'

  server = smtplib.SMTP('smtp.gmail.com', 587)
  server.starttls()
  r = server.login('todorovicn2002@gmail.com', 'msrrftgrvmsmbgvm')
  r = server.sendmail('todorovicn2002@gmail.com', 'todorovicn2002@gmail.com', message.as_string())
  server.quit()
  print('Motion Report sent!')

def formatSecMin(sec):
    hours = int(sec // 3600)
    minutes = int((sec % 3600) // 60)
    seconds = int(sec % 60)
    return f"{hours:02}:{minutes:02}:{seconds:02}"

def calcTimeInState(arr):
  if len(arr) == 0:
    return 0
  totalSum = 0
  for i in range(len(arr) - 1):
    if arr[i] != arr[i + 1]:
       totalSum += arr[i]
  totalSum += arr[-1]
  return totalSum      

# For counting motion detections
def sumLocalMaxima(arr):
  if len(arr) == 0:
    return 0
 
  total_sum = 0
  maximum_found = False
 
  for i in range(len(arr) - 1):
    if arr[i] > arr[i + 1]:
      total_sum += arr[i]
      maximum_found = True
 
  # Check if no local maximum was found
  if not maximum_found:
    total_sum = arr[-1] - arr[0]
  else:
    total_sum += arr[-1]
 
  return total_sum
      
# Daily report
def sendReport():
  message = MIMEMultipart()
  message["Subject"] = 'Report from my Arduino'

  plt.ioff()
  x = np.arange(len(temperatureArray))
  plt.title("Daily temperature")
  plt.xlabel("Hours")
  plt.ylabel("Temperature (C)")
  plt.plot(x, temperatureArray)
  fileName = 'report-temperature-{}.png'.format(datetime.date.today())
  plt.savefig('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName)

  tempGraph = open('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName, 'rb')
  msgTempGraph = MIMEImage(tempGraph.read())
  tempGraph.close()
  message.attach(msgTempGraph)

  plt.clf()

  x = np.arange(len(illuminationArray))
  plt.title("Daily illumination")
  plt.xlabel("Hours")
  plt.ylabel("Illumination (%)")
  plt.plot(x, illuminationArray)
  fileName = 'report-illumination-{}.png'.format(datetime.date.today())
  plt.savefig('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName)

  illumGraph = open('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName, 'rb')
  msgIllumGraph = MIMEImage(illumGraph.read())
  illumGraph.close()
  message.attach(msgIllumGraph)

  plt.clf()

  x = np.arange(len(motionCounterArray))
  plt.title("Number of motion detections")
  plt.xlabel("Hours")
  plt.ylabel("#")
  plt.plot(x, motionCounterArray)
  fileName = 'report-motionCounterArray-{}.png'.format(datetime.date.today())
  plt.savefig('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName)

  motionCounterGraph = open('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName, 'rb')
  msgMotionCounterGraph = MIMEImage(motionCounterGraph.read())
  motionCounterGraph.close()
  message.attach(msgMotionCounterGraph)

  htmlText = """\
    <hrml>
      <head></head>
      <body>
          <h1>Daily report on {}</h1>
          <p>
              The minimum daily temperature was: <strong>{:.2f}</strong> C and the maximum was
              <strong>{:.2f}</strong> C and the average was <strong>{:.2f}</strong> C. 
          </p>
          <p>
              The minimum daily illumination was: <strong>{:.2f}</strong> % and the maximum was
              <strong>{:.2f}</strong> % and the average was <strong>{:.2f}</strong> %. 
          </p>
          <p>
              Total number of motion detections today : <strong>{:.2f}</strong> 
          </p>
          <p>
              Duration how long home secure mode was Turned ON : <strong>{}</strong>
          </p>
          <p>
              Duration how long light auto mode was Turned ON : <strong>{}</strong>
          </p>
      </body>
    </html>
    """.format(datetime.date.today(), np.min(temperatureArray), np.max(temperatureArray), np.average(temperatureArray), np.min(illuminationArray), np.max(illuminationArray), np.average(illuminationArray), sumLocalMaxima(motionCounterArray), formatSecMin(calcTimeInState(securedStateTimerArray)), formatSecMin(calcTimeInState(autoStateTimerArray)))

  mimeText = MIMEText(htmlText, 'html')
  message.attach(mimeText)

  server = smtplib.SMTP('smtp.gmail.com', 587)
  server.starttls()
  r = server.login('todorovicn2002@gmail.com', 'msrrftgrvmsmbgvm')
  r = server.sendmail('todorovicn2002@gmail.com', 'todorovicn2002@gmail.com', message.as_string())
  server.quit()
  print('Daily Report sent!')


# Initialize serial comm
serialCommunication = serial.Serial(PORT, BAUD_RATE)

# Email login
email = imaplib.IMAP4_SSL('imap.gmail.com')
email.login('todorovicn2002@gmail.com', 'msrrftgrvmsmbgvm') # app password

# Check email thread
checkEmailThread = Thread(target=checkMail, args=(email, serialCommunication))
checkEmailThread.start()

# Start a thread to receive the data from the Arduino
receivingThread = Thread(target=receive, args=(serialCommunication,))
receivingThread.start()