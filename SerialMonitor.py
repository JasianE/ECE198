import serial
import time
import serial.tools.list_ports

serial_port = serial.Serial('COM5', 9600, timeout=5)
time.sleep(3)
file_path = "data.txt"
with open(file_path, "a") as file:
    while True:
        if serial_port.in_waiting > 0:
            line = serial_port.readline().decode('ascii', errors='ignore').strip()
            file.write(line + '\n')
            file.flush()
