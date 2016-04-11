//
// UserProfilePage.xaml.h
// Declaration of the UserProfilePage class
//

#pragma once

#include "UserProfilePage.g.h"
#include "MainPage.xaml.h"
#include "TrackDisplayPage.xaml.h"

namespace UXBlumIO
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class UserProfilePage sealed
	{

		UBERSNIP_TRACKLIST^ _trackList;
		UBERSNIP_TRACK^ ref_track;
		UBERSNIP_USER^ _user;
		MainPage^ rootPage;

		void goBack(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args) {
			try {
				Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = Windows::UI::Core::AppViewBackButtonVisibility::Collapsed;

				Windows::UI::Xaml::Interop::IBindableObservableVector^ params = ref new Platform::Collections::Vector<Platform::Object^>();
				params->Append(this->rootPage);
				params->Append(this->ref_track);
				this->Frame->Navigate(TrackDisplayPage::typeid, params);
			}
			catch (Platform::DisconnectedException^ e) {

			}

		}
	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	public:
		UserProfilePage();

		property UBERSNIP_USER^ User {
			UBERSNIP_USER^ get() {
				return this->_user;
			}

			void set(UBERSNIP_USER^ val) {
				this->_user = val;
			}
		}

		property UBERSNIP_TRACKLIST^ TrackList {
			UBERSNIP_TRACKLIST^ get() {
				return this->_trackList;
			}

			void set(UBERSNIP_TRACKLIST^ tList) {
				this->_trackList = tList;
			}
		}
	};
}
