import sys
from PyQt5.QtWidgets import * 
from PyQt5 import QtCore, QtGui 
from PyQt5.QtGui import * 
from PyQt5.QtCore import *
from PyQt5 import QtSerialPort
import pyqtgraph as pg
from random import uniform, randint
import serial
import csv
import paho.mqtt.client as mqtt

#define global variables
global containerColor
containerColor = (255, 0, 0)
global sp1Color 
sp1Color = (0, 255, 0)
global sp2Color 
sp2Color = (0, 0, 255)
global graphBackground 
graphBackground = (255, 255, 255)
global entireBackground 
entireBackground = QColor('black')

global graphFrameLimit 
graphFrameLimit = 20

global SP1GraphsX
SP1GraphsX = [0]
global SP1AltY
SP1AltY = [0]
global contGraphsX
contGraphsX = [0]
global containerAltY
containerAltY = [0]
global containerBattY
containerBattY = "0.0"
global latitude
latitude = [34]
global longitude 
longitude = [-86]
global utcTimeY
utcTimeY = "00:00:00"
global serialLine
serialLine = "blank" + "\n" + "blank" + "\n" + "blank" + "\n" + "blank"
global serialLine2
serialLine2 = "blank" + "\n" + "blank" + "\n" + "blank"
global serialLine3
serialLine3 = "blank" + "\n" + "blank" + "\n" + "blank"

global simIndex 
simIndex = 0
global simIndexArray
simIndexArray = []

###MQTT SETUP###
# Define event callbacks
def on_connect(client, userdata, flags, rc):
    print("rc: " + str(rc))
def on_message(client, obj, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
def on_publish(client, obj, mid):
    print("mid: " + str(mid))
def on_subscribe(client, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))
def on_log(client, obj, level, string):
    print(string)
# setup mqtt client call backs
mqttc = mqtt.Client()
# Assign event callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
# Uncomment to enable debug messages
mqttc.on_log = on_log
topic = 'teams/1010' # team number
# Connect
mqttc.username_pw_set("t1010", "t1010pass") # made up username and password
#mqttc.connect(url.hostname, url.port) # establish connection
mqttc.connect("cansat.info", 1883)
###MQTT SETUP###


