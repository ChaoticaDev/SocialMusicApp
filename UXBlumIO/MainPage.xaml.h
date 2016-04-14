//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include "MyFavoritesStream.xaml.h"
#include "external\SampleConfiguration.h"


using namespace Windows::UI::Core;

namespace UXBlumIO
{

	
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>


	public ref class MainPage sealed
	{
		Windows::UI::Xaml::Interop::IBindableObservableVector^ tracks = ref new Platform::Collections::Vector<UBERSNIP_TRACK^>();
		Windows::UI::Xaml::Interop::IBindableObservableVector^ categories = ref new Platform::Collections::Vector<GENERIC_ITEM^>();

		UBERSNIP_ACCOUNTS_MANAGER^ accManager = ref new UBERSNIP_ACCOUNTS_MANAGER();
		KeenIOAPI::KEENIO_PROJECT^ keen_project = ref new KeenIOAPI::KEENIO_PROJECT();
		static Array<Scenario>^ scenariosInner;
		
	public:
		MainPage();

		static property String^ FEATURE_NAME
		{
			String^ get()
			{
				return L"Background tasks";
			}
		}

		static property Array<Scenario>^ scenarios
		{
			Array<Scenario>^ get()
			{
				return scenariosInner;
			}
		}
		property double PageWidth {
			double get() {
				return this->Frame->Width;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ TrackList {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->tracks;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Categories {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->categories;
			}
		}

		property UBERSNIP_ACCOUNTS_MANAGER^ AccountManager {
			UBERSNIP_ACCOUNTS_MANAGER^ get() {
				return this->accManager;
			}

			void set(UBERSNIP_ACCOUNTS_MANAGER^ val) {
				this->accManager = val;
			}
		}

		property Windows::UI::Xaml::Controls::Frame^ ContentFrame {
			Windows::UI::Xaml::Controls::Frame^ get() {
				return this->contentFrame;
			}
		}

		property Windows::UI::Xaml::Controls::ComboBox^ AccountAddedUsers {
			Windows::UI::Xaml::Controls::ComboBox^ get() {
				return this->menu_create_tb;
			}
		}

		property Windows::UI::Xaml::Controls::MediaElement^ MediaPlayer {
			Windows::UI::Xaml::Controls::MediaElement^ get() {
				return this->mediaElement;
			}
		}

		property bool StreamToggleSwitch {
			bool get() {
				return this->toggleButton->IsOn;
			}

			void set(bool on) {
				this->toggleButton->IsOn = on;
			}
		}

		void FavTracks() {
			this->loadUserFavTracks(this->AccountManager->ActiveAccount->UID);
		}

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void ic_menu_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void ic_menu_add_PointerReleased(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void HamburgerButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadTracks();
		void toggleButton_Toggled(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void loadUserTracks(Platform::String^ user_id);
		void loadMyTracks();
		void *loadInitialCategories(Platform::Object^ _cats);
		void restoreTracks();
		void loadUserFavTracks(Platform::String^ user_id);
		void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void splitView1_PaneClosing(Windows::UI::Xaml::Controls::SplitView^ sender, Windows::UI::Xaml::Controls::SplitViewPaneClosingEventArgs^ args);
		void GridView_SelectionChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::SelectionChangedEventArgs^ e);
		void InitTask();
		void AttachProgressAndCompletedHandlers(Windows::ApplicationModel::Background::IBackgroundTaskRegistration^ task);
		void RegisterBackgroundTask();
		void UnregisterBackgroundTask(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void UpdateUI();

		void loadCats(UberSnip::HELPER::ASYNC_RESPONSE^ responseItem, MainPage^ rootPage) {
			Platform::String^ BR = responseItem->Body;


			cJSON* cats = cJSON_Parse(_string(BR));
			cats = cats->child;
			int *cat_count = new int(cJSON_GetArraySize(cats));
			rootPage->Categories->Clear();

			GENERIC_ITEM^ all_item = ref new GENERIC_ITEM();
			all_item->Title = "All";

			rootPage->Categories->Append(all_item);
			for (int i = 0; i < *cat_count; i++) {
				cJSON* cat = new cJSON();
				cat = cJSON_GetArrayItem(cats, i);

				string *track_title = new string(cJSON_GetObjectItem(cat, "name")->valuestring);

				GENERIC_ITEM gitem;
				gitem.Title = "Hi";

				GENERIC_ITEM^ ubersnipCategory = ref new GENERIC_ITEM();
				ubersnipCategory->Title = _String(*track_title);

				rootPage->Categories->Append(ubersnipCategory);
			}
		}

		void handler(void) {

		}
		
		void ASynTaskCategories(void)
		{
			concurrency::task_completion_event<Platform::String^> taskDone;
			MainPage^ rootPage = this;
			UberSnip::HELPER::ASYNC_RESPONSE^ responseItem = ref new UberSnip::HELPER::ASYNC_RESPONSE();

			auto dispatch = CoreWindow::GetForCurrentThread()->Dispatcher;
			auto op2 = create_async([taskDone, responseItem, rootPage, dispatch] {
				return create_task([taskDone, responseItem, rootPage, dispatch]() -> Platform::String^
				{

					//UBERSNIP API 0.4
					UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

					UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/categories.php";
					try{
					UberSnipAPI->Http->request();
					}
					catch (...) {
						printf("Error");
						printf("Error");
					}
					taskDone.set(UberSnipAPI->Client->BodyResponse);
					responseItem->Body = UberSnipAPI->Client->BodyResponse;
					dispatch->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([=]()
					{
						rootPage->loadCats(responseItem, rootPage);
					}));
					
					for (int i = 0; i < 100;) {

					}


					return "(done)";

				}).then([taskDone, responseItem, rootPage](Platform::String^ BodyResponse) {
				
	/**/
				});

			});
			
				//rootPage->loadCats(responseItem, rootPage);
		}

		void test() {
		}
	};
}
