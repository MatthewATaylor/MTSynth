#include "FrequencyTable.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			FrequencyTable::FrequencyTable() {
				const double C0 = 16.35160;
				for (uint8 i = 0; i < 128; ++i) {
					table[i] = C0 * std::pow(2.0, i / 12.0);
					FDebugPrint((std::to_string(i) + " " + std::to_string(table[i]) + "\n").c_str());
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
