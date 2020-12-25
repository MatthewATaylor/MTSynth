#include "FrequencyTable.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			FrequencyTable::FrequencyTable() {
				const double C2 = 65.40639;
				for (uint8 i = 0; i < 128; ++i) {
					table[i] = C2 * std::pow(2.0, i / 12.0);
				}
			}

			double *FrequencyTable::get() {
				// Only generate table once
				static FrequencyTable *frequencyTable = new FrequencyTable;
				
				return frequencyTable->table;
			}
		}
	}
}
