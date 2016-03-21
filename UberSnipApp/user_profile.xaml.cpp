//
// user_profile.xaml.cpp
// Implementation of the user_profile class
//

#include "pch.h"
#include "user_profile.xaml.h"

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

void user_profile::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {
	UberSnipApp::UBERSNIP_LOGIN_HANDLER^ it;
	if (dynamic_cast<UberSnipApp::UBERSNIP_LOGIN_HANDLER^>(e->Parameter) != nullptr) {
		it = dynamic_cast<UberSnipApp::UBERSNIP_LOGIN_HANDLER^>(e->Parameter);
	}
	else {
		printf("failed conversion");
	}

	this->LoginManager = it;

	//Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
	//this->profileImage = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
	//this->LoginManager->LoggedInAs

	this->profileImage->Source = this->LoginManager->Profile->Avatar;
	this->LoadMyTracks();
	this->LoadMyFollowers();
	Windows::UI::Xaml::Controls::Page::OnNavigatedTo(e);
}

void user_profile::LoadMyFollowers(void) {
	UBERSNIP_CLIENT* uCLIENT = new UBERSNIP_CLIENT();
	uCLIENT->kHTTP.reqURL = "http://api.ubersnip.com/relationships.php"; 
	uCLIENT->kHTTP.addParam("user_id", STRING_UTILS::StringToAscIIChars(this->LoginManager->UID));
	uCLIENT->request(uCLIENT->kHTTP);

	Platform::String^ gg = STRING_UTILS::StringFromAscIIChars(uCLIENT->body);

	cJSON* cats = cJSON_Parse(uCLIENT->body.c_str());
	cats = cJSON_GetObjectItem(cats, "relations");
	int *cat_count = new int(cJSON_GetArraySize(cats));

	for (int i = 0; i < *cat_count; i++) {

		cJSON* curr_cats = cats->child;

		for (int m = 0; m < i; m++) {
			curr_cats = cats->child->next;
		}
		string *username = new string(cJSON_GetObjectItem(curr_cats, "username")->valuestring);
		string *avatar = new string(cJSON_GetObjectItem(curr_cats, "avatar")->valuestring);


		UberSnipApp::USER_PROFILE^ ubersnipFollower = ref new UberSnipApp::USER_PROFILE();
		ubersnipFollower->Username = STRING_UTILS::StringFromAscIIChars(*username);
		ubersnipFollower->SetAvatar(STRING_UTILS::StringFromAscIIChars(avatar->c_str()));

		this->myFollowers->Users->Append(ubersnipFollower);
	}
}
void user_profile::LoadMyTracks(void) {
	UBERSNIP_CLIENT* uCLIENT = new UBERSNIP_CLIENT();
	uCLIENT->kHTTP.reqURL = "http://api.ubersnip.com/tracks.php";
	uCLIENT->kHTTP.addParam("uid", STRING_UTILS::StringToAscIIChars(this->LoginManager->UID));
	uCLIENT->request(uCLIENT->kHTTP);

	Platform::String^ gg = STRING_UTILS::StringFromAscIIChars(uCLIENT->body);

	cJSON* tracks = cJSON_Parse(uCLIENT->body.c_str());
	tracks = cJSON_GetObjectItem(tracks, "tracks");
	int *track_count = new int(cJSON_GetArraySize(tracks));

	for (int i = 0; i < *track_count; i++) {
		cJSON* track = new cJSON();
		track = cJSON_GetArrayItem(tracks, i)->child;

		cJSON* currTrack = track;
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
		string* genre = new string(currTrack->valuestring);

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
		ubersnipTrack->Genre = STRING_UTILS::StringFromAscIIChars(*genre);
		ubersnipTrack->SetImage(STRING_UTILS::StringFromAscIIChars(track_cover->c_str()));

		this->MyTracks->Tracks->Append(ubersnipTrack);
	}
}

user_profile::user_profile()
{
	InitializeComponent();
	this->MyTracks = ref new UberSnipApp::UBERSNIP_TRACKS();
	this->genericData = ref new GENERIC_DATA();
	this->myFollowers = ref new USER_DATA();

}
