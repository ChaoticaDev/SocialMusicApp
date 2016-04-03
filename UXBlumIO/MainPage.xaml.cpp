//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "MyFavoritesStream.xaml.h"
#include "LoginPage.xaml.h"

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{

	InitializeComponent();

	UBERSNIP_TRACK^ track = ref new UBERSNIP_TRACK();
	track->Artist = "K.Bust";
	track->Title = "Don't wanna try";
	track->SetImage("43.jpg");
	this->tracks->Append(track);

	this->loadTracks();

}

void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	
	this->contentFrame->Navigate(MyFavoritesStream::typeid, this);

	//return;

}

void UXBlumIO::MainPage::ic_menu_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{

}


void UXBlumIO::MainPage::ic_menu_add_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	this->contentFrame->Navigate(LoginPage::typeid, this);
}


void UXBlumIO::MainPage::HamburgerButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (this->splitView1->IsPaneOpen == true) {
		//this->splitView1->OpenPaneLength = 50;
		this->splitView1->IsPaneOpen = false;
	}
	else {
		//this->splitView1->OpenPaneLength = 150;
		this->splitView1->IsPaneOpen = true;
	}

	//this->splitView1->IsPaneOpen = !this->splitView1->IsPaneOpen;
}

void MainPage::loadTracks() {
	UberSnip::UBERSNIP_CLIENT* uCLIENT = new UberSnip::UBERSNIP_CLIENT();
	
	uCLIENT->Http->RequestURL = "http://api.ubersnip.com/tracks.php";
	uCLIENT->Http->request();



	cJSON* tracks = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(uCLIENT->Client->BodyResponse).c_str());
	tracks = cJSON_GetObjectItem(tracks, "tracks");
	int *track_count = new int(cJSON_GetArraySize(tracks));
	this->tracks->Clear();

	for (int i = 0; i < *track_count; i++) {
		cJSON* curr_track = tracks->child;
		for (int m = 0; m < i; m++) {
			curr_track = tracks->child->next;
		}


		Platform::String^ track_id = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "ID")->valuestring);
		Platform::String^ track_title = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "title")->valuestring);
		Platform::String^ track_cover = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "cover")->valuestring);
		Platform::String^ track_artist = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(curr_track, "user"), "username")->valuestring);

		UBERSNIP_TRACK^ ubersnipTrack = ref new UBERSNIP_TRACK();
		ubersnipTrack->ID = track_id;
		ubersnipTrack->Title = track_title;
		ubersnipTrack->Artist = track_artist;
		ubersnipTrack->SetImageURI(track_cover);

		this->tracks->Append(ubersnipTrack);

	}
}