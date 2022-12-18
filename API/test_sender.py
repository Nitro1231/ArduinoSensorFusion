import json
import requests


base_url = 'http://127.0.0.1:8080/'
headers = { 'Content-Type': 'application/json' }


def post(route, data):
    response = requests.post(base_url + route, headers=headers, data=json.dumps(data))
    print('response: ', response)
    print('response.text: ', response.text)


def get(route, data):
    response = requests.get(base_url + route, headers=headers, data=json.dumps(data))
    print('response: ', response)
    print('response.text: ', response.text)


test_data = {
    'id': '1',
    'data': {'x': 0, 'y': 0, 'z': 0},
    'key1': 'value1',
    'key2': 'value2'
}

post('update_sensors', test_data)
get('get_data', test_data)