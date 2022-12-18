from collections import defaultdict
from flask import Flask, request, json


database = defaultdict(list)
app = Flask(__name__)


@app.route('/test', methods=['POST'])
def test():
    data = json.loads(request.get_data())
    print('[Test] data: ', data)

    response = { 'result': 'ok' }
    return json.dumps(response)


@app.route('/update_sensors', methods=['POST'])
def update_sensors():
    data = json.loads(request.get_data())
    print('[update_sensors]', data['id'], data['data'])
    database[data['id']].append(data['data'])

    response = { 'result': 'ok' }
    return json.dumps(response)


@app.route('/get_data', methods=['GET'])
def get_data():
    data = json.loads(request.get_data())
    print('[get_data]', data['id'], database[data['id']])

    return json.dumps(database[data['id']])


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=8080)