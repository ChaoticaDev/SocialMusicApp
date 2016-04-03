//
// MyFavoritesStream.xaml.h
// Declaration of the MyFavoritesStream class
//

#pragma once

#include "MyFavoritesStream.g.h"
#include "MainPage.xaml.h"

using namespace Windows::UI::Xaml::Data;


namespace UXBlumIO
{


	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MyFavoritesStream sealed
	{
		MainPage^ rootPage;
		UBERSNIP_TRACKLIST^ _trackList;

	private:
		void GoBack(void);
	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	public:
		MyFavoritesStream();

		property MainPage^ ROOT{
			MainPage^ get() {
				return this->rootPage;
			}
		}

		// PROPERTY ACCESSIBLE BY XAML CODE - STORES TRACK LIST
		property UBERSNIP_TRACKLIST^ TrackList {
			UBERSNIP_TRACKLIST^ get() {
				return this->_trackList;
			}

			void set(UBERSNIP_TRACKLIST^ tList) {
					this->_trackList = tList;
			}
		}

	private:
		void gridView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
	};
}
