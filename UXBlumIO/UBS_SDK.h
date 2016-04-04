
#include "pch.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <istream>
#include <fstream>
#include <map>
#include <string>
			#include <cvt/wstring>
			#include <codecvt>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 15900
#define DEFAULT_PORT "80"


namespace UXBlumIO {
}