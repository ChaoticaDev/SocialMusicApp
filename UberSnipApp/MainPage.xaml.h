//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once


using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

#include "MainPage.g.h"
#include "KeenIOSDK_CLIENT.h"
#include "PlatformStringUtils.h"

#include "cJSON/cJSON.h"

namespace UberSnipApp
{

	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>

	public ref class UBERSNIP_LOGIN_HANDLER sealed{

	private:
		Platform::String^ _username;
		Platform::String^ _password;
		Platform::String^ _LoggedInAs;
		Platform::String^ _UID = "0";
		Platform::String^ _token;
		bool loggedIn = false;
	public:


		property Platform::String^ Username {
			Platform::String^ get() {
				return this->_username;
			}

			void set(Platform::String^ val) {
				this->_username = val;
			}
		}

		int login(Platform::String^ username, Platform::String^ password) {

			UBERSNIP_CLIENT* uCLIENT = new UBERSNIP_CLIENT();
			uCLIENT->kHTTP.reqURL = "http://api.ubersnip.com/login.php";
			uCLIENT->kHTTP.addParam("username", STRING_UTILS::StringToAscIIChars(username));
			uCLIENT->kHTTP.addParam("password", STRING_UTILS::StringToAscIIChars(password));
			uCLIENT->request(uCLIENT->kHTTP);

			int err = uCLIENT->body.find("__api_err");

			if (err < 0) {
				this->loggedIn = true;
				this->LoggedInAs = username;

				cJSON* ubs_data = cJSON_Parse(uCLIENT->body.c_str());

				this->_token = STRING_UTILS::StringFromAscIIChars(cJSON_GetObjectItem(ubs_data, "token")->valuestring);
				this->_UID = STRING_UTILS::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "uid")->valuestring);
			}
			return err;
		}

		property Platform::String^ Password {
			Platform::String^ get() {
				return this->_password;
			}

			void set(Platform::String^ val) {
				this->_password = val;
			}
		}
	

		property Platform::String^ LoggedInAs {
			Platform::String^ get() {
				return this->_LoggedInAs;
			}

			void set(Platform::String^ val) {
				this->_LoggedInAs = val;
			}
		}


		property Platform::String^ UID {
			Platform::String^ get() {
				return this->_UID;
			}

			void set(Platform::String^ val) {
				this->_UID = val;
			}
		}

		property Platform::String^ Token {
			Platform::String^ get() {
				return this->_token;
			}
		}

		property bool LoggedIn {
			bool get() {
				return this->loggedIn;
			}

			/*void set(bool val) {
				this->loggedIn = val;
			}*/
		}
	};


	[Windows::UI::Xaml::Data::Bindable]
	public ref class UBERSNIP_TRACKSET sealed {
	private:
		Platform::String^ _ID;
		Platform::String^ _uid;
		Platform::String^ _artist;
		Platform::String^ _album;
		Platform::String^ _title;
		Platform::String^ _description;
		Platform::String^ _filename;
		Platform::String^ _views;
		Platform::String^ _likes;
		Platform::String^ _genre;
		Windows::UI::Xaml::Media::ImageSource^ _Image;

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:
		UBERSNIP_TRACKSET() {

		};

		property Platform::String^ ID {
			Platform::String^ get() {
				return this->_ID;
			}

			void set(Platform::String^ val) {
				this->_ID = val;
			}
		}

		property Platform::String^ UID {
			Platform::String^ get() {
				return this->_uid;
			}

			void set(Platform::String^ val) {
				this->_uid = val;
			}
		}

		property Platform::String^ Artist {
			Platform::String^ get() {
				return this->_artist;
			}

			void set(Platform::String^ val) {
				this->_artist = val;
			}
		}

		property Platform::String^ Album {
			Platform::String^ get() {
				return this->_album;
			}

			void set(Platform::String^ val) {
				this->_album = val;
			}
		}

		property Platform::String^ Genre {
			Platform::String^ get() {
				return this->_genre;
			}

			void set(Platform::String^ val) {
				this->_genre = val;
			}
		}


		property Platform::String^ Title {
			Platform::String^ get() {
				return this->_title;
			}

			void set(Platform::String^ val) {
				this->_title = val;
			}
		}


		property Platform::String^ Description {
			Platform::String^ get() {
				return this->_description;
			}

			void set(Platform::String^ val) {
				this->_description = val;
			}
		}


		property Platform::String^ FileName {
			Platform::String^ get() {
				return this->_filename;
			}

			void set(Platform::String^ val) {
				this->_filename = val;
			}
		}


		property Platform::String^ Views {
			Platform::String^ get() {
				return this->_views;
			}

			void set(Platform::String^ val) {
				this->_views = val;
			}
		}


		property Platform::String^ Likes {
			Platform::String^ get() {
				return this->_likes;
			}

			void set(Platform::String^ val) {
				this->_likes = val;
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
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class UBERSNIP_TRACKS sealed {
	private:
		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks;

	public:
		UBERSNIP_TRACKS() {
			tracks = ref new Platform::Collections::Vector<UBERSNIP_TRACKSET^>();
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
	

	///[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	[Windows::UI::Xaml::Data::Bindable]
	public ref class Item sealed
	{
		Platform::String^  _Title;
		Platform::String^ _TrackName;
		Platform::String^ _TrackAlbum;
		Platform::String^ _TrackArtist;
		Windows::UI::Xaml::Media::ImageSource^ _Image;
		event PropertyChangedEventHandler^ _PropertyChanged;


	public:

		Item();
		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}

		//Artist Name
		property Platform::String^ TrackName
		{
			Platform::String^ get()
			{
				return this->_TrackName;
			}
			void set(Platform::String^ value)
			{
				this->_TrackName = value;
				OnPropertyChanged("TrackName");
			}
		}


		//Album Name
		property Platform::String^ Title
		{
			Platform::String^ get()
			{
				return this->_Title;
			}
			void set(Platform::String^ value)
			{
				this->_Title = value;
				OnPropertyChanged("Title");
			}
		}


		//Title
		property Platform::String^ TrackAlbum
		{
			Platform::String^ get()
			{
				return this->_TrackAlbum;
			}
			void set(Platform::String^ value)
			{
				this->_TrackAlbum = value;
				OnPropertyChanged("TrackAlbum");
			}
		}


		//Track Owner : Stage name
		property Platform::String^ TrackArtist
		{
			Platform::String^ get()
			{
				return this->_TrackArtist;
			}
			void set(Platform::String^ value)
			{
				this->_TrackArtist = value;
				OnPropertyChanged("TrackArtist");
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
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class StoreData sealed
	{
		Windows::UI::Xaml::Interop::IBindableObservableVector^  _items;
		Windows::UI::Xaml::Interop::IBindableObservableVector^  _tracks;

	public:
		StoreData()
		{
			_items = ref new Platform::Collections::Vector<Item^>();
			Windows::Foundation::Uri^ _baseUri = ref new Windows::Foundation::Uri("ms-appx:///");
			Platform::String^ LONG_LOREM_IPSUM = "";//"Curabitur class aliquam vestibulum nam curae maecenas sed integer cras phasellus suspendisse quisque donec dis praesent accumsan bibendum pellentesque condimentum adipiscing etiam consequat vivamus dictumst aliquam duis convallis scelerisque est parturient ullamcorper aliquet fusce suspendisse nunc hac eleifend amet blandit facilisi condimentum commodo scelerisque faucibus aenean ullamcorper ante mauris dignissim consectetuer nullam lorem vestibulum habitant conubia elementum pellentesque morbi facilisis arcu sollicitudin diam cubilia aptent vestibulum auctor eget dapibus pellentesque inceptos leo egestas interdum nulla consectetuer suspendisse adipiscing pellentesque proin lobortis sollicitudin augue elit mus congue fermentum parturient fringilla euismod feugiat";

		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Items
		{
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get()
			{
				return _items;
			}

		}
	};
	public ref class MainPage sealed
	{

		StoreData^ dataSrc;
		UBERSNIP_TRACKS^ UberSnipTracks;
		GENERIC_DATA^ genericData;
		UBERSNIP_LOGIN_HANDLER^ _LoginManager;
	public:
		MainPage();

		property UBERSNIP_TRACKS^ DataSource
		{
			UBERSNIP_TRACKS^ get() { return this->UberSnipTracks; };

		}

		property GENERIC_DATA^ CategorySource
		{
			GENERIC_DATA^ get() { return this->genericData; };

		}

		property UBERSNIP_LOGIN_HANDLER^ LoginManager {
			UBERSNIP_LOGIN_HANDLER^ get() {
				return _LoginManager;
			}

			void set(UBERSNIP_LOGIN_HANDLER^ val) {
				this->_LoginManager = val;
			}
		}

	private:
		void ListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void activeTextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadInitialTracks(void);
		void loadInitialCategories(void);
		void image_Copy_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void siteLogin(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
