#include "FrequencyTable.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			FrequencyTable::FrequencyTable() {
				const double C = 8.1758; // C-1
				for (uint8 i = 0; i < 128; ++i) {
					table[i] = C * std::pow(2.0, i / 12.0);
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
