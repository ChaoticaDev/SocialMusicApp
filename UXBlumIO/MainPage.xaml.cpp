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
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{

	InitializeComponent();


	this->keen_project->ID = "";
	this->keen_project->MasterKey = "";
	this->keen_project->LoadProject();

	GENERIC_ITEM^ item = ref new GENERIC_ITEM();
	item->Title = "All";
	this->Categories->Append(item);

}
void MainPage::restoreTracks() {
	this->contentFrame->Navigate(MyFavoritesStream::typeid, this);
}
void MainPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {


	MainPage^ returnPage = dynamic_cast<MainPage^>(e->Parameter);

	if (returnPage != nullptr) {
		this->AccountManager = returnPage->AccountManager;
		this->toggleButton->IsOn = returnPage->StreamToggleSwitch;
		this->tracks = returnPage->tracks;
	}
	else {
		string dataParam[2] = { "app_event", ".LAUNCH" };
		
		//this->keen_project->SentEvent("SOCIAL.APP", "app_event", ".LAUNCH");
		UberSnip::HELPER::KeenIO::SendData(App::KeenIOProjectID, "SOCIAL.APP", "app_event", ".LAUNCH");
	}
		
	this->restoreTracks();
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
	UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

	UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/tracks.php";
	UberSnipAPI->Http->request();

	int err = UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).find("__api_err");

	if (err < 0) {
		if (UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).length() < 3) {
			return;
		}
	}

	

	cJSON* tracks = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());
	tracks = cJSON_GetObjectItem(tracks, "tracks");
	int *track_count = new int(cJSON_GetArraySize(tracks));
	this->tracks->Clear();


	UBERSNIP_TRACK^ ubersnipTrack [1000];
	cJSON* curr_track = tracks->child;
	for (int i = 0; i < *track_count; i++) {
		ubersnipTrack[i] = ref new UBERSNIP_TRACK();

		//GO TO NEXT JSON TRACK OBJECT
		//for (int m = 0; m < i; m++) {
		//}

			if (curr_track == nullptr) {
				break;
			}
		//GET THE ID OF TRACK
		ubersnipTrack[i]->ID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "ID")->valuestring);

		//GET TRACK TITLE
		ubersnipTrack[i]->Title = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "title")->valuestring);

		//GET TRACK COVER IMAGE
		Platform::String^ track_cover = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "art")->valuestring);

		//GET TRACK ARTIST/OWNER USERNAME
		ubersnipTrack[i]->Artist = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(curr_track, "user"), "username")->valuestring);

		//GET TRACK FILENAME
		ubersnipTrack[i]->FileName = "https://ubersnip.com/uploads/tracks/" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "name")->valuestring);


		//SETS THE IMAGE URL OF TRACK COVER
		ubersnipTrack[i]->SetImageURI("https://ubersnip.com//thumb.php?src=" + track_cover + "&t=m&w=112&h=112");

		//ADD TO TRACK LIST
		this->tracks->Append(ubersnipTrack[i]);

			curr_track = curr_track->next;
	}

	if ( this->contentFrame != nullptr)
	this->contentFrame->Navigate(MyFavoritesStream::typeid, this);
}
void MainPage::loadUserTracks(Platform::String^ user_id) {

}
void MainPage::loadUserFavTracks(Platform::String^ user_id) {

	UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

	UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/likes.php";
	UberSnipAPI->Http->addParam("UID", user_id);
	UberSnipAPI->Http->request();

	int err = UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).find("__api_err");

	if (err < 0) {
		if (UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).length() < 3) {
			return;
		}
	}



	cJSON* tracks = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());
	tracks = cJSON_GetObjectItem(tracks, "tracks");
	int *track_count = new int(cJSON_GetArraySize(tracks));
	this->tracks->Clear();


	UBERSNIP_TRACK^ ubersnipTrack[1000];
	cJSON* curr_track = tracks->child;
	for (int i = 0; i < *track_count; i++) {
		ubersnipTrack[i] = ref new UBERSNIP_TRACK();

		//GO TO NEXT JSON TRACK OBJECT
		//for (int m = 0; m < i; m++) {
		//}

		if (curr_track == nullptr) {
			break;
		}
		//GET THE ID OF TRACK
		ubersnipTrack[i]->ID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "ID")->valuestring);

		//GET TRACK TITLE
		ubersnipTrack[i]->Title = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "title")->valuestring);

		//GET TRACK COVER IMAGE
		Platform::String^ track_cover = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "art")->valuestring);

		//GET TRACK ARTIST/OWNER USERNAME
		ubersnipTrack[i]->Artist = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(cJSON_GetObjectItem(curr_track, "user"), "username")->valuestring);

		//GET TRACK FILENAME
		ubersnipTrack[i]->FileName = "https://ubersnip.com/uploads/tracks/" + UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "name")->valuestring);


		//SETS THE IMAGE URL OF TRACK COVER
		ubersnipTrack[i]->SetImageURI("https://ubersnip.com//thumb.php?src=" + track_cover + "&t=m&w=112&h=112");

		//ADD TO TRACK LIST
		this->tracks->Append(ubersnipTrack[i]);

		curr_track = curr_track->next;

	}

	if (this->contentFrame != nullptr)
		this->contentFrame->Navigate(MyFavoritesStream::typeid, this);
}
void MainPage::loadMyTracks() {

	//UBERSNIP API 0.4
	UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

	UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/tracks.php";
	UberSnipAPI->Http->request();
	//UBERSNIP API 0.4


	cJSON* tracks = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());
	
	//GET THE TRACKS JSON OBJECT
	tracks = cJSON_GetObjectItem(tracks, "tracks");

	//GET NUMBER OF JSON PARSED TRACKS
	int *track_count = new int(cJSON_GetArraySize(tracks));

	//CLEAR TRACK LIST
	this->tracks->Clear();

	//LOOP THROUGH TRACKS PARSED AS JSON
	for (int i = 0; i < *track_count; i++) {

		//GO TO NEXT JSON OBJECT (OR TRACK)
		cJSON* curr_track = tracks->child;
		for (int m = 0; m < i; m++) {
			curr_track = tracks->child->next;
		}

		//GET TRACK ID
		Platform::String^ track_id = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "ID")->valuestring);

		//GET TRACK TITLE
		Platform::String^ track_title = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "title")->valuestring);

		//GET TRACK COVER
		Platform::String^ track_cover = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "cover")->valuestring);

		//CREATE NEW TRACK
		UBERSNIP_TRACK^ ubersnipTrack = ref new UBERSNIP_TRACK();
		ubersnipTrack->ID = track_id;
		ubersnipTrack->Title = track_title;
		ubersnipTrack->SetImageURI(track_cover);

		//ADD TRACK TO TRACKLIST
		this->tracks->Append(ubersnipTrack);
	}
}
void UXBlumIO::MainPage::toggleButton_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	//IF SHOWING ALL TRACKS
	if (this->toggleButton->IsOn == true) {

		// LOAD PUBLIC STREAM
		this->loadTracks();
	}
	else {

		//IF LOGGED IN
		if (this->AccountManager->ActiveAccount != nullptr) {

			//LOAD TRACKS FAVORITED BY USER
			this->loadUserFavTracks(this->AccountManager->ActiveAccount->UID);
		}
		else {

			//GO TO LOGIN PAGE
			this->contentFrame->Navigate(LoginPage::typeid, this);
		}
	}
}
void UXBlumIO::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
}
