import serial
import csv
import struct
import time

# Open serial port
ser = serial.Serial('COM6', baudrate=9600, bytesize=8, parity=serial.PARITY_NONE, stopbits=1)

# Open CSV file for writing
with open('data.csv', 'a', newline='') as csvfile:
    csvwriter = csv.writer(csvfile)
    
    print("Listening for data...")

    # Read and write data every second
    while True:
        try:
            # Read a line of data from serial port
            data = ser.read().strip()
            if data:
                # Convert byte data to unsigned integer
                value = struct.unpack('B', data)[0]
                print("Received:", value)
                
                # Write value to CSV file
                csvwriter.writerow([value])
                csvfile.flush()  # Commit changes to the file
                print("Data written to CSV file.")
        except Exception as e:
            print("Error:", e)