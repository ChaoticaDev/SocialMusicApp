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
	[Windows::UI::Xaml::Data::Bindable]
	public ref class GENERIC_ITEM sealed {
	private:
		Platform::String^ _title;
		Windows::UI::Xaml::Media::ImageSource^ _Image;

		event PropertyChangedEventHandler^ _PropertyChanged;

		void OnPropertyChanged(Platform::String^ propertyName)
		{
			PropertyChangedEventArgs^ pcea = ref new  PropertyChangedEventArgs(propertyName);
			_PropertyChanged(this, pcea);
		}
	public:
		GENERIC_ITEM() {

		};

		property Platform::String^ Title {
			Platform::String^ get() {
				return this->_title;
			}

			void set(Platform::String^ val) {
				this->_title = val;
				OnPropertyChanged("Title");
			}
		}

		void SetImage(Platform::String^ path)
		{
			Windows::Foundation::Uri^ uri = ref new Windows::Foundation::Uri(path);
			_Image = ref new Windows::UI::Xaml::Media::Imaging::BitmapImage(uri);
		}

	};

	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class GENERIC_DATA sealed {
	private:
		Windows::UI::Xaml::Interop::IBindableObservableVector^ items;

	public:
		GENERIC_DATA() {
			items = ref new Platform::Collections::Vector<GENERIC_ITEM^>();
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Items {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->items;
			}
		}
	};

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
				this->Frame->GoBack();
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
