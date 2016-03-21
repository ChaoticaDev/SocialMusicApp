#pragma once

class APP_SETTINGS {
public:
	static Platform::Object^ ReadProperty(Platform::String^ s) {


		Windows::Storage::ApplicationDataContainer^ localSettings =
			Windows::Storage::ApplicationData::Current->LocalSettings;
		Windows::Storage::StorageFolder^ localFolder =
			Windows::Storage::ApplicationData::Current->LocalFolder;


		return localSettings->Values->Lookup(s);
	}

	static void WriteProperty(Platform::String^ key, Platform::Object^ data) {

		Windows::Storage::ApplicationDataContainer^ localSettings =
			Windows::Storage::ApplicationData::Current->LocalSettings;
		Windows::Storage::StorageFolder^ localFolder =
			Windows::Storage::ApplicationData::Current->LocalFolder;

		Platform::Object^ mdata = dynamic_cast<Platform::String^>(data);
		localSettings->Values->Insert(key, mdata);
	}
};