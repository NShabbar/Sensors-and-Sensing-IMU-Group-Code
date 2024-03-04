import serial
import os

# Define the serial port and baudrate
ser = serial.Serial('COM10', 115200)  

# Define the file paths
script_dir = os.path.dirname(__file__)
acc_file_path = os.path.join(script_dir, "Lab4AccData.txt")
mag_file_path = os.path.join(script_dir, "Lab4MagData.txt")

# Open files for writing
acc_file = open(acc_file_path, "w")
mag_file = open(mag_file_path, "w")

try:
    while True:
        line = ser.readline().decode().strip()
        if line.startswith("ACC"):
            data = line.split(": ")[1]
            acc_file.write(data + "\n")
        elif line.startswith("MAG"):
            data = line.split(": ")[1]
            mag_file.write(data + "\n")
finally:
    # Close the serial connection and files
    ser.close()
    acc_file.close()
    mag_file.close()
