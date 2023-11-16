import socket
import time
import json
from lamps import Lamp, LampGroup

import threading

class Server:
    def __init__(self):
        self.lamps = Lamp.getAvailableDevices()
        self.cmd = {
            "getLamps":self.getLamps
        }

    def getLamps(self):
        lamps = []
        for lamp in self.lamps:
            lamps.append({"id":lamp.id})
        return json.dumps(lamps)

    # Ensures the connection is still active
    def keepalive(self, conn, addr):
        print("Client connected")
        with conn:
            conn.settimeout(3)
            while True:
                try:
                    data = conn.recv(1024)
                    if not data: break

                    else:
                        print(data)
                        data = json.loads(data)
                        print(data)
                        res = self.cmd[data["cmd"]]()
                        print(res)
                        print("")
                        conn.sendall(res.encode())
                except Exception as e:
                    break
            print("Client disconnected")

    # Listens for connections to the server and starts a new keepalive thread
    def listenForConnections(self):
        HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
        PORT = 65432  # Port to listen on (non-privileged ports are > 1023)
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
            sock.bind((HOST, PORT))
            while True:
                sock.listen()
                conn, addr = sock.accept()
                t = threading.Thread(target=self.keepalive, args=(conn, addr))
                t.start()


    def start(self):
        server = threading.Thread(target=self.listenForConnections)
        server.start()