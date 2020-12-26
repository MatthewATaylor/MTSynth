#pragma once

#include <cmath>

#include "pluginterfaces/base/ftypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class FrequencyTable {
			private:
				double table[128];

				FrequencyTable();

			public:
				// Get table of 128 frequencies (starting at C-1, where A4 = 440 Hz)
				static double *get();
			};
		}
	}
}
