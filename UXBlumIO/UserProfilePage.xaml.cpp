//
// UserProfilePage.xaml.cpp
// Implementation of the UserProfilePage class
//

#include "pch.h"
#include "UserProfilePage.xaml.h"

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

UserProfilePage::UserProfilePage()
{
	InitializeComponent();

	Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->BackRequested += ref new Windows::Foundation::EventHandler<Windows::UI::Core::BackRequestedEventArgs^>(this, &UserProfilePage::goBack);
}

void UserProfilePage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {

	//GET COLLECTION OF PARAMS
	Windows::UI::Xaml::Interop::IBindableObservableVector^ param = dynamic_cast<Windows::UI::Xaml::Interop::IBindableObservableVector^>(e->Parameter);

	//GET ROOTPAGE PASSED AS PARAMETER
	this->rootPage = dynamic_cast<MainPage^>(param->GetAt(0));

	//GET TRACK PASSED AS PARAMETER
	UBERSNIP_USER^ user = dynamic_cast<UBERSNIP_USER^>(param->GetAt(1));
	this->User = user ;

	this->ref_track = dynamic_cast<UBERSNIP_TRACK^>(param->GetAt(2));


	//IF TRACK FOUND
	if (user != nullptr) {

		UberSnip::HELPER::KeenIO::SendData(App::KeenIOProjectID, "SOCIAL.APP", "profile_visit", "https://ubersnip.com/" + this->User->Username);

	}
	else {

		//FAILED TO GET TRACK INFO
		//GO BACK TO HOME SCREEN
		this->rootPage->Frame->Navigate(MainPage::typeid, this->rootPage);
	}
}
