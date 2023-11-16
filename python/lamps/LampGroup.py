 



class LampGroup:
    def __init__(self):
        self.group = []

    def add(self, lamp):
        self.group.append(lamp)

    def turnOn(self):
        for lamp in self.group:
            lamp.turnOn()

    def turnOff(self):
        for lamp in self.group:
            lamp.turnOff()

    def turnOff(self):
        for lamp in self.group:
            lamp.turnOff()

    def turnOff(self):
        for lamp in self.group:
            lamp.turnOff()

    def changeColor(self, r, g, b):
        for lamp in self.group:
            lamp.changeColor(r, g, b)

    def changeBrightness(self, value):
        for lamp in self.group:
            lamp.changeBrightness(value)

    def stop(self):
        for lamp in self.group:
            lamp.stop()

    def __del__(self):
        for lamp in self.group:
            lamp.stop()
