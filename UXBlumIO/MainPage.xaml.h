//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "MyFavoritesStream.xaml.h"


namespace UXBlumIO
{

	
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>


	public ref class MainPage sealed
	{
		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks = ref new Platform::Collections::Vector<UBERSNIP_TRACK^>();
		Windows::UI::Xaml::Interop::IBindableObservableVector^ categories = ref new Platform::Collections::Vector<GENERIC_ITEM^>();

		UBERSNIP_ACCOUNTS_MANAGER^ accManager = ref new UBERSNIP_ACCOUNTS_MANAGER();

	public:
		MainPage();

		property double PageWidth {
			double get() {
				return this->Frame->Width;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ TrackList {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->tracks;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Categories {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->categories;
			}
		}

		property UBERSNIP_ACCOUNTS_MANAGER^ AccountManager {
			UBERSNIP_ACCOUNTS_MANAGER^ get() {
				return this->accManager;
			}

			void set(UBERSNIP_ACCOUNTS_MANAGER^ val) {
				this->accManager = val;
			}
		}

		property Windows::UI::Xaml::Controls::Frame^ ContentFrame {
			Windows::UI::Xaml::Controls::Frame^ get() {
				return this->contentFrame;
			}
		}

		property Windows::UI::Xaml::Controls::ComboBox^ AccountAddedUsers {
			Windows::UI::Xaml::Controls::ComboBox^ get() {
				return this->menu_create_tb;
			}
		}

		property Windows::UI::Xaml::Controls::MediaElement^ MediaPlayer {
			Windows::UI::Xaml::Controls::MediaElement^ get() {
				return this->mediaElement;
			}
		}

		property bool StreamToggleSwitch {
			bool get() {
				return this->toggleButton->IsOn;
			}

			void set(bool on) {
				this->toggleButton->IsOn = on;
			}
		}

		void FavTracks() {
			this->loadUserFavTracks(this->AccountManager->ActiveAccount->UID);
		}

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void ic_menu_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void ic_menu_add_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void HamburgerButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadTracks();
		void toggleButton_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadUserTracks(Platform::String^ user_id);
		void loadMyTracks();
		void restoreTracks();
		void loadUserFavTracks(Platform::String^ user_id);
		void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
