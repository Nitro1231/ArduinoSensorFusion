from Key import *
from pythonosc import dispatcher
from pythonosc import osc_server


def print_handler(*args):
    print(args)


if __name__ == '__main__':
    dispatcher = dispatcher.Dispatcher()
    dispatcher.map('/quat', print_handler)
    # dispatcher.map('/euler', print_handler)
    # dispatcher.map('/rpy', print_handler)

    server = osc_server.ThreadingOSCUDPServer((OSC_SERVER_HOST, OSC_SERVER_PORT), dispatcher)
    print('Serving on {}'.format(server.server_address))
    server.serve_forever()
