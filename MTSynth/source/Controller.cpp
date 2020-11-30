#include "Controller.h"

namespace Steinberg {
	namespace mts {
		const FUID Controller::ID(0xD3B78CC4, 0xEE464175, 0xBCAA060F, 0xE46E5C08);

		FUnknown *Controller::createInstance(void*) {
			return static_cast<Vst::IEditController*>(new Controller());
		}

		tresult PLUGIN_API Controller::initialize(FUnknown *context) {
			tresult result = EditController::initialize(context);
			if (result == kResultTrue) {
				parameters.addParameter(
					STR16("Test"), nullptr, 0, 0,
					Vst::ParameterInfo::kCanAutomate,
					testParamID
				);
			}
			return result;
		}

		tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
			if (!state) {
				return kResultFalse;
			}

			IBStreamer streamer(state, kLittleEndian);
			float testValue = 0.0f;
			if (!streamer.readFloat(testValue)) {
				return kResultFalse;
			}

			return kResultOk;
		}
	}
}
