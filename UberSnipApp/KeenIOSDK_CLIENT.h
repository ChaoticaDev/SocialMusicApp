#define WIN32_LEAN_AND_MEAN

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

using namespace std;


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 15900
#define DEFAULT_PORT "80"

/*
[
{"property_name":"path","operator":"ne","property_value":"/"},
{"property_name":"action","operator":"eq","property_value":"/"},
{"property_name":"action","operator":"gt","property_value":"/"},
{"property_name":"action","operator":"gte","property_value":"/"},
{"property_name":"action","operator":"lt","property_value":"/"},
{"property_name":"action","operator":"lte","property_value":"/"},
{"property_name":"action","operator":"contains","property_value":"/"},
{"property_name":"action","operator":"not_contains","property_value":"/"}
]
*/

class UBERSNIP_HTTP {
public:
	string reqURL;
	map<string, string> _headers;
	map<string, string> _params;

	string _writeKey = "cdb8c4e78721169c3dc475c8d417c6038427913cf21b6785d01b27eb1cb55093f01f40d4e72199120ba88cbb1c1a787c786c001cb12d3a54dc5b5b0d69296db93943eb83f3b73990dba14f91f325f9771d0bf336d4415f219ae6345e1fb61690";
	string _readKey = "e415c20339feba31336ddce0623be502b629716d30fb8c2de930c97683613f189239f29ecb1ee2aef2f7bc96f7b06d45ab3fd6aea5a221ff4cc3dc612dd3062c7536a97d2cabf469ff386e4a750e59f82e50b8f9699a88015585d35cc65abb70";
	string _masterKey = "436D4D2EF7282BA17F712515ED39224F0439F892CDAA81D05437DB7137BB2D9C";

	void addDefHeaders(void) {
		this->addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
		this->addHeader("Accept-Encoding", "gzip, deflate, sdch");
		this->addHeader("Accept-Language", "en-US,en;q=0.8");
		this->addHeader("Connection", "keep-alive");
		this->addHeader("Host", "api.ubersnip.com");
		this->addHeader("Upgrade-Insecure-Requests", "1");
		this->addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");
	}

	string masterKey(string key = "") {
		if (key != "") {
			this->_masterKey = key;
		}
		return this->_masterKey;
	}

	string readKey(string key = "") {
		if (key != "") {
			this->_readKey = key;
		}
		return this->_readKey;
	}

	string writeKey(string key = "") {
		if (key != "") {
			this->_writeKey = key;
		}
		return this->_writeKey;
	}

	void addHeader(string head, string content) {
		this->_headers[head] = content;
	}

	void addParam(string head, string content) {
		this->_params[head] = content;
	}

	string headers() {
		string plain;

		for each (std::pair<string, string> header in this->_headers)
		{
			plain += header.first + ":" + header.second + "\r\n";
		}

		return plain;
	}

	string params() {
		string plain = this->reqURL;
		int index = 0;
		for each (std::pair<string, string> header in this->_params)
		{
			if (index < 1) {
				plain += "?" + header.first + "=" + header.second;
			}
			else {
				plain += "&" + header.first + "=" + header.second;
			}
			index++;
		}

		return plain;
	}
};


class UBERSNIP_CLIENT {
public:
	string body;
	UBERSNIP_HTTP kHTTP;
	bool err = false;

	bool recvraw(SOCKET socket, char *buf, int buflen)
	{
		unsigned char* p = (unsigned char*)malloc(sizeof(unsigned char*));
		while (buflen > 0)
		{
			int received = recv(socket, buf, buflen, 0);
			if (received < 1) return false;
			p += received;
			buflen -= received;
			//printf(".");
		}
		return true;
	}

	int request(UBERSNIP_HTTP kHTTP)
	{
		WSADATA wsaData;
		SOCKET ConnectSocket = INVALID_SOCKET;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;
		char *sendbuf = "this is a test";
		char recvbuf[DEFAULT_BUFLEN] = "";
		int iResult;
		int recvbuflen = DEFAULT_BUFLEN;


#ifdef DEBUG

		printf("WinSock initialized\n");

#else

#endif // DEBUG

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
#ifdef DEBUG

		printf("Resolving KeenIO IP");

#else

#endif // DEBUG

		// Resolve the server address and port
		iResult = getaddrinfo("107.180.44.147", DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to an address until one succeeds
		for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

			// Create a SOCKET for connecting to server
			ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);
			if (ConnectSocket == INVALID_SOCKET) {
				printf("socket failed with error: %ld\n", WSAGetLastError());
				WSACleanup();
				return 1;
			}
			else {
#ifdef DEBUG

				printf("Established socket @ https://api.keen.io/");

#else

#endif // DEBUG
			}

			// Connect to server.
			iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(ConnectSocket);
				ConnectSocket = INVALID_SOCKET;
				continue;
			}
			else {
#ifdef DEBUG

				printf("Connected to KeenIO's servers");

#else

#endif // DEBUG
			}
			break;
		}

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		// Send an initial buffer

		string headers = "GET " + kHTTP.params();
		headers += "\r\n";
		headers += kHTTP.headers();

		iResult = send(ConnectSocket, headers.c_str(), (int)strlen(headers.c_str()), 0);

		if (iResult == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		//printf("Bytes Sent: %ld\n", iResult);

#ifdef DEBUG

		printf("Sent HTTP request\n");

#else

#endif // DEBUG


		// shutdown the connection since no more data will be sent
		//iResult = shutdown(ConnectSocket, SD_SEND);
		if (iResult == SOCKET_ERROR) {
			printf("shutdown failed with error: %d\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}


#ifdef DEBUG

		printf("Waiting for KeenIO's response...");

#else

#endif // DEBUG
		// Receive until the peer closes the connection
		//iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

		string rec;
		u_long iMode = 1;
		//ioctlsocket(ConnectSocket, FIONBIO, &iMode);

		while (recvraw(ConnectSocket, recvbuf, 1)) {
			rec += recvbuf;
		}



#ifdef DEBUG

		printf("Cleaning up");

#else

#endif // DEBUG
		//printf("\n\n%s\n\n", (char*)rec.c_str());
		closesocket(ConnectSocket);
		WSACleanup();

		this->body = rec;

		int keenErr = this->body.find("\"error_code\":");
		if (keenErr >= 0) {
			this->err = true;
		}
		return 0;
	}
};