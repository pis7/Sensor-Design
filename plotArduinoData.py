'''
Title: Obtain annd Plot Arduino Data from Serial
Author: Parker Schless (pis7)
Date: 6/1/23
Version: 1.0

References:
- PySerial library: https://pyserial.readthedocs.io/en/latest/shortintro.html
- Read serial from Arduino: http://arduino-er.blogspot.com/2015/04/python-to-plot-graph-of-serial-data.html
- Write CSV file: https://www.pythontutorial.net/python-basics/python-write-csv-file/
'''

import serial
import matplotlib.pyplot as plt
import atexit
import numpy as np
import csv
import time

# Initialize variables - CHANGE THESE AS NEEDED ------------------------------------------------------------
values = []
count = 0 # Current number of samples obtained
arduinoDelay = 100 # Delay time in msec used in Arduino code (MUST MATCH), use multiple of 100
totalSamples = 301 # (Use multiple of 10 with an extra sample added), Total time = Arduino serial delay * totalSamples
vref = 5.0 # Reference voltage used, keep at 5.0V unless explicityly stated otherwise

# Initialize serial port, CHANGE COM PORT AS NEEDED (BAUDRATE MUST MATCH THAT IN ARDUINO ALSO)
BAUDRATE = 9600 
serialArduino = serial.Serial('COM11', BAUDRATE)

# ----------------------------------------------------------------------------------------------------------

# Handles closing serial port upon termination
def exit_handler():
    serialArduino.close()
atexit.register(exit_handler)

# Delay 3 seconds before starting data collection
print('3')
time.sleep(1)
print('2')
time.sleep(1)
print('1')
time.sleep(1)
print('Starting data collection')

# Main loop
while True:
    if count < totalSamples+1: # Obtain the specified number of samples from serial (+1 because we throw out the first junk sample at the end)
        valueRead = 0 # Initialize valueRead
        try: # Catch error if anomalous reading
            valueRead = float(serialArduino.readline()[0:4]) # Choose first 4 bytes (voltage reading) of serial string, later bytes are carriage return and newline
        except: # If anomaly in serial data read, set this sample's value to the previous one to allow for data continuity
            if count != 0:
                valueRead = values[-1]
        count += 1
        values.append(valueRead) # Append to values
    else:
        values = values[1:] # Throw out first value in values - usually junk
        break


times = range(0, arduinoDelay*(len(values)), arduinoDelay) # Converts sample number to time in msec for plotting
data = np.empty(shape=(2, len(values)))
data[0] = times
data[1] = values
data = np.transpose(data)

print('Data collection completed')


# Plot Data (Optional for quick viewing) CHANGE FOLLOWING PLOT PARAMETERS AS NEEDED ----------------------------------------------------------------
plt.plot(times, values)
plt.ylim([0, vref]) # Sets y-axis range between 0V and reference voltage
plt.xticks(range(0, arduinoDelay*(len(values)+1), arduinoDelay*10*(totalSamples // 100))) # Sets appropriate ticks for x-axis
plt.xlabel('Time (msec)')
plt.ylabel('Output Voltage (V)')
plt.title('Output Voltage vs. Time for Sensor')
#plt.show() # Show plot upon running
plt.savefig('sensor.pdf') # Save plot to pdf file (CHANGE NAME AS NEEDED)

# Write data to CSV file for analysis (CHANGE NAME AS NEEDED) --------------------------------------------------------------------------------------
with open('sensor.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    for row in data:
        writer.writerow(row)