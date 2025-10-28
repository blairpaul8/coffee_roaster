import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("roast_20251027_193245.csv")
plt.plot(df["time_s"], df["temp_F"])
plt.title("Roast Profile")
plt.xlabel("Time (s)")
plt.ylabel("Temperature (°F)")
plt.show()
