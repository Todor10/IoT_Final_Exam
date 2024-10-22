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

temp = requests.get(READ_FIELD1_URL)
illum = requests.get(READ_FIELD2_URL)

dataJsonT = temp.json()
dataJsonI = illum.json()

#Extract temperature
feeds = dataJsonT["feeds"]
temperature = []
for x in feeds:
    x = float(x["field1"])
    temperature.append(x)

#Extract illumination
feeds_illum = dataJsonI["feeds"]
illumination = []
for x in feeds_illum:
    x = float(x["field2"])
    illumination.append(x)

# Buffer for accumulating serial data
dataBuffer = []

# Process data from Arduino
def processData():
  if len(dataBuffer) == 2:
    processedData = {
       "temp_value": dataBuffer[0],
       "ilum_value": dataBuffer[1]
    }
    print(f"Processed data: {processedData}")
    sendTS(processedData)
    dataBuffer.clear()

# Send data to TS
def sendTS(data):
  resp = urllib.request.urlopen("{}&field1={}&field2={}".format(WRITE_URL, data["temp_value"], data["ilum_value"]))

#Receive data from the serial port
def receive(serialCom):
  receviedMessage = ""
  while True:
      if serialCom.in_waiting > 0:
          receviedMessage = serialCom.readline().decode('ascii').strip() # readline
          # print(receviedMessage)
          dataBuffer.append(receviedMessage)
          processData()


def checkMail(email, serialCommunication):
   email.select('inbox')
   while True:
      retcode, responseLightOn = email.search(None, '(SUBJECT "ON" UNSEEN)')
      retcode, responseLightOff = email.search(None, '(SUBJECT "OFF" UNSEEN)')
      retcode, responseSE = email.search(None, '(SUBJECT "SEND REPORT" UNSEEN)')

      # Write ON in serial
      if len(responseLightOn[0]) > 0:
        text = "LIGHT ON"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseLightOn[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')

      # Write OFF in serial
      if len(responseLightOff[0]) > 0:
        text = "LIGHT OFF"
        serialCommunication.write(text.encode('ascii'))  
        emailIds = responseLightOff[0].split()
        for id in emailIds:
            email.store(id, '+FLAGS', '\\Seen')

      

       # SEND REPORT 
      if len(responseSE[0]) > 0:
        emailIds = responseSE[0].split()
        for id in emailIds:
          email.store(id, '+FLAGS', '\\Seen')
        sendReport()

def sendReport():
  message = MIMEMultipart()
  message["Subject"] = 'Report from my Arduino'

  plt.ioff()
  x = np.arange(len(temperature))
  # x = np.linspace(0, 23, (1*10))
  # fig = plt.figure()
  plt.title("Daily temperature")
  plt.xlabel("Hours")
  plt.ylabel("Temperature (C)")
  plt.plot(x, temperature)
  fileName = 'report-temperature-{}.png'.format(datetime.date.today())
  plt.savefig('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName)

  tempGraph = open('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName, 'rb')
  msgTempGraph = MIMEImage(tempGraph.read())
  tempGraph.close()
  message.attach(msgTempGraph)

  plt.clf()

  # plt.ioff()
  x = np.arange(len(illumination))
  # x = np.linspace(0, 23, (1*10))
  # fig = plt.figure()
  plt.title("Daily illumination")
  plt.xlabel("Hours")
  plt.ylabel("Illumination (%)")
  plt.plot(x, illumination)
  fileName = 'report-illumination-{}.png'.format(datetime.date.today())
  plt.savefig('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName)

  illumGraph = open('C:\\Users\\user\\Desktop\\Faks\\lV godina\\IoT\\Exercise\\thingSpeak\\' + fileName, 'rb')
  msgIllumGraph = MIMEImage(illumGraph.read())
  illumGraph.close()
  message.attach(msgIllumGraph)

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
      </body>
    </html>
    """.format(datetime.date.today(), np.min(temperature), np.max(temperature), np.average(temperature), np.min(illumination), np.max(illumination), np.average(illumination))

  mimeText = MIMEText(htmlText, 'html')
  message.attach(mimeText)

  server = smtplib.SMTP('smtp.gmail.com', 587)
  server.starttls()
  r = server.login('todorovicn2002@gmail.com', 'msrrftgrvmsmbgvm')
  r = server.sendmail('todorovicn2002@gmail.com', 'todorovicn2002@gmail.com', message.as_string())
  server.quit()
  print('Report sent!')


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