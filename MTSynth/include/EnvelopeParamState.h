#pragma once

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class EnvelopeParamState {
			public:
				static const double MAX_TIME_S;

				ParamValue a;
				ParamValue d;
				ParamValue s;
				ParamValue r;

				tresult getState(IBStreamer *streamer);
				tresult setState(IBStreamer *streamer);
			};
		}
	}
}
