﻿//
// MyFavoritesStream.xaml.cpp
// Implementation of the MyFavoritesStream class
//

#include "pch.h"
#include "StreamView.xaml.h"
#include "MyFavoritesStream.xaml.h"
#include "TrackDisplayPage.xaml.h"

using namespace UXBlumIO;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

UBERSNIP_ACCOUNTS_MANAGER^ accManager;

MyFavoritesStream::MyFavoritesStream()
{
	InitializeComponent();
	this->_trackList = ref new UBERSNIP_TRACKLIST();
	//Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs^>(this, &MyFavoritesStream::GoBack);

}

void MyFavoritesStream::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	/*DATA_UTILS::KeenIOCollectionData2^ it;
	if (dynamic_cast<DATA_UTILS::KeenIOCollectionData2^>(e->Parameter) != nullptr) {
		it = dynamic_cast<DATA_UTILS::KeenIOCollectionData2^>(e->Parameter);
	}*/

	MainPage^ mp = dynamic_cast<MainPage^>(e->Parameter);

	if (mp != nullptr) {
		this->rootPage = mp;
		for (size_t i = 0; i < this->rootPage->TrackList->Size; i++)
		{
			UBERSNIP_TRACK^ tmptrack = dynamic_cast<UBERSNIP_TRACK^>(mp->TrackList->GetAt(i));
			if (tmptrack->Active != Windows::UI::Xaml::Visibility::Collapsed) {
				this->TrackList->Tracks->Append(mp->TrackList->GetAt(i));
			}
		}
	}

	int* x = 0;
	x++;

	return;

}

void UXBlumIO::MyFavoritesStream::gridView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	UBERSNIP_TRACK^ track = dynamic_cast<UBERSNIP_TRACK^>(gridView->SelectedItem);
	
	if (track != nullptr) {
		Windows::UI::Xaml::Interop::IBindableObservableVector^ track_data = ref new Platform::Collections::Vector<Platform::Object^>;
		track_data->Append(this->rootPage);
		track_data->Append(track);
		this->rootPage->Frame->Navigate(TrackDisplayPage::typeid, track_data);
	}
	
}
