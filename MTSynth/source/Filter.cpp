#include "Filter.h"

unsigned int Filter::get(unsigned int input, double rc, Type type) {
	return 0;
	/*
	auto currentTime = std::chrono::steady_clock::now();
	double elapsedTime_s =
		std::chrono::duration_cast<std::chrono::duration<double, std::chrono::seconds>>(
			currentTime - prevTime
		).count();

	// High-pass RC filter -> low-pass RC filter

	// High-pass filter component
	double highPassAlpha = rc / (rc + elapsedTime_s);
	unsigned int highPassOutput = highPassAlpha * prevHighPassOutput +
		highPassAlpha * (input - prevInput);

	// Low-pass filter component
	double lowPassAlpha = elapsedTime_s / (rc + elapsedTime_s);
	unsigned int lowPassOutput = lowPassAlpha * highPassOutput +
		(1 - lowPassAlpha) * prevLowPassOutput;

	prevInput = input;
	prevHighPassOutput = highPassOutput;
	prevLowPassOutput = lowPassOutput;

	prevTime = currentTime;

	switch (type) {
	case Type::LOW_PASS:
		return input - highPassOutput;
		break;
	case Type::HIGH_PASS:
		return highPassOutput - lowPassOutput;
		break;
	case Type::BAND_PASS:
		return lowPassOutput;
		break;
	}

	throw std::exception("Unimplemented filter type");
	*/
}
