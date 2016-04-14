//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"
#include "external\JSON\cJSON.h"
#include "UBS_SDK.h"
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

#include <ppltasks.h>
using namespace concurrency;

#define _StringFromAscIIChars(x) UberSnip::UTILS::STRING::StringFromAscIIChars(x)
#define _String(x) UberSnip::UTILS::STRING::StringFromAscIIChars(x)
#define _StringToAscIIChars(x) UberSnip::UTILS::STRING::StringToAscIIChars(x).c_str()
#define _string(x) UberSnip::UTILS::STRING::StringToAscIIChars(x).c_str()

using namespace Windows::UI::Xaml::Data;
using namespace std;
namespace UXBlumIO
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>

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

				static Platform::String^ URLEncode(const std::string &value) {
					ostringstream escaped;
					escaped.fill('0');
					escaped << hex;

					for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
						string::value_type c = (*i);

						// Keep alphanumeric and other accepted characters intact
						if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
							escaped << c;
							continue;
						}

						// Any other characters are percent-encoded
						escaped << uppercase;
						escaped << '%' << setw(2) << int((unsigned char)c);
						escaped << nouppercase;
					}

					return StringFromAscIIChars(escaped.str());
				}

				static Platform::String^ url_encode(const std::string &value) {
					ostringstream escaped;
					escaped.fill('0');
					escaped << hex;

					for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
						string::value_type c = (*i);

						// Keep alphanumeric and other accepted characters intact
						if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
							escaped << c;
							continue;
						}

						// Any other characters are percent-encoded
						escaped << uppercase;
						escaped << '%' << setw(2) << int((unsigned char)c);
						escaped << nouppercase;
					}

					return StringFromAscIIChars(escaped.str());
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



		ref class CLIENT sealed {

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

		public ref class HTTP sealed {

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
					dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
					dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
						dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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

					dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
					dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
					dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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

	namespace KeenIOAPI{


		public ref class CLIENT sealed {

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

		public ref class HTTP sealed {

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

		static inline bool is_base64(unsigned char c) {
			return (isalnum(c) || (c == '+') || (c == '/'));
		}

		std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
			static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			std::string ret;
			int i = 0;
			int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];

			while (in_len--) {
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3) {
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;

					for (i = 0; (i < 4); i++)
						ret += base64_chars[char_array_4[i]];
					i = 0;
				}
			}

			if (i)
			{
				for (j = i; j < 3; j++)
					char_array_3[j] = '\0';

				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					ret += base64_chars[char_array_4[j]];

				while ((i++ < 3))
					ret += "%3D";

			}

			return ret;

		}

		std::string base64_decode(std::string const& encoded_string) {
			static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			int in_len = encoded_string.size();
			int i = 0;
			int j = 0;
			int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3];
			std::string ret;

			while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
				char_array_4[i++] = encoded_string[in_]; in_++;
				if (i == 4) {
					for (i = 0; i < 4; i++)
						char_array_4[i] = base64_chars.find(char_array_4[i]);

					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

					for (i = 0; (i < 3); i++)
						ret += char_array_3[i];
					i = 0;
				}
			}

			if (i) {
				for (j = i; j < 4; j++)
					char_array_4[j] = 0;

				for (j = 0; j < 4; j++)
					char_array_4[j] = base64_chars.find(char_array_4[j]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
			}

			return ret;
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



		//ADD A PARAMETER TO MAP
		void addDataParam(Platform::String^ head, Platform::String^ event_name, Platform::String^ event_property) {

			int len_s = strlen(UberSnip::UTILS::STRING::StringToAscIIChars("{ \"" + event_name + "\" : \"" + event_property + "\" }").c_str());
			this->_params[head] = UberSnip::UTILS::STRING::StringFromAscIIChars(this->base64_encode((const unsigned char*)UberSnip::UTILS::STRING::StringToAscIIChars("{ \"" + event_name + "\" : \"" + event_property + "\" }").c_str(), len_s));

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
				dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
			iResult = getaddrinfo("108.168.254.50", DEFAULT_PORT, &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed with error: %d\n", iResult);
				WSACleanup();
				dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
					dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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

				dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
				return;
			}

			// Send an initial buffer

			Platform::String^ headers = "GET " + this->params();
			headers += "\r\n";
			headers += this->headers();

			iResult = send(ConnectSocket, UberSnip::UTILS::STRING::StringToAscIIChars(headers).c_str(), (int)strlen(UberSnip::UTILS::STRING::StringToAscIIChars(headers).c_str()), 0);

			if (iResult == SOCKET_ERROR) {
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
				dynamic_cast<CLIENT^>(this->sdk_client)->APIError = true;
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
			sdk->BodyResponse = UberSnip::UTILS::STRING::StringFromAscIIChars(rec);

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

		class KEENIO_CLIENT {

		public:
			HTTP^ Http = ref new HTTP();
			CLIENT^ Client = ref new CLIENT();



			KEENIO_CLIENT() {
				this->Client->http = ref new HTTP();
				dynamic_cast<HTTP^>(this->Client->http)->SDKClient = this->Client;
				this->Http = dynamic_cast<HTTP^>(this->Client->http);
			}

		};

		public ref class KEENIO_AUTH sealed {
			Platform::String^ _master_key;
			Platform::String^ _write_key;
			Platform::String^ _read_key;
		public:
			KEENIO_AUTH() {

			}


			property Platform::String^ MasterKey {
				Platform::String^ get() {
					return this->_master_key;
				}

				void set(Platform::String^ val) {
					this->_master_key = val;
				}
			}
		};

		public ref class KEENIO_GENERIC_PROPERTY sealed{
			Platform::String^ _property;
			Platform::String^ _value;
		public:
			KEENIO_GENERIC_PROPERTY() {

			}

			property Platform::String^ PropertyName {
				Platform::String^ get() {
					return this->_property;
				}

				void set(Platform::String^ val) {
					this->_property = val;
				}
			}

			property Platform::String^ PropertyValue {
				Platform::String^ get() {
					return this->_value;
				}

				void set(Platform::String^ val) {
					this->_value = val;
				}
			}
		};

		public ref class KEENIO_EVENT sealed {
			Platform::String^ _name;
			Windows::UI::Xaml::Interop::IBindableObservableVector^ _properties = ref new Platform::Collections::Vector<KEENIO_GENERIC_PROPERTY^>();
			Platform::String^ _url;
			Platform::String^ _proj_id;

			
			KEENIO_AUTH^ auth = ref new KEENIO_AUTH();

			bool verified = false;
			bool error = false;
		public:
			KEENIO_EVENT() {

			}

			void LoadProperties() {
				KEENIO_CLIENT * UberSnipAPI = new KEENIO_CLIENT();

				UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + _proj_id  + "/events/" + this->_name;
				UberSnipAPI->Http->addParam("api_key", this->auth->MasterKey);

				UberSnipAPI->Http->request();

				cJSON* response = cJSON_Parse(_StringToAscIIChars(UberSnipAPI->Client->BodyResponse));

				cJSON* arr_events;

				try {
					arr_events = cJSON_GetObjectItem(response, "properties");
				}
				catch (std::exception_ptr e) {
					this->verified = false;
					this->error = true;
					return;
				}

				for (cJSON* c_event = arr_events->child; c_event != nullptr; c_event = c_event->next) {
					KEENIO_GENERIC_PROPERTY^ k_prop = ref new KEENIO_GENERIC_PROPERTY();
					k_prop->PropertyName = _String(c_event->valuestring);
					k_prop->PropertyValue = _String(c_event->string);
					this->_properties->Append(k_prop);
				}

				this->verified = true;
				this->error = false;
			}

			property KEENIO_AUTH^ Auth {
				KEENIO_AUTH^ get() {
					return this->auth;
				}

				void set(KEENIO_AUTH^ val) {
					this->auth = val;
				}
			}

			property Platform::String^ Name {
				Platform::String^ get() {
					return this->_name;
				}

				void set(Platform::String^ val) {
					this->_name = val;
				}
			}

			property Platform::String^ ProjectID {
				Platform::String^ get() {
					return this->_proj_id;
				}

				void set(Platform::String^ val) {
					this->_proj_id = val;
				}
			}

			property Platform::String^ URL {
				Platform::String^ get() {
					return this->_url;
				}

				void set(Platform::String^ val) {
					this->_url = val;
				}
			}

			property Windows::UI::Xaml::Interop::IBindableObservableVector^ Properties {
				Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
					return this->_properties;
				}
			}
		};

		public ref class KEENIO_PROJECT sealed{
			Platform::String^ _events_url;
			Windows::UI::Xaml::Interop::IBindableObservableVector^ _events = ref new Platform::Collections::Vector<KEENIO_EVENT^>();
			Platform::String^ _id;
			Platform::String^ _org_id;
			Platform::String^ _name;
			Platform::String^ _queries_url;
			Platform::String^ _partners_url;
			Platform::String^ _url;

			KEENIO_AUTH^ auth = ref new KEENIO_AUTH();

			bool verified = false;
			bool error = false;

		public:
			KEENIO_PROJECT() {
				
			}

			void LoadProject() {
				KEENIO_CLIENT * UberSnipAPI = new KEENIO_CLIENT();

				UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + this->_id;
				UberSnipAPI->Http->addParam("api_key", this->MasterKey);

				UberSnipAPI->Http->request();


				cJSON* response = cJSON_Parse(_StringToAscIIChars(UberSnipAPI->Client->BodyResponse));

				cJSON* arr_events ;

				try {
					arr_events = cJSON_GetObjectItem(response, "events");
				}
				catch (std::exception_ptr e) {
					this->verified = false;
					this->error = true;
					return;
				}

				if(arr_events == nullptr) {
					this->verified = false;
					this->error = true;
					return;
				}

				for (cJSON* c_event = arr_events->child; c_event != nullptr; c_event = c_event->next) {
					KEENIO_EVENT^ k_event = ref new KEENIO_EVENT();
					k_event->Name = _String(cJSON_GetObjectItem(c_event, "name")->valuestring);
					k_event->URL = _String(cJSON_GetObjectItem(c_event, "url")->valuestring);
					k_event->Auth = this->auth;
					k_event->ProjectID = this->_id;
					k_event->LoadProperties();
					this->_events->Append(k_event);
				}

				this->_name = _String(cJSON_GetObjectItem(response, "name")->valuestring);
				this->_org_id = _String(cJSON_GetObjectItem(response, "organization_id")->valuestring);
				this->_partners_url = _String(cJSON_GetObjectItem(response, "partners_url")->valuestring);
				this->_queries_url = _String(cJSON_GetObjectItem(response, "queries_url")->valuestring);
				this->_url = _String(cJSON_GetObjectItem(response, "url")->valuestring);

				this->verified = true;
				this->error = false;
			}

			void SendEvent(Platform::String^ collection, Platform::String^ event_name, Platform::String^ event_property) {

				if (!this->verified) {return;}

				KeenIOAPI::KEENIO_CLIENT * UberSnipAPI = new KeenIOAPI::KEENIO_CLIENT();

				UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + this->_id + "/events/" + collection;
				UberSnipAPI->Http->addParam("api_key", this->MasterKey);

				UberSnipAPI->Http->addDataParam("data", event_name, event_property);
				UberSnipAPI->Http->request();
			}

			property Platform::String^ EventsURL {
				Platform::String^ get() {
					return this->_events_url;
				}

				void set(Platform::String^ val) {
					this->_events_url = val;
				}
			}

			property Platform::String^ ID {
				Platform::String^ get() {
					return this->_id;
				}

				void set(Platform::String^ val) {
					this->_id = val;
				}
			}

			property Platform::String^ OrganizationID {
				Platform::String^ get() {
					return this->_org_id;
				}

				void set(Platform::String^ val) {
					this->_org_id = val;
				}
			}

			property Platform::String^ Name {
				Platform::String^ get() {
					return this->_name;
				}

				void set(Platform::String^ val) {
					this->_name = val;
				}
			}

			property Platform::String^ QueriesURL {
				Platform::String^ get() {
					return this->_queries_url;
				}

				void set(Platform::String^ val) {
					this->_queries_url = val;
				}
			}

			property Platform::String^ PartnersURL {
				Platform::String^ get() {
					return this->_partners_url;
				}

				void set(Platform::String^ val) {
					this->_partners_url = val;
				}
			}

			property Platform::String^ URL {
				Platform::String^ get() {
					return this->_url;
				}

				void set(Platform::String^ val) {
					this->_url = val;
				}
			}

			property Platform::String^ MasterKey {
				Platform::String^ get() {
					return this->auth->MasterKey;
				}

				void set(Platform::String^ val) {
					this->auth->MasterKey = val;
				}
			}

			property Windows::UI::Xaml::Interop::IBindableObservableVector^ Events {
				Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
					return this->_events;
				}
			}
		};

	}

	[Windows::UI::Xaml::Data::Bindable]
	public ref class UBERSNIP_USER sealed {
		Platform::String^ _Username;
		Platform::String^ _FirstName;
		Platform::String^ _LastName;
		Platform::String^ _Country;
		Platform::String^ _City;
		Platform::String^ _Website;
		Platform::String^ _Description;
		Platform::String^ _TwitterHandle;
		Platform::String^ _FacebookHandle;
		Platform::String^ _YouTubeHandle;
		Platform::String^ _SoundCloudHandle;
		Windows::UI::Xaml::Media::ImageSource^ _Image;
		Windows::UI::Xaml::Media::ImageSource^ _ImageCover;
		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}

	public:
		UBERSNIP_USER() {

		}

		property Platform::String^ Username {
			Platform::String^ get() {
				return this->_Username;
			}

			void set(Platform::String^ val) {
				this->_Username = val;
				this->OnPropertyChanged("Username");
			}
		}

		property Platform::String^ FirstName {
			Platform::String^ get() {
				return this->_FirstName;
			}

			void set(Platform::String^ val) {
				this->_FirstName = val;
				this->OnPropertyChanged("FirstName");
			}
		}

		property Platform::String^ LastName {
			Platform::String^ get() {
				return this->_LastName;
			}

			void set(Platform::String^ val) {
				this->_LastName = val;
				this->OnPropertyChanged("LastName");
			}
		}

		property Platform::String^ Country {
			Platform::String^ get() {
				return this->_Country;
			}

			void set(Platform::String^ val) {
				this->_Country = val;
				this->OnPropertyChanged("Country");
			}
		}

		property Platform::String^ City {
			Platform::String^ get() {
				return this->_City;
			}

			void set(Platform::String^ val) {
				this->_City = val;
				this->OnPropertyChanged("City");
			}
		}

		property Platform::String^ Website {
			Platform::String^ get() {
				return this->_Website;
			}

			void set(Platform::String^ val) {
				this->_Website = val;
				this->OnPropertyChanged("Website");
			}
		}

		property Platform::String^ Description {
			Platform::String^ get() {
				return this->_Description;
			}

			void set(Platform::String^ val) {
				this->_Description = val;
				this->OnPropertyChanged("Description");
			}
		}

		property Platform::String^ Twitter {
			Platform::String^ get() {
				return this->_TwitterHandle;
			}

			void set(Platform::String^ val) {
				this->_TwitterHandle = val;
				this->OnPropertyChanged("Twitter");
			}
		}

		property Platform::String^ Facebook {
			Platform::String^ get() {
				return this->_FacebookHandle;
			}

			void set(Platform::String^ val) {
				this->_FacebookHandle = val;
				this->OnPropertyChanged("Facebook");
			}
		}

		property Platform::String^ YouTube {
			Platform::String^ get() {
				return this->_YouTubeHandle;
			}

			void set(Platform::String^ val) {
				this->_YouTubeHandle = val;
				this->OnPropertyChanged("YouTube");
			}
		}

		property Platform::String^ SoundCloud {
			Platform::String^ get() {
				return this->_SoundCloudHandle;
			}

			void set(Platform::String^ val) {
				this->_SoundCloudHandle = val;
				this->OnPropertyChanged("SoundCloud");
			}
		}

		//USER ACCOUNT AVATAR
		property Windows::UI::Xaml::Media::ImageSource^ Image {
			Windows::UI::Xaml::Media::ImageSource^ get() {
				return this->_Image;
			}
		}

		//USER ACCOUNT COVER IMAGE
		property Windows::UI::Xaml::Media::ImageSource^ CoverImage {
			Windows::UI::Xaml::Media::ImageSource^ get() {
				return this->_ImageCover;
			}
		}

		void SetImage(Platform::String^ path, Platform::String^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			if (img == "Image") {
				this->_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
			}
			else {
				this->_ImageCover = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
			}
			this->OnPropertyChanged("Image");
		}

		void SetImageURI(Platform::String^ path, Platform::String^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			if (img == "Image") {
				this->_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
			}
			else {
				this->_ImageCover = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
			}
			this->OnPropertyChanged("Image");
		}
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class UBERSNIP_ACCOUNT sealed {

		Platform::String^ _Username;
		Platform::String^ _UID;
		Platform::String^ _Password;
		Platform::String^ _Token;

		Windows::UI::Xaml::Media::ImageSource^ _Image;
		Windows::UI::Xaml::Media::ImageSource^ _ImageCover;

		bool loggedIn;
		bool err = false;

		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks;

		Platform::Object^ accMGR;

		void SetImage(Platform::String^ path, Platform::String^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			if (img == "Image") {
				this->_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
				OnPropertyChanged("Image");
			}
			else {
				this->_ImageCover = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
				OnPropertyChanged("ImageCover");
			}
		}

		void SetImageURI(Platform::String^ path, Platform::String^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			if (img == "Image") {
				this->_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
				OnPropertyChanged("Image");
			}
			else {
				this->_ImageCover = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
				OnPropertyChanged("ImageCover");
			}
		}

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:

		UBERSNIP_ACCOUNT() {
			this->tracks = ref new Platform::Collections::Vector<Platform::Object^>();
		}


		void login() {

			UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();
			UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/login.php";
			UberSnipAPI->Http->addParam("username", this->_Username);
			UberSnipAPI->Http->addParam("password", this->_Password);
			UberSnipAPI->Http->request();

			int err = UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).find("__api_err");

			if (err < 0) {
				if (UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).length() < 3) {
					this->err = true;
					return;
				}
			}
			else {
				this->err = true;
			}

			if (err < 0) {
				this->loggedIn = true;

				cJSON* ubs_data = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());

				/*UBERSNIP_ACCOUNT^ myacc = UserByUsername(username);

				if (myacc == nullptr) {
				myacc = ref new UBERSNIP_ACCOUNT();
				}
				else {

				}*/

				this->_Token = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(ubs_data, "token")->valuestring);
				//this->_UID = STRING_UTILS::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "uid")->valuestring);
				this->SetImageURI("https://ubersnip.com//thumb.php?src=" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "image")->valuestring) + "&t=m&w=112&h=112", "Image");
				this->SetImageURI("https://ubersnip.com//thumb.php?src=" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "cover")->valuestring) + "&t=m&w=112&h=112", "CoverImage");
				this->_UID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "idu")->valuestring);
				Platform::String^ bio = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "bio")->valuestring);

				//this->_profile->SetAvatar(avatar);
				//this->_profile->SetCoverImage(coverimage);
				//this->_profile->Bio = bio;
				//this->_profile->Username = username;

				//APP_SETTINGS::WriteProperty("AuthToken", this->Token);

			}
			//return err;
		}

		//STORES ACCOUNT USERNAME
		property Platform::String^ Username {
			Platform::String^ get() {
				return this->_Username;
			}

			void set(Platform::String^ uname) {
				this->_Username = uname;
			}
		}

		property bool Error {
			bool get() {
				return this->err;
			}
		}

		//STORES ACCOUNT UID
		property Platform::String^ UID {
			Platform::String^ get() {
				return this->_UID;
			}

			void set(Platform::String^ uname) {
				this->_UID = uname;
			}
		}

		//STORES ACCOUNT AVATAR
		property Windows::UI::Xaml::Media::ImageSource^ Image {
			Windows::UI::Xaml::Media::ImageSource^ get() {
				return this->_Image;
			}
		}

		//STORES ACCOUNT COVER IMAGE
		property Windows::UI::Xaml::Media::ImageSource^ CoverImage {
			Windows::UI::Xaml::Media::ImageSource^ get() {
				return this->_ImageCover;
			}
		}

		//STORES ACCOUNT PASSWORD
		property Platform::String^ Password {
			Platform::String^ get() {
				return this->_Password;
			}

			void set(Platform::String^ uname) {
				this->_Password = uname;
			}
		}

		//IS USER LOGGEDIN?
		property bool LoggedIn {
			bool get() {
				return this->loggedIn;
			}
		}

		//READ ONLY
		//STORES AUTH TOKEN
		property Platform::String^ AuthToken {
			Platform::String^ get() {
				return this->_Token;
			}

			void set(Platform::String^ val) {
				this->_Token = val;
			}
		}

	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class UBERSNIP_ACCOUNTS_MANAGER sealed {
		//ACCOUNTS LIST
		Windows::UI::Xaml::Interop::IBindableObservableVector^ accounts;

		//ALLOWS US TO KEEP TRACK OF CURRENT ACCOUNT USERNAME
		UBERSNIP_ACCOUNT^ activeAccount;

		bool loggedIn = false;
		Platform::String^ LoggedInAs = "NaN";

	public:
		UBERSNIP_ACCOUNTS_MANAGER() {
			this->accounts = ref new Platform::Collections::Vector<UBERSNIP_ACCOUNT^>();
		}


		//RETURN AN ACCOUNT BY USERNAME
		UBERSNIP_ACCOUNT^ UserByUsername(Platform::String^ uname) {

			for (size_t i = 0; i < this->accounts->Size; i++) {
				UBERSNIP_ACCOUNT^ acc = dynamic_cast<UBERSNIP_ACCOUNT^>(this->accounts->GetAt(i));
				if (acc->Username == uname) {
					return dynamic_cast<UBERSNIP_ACCOUNT^>(this->accounts->GetAt(i));
				}
			}

			return nullptr;
		}

		//RETURN AN ACCOUNT BY ID
		UBERSNIP_ACCOUNT^ UserByID(int i) {

			return dynamic_cast<UBERSNIP_ACCOUNT^>(this->accounts->GetAt(i));

		}

		property UBERSNIP_ACCOUNT^ ActiveAccount {
			UBERSNIP_ACCOUNT^ get() {
				return this->activeAccount;
			}

			void set(UBERSNIP_ACCOUNT^ acc) {
				this->activeAccount = acc;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Accounts {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->accounts;
			}
		}
	};


	[Windows::UI::Xaml::Data::Bindable]
	public ref class UBERSNIP_LIKE sealed {
	private:
		Platform::String^ _ID;
		Platform::String^ _UID;
		Platform::String^ _OWNER;
		Windows::UI::Xaml::Media::ImageSource^ _Image;

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:
		UBERSNIP_LIKE() {

		};

		property Platform::String^ ID {
			Platform::String^ get() {
				return this->_ID;
			}

			void set(Platform::String^ val) {
				this->_ID = val;
				OnPropertyChanged("ID");
			}
		}

		property Platform::String^ Owner {
			Platform::String^ get() {
				return this->_OWNER;
			}

			void set(Platform::String^ val) {
				this->_OWNER = val;
				OnPropertyChanged("Owner");
			}
		}

		property Platform::String^ OwnerID {
			Platform::String^ get() {
				return this->_UID;
			}

			void set(Platform::String^ val) {
				this->_UID = val;
				OnPropertyChanged("OwnerID");
			}
		}

		//Image
		property Windows::UI::Xaml::Media::ImageSource^ Image
		{
			Windows::UI::Xaml::Media::ImageSource^ get()
			{
				return _Image;
			}
			void set(Windows::UI::Xaml::Media::ImageSource^ value)
			{
				_Image = value;
				OnPropertyChanged("Image");
			}
		}

		void SetImage(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

		void SetImageURI(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}
	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class UBERSNIP_LIKELIST sealed {
	private:
		Windows::UI::Xaml::Interop::IBindableObservableVector^ likes = ref new Platform::Collections::Vector<UBERSNIP_LIKE^>();

	public:
		UBERSNIP_LIKELIST() {
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Likes {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->likes;
			}
		}
	};



	[Windows::UI::Xaml::Data::Bindable]
	public ref class UBERSNIP_COMMENT sealed {
	private:
		Platform::String^ _ID;
		Platform::String^ _OWNER;
		Platform::String^ _UID;
		Platform::String^ _commentText;
		Windows::UI::Xaml::Media::ImageSource^ _Image;

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:
		UBERSNIP_COMMENT() {

		};

		property Platform::String^ ID {
			Platform::String^ get() {
				return this->_ID;
			}

			void set(Platform::String^ val) {
				this->_ID = val;
				OnPropertyChanged("ID");
			}
		}

		property Platform::String^ Owner {
			Platform::String^ get() {
				return this->_OWNER;
			}

			void set(Platform::String^ val) {
				this->_OWNER = val;
				OnPropertyChanged("Owner");
			}
		}

		property Platform::String^ OwnerID {
			Platform::String^ get() {
				return this->_UID;
			}

			void set(Platform::String^ val) {
				this->_UID = val;
				OnPropertyChanged("OwnerID");
			}
		}

		property Platform::String^ CommentText {
			Platform::String^ get() {
				return this->_commentText;
			}

			void set(Platform::String^ val) {
				this->_commentText = val;
				OnPropertyChanged("CommentText");
			}
		}

		//Image
		property Windows::UI::Xaml::Media::ImageSource^ Image
		{
			Windows::UI::Xaml::Media::ImageSource^ get()
			{
				return _Image;
			}
			void set(Windows::UI::Xaml::Media::ImageSource^ value)
			{
				_Image = value;
				OnPropertyChanged("Image");
			}
		}

		void SetImage(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

		void SetImageURI(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}
	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class UBERSNIP_COMMENTLIST sealed {
	private:
		Windows::UI::Xaml::Interop::IBindableObservableVector^ comments;

	public:
		UBERSNIP_COMMENTLIST() {
			comments = ref new Platform::Collections::Vector<UBERSNIP_COMMENT^>();
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Comments {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->comments;
			}
		}
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class UBERSNIP_TRACK sealed {
	private:
		Platform::String^ _ID;
		Platform::String^ _uid;
		Platform::String^ _artist;
		Platform::String^ _name;
		Platform::String^ _album;
		Platform::String^ _title;
		Platform::String^ _description;
		Platform::String^ _filename;
		Platform::String^ _record;
		Platform::String^ _release;
		Platform::String^ _views;
		Platform::String^ _likes;
		Platform::String^ _tags;
		Platform::String^ _filesize;
		Platform::String^ _creationTime;
		Windows::UI::Xaml::Media::ImageSource^ _Image;
		Windows::UI::Xaml::Visibility _visible = Windows::UI::Xaml::Visibility::Visible;

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:
		UBERSNIP_TRACK() {

		};

		property Platform::String^ ID {
			Platform::String^ get() {
				return this->_ID;
			}

			void set(Platform::String^ val) {
				this->_ID = val;
				OnPropertyChanged("ID");
			}
		}

		property Platform::String^ UID {
			Platform::String^ get() {
				return this->_uid;
			}

			void set(Platform::String^ val) {
				this->_uid = val;
				OnPropertyChanged("UID");
			}
		}

		property Platform::String^ Artist {
			Platform::String^ get() {
				return this->_artist;
			}

			void set(Platform::String^ val) {
				this->_artist = val;
				OnPropertyChanged("Artist");
			}
		}

		property Platform::String^ Record {
			Platform::String^ get() {
				return this->_record;
			}

			void set(Platform::String^ val) {
				this->_record = val;
				OnPropertyChanged("Record");
			}
		}

		property Platform::String^ Release {
			Platform::String^ get() {
				return this->_release;
			}

			void set(Platform::String^ val) {
				this->_release = val;
				OnPropertyChanged("Release");
			}
		}

		property Platform::String^ Album {
			Platform::String^ get() {
				return this->_album;
			}

			void set(Platform::String^ val) {
				this->_album = val;
				OnPropertyChanged("Album");
			}
		}

		property Platform::String^ CreationTime {
			Platform::String^ get() {
				return this->_creationTime;
			}

			void set(Platform::String^ val) {
				this->_creationTime = val;
				OnPropertyChanged("CreationTime");
			}
		}

		property Platform::String^ Name {
			Platform::String^ get() {
				return this->_name;
			}

			void set(Platform::String^ val) {
				this->_name = val;
				OnPropertyChanged("Name");
			}
		}

		property Platform::String^ Tags {
			Platform::String^ get() {
				return this->_tags;
			}

			void set(Platform::String^ val) {
				this->_tags = val;
				OnPropertyChanged("Tags");
			}
		}


		property Platform::String^ Title {
			Platform::String^ get() {
				return this->_title;
			}

			void set(Platform::String^ val) {
				this->_title = val;
				OnPropertyChanged("Title");
			}
		}


		property Platform::String^ Description {
			Platform::String^ get() {
				return this->_description;
			}

			void set(Platform::String^ val) {
				this->_description = val;
				OnPropertyChanged("Description");
			}
		}


		property Platform::String^ FileName {
			Platform::String^ get() {
				return this->_filename;
			}

			void set(Platform::String^ val) {
				this->_filename = val;
				OnPropertyChanged("FileName");
			}
		}


		property Platform::String^ Views {
			Platform::String^ get() {
				return this->_views;
			}

			void set(Platform::String^ val) {
				this->_views = val;
				OnPropertyChanged("Views");
			}
		}

		property Platform::String^ FileSizeInBytes {
			Platform::String^ get() {
				return this->_filesize;
			}

			void set(Platform::String^ val) {
				this->_filesize = val;
				OnPropertyChanged("FileSizeInBytes");
			}
		}


		property Platform::String^ Likes {
			Platform::String^ get() {
				return this->_likes;
			}

			void set(Platform::String^ val) {
				this->_likes = val;
				OnPropertyChanged("Likes");
			}
		}

		//IS TRACK VISIBLE
		property Windows::UI::Xaml::Visibility Active {
			Windows::UI::Xaml::Visibility get() {
				return this->_visible;
			}

			void set(Windows::UI::Xaml::Visibility val){
				this->_visible = val;
			}
		}

		//Image
		property Windows::UI::Xaml::Media::ImageSource^ Image
		{
			Windows::UI::Xaml::Media::ImageSource^ get()
			{
				return _Image;
			}
			void set(Windows::UI::Xaml::Media::ImageSource^ value)
			{
				_Image = value;
				OnPropertyChanged("Image");
			}
		}

		void SetImage(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

		void SetImageURI(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}
	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class UBERSNIP_TRACKLIST sealed {
	private:
		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks;

	public:
		UBERSNIP_TRACKLIST() {
			tracks = ref new Platform::Collections::Vector<UBERSNIP_TRACK^>();
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Tracks {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->tracks;
			}
		}
	};

	[Windows::UI::Xaml::Data::Bindable]
	public ref class GENERIC_ITEM sealed {
	private:
		Platform::String^ _title = "";
		Platform::String^ _description;
		Windows::UI::Xaml::Media::ImageSource^ _Image;

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:
		GENERIC_ITEM() {

		};

		property Platform::String^ Title {
			Platform::String^ get() {
				return this->_title;
			}

			void set(Platform::String^ val) {
				this->_title = val;
				OnPropertyChanged("Title");
			}
		}

		property Platform::String^ Description {
			Platform::String^ get() {
				return this->_description;
			}

			void set(Platform::String^ val) {
				this->_description = val;
				OnPropertyChanged("Description");
			}
		}

		void SetImage(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class GENERIC_DATA sealed {
	private:
		Windows::UI::Xaml::Interop::IBindableObservableVector^ items;

	public:
		GENERIC_DATA() {
			items = ref new Platform::Collections::Vector<GENERIC_ITEM^>();
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Items {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->items;
			}
		}
	};


	ref class App sealed
	{
	public:
		property Platform::String^ KeenIOMasterKey {
			static Platform::String^ get() {
				return "";
			}
		}
		property Platform::String^ KeenIOProjectID {
			static Platform::String^ get() {
				return "";
			}
		}

	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	internal:
		App();

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
	};

	namespace UberSnip {
		namespace HELPER {


			public ref class ASYNC_RESPONSE sealed {
				Platform::String^ _body;

			public:
				ASYNC_RESPONSE(){}

				property Platform::String^ Body {
					Platform::String^ get() {
						return this->_body;
					}

					void set(Platform::String^ val) {
						this->_body = val;
					}
				}
			};

			public ref class Accounts sealed{


			public :
				static UBERSNIP_ACCOUNT^ login(Platform::String^ username, Platform::String^ password) {
					UBERSNIP_ACCOUNT^ myacc;
					myacc->Username = username;
					myacc->Password = password;
					myacc->login();
					return myacc;
				}

				static UBERSNIP_USER^ getUserData(Platform::String^ username_or_id, Platform::String^ method) {
					UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

					UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/users.php";
					if (method == "username") {
						UberSnipAPI->Http->addParam("username", username_or_id);
					}
					else if (method == "UID") {
						UberSnipAPI->Http->addParam("UID", username_or_id);
					}
					else {
						UberSnipAPI->Http->addParam("UID", username_or_id);
					}

					UberSnipAPI->Http->request();

					int err = UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).find("__api_err");

					if (err < 0) {
						if (UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).length() < 3) {
							return nullptr;
						}
					}


					cJSON* user_data = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());
					UBERSNIP_USER^ user = ref new UBERSNIP_USER();
					user->City = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "city")->valuestring);
					user->Country = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "country")->valuestring);
					user->Description = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "description")->valuestring);
					user->Facebook = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "facebook")->valuestring);
					user->FirstName = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "first_name")->valuestring);
					user->LastName = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "last_name")->valuestring);
					user->SoundCloud = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "soundcloud")->valuestring);
					user->Twitter = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "twitter")->valuestring);
					user->Username = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "username")->valuestring);
					user->Website = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "website")->valuestring);
					user->YouTube = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "youtube")->valuestring);
					user->SetImageURI("https://ubersnip.com//thumb.php?src=" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "image")->valuestring) + "&t=a&w=112&h=112", "Image");
					user->SetImageURI("https://ubersnip.com//thumb.php?src=" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(user_data, "cover")->valuestring) + "&t=c&w=1100&h=200", "CoverImage");

					return user;
				}

				Accounts() {

				}
			};

			public ref class KeenIO sealed {
			public:
				static void SendData(Platform::String^ proj_id, Platform::String^ collection, Platform::String^ event_name, Platform::String^ event_property) {
					KeenIOAPI::KEENIO_CLIENT * UberSnipAPI = new KeenIOAPI::KEENIO_CLIENT();

					UberSnipAPI->Http->RequestURL = "https://api.keen.io/3.0/projects/" + proj_id + "/events/" + collection;
					UberSnipAPI->Http->addParam("api_key", App::KeenIOMasterKey);

					UberSnipAPI->Http->addDataParam("data", event_name, event_property);
					UberSnipAPI->Http->request();

				}

				static Platform::String^ ReceiveData(Platform::String^ url) {
					KeenIOAPI::KEENIO_CLIENT * UberSnipAPI = new KeenIOAPI::KEENIO_CLIENT();

					UberSnipAPI->Http->RequestURL = url;
					UberSnipAPI->Http->request();

					return UberSnipAPI->Client->BodyResponse;

				}
			};

			public ref class Comments sealed{


			public :
				static void post_comment(Platform::String^ token, Platform::String^ track_id, Platform::String^ message) {
					
					UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

					UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/request/add_comment.php";
					UberSnipAPI->Http->addDefHeaders();
					UberSnipAPI->Http->addParam("token", token);
					UberSnipAPI->Http->addParam("track_id", track_id);
					UberSnipAPI->Http->addParam("message", UberSnip::UTILS::STRING::URLEncode(UberSnip::UTILS::STRING::StringToAscIIChars(message)));
					UberSnipAPI->Http->request();
				}

				Comments() {

				}
			};

			public ref class LocalStorage sealed {
				static Platform::Object^ ReadProperty(Platform::String^ s) {


					Windows::Storage::ApplicationDataContainer^ localSettings =
						Windows::Storage::ApplicationData::Current->LocalSettings;
					Windows::Storage::StorageFolder^ localFolder =
						Windows::Storage::ApplicationData::Current->LocalFolder;


					return localSettings->Values->Lookup(s);
				}

				static void WriteProperty(Platform::String^ key, Platform::Object^ data) {

					Windows::Storage::ApplicationDataContainer^ localSettings =
						Windows::Storage::ApplicationData::Current->LocalSettings;
					Windows::Storage::StorageFolder^ localFolder =
						Windows::Storage::ApplicationData::Current->LocalFolder;

					Platform::Object^ mdata = dynamic_cast<Platform::String^>(data);
					localSettings->Values->Insert(key, mdata);
				}
			};
		}
	}
}
