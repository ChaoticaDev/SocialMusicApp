//
// StreamView.xaml.h
// Declaration of the StreamView class
//

#pragma once

#include "StreamView.g.h"
#include "MainPage.xaml.h"

using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace UXBlumIO
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class StreamView sealed
	{
		MainPage^ rootPage;
	private:
		void goBack(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args) {
			try {
				Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = Windows::UI::Core::AppViewBackButtonVisibility::Collapsed;
				this->Frame->Navigate(MainPage::typeid, this->rootPage);
			}
			catch (Platform::DisconnectedException^ e) {
				
			}
			
		}
	public:
		StreamView();
	private:
		void Image_DoubleTapped(Platform::Object^ sender, Windows::UI::Xaml::Input::DoubleTappedRoutedEventArgs^ e);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	};
}
