#!/usr/bin/env python3

import spidev
import time
import RPi.GPIO as GPIO
import sys
import select
import csv

# SPI setup for MAX6675
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 1000000
spi.mode = 0b00

# GPIO setup for heater relay
HEAT_RELAY = 27
GPIO.setmode(GPIO.BCM)
GPIO.setup(HEAT_RELAY, GPIO.OUT)

# Initial relay state
GPIO.output(HEAT_RELAY, GPIO.HIGH)
heat_on = False

# Setpoint stages
setpoints = {
    "pre": 360,
    "dry": 370,
    "brown": 400,
    "dev": 445,
}

current_stage = "pre"
low = setpoints[current_stage]

# File setup for logging
timestamp = time.strftime("%Y%m%d_%H%M%S")
csv_filename = f"roast_csv/roast_{timestamp}.csv"
csv_file = open(csv_filename, "w", newline="")
csv_writer = csv.writer(csv_file)
csv_writer.writerow(["time_s", "temp_F", "heat_on", "stage"])  # header

print(f"Logging to {csv_filename}")
print(f"Starting in stage: {current_stage} ({low} °F)")
print("Commands: stage <name>, on, off, exit")


# Functions
def read_temp():
    raw = spi.xfer2([0x00, 0x00])
    value = (raw[0] << 8) | raw[1]
    if value & 0x04:
        return None
    value >>= 3
    return value * 0.25  # °C


def toggle_relay(cmd):
    global heat_on
    if cmd == "off":
        GPIO.output(HEAT_RELAY, GPIO.HIGH)
        heat_on = False
    elif cmd == "on":
        GPIO.output(HEAT_RELAY, GPIO.LOW)
        heat_on = True
    else:
        print("Invalid relay command")


# --- Main loop ---
start_time = time.time()

try:
    while True:
        temp_c = read_temp()
        now = time.time() - start_time

        if temp_c is None:
            print("Thermocouple error")
            time.sleep(1)
            continue

        temp_f = temp_c * 9 / 5 + 32

        if temp_f < low and not heat_on:
            toggle_relay("on")
        elif temp_f > low and heat_on:
            toggle_relay("off")

        # Log data
        csv_writer.writerow(
            [round(now, 1), round(temp_f, 1), int(heat_on), current_stage]
        )
        csv_file.flush()

        # Print status
        print(
            f"{now:6.1f}s | {temp_f:6.1f} °F | Heat: {'ON ' if heat_on else 'OFF'} | Stage: {current_stage}"
        )

        if sys.stdin in select.select([sys.stdin], [], [], 0)[0]:
            cmd = sys.stdin.readline().strip().lower()
            if cmd.startswith("stage"):
                parts = cmd.split(maxsplit=1)
                if len(parts) == 2 and parts[1] in setpoints:
                    current_stage = parts[1]
                    low = setpoints[current_stage]
                    print(f"→ Switched to stage: {current_stage} ({low} °F)")
                else:
                    print("Unknown or missing stage name.")
            elif cmd == "off":
                toggle_relay("off")
                print("Heater manually turned OFF.")
            elif cmd == "on":
                toggle_relay("on")
                print("Heater manually turned ON.")
            elif cmd == "exit":
                print("Stopping roast...")
                break

        time.sleep(1)

except KeyboardInterrupt:
    print("\nExiting...")

finally:
    toggle_relay("off")
    GPIO.cleanup()
    csv_file.close()
    print(f"Data saved to {csv_filename}")
