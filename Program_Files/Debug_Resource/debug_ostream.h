/*==============================================================================

	Make Debug Ostream [Debug_Ostream.h]

	Author : Choi HyungJoon

==============================================================================*/
#ifndef DEBUG_OSTREAM_H
#define DEBUG_OSTREAM_H

#include <Windows.h>
#include <sstream>

using namespace std;

namespace Debug
{
	class Debug_Buf : public basic_stringbuf<char, char_traits<char>>
	{
	public:
		virtual ~Debug_Buf()
		{
			sync();
		}
	protected:
		int sync()
		{
			OutputDebugStringA(str().c_str());
			str(basic_string<char>());
			return 0;
		}
	};

	class Debug_Ostream : public basic_ostream<char, char_traits<char>>
	{
	public:
		Debug_Ostream()
			: basic_ostream<char, char_traits<char>>(new Debug_Buf()) {}
		~Debug_Ostream() { delete rdbuf(); }
	};

	extern Debug_Ostream D_Out;
}
#endif // BASIC_DEBUG_OSTREAM_H