from aiohttp import web
import socketio

sio = socketio.AsyncServer()
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


if __name__ == '__main__':
    web.run_app(app, port=3412)
