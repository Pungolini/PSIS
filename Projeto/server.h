

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <string.h>
#include <poll.h>
#include "board_library.h"
#include "playerManagement.h"



#define MAXPLAYERS 100
#define SERVER_PORT 4000






extern short BOARD_DIM;
extern short NUMPLAYERS;
