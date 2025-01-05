#include "textureStorage.h"


namespace tetris
{
	TexturesStorage::TexturesStorage()
	{
		const char* tetrisTextureDataFileName = "tetrisdata.data";
		const auto  tetrisTextureDataLength{ 1831 };


	}

	void TexturesStorage::readData(uint8_t* buffer, const char* fileName,
		int dataLength)
	{
		std::ifstream inFile(fileName, std::ios::binary);
		if (!inFile.is_open())
		{
#ifdef LOGS
			std::cerr << "Cannot open file " << fileName << ", abort.\n";
#endif
			initOk = false; return;
		}

		// decrypt data file:
		char byte;
		char key = 'K';
		int index{ 0 };
		while (inFile.get(byte))
		{
			buffer[index++] = static_cast<uint8_t>(byte ^ key);
		}


	}
}