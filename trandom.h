#pragma once
#include <random>

namespace tetris
{
	class TRandom
	{
	private:
		std::mt19937 generator;
		std::uniform_int_distribution<> distribution;

	public:
		TRandom() :
			generator(std::random_device{}()),
			distribution(0, 6) {}

		int GetRandom()
		{
			return distribution(generator);
		}
	};
}