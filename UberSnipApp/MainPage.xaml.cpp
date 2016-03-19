//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//


#include "pch.h"
#include "MainPage.xaml.h"

using namespace UberSnipApp;

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

Item::Item() :_Title(""), _TrackName(""), _TrackAlbum("") {

}

void UberSnipApp::MainPage::loadInitialTracks(void) {
	UBERSNIP_CLIENT* uCLIENT = new UBERSNIP_CLIENT();
	uCLIENT->kHTTP.reqURL = "http://api.ubersnip.com/tracks.php";
	uCLIENT->kHTTP.addParam("ID", "*");
	uCLIENT->request(uCLIENT->kHTTP);

	Platform::String^ gg = STRING_UTILS::StringFromAscIIChars(uCLIENT->body);

	cJSON* tracks = cJSON_Parse(uCLIENT->body.c_str());
	int track_count = cJSON_GetArraySize(tracks);

	for (int i = 0; i < track_count; i++) {
		cJSON* track = new cJSON();
		track = cJSON_GetArrayItem(tracks, i)->child;

		cJSON* currTrack = track->child;
		string *track_id = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_uid = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_title = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_description = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* file_name = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_album = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_cover = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_views = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_likes = new string(currTrack->valuestring);

		currTrack = currTrack->next;
		string* track_owner = new string(currTrack->valuestring);



		UBERSNIP_TRACKSET^ ubersnipTrack = ref new UBERSNIP_TRACKSET();
		ubersnipTrack->ID = STRING_UTILS::StringFromAscIIChars(*track_id);
		ubersnipTrack->UID = STRING_UTILS::StringFromAscIIChars(*track_uid);
		ubersnipTrack->Title = STRING_UTILS::StringFromAscIIChars(*track_title);
		ubersnipTrack->Description = STRING_UTILS::StringFromAscIIChars(*track_description);
		ubersnipTrack->FileName = STRING_UTILS::StringFromAscIIChars(*file_name);
		ubersnipTrack->Views = STRING_UTILS::StringFromAscIIChars(*track_views);
		ubersnipTrack->Likes = STRING_UTILS::StringFromAscIIChars(*track_likes);
		ubersnipTrack->Album = STRING_UTILS::StringFromAscIIChars(*track_album);
		ubersnipTrack->Artist = STRING_UTILS::StringFromAscIIChars(*track_owner);
		ubersnipTrack->SetImage(STRING_UTILS::StringFromAscIIChars(track_cover->c_str()));

		this->UberSnipTracks->Tracks->Append(ubersnipTrack);
	}
}


void UberSnipApp::MainPage::loadInitialCategories(void) {
	UBERSNIP_CLIENT* uCLIENT = new UBERSNIP_CLIENT();
	uCLIENT->kHTTP.reqURL = "http://api.ubersnip.com/categories.php";
	uCLIENT->kHTTP.addParam("ID", "*");
	uCLIENT->request(uCLIENT->kHTTP);

	Platform::String^ gg = STRING_UTILS::StringFromAscIIChars(uCLIENT->body);

	cJSON* cats = cJSON_Parse(uCLIENT->body.c_str());
	cats = cats->child;
	int *cat_count = new int(cJSON_GetArraySize(cats));

	for (int i = 0; i < *cat_count; i++) {
		cJSON* cat = new cJSON();
		cat = cJSON_GetArrayItem(cats, i)->child;

		string *track_title = new string(cat->valuestring);


		GENERIC_ITEM^ ubersnipCategory = ref new GENERIC_ITEM();
		ubersnipCategory->Title = STRING_UTILS::StringFromAscIIChars(*track_title);

		this->genericData->Items->Append(ubersnipCategory);
	}
}

MainPage::MainPage()
{
	InitializeComponent();

	this->UberSnipTracks = ref new UBERSNIP_TRACKS();
	this->genericData = ref new GENERIC_DATA();
	this->LoginManager = ref new UberSnipApp::UBERSNIP_LOGIN_HANDLER();

	//UBERSNIP_TRACKSET^ im = ref new UBERSNIP_TRACKSET();
	//im->Title = "Can't make you want me.";
	//im->Artist = "Jules M.";
	//im->Album = "UberSnip Featured";
	//im->SetImage("https://ubersnip.com//thumb.php?src=256676423_713478081_438351567.jpg&t=m&w=300&h=300");
	//this->UberSnipTracks->Tracks->Append(im);

	this->loadInitialTracks();
	this->loadInitialCategories();
}


void UberSnipApp::MainPage::ListView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	UBERSNIP_TRACKSET^ im = dynamic_cast<UBERSNIP_TRACKSET^>(dynamic_cast<ListView^>(sender)->SelectedItem);
	
	if (im == nullptr) {
		printf("null ptr");
	}
	else {
		//mediaElement->Source = ref new Windows::Foundation::Uri(im->TrackName);
		//mediaElement->Play();

		activeTextBlock->Text = im->Artist;
		activeTrackTitle->Text = im->Title;
		activeTrackAlbum->Text = im->Album;

	}


}


void UberSnipApp::MainPage::activeTextBlock_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

}


void UberSnipApp::MainPage::image_Copy_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (this->LoginManager->LoggedIn) return;
	Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapimg = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx://UberSnipApp/Assets/person_yellow.png"));
	this->loginPerson->Source = bitmapimg;
	this->flyout1->ShowAt(MainPage::Frame);


}


void UberSnipApp::MainPage::siteLogin(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	int err = this->LoginManager->login(this->usernameTextBox->Text, this->passwordPasswordBox->Password);
	if (err >= 0) {
		this->passwordPasswordBox->Password = "";
		Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapimg = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx://UberSnipApp/Assets/person_red.png"));
		this->loginPerson->Source = bitmapimg;

	}
	else {
		this->flyout1->Hide(); Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapimg = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(ref new Uri("ms-appx://UberSnipApp/Assets/person.png"));
		this->loginPerson->Source = bitmapimg;
	}
}
