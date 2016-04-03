//
// LoginPage.xaml.cpp
// Implementation of the LoginPage class
//

#include "pch.h"
#include "LoginPage.xaml.h"
#include "MainPage.xaml.h"

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

LoginPage::LoginPage()
{
	InitializeComponent();
}

void LoginPage::OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) {

	this->rootPage = dynamic_cast<MainPage^>(e->Parameter);

	if (rootPage != nullptr) {

	}

}

void UXBlumIO::LoginPage::loginButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	UBERSNIP_ACCOUNT^ myacc = ref new UBERSNIP_ACCOUNT();

	myacc->Username = "montraydavis";
	myacc->Password = "montraydavis";
	myacc->login();


	UBERSNIP_ACCOUNT^ ac = this->rootPage->AccountManager->UserByUsername(myacc->Username);

	if ( ac != nullptr ){
		UBERSNIP_ACCOUNT^ exisisting_account = this->rootPage->AccountManager->UserByUsername(myacc->Username);
		exisisting_account = myacc;
	}
	else {
		this->rootPage->AccountManager->Accounts->Append(myacc);
		this->rootPage->AccountManager->ActiveAccount = myacc;

	}
		this->Frame->GoBack();
	
}
