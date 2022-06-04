#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <queue>
#include <unordered_set>
#include <pthread.h>

class Url
{
public:
};