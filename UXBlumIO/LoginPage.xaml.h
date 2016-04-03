//
// LoginPage.xaml.h
// Declaration of the LoginPage class
//

#pragma once

#include "LoginPage.g.h"
#include "MainPage.xaml.h"

namespace UXBlumIO
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class LoginPage sealed
	{

		MainPage^ rootPage;
	public:
		LoginPage();
	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void loginButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
