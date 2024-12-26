#include "pixel.h"

namespace tetris
{
	PixelArray::PixelArray(int pixelArrayLength)
	{
		if (pixelArrayLength <= 0)
		{
#ifdef LOGS
			std::cerr << "pixel array must be > 0\n";
#endif
			this->pixelArrayLength = 1;
		}
		this->pixelArrayLength = pixelArrayLength;
		pixelArray = new pixel[this->pixelArrayLength];

	}

	PixelArray::~PixelArray()
	{
		delete[] pixelArray;
		pixelArray = nullptr;
	}

	pixel* PixelArray::operator[](const int index)
	{
		if (index < 0 || index >= this->pixelArrayLength) return nullptr;
		return &pixelArray[index];
	}

#ifdef LOGS
	std::ostream& operator<<(std::ostream& os, const pixel& p)
	{
		return os << "[" << p.row << ',' << p.col << ':' <<
			p.pixelRect.x << ' ' << p.pixelRect.y << ' ' <<
			p.pixelRect.w << ' ' << p.pixelRect.h << "]";
	}

	std::ostream& operator<<(std::ostream& os, const PixelArray& pa)
	{
		int count = 0;
		for (int i = 0; i < pa.pixelArrayLength; ++i)
		{
			os << pa.pixelArray[i] << ' ';
			count++;
			if (count > 9)
			{
				os << std::endl;
				count = 0;
			}
		}
		return os;
	}
#endif
}