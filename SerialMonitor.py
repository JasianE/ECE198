import serial
import time
serial_port = serial.Serial('COM3',9600, timeout = 1)
time.sleep(3)
file_path = "data.txt"
with open(file_path, "a") as file:
    while True:
        if serial_port.in_waiting > 0:
            # Read one line from the serial port
            line = serial_port.readline().decode('ascii', errors='ignore').strip()    
            f.write(line + '\n')
            f.flush()        