#pragma once

#include <chrono>
#include <stdexcept>

class Filter {
private:
	unsigned int prevInput = 0;
	unsigned int prevHighPassOutput = 0;
	unsigned int prevLowPassOutput = 0;

	//std::chrono::steady_clock::time_point prevTime = std::chrono::steady_clock::now();

public:
	enum class Type {
		LOW_PASS, HIGH_PASS, BAND_PASS
	};

	unsigned int get(unsigned int input, double rc, Type type);
};
