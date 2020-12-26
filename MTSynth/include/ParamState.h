#pragma once

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class ParamState {
			public:
				static ParamState global;

				enum {
					VOLUME_ID
				};

				ParamValue volume = 0.5;

				tresult getState(IBStream *stream);
				tresult setState(IBStream *stream);
			};
		}
	}
}
