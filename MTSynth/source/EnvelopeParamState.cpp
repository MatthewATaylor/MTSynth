#include "EnvelopeParamState.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const double EnvelopeParamState::MAX_TIME_S = 3.0;

			tresult EnvelopeParamState::getState(IBStreamer *streamer) {
				if (!streamer->writeDouble(a)) {
					return kResultFalse;
				}
				if (!streamer->writeDouble(d)) {
					return kResultFalse;
				}
				if (!streamer->writeDouble(s)) {
					return kResultFalse;
				}
				if (!streamer->writeDouble(r)) {
					return kResultFalse;
				}
				return kResultTrue;
			}

			tresult EnvelopeParamState::setState(IBStreamer *streamer) {
				if (!streamer->readDouble(a)) {
					return kResultFalse;
				}
				if (!streamer->readDouble(d)) {
					return kResultFalse;
				}
				if (!streamer->readDouble(s)) {
					return kResultFalse;
				}
				if (!streamer->readDouble(r)) {
					return kResultFalse;
				}
				return kResultTrue;
			}
		}
	}
}
