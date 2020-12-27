#pragma once

#include <chrono>
#include <stdexcept>

#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Filter {
			private:
				double prevInput = 0.0;
				double prevOutput = 0.0;

			public:
				double getLowPass(double input, double rc, SampleRate sampleRate);
				double getHighPass(double input, double rc, SampleRate sampleRate);
				void reset();
			};
		}
	}
}
