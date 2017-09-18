#include <string>
using namespace std;

typedef const char c_char;
typedef const char* pConstChar;

template<typename T1, typename T2, typename T3>
inline std::string toString(T1 p1, T2 p2, T3 p3)
{
	std::ostringstream stream;
	stream << p1 << p2 << p3;
	return stream.str();
}
