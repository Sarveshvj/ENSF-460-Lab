import serial
import time
import pandas as pd
import matplotlib.pyplot as plt

ser = serial.Serial("COM3", 9600, timeout=0.2)
ser.reset_input_buffer()
ser.readline()  # skip the incomplete first line.

# for 10 sec
data = []
start = time.time()

while time.time() - start < 10:
    line = ser.readline().decode().strip()
    seconds = time.time() - start

    if line.isdigit() and seconds < 10:
        adc = int(line)
        voltage = adc * 3.0 / 1023
        data.append([seconds, adc, voltage])

ser.close()

# Display and save data
df = pd.DataFrame(data, columns=["Time (s)", "ADC Value", "Voltage (V)"])
print(df.to_string(index=False))
df.to_csv("adc_data.csv", index=False)

# graph
plt.figure(1)
plt.plot(df["Time (s)"], df["ADC Value"])
plt.xlabel("Time (s)")
plt.ylabel("ADC Value")

# Voltage graph
plt.figure(2)
plt.plot(df["Time (s)"], df["Voltage (V)"])
plt.xlabel("Time (s)")
plt.ylabel("Voltage (V)")

plt.show()