#pragma once

#include <cmath>
#include <string>

#include "base/source/fdebug.h"
#include "pluginterfaces/base/ftypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class FrequencyTable {
			private:
				double table[128];

				FrequencyTable();

			public:
				// Get table of 128 frequencies (starting at C0)
				static double *get();
			};
		}
	}
}