#thread to grab xbee data from the serial usb port
class xbeeDataThread(QThread):
    def __init__(self):
        super().__init__()

        self.contPacketCount = 0
        self.sp1PacketCount = 0
        self.xbee = QtSerialPort.QSerialPort()
        self.xbee.setPortName('COM3')
        self.xbee.setBaudRate(9600)
        self.line = ",,,,,,"

        if self.xbee.open(QtSerialPort.QSerialPort.ReadWrite) == False:
            return

        self.dataCollectionTimer = QTimer()
        self.dataCollectionTimer.moveToThread(self)
        self.dataCollectionTimer.timeout.connect(self.getData)

    def getData(self):
        global serialLine
        global serialLine2
        global serialLine3

        while(self.xbee.canReadLine()):
            newData = self.xbee.readLine()
            newData = str(newData, 'utf-8')
            self.line += newData
        
        if(self.line != ""):
            print(self.line)
            if(self.line.split(',')[3] == "C"):
                #PUBLISH TO MQTT
                # mqttDat = self.line.strip().split(',')
                # mqttDat.pop()
                # mqttDat.pop()
                # mqttDatStr = ""
                # for i in range(len(mqttDat)):
                #     if(i != len(mqttDat) - 1):
                #         mqttDatStr += str(mqttDat[i]) + ","
                #     else:
                #         mqttDatStr += str(mqttDat[i])

                #print(mqttDatStr)
                mqttc.publish(topic, self.line.strip()) 
                #PUBLISH TO MQTT

                serialLineArray = serialLine.split()
                serialLineArray.pop(0)
                serialLineArray.append(self.line)
                serialLine = serialLineArray[0] + "\n" + serialLineArray[1] + "\n" + serialLineArray[2] + "\n" + serialLineArray[3]
            elif(self.line.split(',')[3] == "SP1" or self.line.split(',')[3] == "SP2"):
                #print(self.line.strip())
                mqttc.publish(topic, self.line.strip()) 
                
                serialLineArray2 = serialLine2.split()
                serialLineArray2.pop(0)
                serialLineArray2.append(self.line)
                serialLine2 = serialLineArray2[0] + "\n" + serialLineArray2[1] + "\n" + serialLineArray2[2]
            else:
                serialLineArray3 = serialLine3.split()
                serialLineArray3.pop(0)
                serialLineArray3.append(self.line)
                serialLine3 = serialLineArray3[0] + "\n" + serialLineArray3[1] + "\n" + serialLineArray3[2]

            self.line = self.line.strip()
            self.line = self.line.split(',')
            if(len(self.line) >= 20):
                if(self.line[3] == "C"):
                #parse through and update variable arrays 
                    self.parseContainerData(self.line)
            elif(len(self.line) >= 7):    
                if(self.line[3] == "SP1"):
                    print("WE GOT PAYLOAD  1 DATA")
                    print(self.line)
                    self.parseSP1Data(self.line)
            for x in range(len(self.line)):
                if(x == "PING_RECIEVED"):
                    print("WE GOT PING RECIEVED")
            #else:
                #print("not container data or ping received")

        self.line = ""

    def parseSP1Data(self, line):

        #line = line.split(',')
        self.sp1PacketCount += 1
        if self.sp1PacketCount > graphFrameLimit:
            SP1GraphsX.pop(0)
            SP1AltY.pop(0)
        SP1GraphsX.append(self.sp1PacketCount)
        SP1AltY.append(float(line[4]))

        with open('Flight_2617_SP1.csv','a',newline='') as fd:
            csvData = csv.writer(fd, delimiter=",")
            csvData.writerow(line)

    def parseContainerData(self, line):
        global containerBattY #have to include global because I am redefining containerBattY every time (for the arrays, I am just appending, not redefining)
        global utcTimeY

        self.contPacketCount += 1
        if self.contPacketCount > graphFrameLimit:
            contGraphsX.pop(0)
            containerAltY.pop(0)
        contGraphsX.append(self.contPacketCount)
        containerAltY.append(float(line[7]))
        containerBattY = line[9]
        utcTimeY = line[10]
        if(float(line[12]) >= -95 and float(line[12]) <= -70 and float(line[11]) >= 25 and float(line[11]) <= 40) :
            latitude.append(float(line[11]))
            longitude.append(float(line[12]))

        with open('Flight_2617_C.csv','a',newline='') as fd:
            csvData = csv.writer(fd, delimiter=",")
            csvData.writerow(line)
        

    def run(self):
        self.dataCollectionTimer.start(50)
        loop = QEventLoop()
        loop.exec_()

