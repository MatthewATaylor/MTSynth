#pragma once

#include <cmath>

#include "pluginterfaces/base/ftypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			struct Frequencies {
				double table[128];
				
				Frequencies();
			};
		}
	}
}
