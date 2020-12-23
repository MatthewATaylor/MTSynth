#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			struct Params {
				enum class IDs {
					VOLUME,
					ACTIVE_VOICES
				};

				static ParamValue volume;
			};
		}
	}
}
