import tinytuya
import json
import time
from datetime import datetime
import random
from threading import Thread, RLock
import queue


class Lamp:
    def __init__(self, device):
        self.device = device
        self.device.turn_on()
        self.device.set_colour(255, 255, 255, nowait=False)
        self.lock = RLock()
        self.isActive = True
        self.thread = Thread(target = self.run)
        self.taskList = queue.Queue()
        self.do = {
            "color": self._changeColor,
            "brightness": self._changeBrightness,
            "turnOn": self._turnOn,
            "turnOff": self._turnOff,
        }

        self.thread.start()
        self.id = None

    def changeColor(self, r, g, b):
        with self.lock:
            self.taskList.put(("color", [r,g,b]))

    def changeBrightness(self, value):
        with self.lock:
            self.taskList.put(("brightness", value))

    def turnOn(self):
        with self.lock:
            self.taskList.put(("turnOn", None))

    def turnOff(self):
        with self.lock:
            self.taskList.put(("turnOff", None))

    def stop(self):
        with self.lock:
            self.isActive = False
        self.thread.join()

    def run(self):
        with self.lock:
            isActive = self.isActive

        while isActive:
            with self.lock:
                isActive = self.isActive
                if not self.taskList.empty():
                    task = self.taskList.get()
                else:
                    task = None


            if task is not None:
                self.do[task[0]](task[1])
            time.sleep(0.001)


        self.device.turn_off()




    def _changeColor(self, colors):
        self.device.set_colour(colors[0], colors[1], colors[2], nowait=False)  # nowait = Go fast don't wait for response

    def _changeBrightness(self, value):
        self.device.set_brightness(value, nowait=False)

    def _turnOn(self, null):
        self.device.turn_on()

    def _turnOff(self, null):
        self.device.turn_off()



    def __del__(self):
        with self.lock:
            self.isActive = False

        self.thread.join()


def getAvailableDevices(fromCloud=False):
    if fromCloud:
        with open("cloud", "r") as f:
            cloudConfig = json.loads(f.read())
        print(cloudConfig)
        cloud = tinytuya.Cloud(**cloudConfig)
        devices = cloud.getdevices(False, oldlist={}, include_map=False)
        with open("devices", "w") as f:
            f.write(json.dumps(devices))
    else:
        with open("devices", "r") as f:
            devices = json.loads(f.read())

    realDevices = []
    for device in devices:
        # Connect to Device
        d = tinytuya.BulbDevice(
            dev_id=device["id"],
            address='Auto',  # Or set to 'Auto' to auto-discover IP address
            local_key=device["key"],
            version=3.3)
        d.set_socketPersistent(True)
        l = Lamp(d)
        l.id = device["id"]
        realDevices.append(l)

    return realDevices
