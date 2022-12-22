import argparse
from pythonosc import dispatcher
from pythonosc import osc_server


def print_handler(*args):
    print(args)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--ip', default='1.233.162.8', help='The ip to listen on')
    parser.add_argument('--port', type=int, default=8080, help='The port to listen on')
    args = parser.parse_args()

    dispatcher = dispatcher.Dispatcher()
    dispatcher.map('/quat', print_handler)
    # dispatcher.map('/euler', print_handler)
    # dispatcher.map('/rpy', print_handler)

    server = osc_server.ThreadingOSCUDPServer((args.ip, args.port), dispatcher)
    print('Serving on {}'.format(server.server_address))
    server.serve_forever()
