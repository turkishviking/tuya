from lamps.Lamp import getAvailableDevices
from lamps.LampGroup import LampGroup
import time
from sounds.Stream import Stream
from datetime import datetime


MIN_TIME = 0.01 #secs



def now():
    return datetime.timestamp(datetime.now())

def saturate(color):
    maxidx = color.index(max(color))
    color[maxidx] = color[maxidx]*1.4
    if color[maxidx] >= 255:
        color[maxidx] = 255
    for i in range(len(color)):
        if i != maxidx:
            color[i] = color[i]*.6
    return color

if __name__ == "__main__":

    devices = getAvailableDevices()
    group = LampGroup()
    for device in devices:
        group.add(device)


    synestesy = {
        "C":[255, 255, 0],
        "C#":[0, 255, 128],
        "D":[0, 255, 255],
        "D#":[0, 128, 255],
        "E":[255, 255, 0],
        "F":[255, 0, 255],
        "F#":[255,128,255],
        "G":[255, 255, 0],
        "G#":[255,255,128],
        "A":[255, 0, 255],
        "A#":[128, 0, 255],
        "B":[255, 128, 128]
    }


    stream = Stream(display=False)
    stream.start()  # open the the stream
    oldVal = [255, 0, 0]

    tBright = now() + 0.075
    tColor = now()
    magMax = 0.
    magMin = 2.
    while 1:


        val = [0, 0, 0]
        data = stream.getChordIntensity()
        try:
            data["y"]
            #stream.fig.draw(data["y"], None, None, data["y3"])
        except Exception as e:
            pass
        #print(chords)
        for note, intensity in data["chords"].items():
            #print(note, intensity)
            for i in range(len(val)):
                if intensity >= 0.65:
                    val[i] += synestesy[note][i] * intensity

        # normalize
        maxi = max(val)
        change = 0
        newVal = [0,0,0]
        for i in range(len(val)):
            val[i] = int(val[i] / (float(maxi) + 0.0000000001)*255)
            newVal[i] = oldVal[i] + (val[i] - oldVal[i])*.5
            if newVal[i] <= 0:
                newVal[i] = 0


        maxi = max(newVal)
        for i in range(len(val)):
            newVal[i] = int(newVal[i] / (float(maxi) + 0.0000000001)*255)
            change += abs(oldVal[i] - newVal[i])
        ##print(val)
        #print(oldVal)
        #print("change", change)
        #print(newVal)
        newVal = saturate(newVal)

        magnitude = data["magnitude"]


        if magnitude != 0:
            if magnitude <= magMin:
                magMin = magnitude

            if magnitude >= magMax:
                magMax = magnitude
            magnitude = magnitude/(magMax+magMin)*500
        else:
            magnitude = 10

        if change > 0 and now() - tColor > MIN_TIME*30:
            group.changeColor(newVal[0], newVal[1], newVal[2])
            oldVal = newVal
            tColor = now()

        elif now() - tBright > MIN_TIME*15:
            #group.changeBrightness(magnitude)
            tBright = now()
        else:
            time.sleep(MIN_TIME/10.)





    stream.stop()





    group.changeColor(255, 0, 255)
    time.sleep(1)
    group.changeBrightness(1)
    time.sleep(1)
    group.turnOff()
    time.sleep(1)
    group.turnOn()
    time.sleep(1)
    group.changeColor(0,255,0)
    time.sleep(1)
    group.changeColor(255,0,255)
    time.sleep(1)
    group.stop()

