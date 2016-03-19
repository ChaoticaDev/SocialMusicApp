#pragma once

class APP_SETTINGS {
public:
	static Platform::String^ ReadProperty(Platform::String^ s) {


		Windows::Storage::ApplicationDataContainer^ localSettings =
			Windows::Storage::ApplicationData::Current->LocalSettings;
		Windows::Storage::StorageFolder^ localFolder =
			Windows::Storage::ApplicationData::Current->LocalFolder;


		return dynamic_cast<Platform::String^>(localSettings->Values->Lookup(s));
	}
};