#main class that display the ground station
class Display(QWidget):
    def __init__(self):
        super().__init__()

        p = self.palette()
        p.setColor(self.backgroundRole(), entireBackground)
        self.setPalette(p)
        #self.createSP1AirTempGraph()
        self.createContAltGraph()
        self.createGPSGraph()
        self.createRightButtons()
        self.createLegend()
        self.createBottomBoxes()
        self.createTitle()
        self.createSerialBox()
        self.createSP1RotationGraph()
        self.createSP1AltGraph()
        self.createSimButton()
        self.changeGraphics()

        self.dataCollectionThread = xbeeDataThread()
        self.dataCollectionThread.start()

        self.updateGraphsThread = updateGraphs(self.updateAllGraphs)
        self.updateGraphsThread.start()

    #adds all of the graphs/buttons to the layout
    def changeGraphics(self):
        grid = QGridLayout()
        grid.setSpacing(25)
        grid.setContentsMargins(40, 40, 40, 40)
        grid.addWidget(self.title, 0, 0, 1, 4, Qt.AlignCenter)
        grid.addWidget(self.altitudeGraph, 1, 0)  
        grid.addWidget(self.SP1rotationGraph, 1, 1)
        grid.addWidget(self.legendWid, 1, 2, Qt.AlignCenter)
        grid.addWidget(self.gpsGraph, 2, 0)
        grid.addWidget(self.SP1AltitudeGraph, 2, 1)
        grid.addWidget(self.commandWid, 2, 2)
        grid.addWidget(self.utcBox, 3, 0, Qt.AlignCenter)
        grid.addWidget(self.battBox, 3, 1, Qt.AlignCenter)
        grid.addWidget(self.mqttSimWid, 3, 2, Qt.AlignCenter)
        grid.addWidget(self.serialBox, 5, 0, 1, 2, Qt.AlignCenter)
        grid.addWidget(self.serialBox2, 5, 2, 1, 1, Qt.AlignCenter)
        grid.addWidget(self.serialBox3, 6, 0, 1, 4, Qt.AlignCenter)
        #grid.addWidget(self.simButt, 3, 3, 1, 1, Qt.AlignCenter)
        self.setLayout(grid)

    #create the altitude real time graph
    def createContAltGraph(self):
        self.altitudeGraph = pg.PlotWidget()
        self.altitudeGraph.clear()
        self.altitudeGraph.setRange(yRange=[0, 200])
        self.altitudeGraph.setTitle('Container Altitude', **{'color': '#000', 'size': '14pt'})
        self.altitudeGraph.setLabels(left='Altitude (m)', bottom='Time (s)')
        pen = pg.mkPen(color=containerColor)
        self.altitudeGraph.setBackground(graphBackground)
        self.altitudeGraph.getAxis('bottom').setPen('w')
        self.altitudeGraph.getAxis('left').setPen('w')
        self.contAltitudePlot = self.altitudeGraph.plot(contGraphsX, containerAltY, pen=pen, name='Container')
        app.processEvents()
    
    def createSP1AltGraph(self):
        self.SP1AltitudeGraph = pg.PlotWidget()
        self.SP1AltitudeGraph.clear()
        self.SP1AltitudeGraph.setRange(yRange=[0, 200])
        self.SP1AltitudeGraph.setTitle('SP1 Altitude', **{'color': '#000', 'size': '14pt'})
        self.SP1AltitudeGraph.setLabels(left='Altitude (m)', bottom='Time (s)')
        pen = pg.mkPen(color=sp2Color)
        self.SP1AltitudeGraph.setBackground(graphBackground)
        self.SP1AltitudeGraph.getAxis('bottom').setPen('w')
        self.SP1AltitudeGraph.getAxis('left').setPen('w')
        self.SP1AltitudePlot = self.SP1AltitudeGraph.plot(SP1GraphsX, SP1AltY, pen=pen, name='SP1')
        app.processEvents()

    def createGPSGraph(self):
        self.gpsGraph = pg.PlotWidget()
        self.gpsGraph.clear()
        self.gpsGraph.setRange(yRange=[-85, -87])
        self.gpsGraph.setTitle('GPS', **{'color': '#000', 'size': '14pt'})
        self.gpsGraph.setLabels(left='Longitude (°)', bottom='Latitude (°)')
        self.gpsGraph.setBackground(graphBackground)
        pen = pg.mkPen(color=containerColor)
        self.gpsGraph.getAxis('bottom').setPen('w')
        self.gpsGraph.getAxis('left').setPen('w')
        self.gpsGraphPlot = self.gpsGraph.plot(latitude, longitude, pen=pen, name='GPS', symbol='o')

    #creates the other (currently) non-real time graphs
    def createSP1RotationGraph(self):
        self.SP1rotationGraph = pg.PlotWidget()
        self.SP1rotationGraph.setRange(yRange=[1790, 1810])
        self.SP1rotationGraph.setTitle('Rotation Rate', **{'color': '#000', 'size': '14pt'})
        self.SP1rotationGraph.setLabels(left='Rotation Rate (°/s)', bottom='Time (s)')
        self.x = list(range(25))  # 100 time points
        self.y = [randint(1800,1804) for _ in range(25)]  # 100 data points
        self.SP1rotationGraph.setBackground(graphBackground)
        pen = pg.mkPen(color=sp1Color)
        self.SP1rotationGraph.plot(self.x, self.y, pen=pen)
        self.x = list(range(25))  # 100 time points
        self.y = [randint(1800,1804) for _ in range(25)]   # 100 data points
        pen = pg.mkPen(color=sp2Color)
        self.SP1rotationGraph.getAxis('bottom').setPen('w')
        self.SP1rotationGraph.getAxis('left').setPen('w')
        self.SP1rotationGraph.plot(self.x, self.y, pen=pen)

    def createSP1AirTempGraph(self):
        self.airTempGraph = pg.PlotWidget()
        self.airTempGraph.setRange(yRange=[26.3, 26.9])
        self.airTempGraph.setTitle('Air Temperature', **{'color': '#000', 'size': '14pt'})
        self.airTempGraph.setLabels(left='Temperature (°C)', bottom='Time (s)')
        self.x = list(range(25))  # 100 time points
        self.y = [uniform(26.6,26.7) for _ in range(25)]  # 100 data points
        self.airTempGraph.setBackground(graphBackground)
        pen = pg.mkPen(color=sp1Color)
        self.airTempGraph.plot(self.x, self.y, pen=pen)
        self.x = list(range(25))  # 100 time points
        self.y = [uniform(26.6,26.7) for _ in range(25)]  # 100 data points
        pen = pg.mkPen(color=sp2Color)
        self.airTempGraph.getAxis('bottom').setPen('w')
        self.airTempGraph.getAxis('left').setPen('w')
        self.airTempGraph.plot(self.x, self.y, pen=pen)

    #creates the mqtt and command buttons
    def createRightButtons(self):
        self.commandWid = QWidget()
        commandLayout = QVBoxLayout()

        commandLabel = QLabel('Commands')
        commandLabel.setAlignment(Qt.AlignCenter)
        commandLabel.setFont(QFont('Airal', 9))
        commandLabel.setStyleSheet('background-color:black; color:white')
        commandLayout.addWidget(commandLabel)
        
        commandBoxes = QWidget()
        commandBoxesLayout = QHBoxLayout()
        commandBox = QComboBox(self)
        commandBox.setFixedSize(120, 50)
        commandBox.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey')
        #commandBox.addItems(["CX_ON", "CX_PING", "SP1_ON", "SP2_ON", "SIM_ENABLE", "SIM_ACTIVATE", "MANUAL_RELEASE"])
        commandBox.addItems(["CX_ON", "CX_OFF", "CX_PING", "SP1_ON", "SP1_OFF", "MANUAL_RELEASE", "CLEAR_FLASH", "SIM_ENABLE", "SIM_ACTIVATE", "SIM_DISABLE"])
        commandBox.setEditable(True)
        line_edit = commandBox.lineEdit()
        line_edit.setAlignment(Qt.AlignCenter)
        line_edit.setReadOnly(True) 
        commandBoxesLayout.addWidget(commandBox)
        sendButt = QPushButton('Send Command')
        sendButt.setFixedSize(100,80)
        sendButt.clicked.connect(self.sendCommand)
        sendButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        commandBoxesLayout.addWidget(sendButt)
        commandBoxes.setLayout(commandBoxesLayout)
        commandLayout.addWidget(commandBoxes)


        self.altCorrectInput = QLineEdit()
        self.altCorrectInput.returnPressed.connect(self.altCorrectEntered)
        commandLayout.addWidget(self.altCorrectInput)

        commandLayout.setAlignment(Qt.AlignCenter)
        self.commandWid.setLayout(commandLayout)
    
    def altCorrectEntered(self):
        dat = "<CMD,2617,CX,SETALTCORRECTION," + self.altCorrectInput.text() + ">"
        print(dat)
        self.dataCollectionThread.xbee.write(dat.encode())

    #does things when the mqtt button is clicked
    def mqttClicked(self):
        if self.mqttButt.isChecked():
            self.mqttButt.setStyleSheet('background-color:grey; color:white; border:3px solid; border-color:grey') 
            self.mqttButt.setText("MQTT: Enabled")
        else:
            self.mqttButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
            self.mqttButt.setText("MQTT: Disabled")
    
    #creates the legend to the right
    def createLegend(self):
        self.legendWid = QWidget()
        legendLayout = QVBoxLayout()
        legendLabel = QLabel("Legend")
        legendLabel.setFont(QFont('Airal', 9))
        legendLabel.setStyleSheet("color: white")
        legendLabel.setAlignment(Qt.AlignCenter)
        legendLayout.addWidget(legendLabel)
        legend = QLabel(self)
        pixmap = QPixmap('image.png')
        legend.setPixmap(pixmap)
        legendLayout.addWidget(legend)
        self.legendWid.setLayout(legendLayout)

    #creates the utc and battery boxes
    def createBottomBoxes(self):
        self.utcBox = QWidget()
        utcLayout = QVBoxLayout()
        utcLabel = QLabel("UTC")
        utcLabel.setFont(QFont('Airal', 10))
        utcLabel.setAlignment(Qt.AlignCenter)
        utcLabel.setStyleSheet("color: white")
        utcLayout.addWidget(utcLabel)
        utcText = QLineEdit()
        utcText.setText("00:13:45")
        utcText.setAlignment(Qt.AlignCenter)
        utcText.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        utcLayout.addWidget(utcText)
        self.utcBox.setLayout(utcLayout)

        self.battBox = QWidget()
        battLayout = QVBoxLayout()
        battLabel = QLabel("Battery Voltage (V)")
        battLabel.setFont(QFont('Arial', 10))
        battLabel.setAlignment(Qt.AlignCenter)
        battLabel.setStyleSheet("color: white")
        battLayout.addWidget(battLabel)
        battText = QLineEdit()
        battText.setText("5.0") 
        battText.setAlignment(Qt.AlignCenter)
        battText.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey')
        battLayout.addWidget(battText)
        self.battBox.setLayout(battLayout)

    def createSerialBox(self):
        self.serialBox = QWidget()
        serialBoxLayout = QVBoxLayout()
        serialBoxLabel = QLabel("Serial Input:")
        serialBoxLabel.setFont(QFont('Arial', 10))
        serialBoxLabel.setAlignment(Qt.AlignCenter)
        serialBoxLabel.setStyleSheet("color: white")
        serialBoxLayout.addWidget(serialBoxLabel)
        serialBoxText = QTextEdit()
        serialBoxText.setAlignment(Qt.AlignCenter)
        serialBoxText.setStyleSheet('background-color:white; color:black; border:3px solid; border-color:grey')
        serialBoxText.setFixedSize(900,80)
        serialBoxLayout.addWidget(serialBoxText)
        #serialBoxLayout.addStretch()
        self.serialBox.setLayout(serialBoxLayout)

        self.serialBox2 = QWidget()
        serialBox2Layout = QVBoxLayout()
        serialBox2Label = QLabel("Serial Input:")
        serialBox2Label.setFont(QFont('Arial', 10))
        serialBox2Label.setAlignment(Qt.AlignCenter)
        serialBox2Label.setStyleSheet("color: white")
        serialBox2Layout.addWidget(serialBox2Label)
        serialBox2Text = QTextEdit()
        serialBox2Text.setAlignment(Qt.AlignCenter)
        serialBox2Text.setStyleSheet('background-color:white; color:black; border:3px solid; border-color:grey')
        serialBox2Text.setFixedSize(400,60)
        serialBox2Layout.addWidget(serialBox2Text)
        #serialBox2Layout.addStretch()
        self.serialBox2.setLayout(serialBox2Layout)

        self.serialBox3 = QWidget()
        serialBox3Layout = QVBoxLayout()
        serialBox3Label = QLabel("Serial Input:")
        serialBox3Label.setFont(QFont('Arial', 10))
        serialBox3Label.setAlignment(Qt.AlignCenter)
        serialBox3Label.setStyleSheet("color: white")
        serialBox3Layout.addWidget(serialBox3Label)
        serialBox3Text = QTextEdit()
        serialBox3Text.setAlignment(Qt.AlignCenter)
        serialBox3Text.setStyleSheet('background-color:white; color:black; border:3px solid; border-color:grey')
        serialBox3Text.setFixedSize(900,60)
        serialBox3Layout.addWidget(serialBox3Text)
        #serialBox3Layout.addStretch()
        self.serialBox3.setLayout(serialBox3Layout)

    def createSimButton(self):
        self.mqttSimWid = QWidget()
        mqttSimLayout = QHBoxLayout()

        self.mqttButt = QPushButton("MQTT: Disabled")
        self.mqttButt.setFixedSize(100, 50)
        self.mqttButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        self.mqttButt.setCheckable(True)
        self.mqttButt.toggle()
        self.mqttButt.clicked.connect(self.mqttClicked)
        mqttSimLayout.addWidget(self.mqttButt)

        self.simButt = QPushButton("Not sending SIMP")
        self.simButt.setFixedSize(150, 50)
        self.simButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        self.simButt.setCheckable(True)
        self.simButt.toggle()
        self.simButt.clicked.connect(self.simButtonClicked)
        mqttSimLayout.addWidget(self.simButt)

        mqttSimLayout.setAlignment(Qt.AlignCenter)
        self.mqttSimWid.setLayout(mqttSimLayout)

    def simButtonClicked(self):
        if self.simButt.isChecked():
            self.simButt.setStyleSheet('background-color:grey; color:white; border:3px solid; border-color:grey') 
            self.simButt.setText("Am sending SIMP")

            with open('simData.txt','r') as fd:
                for line in fd:
                    line = line.replace('$', '2617')
                    line = line.replace('\n', '')
                    line = "<" + line + ">"
                    simIndexArray.append(line)


            self.sendSimDataThread = sendSimData(self.sendSimDataFun)
            self.sendSimDataThread.start()

        else:
            self.simButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
            self.simButt.setText("Not sending SIMP")
    
    def sendSimDataFun(self):
        global simIndex
        cmdToSend = simIndexArray[simIndex]
        self.dataCollectionThread.xbee.write(cmdToSend.encode())
        simIndex += 1


    #creates title for the app
    def createTitle(self):
        self.title = QLabel('Spinister : CanSat Ground Station 2021')
        self.title.setStyleSheet("color: white")
        self.title.setFont(QFont('Arial', 15))

    #updates all of the graphs with data coming from xbee
    def updateAllGraphs(self):
        self.contAltitudePlot.setData(contGraphsX, containerAltY)
        self.SP1AltitudePlot.setData(SP1GraphsX, SP1AltY)
        self.battBox.children()[0].itemAt(1).widget().setText(containerBattY) #path to the battery voltage box
        self.gpsGraphPlot.setData(latitude, longitude)
        #print(self.utcBox.children()[0].itemAt(1).widget())
        self.utcBox.children()[0].itemAt(1).widget().setText(utcTimeY)
        self.serialBox.children()[0].itemAt(1).widget().setText(serialLine)
        self.serialBox2.children()[0].itemAt(1).widget().setText(serialLine2)
        self.serialBox3.children()[0].itemAt(1).widget().setText(serialLine3)


    def sendCommand(self):
        #print(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText()) #path to the command selected
        if(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "CX_PING"):
            print('About to send ping command')
            dat = "<CMD,2617,CX,PING>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "MANUAL_RELEASE"):
            print('About to send release command')
            dat = "<CMD,2617,CX,RELEASE>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "CLEAR_FLASH"):
            print('About to send clear flash command')
            dat = "<CMD,2617,CX,CLEARFLASH>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "CX_ON"):
            print('About to send cxon')
            dat = "<CMD,2617,CX,ON>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "CX_OFF"):
            print('About to send cxoff')
            dat = "<CMD,2617,CX,OFF>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "SIM_ENABLE"):
            print('About to send sim enable')
            dat = "<CMD,2617,SIM,ENABLE>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "SIM_ACTIVATE"):
            print('About to send sim activate')
            dat = "<CMD,2617,SIM,ACTIVATE>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "SIM_DISABLE"):
            print('About to send sim disable')
            dat = "<CMD,2617,SIM,DISABLE>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "SP1_ON"):
            print('About to send SP1_on')
            dat = "<CMD,2617,SP1X,ON>"
            self.dataCollectionThread.xbee.write(dat.encode())
        elif(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "SP1_OFF"):
            print('About to send sp1_off')
            dat = "<CMD,2617,SP1X,OFF>"
            self.dataCollectionThread.xbee.write(dat.encode())

#thread to update the graphs
class updateGraphs(QThread):
    def __init__(self, func):
        super().__init__()
        self.func = func
        self.updateGraphsTimer = QTimer()
        self.updateGraphsTimer.moveToThread(self)
        self.updateGraphsTimer.timeout.connect(self.func)

    def run(self):
        self.updateGraphsTimer.start(50)
        loop = QEventLoop()
        loop.exec_()

class sendSimData(QThread):
    def __init__(self, func):
        super().__init__()
        self.func = func
        self.sendSimDataTimer = QTimer()
        self.sendSimDataTimer.moveToThread(self)
        self.sendSimDataTimer.timeout.connect(self.func)

    def run(self):
        self.sendSimDataTimer.start(1000)
        loop = QEventLoop()
        loop.exec_()

#start the application
if __name__ == "__main__":
    app = QApplication(sys.argv)
    display = Display()
    display.show()
    
    sys.exit(app.exec_())