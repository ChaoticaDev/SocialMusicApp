//
// TrackDisplayPage.xaml.h
// Declaration of the TrackDisplayPage class
//

#pragma once

#include "TrackDisplayPage.g.h"
#include "MainPage.xaml.h"

namespace UXBlumIO
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TrackDisplayPage sealed
	{

		MainPage^ rootPage;
		UBERSNIP_TRACK^ __track = ref new UBERSNIP_TRACK();
		Windows::UI::Xaml::Interop::IBindableObservableVector^ comments = ref new Platform::Collections::Vector<UBERSNIP_COMMENT^>();
		Windows::UI::Xaml::Interop::IBindableObservableVector^ likes = ref new Platform::Collections::Vector<UBERSNIP_LIKE^>();
	public:
		TrackDisplayPage();

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Comments {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->comments;
			}
		}

		property double commentFrameWidth {
			double get() {
				return 0;// this->gridViewComments->Width;
			}
		}

		property Windows::UI::Xaml::Interop::IBindableObservableVector^ Likes {
			Windows::UI::Xaml::Interop::IBindableObservableVector^ get() {
				return this->likes;
			}
		}

		property UBERSNIP_TRACK^ Track {
			UBERSNIP_TRACK^ get() {
				return this->__track;
			}

			void set(UBERSNIP_TRACK^ val) {
				this->__track = val;
			}
		}

	protected:
		void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void goBack(Platform::Object^ sender, Windows::UI::Core::BackRequestedEventArgs^ args) {
			try {
				Windows::UI::Core::SystemNavigationManager::GetForCurrentView()->AppViewBackButtonVisibility = Windows::UI::Core::AppViewBackButtonVisibility::Collapsed;
				this->Frame->Navigate(MainPage::typeid, this->rootPage);
			}
			catch (Platform::DisconnectedException^ e) {

			}

		}
		void loadComments() {
			
			UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

			UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/comments.php";
			UberSnipAPI->Http->addParam("track_id", this->Track->ID);
			UberSnipAPI->Http->request();

			int err = UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).find("__api_err");

			if (err < 0) {
				if (UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).length() < 3) {
					return;
				}
			}



			cJSON* tracks = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());
			tracks = cJSON_GetObjectItem(tracks, "comments");
			int *track_count = new int(cJSON_GetArraySize(tracks));
			this->comments->Clear();


			UBERSNIP_COMMENT^ comment[1000];
			cJSON* curr_track = tracks->child;
			for (int i = 0; i < *track_count; i++) {
				comment[i] = ref new UBERSNIP_COMMENT();

				//GO TO NEXT JSON TRACK OBJECT
				//for (int m = 0; m < i; m++) {
				//}

				if (curr_track == nullptr) {
					break;
				}
				//GET THE ID OF COMMENT
				comment[i]->ID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "id")->valuestring);

				//GET TRACK ARTIST/OWNER USERNAME
				comment[i]->Owner = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "username")->valuestring);

				//GET TRACK ARTIST/OWNER USER_ID
				comment[i]->OwnerID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "uid")->valuestring);

				//GET TRACK COMMENT TEXT
				comment[i]->CommentText = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "message")->valuestring);

				//GET COMMENTER AVATAR
				Platform::String^ track_cover = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "image")->valuestring);


				//SETS THE IMAGE URL OF COMMENTER AVATAR
				comment[i]->SetImageURI("https://ubersnip.com//thumb.php?src=" + track_cover + "&t=a&w=112&h=112");

				//ADD TO COMMENT LIST
				this->comments->Append(comment[i]);

				curr_track = curr_track->next;
			}


			return;
		}

		void loadLikes() {

			UberSnip::UBERSNIP_CLIENT* UberSnipAPI = new UberSnip::UBERSNIP_CLIENT();

			UberSnipAPI->Http->RequestURL = "http://api.ubersnip.com/track_likes.php";
			UberSnipAPI->Http->addParam("track_id", this->Track->ID);
			UberSnipAPI->Http->request();

			int err = UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).find("__api_err");

			if (err < 0) {
				if (UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).length() < 3) {
					return;
				}
			}



			cJSON* tracks = cJSON_Parse(UberSnip::UTILS::STRING::StringToAscIIChars(UberSnipAPI->Client->BodyResponse).c_str());
			tracks = cJSON_GetObjectItem(tracks, "likes");
			int *track_count = new int(cJSON_GetArraySize(tracks));
			this->likes->Clear();


			UBERSNIP_LIKE^ like[1000];
			cJSON* curr_track = tracks->child;
			for (int i = 0; i < *track_count; i++) {
				like[i] = ref new UBERSNIP_LIKE();

				//GO TO NEXT JSON TRACK OBJECT
				//for (int m = 0; m < i; m++) {
				//}

				if (curr_track == nullptr) {
					break;
				}
				//GET THE ID OF COMMENT
				like[i]->ID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "id")->valuestring);

				//GET TRACK ARTIST/OWNER USERNAME
				like[i]->Owner = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "username")->valuestring);

				//GET TRACK ARTIST/OWNER USER_ID
				like[i]->OwnerID = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "by")->valuestring);

				//GET COMMENTER AVATAR
				Platform::String^ track_cover = UberSnip::UTILS::STRING::StringFromAscIIChars(cJSON_GetObjectItem(curr_track, "image")->valuestring);


				//SETS THE IMAGE URL OF COMMENTER AVATAR
				like[i]->SetImageURI("https://ubersnip.com//thumb.php?src=" + track_cover + "&t=a&w=112&h=112");

				//ADD TO COMMENT LIST
				this->likes->Append(like[i]);

				curr_track = curr_track->next;
			}


			return;
		}
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
};
}
