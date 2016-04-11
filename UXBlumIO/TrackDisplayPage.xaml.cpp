//
// TrackDisplayPage.xaml.cpp
// Implementation of the TrackDisplayPage class
//

#include "pch.h"
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

TrackDisplayPage::TrackDisplayPage()
{
	InitializeComponent(); 
	
	//SHOW BACK BUTTON
	try {
		Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = Windows::UI::Core::AppViewBackButtonVisibility::Visible;
	}
	catch (Platform::DisconnectedException^ e) {

	}

	//ASSIGN BACK BUTTON CALL-BACK
	Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs^>(this, &TrackDisplayPage::goBack);


}

void TrackDisplayPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {

	//GET COLLECTION OF PARAMS
	Windows::UI::Xaml::Interop::IBindableObservableVector^ param = dynamic_cast<Windows::UI::Xaml::Interop::IBindableObservableVector^>(e->Parameter);

	//GET ROOTPAGE PASSED AS PARAMETER
	this->rootPage = dynamic_cast<MainPage^>(param->GetAt(0));

	//GET TRACK PASSED AS PARAMETER
	UBERSNIP_TRACK^ track = dynamic_cast<UBERSNIP_TRACK^>(param->GetAt(1));
	this->Track = track;


	//IF TRACK FOUND
	if (track != nullptr) {
		/*
		this->streamPlayerAlbumArtistTitle->Text = track->Artist;
		this->streamPlayerAlbumAlbumTitle->Text = track->Album;
		this->streamPlayerAlbumTrackTitle->Text = track->Title;
		this->streamPlayerFlipVInfo->Source = track->Image;
		this->streamPlayerBGImage->Source = track->Image;*/

		//this->mediaElement = this->rootPage->MediaPlayer;
		this->mediaElement->Source = ref new Windows::Foundation::Uri(track->FileName);
		this->mediaElement->Play();

		UberSnip::HELPER::KeenIO::SendData(App::KeenIOProjectID, "SOCIAL.APP", "track_play", "https://ubersnip.com/snippet/" + track->ID);
		
		//LOAD TRACK COMMENTS
		this->loadComments();

		//LOAD TRACK LIKES
		this->loadLikes();

	}
	else {

		//FAILED TO GET TRACK INFO
		//GO BACK TO HOME SCREEN
		this->rootPage->Frame->Navigate(MainPage::typeid, this->rootPage);
	}
}

void UXBlumIO::TrackDisplayPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//IF NOT LOGGED IN
	if (this->rootPage->AccountManager->ActiveAccount == nullptr) {
		//OPEN LOGIN POPUP-WINDOW
		this->popup1->IsOpen = true;
	}
	else {
		UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

		UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/request/add_comment.php";
		UberSnipAPI->Http->addDefHeaders();
		UberSnipAPI->Http->addParam("token", this->rootPage->AccountManager->ActiveAccount->AuthToken);
		UberSnipAPI->Http->addParam("track_id", this->Track->ID);
		Platform::String^ message;
		this->richEditComment->Document->GetText(Windows::UI::Text::TextGetOptions::None, &message);
		this->richEditComment->Document->SetText(Windows::UI::Text::TextSetOptions::None, "");
		UberSnipAPI->Http->addParam("message", UberSnip::UTILS::STRING::URLEncode(UberSnip::UTILS::STRING::StringToAscIIChars(message)));
		UberSnipAPI->Http->request();
		
		
		this->loadComments();
	}
}


void UXBlumIO::TrackDisplayPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//CLOSE LOGIN POPUP WINDOW
	this->popup1->IsOpen = false;
}


void UXBlumIO::TrackDisplayPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	loginStatusLabel->Text = "Logging in...";
	UBERSNIP_ACCOUNT^ myacc = ref new UBERSNIP_ACCOUNT();

	myacc->Username = this->inputLoginUsername->Text;
	myacc->Password = this->inputLoginPassword->Password;
	myacc->login();

	if (myacc->Error == true) {
		return;
	}

	if (myacc->Error == false) {
		this->popup1->IsOpen = false;
		loginStatusLabel->Text = "";
		UBERSNIP_ACCOUNT^ ac = this->rootPage->AccountManager->UserByUsername(myacc->Username);

		if (ac != nullptr) {
			UBERSNIP_ACCOUNT^ exisisting_account = this->rootPage->AccountManager->UserByUsername(myacc->Username);
			exisisting_account = myacc;
		}
		else {
			this->rootPage->AccountManager->Accounts->Append(myacc);
			this->rootPage->AccountManager->ActiveAccount = myacc;

		}
	}
	else {
		loginStatusLabel->Text = "Invalid Credentials?";
	}

}


void UXBlumIO::TrackDisplayPage::gridViewLikes_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e)
{
	UBERSNIP_LIKE^ like = dynamic_cast<UBERSNIP_LIKE^>(this->gridViewLikes->SelectedItem);

	if (like != nullptr) {
		UBERSNIP_USER^ user = UberSnip::HELPER::Accounts::getUserData(like->OwnerID, "UID");

		Windows::UI::Xaml::Interop::IBindableObservableVector^ params = ref new Platform::Collections::Vector<Platform::Object^>();
		params->Append(this->rootPage);
		params->Append(user);
		params->Append(this->Track);

		this->Frame->Navigate(UserProfilePage::typeid, params);
	}
}
