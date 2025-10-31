import sys
import matplotlib.pyplot as plt

temp = []
seconds = []

for line in sys.stdin:
    line = line.strip()

    if not line:
        break

    line_parts = line.split(",")
    if line_parts[0] == "time_s":
        continue

    temp.append(float(line_parts[1]))
    seconds.append(float(line_parts[0]))

plt.plot(seconds, temp)
plt.ylabel("Temp")
plt.xlabel("Seconds")
plt.show()
