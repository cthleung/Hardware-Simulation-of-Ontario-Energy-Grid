import serial
import random
import time
import threading
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg

from tkinter import Tk, Canvas

ser = serial.Serial(port='COM4', baudrate=115200, timeout=1)

while True:
    rint = random.randint(1,10)
    # rint = 41
    print(rint)
    ser.write((str(rint) + '\r\n').encode())
    time.sleep(1)
