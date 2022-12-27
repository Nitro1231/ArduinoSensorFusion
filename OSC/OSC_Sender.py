from Key import *
import random
import time
from pythonosc import udp_client


if __name__ == '__main__':
  client = udp_client.SimpleUDPClient('127.0.0.1', OSC_SERVER_PORT)

  while (True):
    print('send!')
    client.send_message('/quat_unity', random.random())
    time.sleep(1)