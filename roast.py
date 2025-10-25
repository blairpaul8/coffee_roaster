#!/usr/bin/env python3

import spidev
import time 
import RPi.GPIO as GPIO
import time 

## setup SPI for reading MAX6675
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 1000000
spi.mode = 0b00

## Setup GPIO PIN to control heat relay
HEAT_RELAY = 27
GPIO.setmode(GPIO.BCM)
GPIO.setup(HEAT_RELAY, GPIO.OUT)

## Set Heat to off 
GPIO.output(HEAT_RELAY, GPIO.HIGH)
heat_on = False

def read_temp():
    raw = spi.xfer2([0x00, 0x00])
    value = (raw[0] << 8) | raw[1]

    if value & 0x004:
        return None

    value >>= 3 
    temp_c = value * 0.25
    return temp_c


def toggle_relay(cmd):
    if cmd == "off":
        GPIO.output(HEAT_RELAY, GPIO.HIGH)
        heat_on = False
    elif cmd == "on":
        GPIO.output(HEAT_RELAY, GPIO.LOW)
        heat_on = True
    else:
        print("Toggle Relay Failed")



try:
    while True:
        temp = read_temp()
        
        ## Failed to get a temp reading
        if temp is None:
            print("Not Connected")

        temp_f = (temp * 9.0 / 5.0) + 32
        print(f"Temperature: {temp_f:.2f} F")

        if temp_f > 420.00 and heat_on:
            toggle_relay("off")
            print("Heater Off")
        elif temp_f < 390.00 and not heat_on:
            toggle_relay("on")
            print("Heater On.")
        

        ## May need to adjust sleep time
        time.sleep(1)
finally:

    GPIO.output(HEAT_RELAY, GPIO.HIGH)
    GPIO.cleanup()
    spi.close()



