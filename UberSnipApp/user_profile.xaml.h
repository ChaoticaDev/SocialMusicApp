//
// user_profile.xaml.h
// Declaration of the user_profile class
//

#pragma once

#include "user_profile.g.h"
#include "MainPage.xaml.h"

using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
namespace UberSnipApp
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>


	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class user_profile sealed
	{
	private:
		UberSnipApp::UBERSNIP_TRACKS^ MyTracks;
		UberSnipApp::UBERSNIP_LOGIN_HANDLER^ _LoginManager;
		UberSnipApp::GENERIC_DATA^ genericData;
	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;

	public:
		user_profile();

		property UberSnipApp::UBERSNIP_LOGIN_HANDLER^ LoginManager {
			UberSnipApp::UBERSNIP_LOGIN_HANDLER^ get() {
				return this->_LoginManager;
			}

			void set(UberSnipApp::UBERSNIP_LOGIN_HANDLER^ val) {
				this->_LoginManager = val;
			}
		}

		property UberSnipApp::UBERSNIP_TRACKS^ DataSource
		{
			UberSnipApp::UBERSNIP_TRACKS^ get() { return this->MyTracks; };
		}

		property UberSnipApp::GENERIC_DATA^ FollowersSource
		{
			UberSnipApp::GENERIC_DATA^ get() { return this->genericData; };

		}

	private:
		void LoadMyFollowers(void);
		void LoadMyTracks(void);
	};
}
