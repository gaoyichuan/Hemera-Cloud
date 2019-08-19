from aiohttp import web
import socketio
import json

sio = socketio.AsyncServer(async_mode='aiohttp', async_handlers=True)
app = web.Application()
sio.attach(app)

bitstream_file = '/home/gaoyichuan/workspace/Hemera/output/fpgae.rbf'

@sio.on('connect')
async def connect(sid, environ):
    print('connect ', sid)
    with open(bitstream_file, 'rb') as f:
        bits = f.read()
    await sio.emit('BIT_STREAM', {'raw_data': bits, 'raw_data_length': len(bits)})


@sio.on('BIT_STREAM_RESULT')
async def bitstream_handle(sid, data):
    print('bitstream result: sid={}, success={}'.format(sid, data['success']))
    await sio.emit('GET_STATUS')
    

@sio.on('LED_SWITCH_STATUS')
async def led_status_handle(sid, data):
    print('led status result: sid={}, leds={}'.format(sid, data['leds']))


class TestApp(object):
    def __init__(self):
        self.thread = None

    def start_server(self):
        print('starting server')
        web.run_app(app, port=3412)

    def start(self):
        self.thread = sio.start_background_task(self.start_server)

    def send_msg(self, event, data):
        print('emit msg {}'.format(event))
        sio.emit(event, json.dumps(data))

    def wait(self):
        self.thread.join()


if __name__ == '__main__':
    testapp = TestApp()
    testapp.start()

    print("wtf")

    while True:
        sio.sleep(1)
        testapp.send_msg('GET_STATUS', '')
