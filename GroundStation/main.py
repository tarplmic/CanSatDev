import sys
from PyQt5.QtWidgets import * 
from PyQt5 import QtCore, QtGui 
from PyQt5.QtGui import * 
from PyQt5.QtCore import *
from PyQt5 import QtSerialPort
import pyqtgraph as pg
from random import uniform, randint
import serial

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

global graphsX
graphsX = [0]
global containerAltY
containerAltY = [0]
global containerBattY
containerBattY = "0.0"

#thread to grab xbee data from the serial usb port
class xbeeDataThread(QThread):
    def __init__(self):
        super().__init__()

        self.packetCount = 0
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
        while(self.xbee.canReadLine()):
            newData = self.xbee.readLine()
            newData = str(newData, 'utf-8')
            self.line += newData
        
        if(self.line != ""):
            print(self.line)
            self.line = self.line.strip()
            if(self.line.split(',')[0] == "2617"):
            #parse through and update variable arrays 
                self.parseContainerData(self.line)
                print("we got container data")
            elif(self.line == "PING_RECIEVED"):
                print("WE GOT PING_RECIEVED")
            else:
                print("not container data or ping received")
        self.line = ""

    def parseContainerData(self, line):
        global containerBattY #have to include global because I am redefining containerBattY every time (for the arrays, I am just appending, not redefining)

        line = line.split(',')
        self.packetCount += 1
        if self.packetCount > graphFrameLimit:
            graphsX.pop(0)
            containerAltY.pop(0)
        graphsX.append(self.packetCount)
        containerAltY.append(float(line[7]))
        containerBattY = line[9]
        #print(containerBattY)
        #print(line)

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
        self.createGraphs()
        self.createAltGraph()
        self.createRightButtons()
        self.createLegend()
        self.createBottomBoxes()
        self.createTitle()
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
        grid.addWidget(self.rotationGraph, 1, 1)
        grid.addWidget(self.legendWid, 1, 2, Qt.AlignCenter)
        grid.addWidget(self.gpsGraph, 2, 0)
        grid.addWidget(self.airTempGraph, 2, 1)
        grid.addWidget(self.commandWid, 2, 2)
        grid.addWidget(self.utcBox, 3, 0, Qt.AlignCenter)
        grid.addWidget(self.battBox, 3, 1, Qt.AlignCenter)
        grid.addWidget(self.mqttButt, 3, 2, Qt.AlignCenter)
        self.setLayout(grid)

    #create the altitude real time graph
    def createAltGraph(self):
        self.altitudeGraph = pg.PlotWidget()
        self.altitudeGraph.clear()
        self.altitudeGraph.setRange(yRange=[0, 200])
        self.altitudeGraph.setTitle('Altitude', **{'color': '#FFF', 'size': '14pt'})
        self.altitudeGraph.setLabels(left='Altitude (m)', bottom='Time (s)')
        pen = pg.mkPen(color=containerColor)
        self.altitudeGraph.setBackground(graphBackground)
        self.altitudeGraph.getAxis('bottom').setPen('w')
        self.altitudeGraph.getAxis('left').setPen('w')
        self.altitudePlot = self.altitudeGraph.plot(graphsX, containerAltY, pen=pen, name='Container')
        app.processEvents()

    #creates the other (currently) non-real time graphs
    def createGraphs(self):
        self.rotationGraph = pg.PlotWidget()
        self.rotationGraph.setRange(yRange=[1790, 1810])
        self.rotationGraph.setTitle('Rotation Rate', **{'color': '#FFF', 'size': '14pt'})
        self.rotationGraph.setLabels(left='Rotation Rate (°/s)', bottom='Time (s)')
        self.x = list(range(25))  # 100 time points
        self.y = [randint(1800,1804) for _ in range(25)]  # 100 data points
        self.rotationGraph.setBackground(graphBackground)
        pen = pg.mkPen(color=sp1Color)
        self.rotationGraph.plot(self.x, self.y, pen=pen)
        self.x = list(range(25))  # 100 time points
        self.y = [randint(1800,1804) for _ in range(25)]   # 100 data points
        pen = pg.mkPen(color=sp2Color)
        self.rotationGraph.getAxis('bottom').setPen('w')
        self.rotationGraph.getAxis('left').setPen('w')
        self.rotationGraph.plot(self.x, self.y, pen=pen)

        self.gpsGraph = pg.PlotWidget()
        self.gpsGraph.setTitle('GPS', **{'color': '#FFF', 'size': '14pt'})
        self.gpsGraph.setLabels(left='Longitude (°)', bottom='Latitude (°)')
        self.x = [uniform(34.79,34.80) for _ in range(25)]  # 100 time points
        self.y = [uniform(-86.7,-86.0) for _ in range(25)]  # 100 data points
        self.gpsGraph.setBackground(graphBackground)
        pen = pg.mkPen(color=containerColor)
        self.gpsGraph.getAxis('bottom').setPen('w')
        self.gpsGraph.getAxis('left').setPen('w')
        self.gpsGraph.plot(self.x, self.y, pen=pen, symbol='o')

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
        commandBox.addItems(["CX_ON", "CX_PING", "SP1_ON", "SP2_ON", "SIM_ENABLE", "SIM_ACTIVATE"])
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
        
        self.mqttButt = QPushButton("MQTT: Disabled")
        self.mqttButt.setFixedSize(100, 30)
        self.mqttButt.setStyleSheet('background-color:black; color:white; border:3px solid; border-color:grey') 
        self.mqttButt.setCheckable(True)
        self.mqttButt.toggle()
        self.mqttButt.clicked.connect(self.mqttClicked)

        commandLayout.setAlignment(Qt.AlignCenter)
        self.commandWid.setLayout(commandLayout)
    
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

    #creates title for the app
    def createTitle(self):
        self.title = QLabel('Spinister : CanSat Ground Station 2021')
        self.title.setStyleSheet("color: white")
        self.title.setFont(QFont('Arial', 15))

    #updates all of the graphs with data coming from xbee
    def updateAllGraphs(self):
        self.altitudePlot.setData(graphsX, containerAltY)
        self.battBox.children()[0].itemAt(1).widget().setText(containerBattY) #path to the battery voltage box

    def sendCommand(self):
        #print(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText()) #path to the command selected
        if(self.commandWid.children()[0].itemAt(1).widget().children()[1].currentText() == "CX_PING"):
            print('About to send ping command')
            dat = "<CMD,2617,CX,PING>"
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