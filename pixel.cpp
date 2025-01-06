#include "pixel.h"

namespace tetris
{

	texturePixel& texturePixel::operator=(const matrixPixel& mp)
	{
		row = mp.row;
		col = mp.col;
		return *this;
	}

	texturePixel::operator int() const
	{
		if (pixelTexture) return 1;
		else return 0;
	}





#ifdef LOGS
	std::ostream& operator<<(std::ostream& os, const texturePixel& p)
	{
		return os << "[" << p.row << ',' << p.col << ':' <<
			p.pixelRect.x << ' ' << p.pixelRect.y << ' ' <<
			p.pixelRect.w << ' ' << p.pixelRect.h << "]";
	}

	std::ostream& operator<<(std::ostream& os, const matrixPixel& mp)
	{
		os << "[" << mp.row << ',' << mp.col;
		if (mp.filled)
			os << " +]";
		else os << "]";
		return os;

	}


#endif
}