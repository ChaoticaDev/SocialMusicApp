//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "MyFavoritesStream.xaml.h"
#include "UBS_SDK.h"
#include "external\JSON\cJSON.h"

using namespace Windows::UI::Xaml::Data;


namespace UXBlumIO
{

	
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>

	public ref class UBERSNIP_ACCOUNT sealed {

		Platform::String^ _Username;
		Platform::String^ _UID;
		Platform::String^ _Password;
		Platform::String^ _Token;

		Windows::UI::Xaml::Media::ImageSource^ _Image;
		Windows::UI::Xaml::Media::ImageSource^ _ImageCover;

		bool loggedIn;

		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks;

		Platform::Object^ accMGR;

		void SetImage(Platform::String^ path, Windows::UI::Xaml::Media::ImageSource^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri("ms-appx://UXBlumIO/Assets/" + path);
			img = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

		void SetImageURI(Platform::String^ path, Windows::UI::Xaml::Media::ImageSource^ img)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			img = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}
	public:

		UBERSNIP_ACCOUNT() {
			this->tracks = ref new Platform::Collections::Vector<Platform::Object^>();
		}


		void login() {

			UberSnip::UBERSNIP_CLIENT* uCLIENT = new UberSnip::UBERSNIP_CLIENT();
			uCLIENT->Http->RequestURL = "http://api.ubersnip.com/login.php";
			uCLIENT->Http->addParam("username", this->_Username);
			uCLIENT->Http->addParam("password", this->_Password);
			uCLIENT->Http->request();

			int err = UberSnip::UTILS::STRING::StringToAscIIChars(uCLIENT->Client->BodyResponse).find("__api_err");

			if (err < 0) {
				this->loggedIn = true;

				cJSON* ubs_data = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(uCLIENT->Client->BodyResponse).c_str());

				/*UBERSNIP_ACCOUNT^ myacc = UserByUsername(username);

				if (myacc == nullptr) {
					myacc = ref new UBERSNIP_ACCOUNT();
				}
				else {

				}*/

				this->_Token = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(ubs_data, "token")->valuestring);
				//this->_UID = STRING_UTILS::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "uid")->valuestring);

				this->SetImageURI( UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "avatar")->valuestring), this->_Image );
				this->SetImageURI( UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(ubs_data, "user"), "cover")->valuestring), this->_ImageCover );
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

	public ref class UBERSNIP_ACCOUNTS_MANAGER sealed{
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

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Accounts{
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->accounts;
			}
		}
	};

	public ref class MainPage sealed
	{
		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks = ref new Platform::Collections::Vector<UBERSNIP_TRACK^>();
		UBERSNIP_ACCOUNTS_MANAGER^ accManager = ref new UBERSNIP_ACCOUNTS_MANAGER();
		

	public:
		MainPage();

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ TrackList {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->tracks;
			}
		}

		property UBERSNIP_ACCOUNTS_MANAGER^ AccountManager {
			UBERSNIP_ACCOUNTS_MANAGER^ get() {
				return this->accManager;
			}
		}

		property Windows::UI::Xaml::Controls::Frame^ ContentFrame {
			Windows::UI::Xaml::Controls::Frame^ get() {
				return this->contentFrame;
			}
		}

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void ic_menu_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void ic_menu_add_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void HamburgerButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadTracks();
	};
}
