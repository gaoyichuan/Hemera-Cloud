#pragma once

#include <cstdio>

#define DEBUG

#ifdef DEBUG
#define MSG_DBG(fmt, ...) printf("[DBG]%s:" fmt "\n", __func__, ##__VA_ARGS__)
#else
#define MSG_DBG(fmt, ...)
#endif

#define MSG_ERR(fmt, ...) printf("[ERR]%s:" fmt "\n", __func__, ##__VA_ARGS__)
#define MSG_WRN(fmt, ...) printf("[WRN]%s:" fmt "\n", __func__, ##__VA_ARGS__)
#define MSG_INF(fmt, ...) printf("[INF]%s:" fmt "\n", __func__, ##__VA_ARGS__)

#define REG32(a) (*(volatile uint32_t*)(a))
