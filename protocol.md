# 总体框架

Hemera Cloud 一共包含 3 部分，Arm 是运行在 Zynq Module PS 上的程序，ArmServer 是运行在板子服务器上的程序，WebServer 是网页端服务器。

其中 Arm 和 ArmServer 进行通信，WebServer 和 ArmServer 进行通讯，Arm 和 WebServer 之间不直接进行通讯。

## Arm 和 ArmServer 之间的通讯

ArmServer 先启动，会在服务器上启动一个 socket.io 服务器，监听端口 3412。

每次 Arm 启动，会启动一个 socket.io 客户端连接 ArmServer，之后的通讯基于该客户端。

用 emit 发送消息，用 on 接收消息，具体消息内容见后述。

## WebServer 和 ArmServer 之间的通讯

ArmServer 先启动，会在服务器上启动一个 socket.io 服务器，监听端口 3408。

每次 WebServer 需要板子的时候，都会新建一个 socket.io 客户端连接 ArmServer，原则上一个 socket.io 客户端对应一个板子。

WebServer 的每个 socket.io 客户端对于板子的拥有权，从向 ArmServer 申请板子并且成功开始，到关闭该 socket.io 客户端结束。

如果 WebServer 需要多个板子，需要开启多个 socket.io 客户端。

用 emit 发送消息，用 on 接收消息，具体消息内容见后述。

### 协议详情

ArmServer 和 WebServer 上运行 node 版本的 socket.io，Arm 上运行 C++ 版本的 socket.io。

**node 上的 number 对应 c++ 中的 interger_message 和 double_message，具体对应关系不明。**

string <-> string_message

**Buffer <-> binary_message**

Array <-> array_message

Object <-> object_message

boolean <-> boolean_message

null <-> null_message

需要传输二进制信息（比如 bit_stream 和内存信息）时，直接发送 Buffer 原数据，不进行 hex 或者 base64 编码。

#### 数据结构定义

见 [interfaces.ts](interfaces.ts)

----------------------

#### WebServer -> ArmServer

WebServer 上的 socket.io 客户端通过 `emit('BIT_STREAM', BitStream)` 发送消息，ArmServer 通过 `on('BIT_STREAM', fn)` 接收消息，其他同理。

不定期发送的事件：BOARD_STATUS, QUERY, BIT_STREAM, GET_STATUS, SET_SWITCH

* BOARD_STATUS[返回 BOARD_STATUS_RESULT]

    数据：无

* QUERY[返回 QUERY_RESULT] 请求一个板子

    数据：无

* BIT_STREAM[返回 BIT_STREAM_RESULT] 上传二进制文件

    数据：BitStream

* GET_STATUS 获取板子的状态，原则上说板子应该马上发送一个 LED_SWITCH_STATUS 事件，但不是必须的

    数据：无

* SET_SWITCH[返回 SET_SWITCH_RESULT] 设置开关状态

    数据：LEDStatusWSwitch

#### ArmServer -> WebServer

不定期发送的事件：LED_SWITCH_STATUS

* BOARD_STATUS_RESULT

    数据：BoardStatus

* QUERY_RESULT 请求结果

    数据：Result

* BIT_STREAM_RESULT

    数据：Result

* LED_SWITCH_STATUS

    数据：LEDStatusWSwitch

* SET_SWITCH_RESULT 设置开关状态是否成功

    数据：Result

------------------------

#### ArmServer -> Arm

Arm 上的 socket.io 客户端通过 `on('BIT_STREAM', fn)` 接收消息，ArmServer 通过 `emit('BIT_STREAM', BitStream)` 发送消息，其他同理。

不定期发送的事件：BIT_STREAM, SET_SWITCH, GET_STATUS

* BIT_STREAM[返回 BIT_STREAM_RESULT] 烧录二进制文件，板子接收到该事件之后需将开关置零

    数据：BitStreamStructure

* SET_SWITCH[返回 SET_SWITCH_RESULT] 设置开关状态

    数据：LEDStatusWSwitch

* GET_STATUS[返回 LED_SWITCH_STATUS] LED 状态查询

    数据：无

#### Arm -> ArmServer

不定期发送的事件：LED_SWITCH_STATUS

* BIT_STREAM_RESULT 烧录程序的结果

    数据：Result

* SET_SWITCH_RESULT 设置开关状态是否成功

    数据：Result

* LED_SWITCH_STATUS LED 和开关状态，开关的状态是无效的

    数据：LEDStatusWSwitch
