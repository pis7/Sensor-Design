'''
Title: Obtain annd Plot Arduino Data from Serial
Author: Parker Schless (pis7)
Date: 6/1/23
Version: 1.1

Summary:
Students will use this code in the Sensor Design course to obtain and plot the data (voltage output vs. time)
from their sensors using an Arduino. This code features a 3 second countdown until data collection starts
as well as the ability to change parmeters of the collection such as the delay time between samples for the Arduino,
the total number of samples taken, and the reference voltage used for the Arduino's ADC. After the data
is obtained, it can be plotted via Matplotlib's pyplot library and shown on the screen or saved to a
PDF file. More importantly, the data is written to a CSV file so the students can analyze and plot the data on
their own.

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

# Delay 3 seconds before starting data collection
print('3')
time.sleep(1)
print('2')
time.sleep(1)
print('1')
time.sleep(1)

values = [] # Array to hold values of data points obtained
count = 0 # Current number of samples obtained, keep it assigned to 0 here

# Initialize variables - CHANGE THESE AS NEEDED ------------------------------------------------------------
arduinoDelay = 100 # Delay time in msec used in Arduino code (MUST MATCH), use multiple of 100
totalSamples = 101 # (Use multiple of 10 with an extra sample added), Total time = Arduino serial delay * totalSamples
VREF = 5.0 # Reference voltage used, keep at 5.0V unless explicityly stated otherwise
SENSORNAME = 'mysensor' # Name of sensor to be used for file names
BAUDRATE = 9600 # Serial baud rate (MUST MATCH IN ARDUINO CODE)
COMPORT = 'COM11' # Serial COM Port (SEARCH GOOGLE FOR HOW TO CHECK COM PORT NUMBER ON YOUR OS)

# Note: If error saying permission denied to write to CSV file, close the CSV file before running this code again

# ----------------------------------------------------------------------------------------------------------

# Initialize serial connection
serialArduino = serial.Serial(COMPORT, BAUDRATE)
serialArduino.reset_input_buffer()

# Handles closing serial port upon termination
def exit_handler():
    serialArduino.close()
atexit.register(exit_handler)

# Main loop
print('Starting data collection')
while True:
    serialArduino.reset_input_buffer() # Flush input buffer to prevent invalid reads
    while (serialArduino.in_waiting == 0): # Wait for serial buffer to have something in it
        pass
    if count < totalSamples+1: # Obtain the specified number of samples from serial (+1 because we throw out the first junk sample at the end)
        valueRead = 0 # Initialize valueRead
        try: # Catch error if anomalous reading
            valueRead = float(serialArduino.readline()[0:4]) # Choose first 4 bytes (voltage reading) of serial string, later bytes are carriage return and newline
        except: # If anomaly in serial data read, set this sample's value to the previous one to allow for data continuity
            if count != 0:
                valueRead = values[-1]
        if count != 0: # Only print out data points after first sample taken (one we throw out)
            print('Sample: '+ str(count) + ', Voltage: ' + str(valueRead) + 'V')
        if valueRead > VREF: # If invalid value on ADC, then exit
            print('VALUE ERROR: PLEASE TRY AGAIN')
            exit()
        values.append(valueRead) # Append data point to values
        count += 1
    else:
        values = values[1:] # Throw out first value in values - usually junk
        endtime = time.time()
        break

times = range(0, arduinoDelay*(len(values)), arduinoDelay) # Converts sample number to time in msec for plotting
data = np.empty(shape=(2, len(values))) # Transpose 2D array of times and values so it is written properly to CSV file
data[0] = times
data[1] = values
data = np.transpose(data)

print('Data collection completed')


# Plot Data (Optional for quick viewing) CHANGE FOLLOWING PLOT PARAMETERS AS NEEDED ----------------------------------------
plt.plot(times, values)
plt.ylim([0, VREF]) # Sets y-axis range between 0V and reference voltage
plt.xticks(range(0, arduinoDelay*(len(values)+1), arduinoDelay*10*(totalSamples//100))) # Sets appropriate ticks for x-axis
plt.xlabel('Time (msec)')
plt.ylabel('Output Voltage (V)')
plt.title('Output Voltage vs. Time for ' + SENSORNAME)
#plt.show() # Show plot upon running (blocks creation of CSV file until closed, comment out if not needed)
plt.savefig(SENSORNAME + ' Plot.pdf') # Save plot to pdf file

# Write data to CSV file for analysis --------------------------------------------------------------------------------------
with open(SENSORNAME + ' Data.csv', 'w', newline='') as f:
    writer = csv.writer(f)
    writer.writerow(['Time (msec)', 'Voltage (V)'])
    for row in data:
        writer.writerow(row)