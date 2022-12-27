from Key import *
import argparse
from pythonosc import dispatcher
from pythonosc import osc_server
from pythonosc import udp_client


if __name__ == '__main__':
    client = udp_client.SimpleUDPClient('127.0.0.1', OSC_SERVER_PORT)

    def host_handler(*args):
        print(args)
        client.send_message('/euler_unity', args)

    dispatcher = dispatcher.Dispatcher()
    dispatcher.map('/euler', host_handler)

    server = osc_server.ThreadingOSCUDPServer((OSC_SERVER_HOST, OSC_SERVER_PORT), dispatcher)
    print('Serving on {}'.format(server.server_address))
    server.serve_forever()
