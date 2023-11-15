import numpy as np
import pyaudio
import time
import librosa
import matplotlib.pyplot as plt
import math
from threading import RLock, Thread

class Fig:
    def __init__(self, RATE, CHUNK):
        self.fig, (self.ax, self.ax3) = plt.subplots(2)
        self.x_fft = np.linspace(0, RATE, CHUNK)
        self.x = np.arange(0, 2 * CHUNK, 2)
        self.line, = self.ax.plot(self.x, np.random.rand(CHUNK), 'r')
        #self.line1, = self.ax1.semilogx(self.x_fft, np.random.rand(CHUNK), 'b')
        #self.line2, = self.ax2.semilogx(self.x_fft, np.random.rand(CHUNK), 'g')
        #self.line3, = self.ax3.semilogx(self.x_fft, np.random.rand(CHUNK), 'b')
        self.ax.set_ylim(-2, 2)
        self.ax.set_xlim = (0, CHUNK)
        #self.ax1.set_xlim(20, RATE / 2)
        #self.ax1.set_ylim(-1, 1)
        #self.ax2.set_xlim(20, RATE / 2)
        #self.ax2.set_ylim(-1, 1)
        #self.ax3.set_xlim(20, RATE / 2)
        #self.ax3.set_ylim(-1, 1)
        self.ax3.label_outer()
        self.img = None
        self.fig.colorbar(self.img, ax=self.ax3)
        self.fig.show()


    def draw(self, y=None, y1=None, y2=None, y3=None):
        self.line.set_ydata(y)
        #self.line1.set_ydata(y1)
        #self.line2.set_ydata(y2)
        if y3 is not None:
            self.img = librosa.display.specshow(y3, y_axis='chroma', x_axis='time', ax=self.ax3)
            self.img.set_array(y3)

        self.fig.canvas.draw()
        self.fig.canvas.flush_events()


class Stream(object):
    def __init__(self, display=False):
        self.FORMAT = pyaudio.paFloat32
        self.CHANNELS = 1
        self.RATE = 44100
        self.CHUNK = 1024 * 8
        self.hop_length = 1024
        self.p = None
        self.stream = None
        if display:
            self.fig = Fig(self.RATE, self.CHUNK)
        self.display = display

        self.y = None
        self.y1 = None
        self.y2 = None
        self.y3 = None

        self.chords = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"]
        self.res = {"chords":{"C":0, "C#":0, "D":0, "D#":0, "E":0, "F":0, "F#":0, "G":0, "G#":0, "A":0, "A#":0, "B":0}, "magnitude":0}

        self.isActive =True
        self.lock = RLock()
        self.streamLock = RLock()
        self.thread = Thread(target=self.mainloop)
        self.thread.start()


    def getChordIntensity(self):
        with self.lock:
            res = self.res
        return res

    def start(self):
        self.p = pyaudio.PyAudio()
        self.stream = self.p.open(format=self.FORMAT,
                                  channels=self.CHANNELS,
                                  rate=self.RATE,
                                  input=True,
                                  output=False,
                                  stream_callback=self.callback,
                                  frames_per_buffer=self.CHUNK)

    def stop(self):
        self.isActive = False
        self.stream.close()
        self.p.terminate()
        self.thread.join()

    def callback(self, in_data, frame_count, time_info, flag):
        with self.streamLock:
            self.y = np.frombuffer(in_data, dtype=np.float32)
        return None, pyaudio.paContinue

    def mainloop(self):
        while self.isActive:  # if using button you can set self.stream to 0 (self.stream = 0), otherwise you can use a stop condition
            with self.streamLock:
                y = self.y
            if y is not None:
                # Set the hop length; at 22050 Hz, 512 samples ~= 23ms
                y_harmonic, y_percussive = librosa.effects.hpss(y)
                y1 = y_harmonic
                y2 = y_percussive
                # Beat track on the percussive signal
                tempo, beat_frames = librosa.beat.beat_track(y=y_percussive, sr=self.RATE)

                # Compute MFCC features from the raw signal
                mfcc = librosa.feature.mfcc(y=y, sr=self.RATE, hop_length=self.hop_length, n_mfcc=13)

                # And the first-order differences (delta features)
                mfcc_delta = librosa.feature.delta(mfcc)

                # Stack and synchronize between beat events
                # This time, we'll use the mean value (default) instead of median
                beat_mfcc_delta = librosa.util.sync(np.vstack([mfcc, mfcc_delta]), beat_frames)

                # Compute chroma features from the harmonic signal
                chromagram = librosa.feature.chroma_cqt(y=y_harmonic, sr=self.RATE)

                # stft
                linear = librosa.stft(y=y,
                                      n_fft=2205,
                                      hop_length=self.hop_length,
                                      win_length=int(self.RATE*0.05) )

                # magnitude spectrogram
                mag = np.abs(linear)  # (1+n_fft//2, T)

                mag = librosa.amplitude_to_db(mag)

                # normalize
                mag = np.clip((mag - 20 + 100) / 100, 0, 1)

                # Transpose
                mag = mag.T.astype(np.float32)  # (T, 1+n_fft//2)


                #print(np.mean(mag))
                #print("")


                res = {"chords":{}, "magnitude":np.mean(mag)}
                res["y3"] = chromagram
                res["y"] = y
                for i in range(len(chromagram)):
                    res["chords"][self.chords[i]] = np.mean(chromagram[i])

                #print(res)
                with self.lock:
                    self.res = res

                """
                # Aggregate chroma features between beat events
                # We'll use the median value of each feature between beat frames
                
                beat_chroma = librosa.util.sync(chromagram, beat_frames, aggregate=np.median)
                print(beat_chroma)
                
                # Finally, stack all beat-synchronous features together
                beat_features = np.vstack([beat_chroma, beat_mfcc_delta])
                beat_features
                """

            time.sleep(.01)  # main operations with librosa




