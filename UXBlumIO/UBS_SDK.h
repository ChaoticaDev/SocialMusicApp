
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
	namespace UberSnip {
		namespace UTILS {

			class STRING {
			public:
				Platform::String^ fromAscII;

				static Platform::String^ StringFromAscIIChars(const char* chars)
				{
					size_t newsize = strlen(chars) + 1;
					wchar_t * wcstring = new wchar_t[newsize];
					size_t convertedChars = 0;
					mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
					Platform::String^ str = ref new Platform::String(wcstring);
					delete[] wcstring;
					return str;
				}

				static Platform::String^ StringFromAscIIChars(std::string str_string)
				{
					const char* chars = str_string.c_str();
					size_t newsize = strlen(chars) + 1;
					wchar_t * wcstring = new wchar_t[newsize];
					size_t convertedChars = 0;
					mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
					Platform::String^ str = ref new Platform::String(wcstring);
					delete[] wcstring;
					return str;
				}

				static std::string StringToAscIIChars(Platform::String^ chars) {
					Platform::String^ fooRT = chars;
					stdext::cvt::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
					std::string stringUtf8 = convert.to_bytes(fooRT->Data());
					return stringUtf8;
				}

				static LPCWSTR StringToLPCWSTR(const char* str) {

					wchar_t* wString = new wchar_t[4096];
					MultiByteToWideChar(CP_ACP, 0, str, -1, wString, 4096);
					return wString;
				}

			};

		}


		ref class CLIENT sealed{

			Platform::String^ body;
			bool err = false;
			Platform::Object^ kHTTP;

		public:
			CLIENT() {}
			property Platform::String^ BodyResponse {
				Platform::String^ get() {
					return this->body;
				}

				void set(Platform::String^ val) {
					this->body = val;
				}
			}

			property bool APIError {
				bool get() {
					return this->err;
				}

				void set(bool val) {
					this->err = val;
				}
			}

			property Platform::Object^ http {
				Platform::Object^ get() {
					return this->kHTTP;
				}

				void set(Platform::Object^ val) {
					this->kHTTP = val;
				}
			}

		};

		public ref class HTTP sealed{

			Platform::Object^ sdk_client;
			Platform::String^ reqURL;
			std::map<Platform::String^, Platform::String^> _headers;
			std::map<Platform::String^, Platform::String^> _params;

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

		public:


			void addHeader(Platform::String^ head, Platform::String^ content) {
				this->_headers[head] = content;
			}

			void addDefHeaders(void) {
				this->addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
				this->addHeader("Accept-Encoding", "gzip, deflate, sdch");
				this->addHeader("Accept-Language", "en-US,en;q=0.8");
				this->addHeader("Connection", "keep-alive");
				this->addHeader("Host", "api.ubersnip.com");
				this->addHeader("Upgrade-Insecure-Requests", "1");
				this->addHeader("User-Agent", "Carrots/KeenIO HTTP-1.0");
			}

			void addParam(Platform::String^ head, Platform::String^ content) {
				this->_params[head] = content;
			}

			Platform::String^ headers() {
				Platform::String^ plain;

				for each (std::pair<Platform::String^, Platform::String^> header in this->_headers)
				{
					plain += header.first + ":" + header.second + "\r\n";
				}

				return plain;
			}

			Platform::String^ params() {
				Platform::String^ plain = this->reqURL;
				int index = 0;
				for each (std::pair<Platform::String^, Platform::String^> header in this->_params)
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


			void request()
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
					return;
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
					return;
				}

				// Attempt to connect to an address until one succeeds
				for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

					// Create a SOCKET for connecting to server
					ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
						ptr->ai_protocol);
					if (ConnectSocket == INVALID_SOCKET) {
						printf("socket failed with error: %ld\n", WSAGetLastError());
						WSACleanup();
						return;
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
					return;
				}

				// Send an initial buffer

				Platform::String^ headers = "GET " + this->params();
				headers += "\r\n";
				headers += this->headers();

				iResult = send(ConnectSocket, UTILS::STRING::StringToAscIIChars(headers).c_str(), (int)strlen(UTILS::STRING::StringToAscIIChars(headers).c_str()), 0);

				if (iResult == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(ConnectSocket);
					WSACleanup();
					return;
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
					return;
				}


#ifdef DEBUG

				printf("Waiting for KeenIO's response...");

#else

#endif // DEBUG
				// Receive until the peer closes the connection
				//iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

				std::string rec;
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

				CLIENT^ sdk = dynamic_cast<CLIENT^>(this->sdk_client);
				sdk->BodyResponse = UTILS::STRING::StringFromAscIIChars(rec);

				//int keenErr = UTILS::STRING::StringToAscIIChars(sdk_client->BodyResponse).find("\"error_code\":");
				//if (keenErr >= 0) {
					//sdk_client->APIError = true;
				//}
				return;
			}

			property Platform::Object^ SDKClient {
				Platform::Object^ get() {
					return this->sdk_client;
				}

				void set(Platform::Object^ val) {
					this->sdk_client = val;
				}
			}

			HTTP() {

			}

			property Platform::String^ RequestURL {
				Platform::String^ get() {
					return this->reqURL;
				}

				void set(Platform::String^ val) {
					this->reqURL = val;
				}
			}

		};

		class UBERSNIP_CLIENT {

		public:
			HTTP^ Http = ref new HTTP();
			CLIENT^ Client = ref new CLIENT();



			UBERSNIP_CLIENT() {
				this->Client->http = ref new HTTP();
				dynamic_cast<HTTP^>(this->Client->http)->SDKClient = this->Client;
				this->Http = dynamic_cast<HTTP^>(this->Client->http);
			}

		};
	}
}