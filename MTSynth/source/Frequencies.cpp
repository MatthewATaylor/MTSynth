#include "Frequencies.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			Frequencies::Frequencies() {
				// 0 = C2, 127 = G8, A4 = 440 Hz
				const double C2 = 65.40639;
				for (uint8 i = 0; i < 128; ++i) {
					table[i] = C2 * std::pow(2.0 / 12.0, i);
				}
			}
		}
	}
}
