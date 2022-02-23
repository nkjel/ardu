import sys
import serial
import time
import os
from serial.tools import list_ports


from PyQt5.QtWidgets import QApplication, QWidget, QDesktopWidget, QHBoxLayout, QVBoxLayout, QLineEdit, QTextEdit, QPushButton,QComboBox
from PyQt5.QtCore import QCoreApplication, QThread, pyqtSignal


PORT = 'COM5'
BAUD = 9600

ser:serial.Serial = None
displayText:str = ''
logText:str = ''
userInput = ''
portList:list = []
portidx = 3

def getSerialPort():
	port_list = list(list_ports.grep(""))
	if len(port_list) == 0:
		raise LookupError("시리얼통신 장비를 찾을 수 없습니다.")

	indexPortListStr = [f"[{index}] Port: {port.device}, Description: {port.description}"
						   for index, port in enumerate(port_list)]
	return (indexPortListStr)


def keyboardInterruptHandler():
    sys.exit(0)


def prepareSerial():

    global ser
    global logText
    # 시리얼 통신에 사용될 포트 연결
    ser = serial.Serial(PORT, BAUD)
    # ser = serial.Serial(selectionSerialPort(), BAUD)

    # print(ser)

    time.sleep(1) # 포트를 연결해 통신이 가능한 상태가 되는데 시간이 필요
    print('시리얼통신 준비완료')
    # 시리얼 데이터 읽어오는 쓰레드 준비 및 시작
    # thread = threading.Thread(target=read_serial, args=(ser,))
    # thread.start()

    return ser != None

class ReadSerialWorker(QThread):
    updatePrintText = pyqtSignal(str)
    
    def run(self):
        # readList = []

        while True:

            # 시리얼통신에서 읽어올수 있는 데이터가 존재할때
            # 1byte씩 읽어와 char형으로 변환해 리스트에 추가
            bytes = ser.read()
            # print(bytes)
            global displayText
            displayText += bytes.decode('utf-8')

            # 객체일 경우
            # if hasattr(obj, 'name'):
            if 'myApp' in globals():
                self.updatePrintText.emit(displayText)

            # readList.clear()

class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.flagList = {'setport':self.portSelectVaildcheck,'serial':self.serialWriteintoArduino}
        self.flag = 'setport'
        self.setPort()
        self.initUI()
    def prepare(self):
        
        if prepareSerial():
            self.readSerialworker = ReadSerialWorker()
            self.readSerialworker.updatePrintText.connect(self.updateText)
            self.readSerialworker.start()
            self.logTextEdit.append('시리얼통신 준비완료')

    def initUI(self):
        global logText

        #startbutton
        startBtn = QPushButton('Start',self)
        startBtn.released.connect(self.startReleased)

        #stopbutton
        stopBtn = QPushButton('Stop',self)
        stopBtn.released.connect(self.stopReleased)

        #clearbutton
        clearBtn = QPushButton('Clear',self)
        clearBtn.released.connect(self.clearReleased)

        #quitbutton
        quitBtn = QPushButton('Quit',self)
        quitBtn.clicked.connect(QCoreApplication.instance().quit)

        #bps setting 
        bpsSettingCombobox = QComboBox(self)
        bpsSettingCombobox.addItem('9600')
        bpsSettingCombobox.addItem('115200')
        bpsSettingCombobox.activated[str].connect(self.onActivated)

    
        #port
        portSettingCombobox = QComboBox(self)
        portSettingCombobox.addItems(self.meg)
        # portSettingCombobox.activated[str].connect(self.onActivated)
        

        #set horizon layout
        hbox = QHBoxLayout()
        hbox.addStretch(1)
        hbox.addWidget(startBtn)
        hbox.addWidget(stopBtn)
        hbox.addWidget(clearBtn)
        hbox.addWidget(quitBtn)
        hbox.addWidget(bpsSettingCombobox)
        hbox.addStretch(1)
    
        #set line edit
        self.inputLineEdit = QLineEdit()
        self.inputLineEdit.returnPressed.connect(self.onReturnPressed)

        #set data print widget 
        self.printTextEdit = QTextEdit()
        self.printTextEdit.setReadOnly(True)
        
        #set log print widget
        self.logTextEdit = QTextEdit()
        self.logTextEdit.setReadOnly(True)

        #set vertical layout
        vbox = QVBoxLayout()
        vbox.addLayout(hbox)
        vbox.addWidget(portSettingCombobox)
        vbox.addWidget(self.inputLineEdit)
        vbox.addWidget(self.printTextEdit)
        vbox.addWidget(self.logTextEdit)
        self.setLayout(vbox)

        self.setWindowTitle('시리얼 모니터')
        self.resize(400, 600)
        self.center()
        self.logTextEdit.append(logText)
        self.show()


    def center(self):
        qr = self.frameGeometry()
        cp = QDesktopWidget().availableGeometry().center()
        qr.moveCenter(cp)

        self.move(qr.topLeft())
###############################################
#시리얼 통신 전 포트 설정을 위해 사용자에게 출력하는 메세지
    def setPort(self):
        # global portidx
        # portidx = 0
        # self.inputLineEdit.setReadOnly(False)
        self.meg = getSerialPort()
        # self.logTextEdit.append('시리얼 통신 준비')
        # self.printTextEdit.setText('포트 인덱스를 입력해주세요.')
        # self.printTextEdit.append('사용 가능한 포트 목록')
        # for k in meg:
        #     self.printTextEdit.append(k)
        #     portidx = portidx +1
################################################

    def updateLog(self,text):
        self.logTextEdit.append(text)

    def updateText(self, text):
        self.printTextEdit.setText(text)

    def onReturnPressed(self):
        # global logText
        # logText += str(text)
        # self.logTextEdit.append(displayText)
        # if(self.flagList == 'serial'):
        # 포트 선택 창이면 포트 입력 필터 함수로 이동
        # 시리얼 모드면 시리얼 입력 함수로 이동 
        self.flagList[self.flag]()
        self.inputLineEdit.clear()

    def portSelectVaildcheck(self):
        global portidx
        text = self.inputLineEdit.text()
        if not str.isdigit(text):
            self.printTextEdit.append('숫자를 입력해주세요')
        elif not (0<= int(text) < portidx):
            self.printTextEdit.append('선택가능한 범위가 아닙니다.')
        else :
            self.flag = 'serial'
            self.printTextEdit.clear()
            self.prepare()
            self.inputLineEdit.setReadOnly(True)


    def serialWriteintoArduino(self):
        text = self.inputLineEdit.text()
        if ser != None:
            text = self.inputLineEdit.text()
            ser.write(text.encode())

    def startReleased(self):
        if ser != None:
            ser.write('1'.encode())
        self.updateLog('초음파 센서 시작')

    def stopReleased(self):
        if ser != None:
            ser.write('0'.encode())
        self.updateLog('초음파 센서 종료')

    def clearReleased(self):
        global displayText
        self.updateLog('데이터 출력 창 초기화')
        self.printTextEdit.clear()
        displayText = ''

    def onActivated(self,text):
        global BAUD
        self.updateLog(f'change bitrate : {text}')
        BAUD = int(text)


if __name__ == "__main__":

    portList =  getSerialPort()
    app = QApplication(sys.argv)
    myApp = MyApp() #런루프
    sys.exit(app.exec_())
    
