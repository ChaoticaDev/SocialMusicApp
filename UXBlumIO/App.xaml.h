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

		void SetImage(Platform::String^ path, Windows::UI::Xaml::Media::ImageSource^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			img = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
			OnPropertyChanged("Image");
		}

		void SetImageURI(Platform::String^ path, Windows::UI::Xaml::Media::ImageSource^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			img = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
			OnPropertyChanged("Image");
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
				this->SetImageURI("https://ubersnip.com//thumb.php?src=" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "image")->valuestring) + "&t=m&w=112&h=112", this->_Image);
				this->SetImageURI("https://ubersnip.com//thumb.php?src=" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "cover")->valuestring) + "&t=m&w=112&h=112", this->_ImageCover);
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
		Platform::String^ _title;
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

	namespace UberSnip {
		namespace HELPER {

			public ref class Accounts sealed{


			public :
				static UBERSNIP_ACCOUNT^ login(Platform::String^ username, Platform::String^ password) {
					UBERSNIP_ACCOUNT^ myacc;
					myacc->Username = username;
					myacc->Password = password;
					myacc->login();
					return myacc;
				}

				Accounts() {

				}
			};
		}
	}

	ref class App sealed
	{
	protected:
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	internal:
		App();

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnNavigationFailed(Platform::Object ^sender, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs ^e);
	};
}
