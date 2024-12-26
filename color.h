#pragma once

using uchar = unsigned char;

namespace tetris
{
	class Color
	{
	private:
		unsigned char red{ 0u };
		unsigned char green{ 0u };
		unsigned char blue{ 0u };
		unsigned char alpha{ 0u };

	public:
		constexpr Color(uchar r = 0u, uchar g = 0u, uchar b = 0u, uchar a = 0u) :
			red(r), green(g), blue(b), alpha(a) {}
	};


}