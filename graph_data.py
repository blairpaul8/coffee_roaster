import sys
import matplotlib.pyplot as plt

num_seconds = 0
temp_list = []
seconds_list = []
for line in sys.stdin:
    num_seconds += 1
    line = line.strip()
    if not line:
        break

    line_parts = line.split()
    temp = line_parts[1]
    if temp == "On.":
        continue
    if temp == "Off":
        continue
    temp_list.append(float(temp))
    seconds_list.append(num_seconds)

plt.plot(seconds_list, temp_list)
plt.ylabel("Temperature")
plt.xlabel("Seconds")
plt.show()
