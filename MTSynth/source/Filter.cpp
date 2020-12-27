#include "Filter.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			double Filter::getLowPass(double input, double rc, SampleRate sampleRate) {
				double elapsedTime = 1.0 / sampleRate;
				double alpha = elapsedTime / (rc + elapsedTime);
				double output = alpha * input + (1 - alpha) * prevOutput;

				prevOutput = output;

				return output;
			}
			
			double Filter::getHighPass(double input, double rc, SampleRate sampleRate) {
				double elapsedTime = 1.0 / sampleRate;
				double alpha = rc / (rc + elapsedTime);
				double output = alpha * prevOutput + alpha * (input - prevInput);
				
				prevInput = input;
				prevOutput = output;

				return output;
			}

			void Filter::reset() {
				prevInput = 0.0;
				prevOutput = 0.0;
			}
		}
	}
}
