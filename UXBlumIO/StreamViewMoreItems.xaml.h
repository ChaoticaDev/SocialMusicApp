//
// StreamViewMoreItems.xaml.h
// Declaration of the StreamViewMoreItems class
//

#pragma once

#include "StreamViewMoreItems.g.h"

namespace UXBlumIO
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class StreamViewMoreItems sealed
	{
	private:
		void goBack(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args) {
			try {
				Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = Windows::UI::Core::AppViewBackButtonVisibility::Collapsed;
				this->Frame->GoBack();
			}
			catch (Platform::DisconnectedException^ e) {

			}

		}
	public:
		StreamViewMoreItems();
	};
}
