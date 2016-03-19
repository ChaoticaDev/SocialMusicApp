#include <cvt/wstring>
#include <codecvt>

class STRING_UTILS {
private:
	Platform::String^ fromAscII;

public:
	static Platform::String^ StringFromAscIIChars(const char* chars)
	{
		size_t newsize = strlen(chars) + 1;
		wchar_t * wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
		Platform::String^ str = ref new Platform::String(wcstring);
		delete[] wcstring;
		return str;
	}

	static Platform::String^ StringFromAscIIChars(string str_string)
	{
		const char* chars = str_string.c_str();
		size_t newsize = strlen(chars) + 1;
		wchar_t * wcstring = new wchar_t[newsize];
		size_t convertedChars = 0;
		mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
		Platform::String^ str = ref new Platform::String(wcstring);
		delete[] wcstring;
		return str;
	}

	static string StringToAscIIChars(Platform::String^ chars) {
		Platform::String^ fooRT = chars;
		stdext::cvt::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
		std::string stringUtf8 = convert.to_bytes(fooRT->Data());
		return stringUtf8;
	}

	static LPCWSTR StringToLPCWSTR(const char* str) {
		
		wchar_t* wString = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, str, -1, wString, 4096);
		return wString;
	}
};