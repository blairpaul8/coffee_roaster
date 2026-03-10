from flask import Flask, render_template, jsonify
import threading
import time
import spidev
import RPi.GPIO as GPIO

app = Flask(__name__)

# -------------------------
# Hardware Setup
# -------------------------
spi = spidev.SpiDev()
spi.open(0, 0)
spi.max_speed_hz = 1000000
spi.mode = 0b00

HEAT_RELAY = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(HEAT_RELAY, GPIO.OUT)
GPIO.output(HEAT_RELAY, GPIO.HIGH)  # heater OFF

# -------------------------
# Roast Configuration
# -------------------------
setpoints = {
    "pre": 360,
    "dry": 370,
    "brown": 400,
    "dev": 460,
}

roast_state = {
    "running": False,
    "stage": "idle",
    "start_time": None,
    "temp_f": 0,
    "heat_on": False,
}

lock = threading.Lock()
roast_thread = None


# -------------------------
# Hardware Functions
# -------------------------
def read_temp():
    try:
        raw = spi.xfer2([0x00, 0x00])
        value = (raw[0] << 8) | raw[1]

        if value & 0x04:
            return None

        value >>= 3
        return value * 0.25

    except:
        return None


def heater(on):
    GPIO.output(HEAT_RELAY, GPIO.LOW if on else GPIO.HIGH)

    with lock:
        roast_state["heat_on"] = on


# -------------------------
# Roast Thread
# -------------------------
def roast_loop():
    with lock:
        roast_state["start_time"] = time.time()

    while True:
        with lock:
            if not roast_state["running"]:
                break
            stage = roast_state["stage"]
            start_time = roast_state["start_time"]

        temp_c = read_temp()
        if temp_c is None:
            time.sleep(1)
            continue

        temp_f = temp_c * 9 / 5 + 32

        with lock:
            roast_state["temp_f"] = round(temp_f, 1)

        setpoint = setpoints.get(stage, 0)

        if temp_f > setpoint:
            heater(False)
        else:
            heater(True)

        elapsed = time.time() - start_time

        # Stage timing
        with lock:
            if stage == "dry" and elapsed > 120:
                roast_state["stage"] = "brown"

            elif stage == "brown" and elapsed > 240:
                roast_state["stage"] = "dev"

        time.sleep(1)

    heater(False)


# -------------------------
# Routes
# -------------------------
@app.route("/")
def home():
    return render_template("index.html")


@app.route("/preheat", methods=["POST"])
def preheat():
    global roast_thread

    with lock:
        if roast_state["running"]:
            return jsonify({"status": "already running"})

        roast_state["running"] = True
        roast_state["stage"] = "pre"

    roast_thread = threading.Thread(target=roast_loop, daemon=True)
    roast_thread.start()

    return jsonify({"status": "preheating"})


@app.route("/start", methods=["POST"])
def start():
    with lock:
        roast_state["stage"] = "dry"
        roast_state["start_time"] = time.time()

    return jsonify({"status": "roasting"})


@app.route("/stop", methods=["POST"])
def stop():
    with lock:
        roast_state["running"] = False
        roast_state["stage"] = "idle"

    heater(False)

    return jsonify({"status": "stopped"})


@app.route("/reset", methods=["POST"])
def reset():
    global roast_thread

    with lock:
        roast_state["running"] = False
        roast_state["stage"] = "idle"
        roast_state["start_time"] = None
        roast_state["temp_f"] = 0
        roast_state["heat_on"] = False

    heater(False)

    return jsonify({"status": "reset"})


@app.route("/status")
def status():
    with lock:
        return jsonify(roast_state)


# -------------------------
# Main
# -------------------------
if __name__ == "__main__":
    try:
        app.run(host="0.0.0.0", port=5000, debug=False)
    finally:
        heater(False)
        GPIO.cleanup()
