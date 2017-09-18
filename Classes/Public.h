#include <windows.h>
#include <xstring>
using namespace std;
void StringToWstring(std::wstring& szDst, std::string str)
{
	std::string temp = str;
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL,0); 
	wchar_t * wszUtf8 = new wchar_t[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	szDst = wszUtf8;
	std::wstring r = wszUtf8;
	delete[] wszUtf8;
}

std::string TCHAR2STRING(TCHAR *STR)
{

	int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);

	char* chRtn =new char[iLen*sizeof(char)];

	WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);

	std::string str(chRtn);

	return str;

}

wchar_t* WstringToWchar(const wstring& ws)
{
	 return const_cast<wchar_t*>(ws.c_str());  
}

template<typename T1,  typename T2,typename T3>
inline std::string toString (T1 p1, T2 p2,T3 p3)
{
	std::ostringstream stream;
	stream << p1 << p2 << p3;
	return stream.str();
}

