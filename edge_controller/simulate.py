from loguru import logger
import json
import paho.mqtt.client as mqtt
import time
import random
import logging
logging.basicConfig(level=logging.DEBUG)
from datetime import datetime

running = True


def run(client):
    while running:
        visual = {"timestamp": str(datetime.now())}

        msgType = random.randint(1, 5)

        if msgType == 1:
            # colour information
            visual["event"] = "spectrum"

            for x in range(1, 9):
                visual[str(x)] = str(random.randint(1000, 9000))

        elif msgType == 2:
            # uv/nir/clear
            visual["event"] = "nir-clear"
            visual["nir"]= str(random.randint(1000, 9000))
            visual["clr"] = str(random.randint(1000, 9000))

        elif msgType == 3:
            # uv/nir/clear
            visual["event"] = "uv"
            visual["uvi"] = str(random.randint(1000, 9000))
            visual["lux"] = str(random.randint(1000, 9000))
        elif msgType == 5:
            # gain/resoultion
            visual["event"] = "cfg"
        else:
            visual["event"] = "-"

        json_data = json.dumps(visual)
        logger.info(json_data)
        client.publish("sensors/visual", json_data)
        logger.info("published")
        time.sleep(3)


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, reason_code, properties):
    logger.info(f"Connected with result code {reason_code}")
    


mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.enable_logger()
mqttc.on_connect = on_connect

# mqttc.loop_start()
mqttc.connect("192.168.1.150", 1885, 60)
mqttc.loop_start()
run(mqttc)

