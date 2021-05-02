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

#define global variables
global containerColor
containerColor = (255, 0, 0)
global sp1Color 
sp1Color = (0, 255, 0)
global sp2Color 
sp2Color = (0, 0, 255)
global graphBackground 
graphBackground = (70, 70, 70)
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
serialLine = ""

#thread to grab xbee data from the serial usb port
class xbeeDataThread(QThread):
    def __init__(self):
        super().__init__()

        self.contPacketCount = 0
        self.sp1PacketCount = 0
        self.xbee = QtSerialPort.QSerialPort()
        self.xbee.setPortName('COM3')
        self.xbee.setBaudRate(9600)
        self.line = ""

        if self.xbee.open(QtSerialPort.QSerialPort.ReadWrite) == False:
            return

        self.dataCollectionTimer = QTimer()
        self.dataCollectionTimer.moveToThread(self)
        self.dataCollectionTimer.timeout.connect(self.getData)

    def getData(self):
        global serialLine

        while(self.xbee.canReadLine()):
            newData = self.xbee.readLine()
            newData = str(newData, 'utf-8')
            self.line += newData
        
        if(self.line != ""):
            serialLine = self.line
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
                #elif(self.line == "PING_RECIEVED"):
                #    print("WE GOT PING_RECIEVED")
            # elif(self.line == "RELEASE CMD RECIEVED"):
                #    print("VERIFIED RELEASE CMD RECEIVED")
            for x in range(len(self.line)):
                if(x == "PING_RECIEVED"):
                    print("WE GOT PING RECIEVED")
            #else:
                #print("not container data or ping received")

        self.line = ""

    def parseSP1Data(self, line):
        global containerBattY #have to include global because I am redefining containerBattY every time (for the arrays, I am just appending, not redefining)
        global utcTimeY

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
        #print(containerBattY)
        #print(line)

    def parseContainerData(self, line):
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
        grid.addWidget(self.mqttButt, 3, 2, Qt.AlignCenter)
        grid.addWidget(self.serialBox, 4, 0, 1, 4, Qt.AlignCenter)
        self.setLayout(grid)

    #create the altitude real time graph
    def createContAltGraph(self):
        self.altitudeGraph = pg.PlotWidget()
        self.altitudeGraph.clear()
        self.altitudeGraph.setRange(yRange=[0, 200])
        self.altitudeGraph.setTitle('Container Altitude', **{'color': '#FFF', 'size': '14pt'})
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
        self.SP1AltitudeGraph.setTitle('SP1 Altitude', **{'color': '#FFF', 'size': '14pt'})
        self.SP1AltitudeGraph.setLabels(left='Altitude (m)', bottom='Time (s)')
        pen = pg.mkPen(color=sp1Color)
        self.SP1AltitudeGraph.setBackground(graphBackground)
        self.SP1AltitudeGraph.getAxis('bottom').setPen('w')
        self.SP1AltitudeGraph.getAxis('left').setPen('w')
        self.SP1AltitudePlot = self.SP1AltitudeGraph.plot(SP1GraphsX, SP1AltY, pen=pen, name='SP1')
        app.processEvents()

    def createGPSGraph(self):
        self.gpsGraph = pg.PlotWidget()
        self.gpsGraph.clear()
        self.gpsGraph.setRange(yRange=[-85, -87])
        self.gpsGraph.setTitle('GPS', **{'color': '#FFF', 'size': '14pt'})
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
        self.SP1rotationGraph.setTitle('Rotation Rate', **{'color': '#FFF', 'size': '14pt'})
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
        self.airTempGraph.setTitle('Air Temperature', **{'color': '#FFF', 'size': '14pt'})
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
        commandBox.addItems(["CX_ON", "CX_OFF", "CX_PING", "MANUAL_RELEASE", "CLEAR_FLASH"])
        commandBox.setEditable(True)
        line_edit = commandBox.lineEdit()
        line_edit.setAlignment(Qt.AlignCenter)
        line_edit.setReadOnly(True) 
        commandBoxesLayout.addWidget(commandBox)
        sendButt = QPushButton('Send Command')
        sendButt.clicked.connect(self.sendCommand)
        sendButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        commandBoxesLayout.addWidget(sendButt)
        commandBoxes.setLayout(commandBoxesLayout)
        commandLayout.addWidget(commandBoxes)


        self.altCorrectInput = QLineEdit()
        self.altCorrectInput.returnPressed.connect(self.altCorrectEntered)
        commandLayout.addWidget(self.altCorrectInput)
        
        self.mqttButt = QPushButton("MQTT: Disabled")
        self.mqttButt.setFixedSize(100, 30)
        self.mqttButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        self.mqttButt.setCheckable(True)
        self.mqttButt.toggle()
        self.mqttButt.clicked.connect(self.mqttClicked)

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
        serialBoxText = QLineEdit()
        serialBoxText.setAlignment(Qt.AlignCenter)
        serialBoxText.setStyleSheet('background-color:white; color:black; border:3px solid; border-color:grey')
        serialBoxText.setFixedSize(600,35)
        serialBoxLayout.addWidget(serialBoxText)
        #serialBoxLayout.addStretch()
        self.serialBox.setLayout(serialBoxLayout)


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

#start the application
if __name__ == "__main__":
    app = QApplication(sys.argv)
    display = Display()
    display.show()
    
    sys.exit(app.exec_())