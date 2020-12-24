#include "Controller.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const FUID Controller::ID(0xD3B78CC4, 0xEE464175, 0xBCAA060F, 0xE46E5C08);

			FUnknown *Controller::createInstance(void*) {
				return static_cast<IEditController*>(new Controller());
			}

			// EditController
			tresult PLUGIN_API Controller::initialize(FUnknown *context) {
				tresult result = EditController::initialize(context);
				if (result == kResultTrue) {
					// Parameters
					Parameter *param = new RangeParameter(
						USTRING("Volume"), static_cast<ParamID>(Params::IDs::VOLUME),
						USTRING("%"), 0, 100, 80
					);
					param->setPrecision(1);
					parameters.addParameter(param);

					parameters.addParameter(
						new RangeParameter(
							USTRING("Active Voices"), static_cast<ParamID>(Params::IDs::ACTIVE_VOICES),
							nullptr, 0, 64, 0, 64, ParameterInfo::kIsReadOnly
						)
					);
				}
				return kResultTrue;
			}
			tresult PLUGIN_API Controller::terminate() {
				return EditController::terminate();
			}
			tresult PLUGIN_API Controller::setComponentState(IBStream *state) {
				ParamValue volume;
				IBStreamer streamer(state, kLittleEndian);
				if (!streamer.readDouble(volume)) {
					return kResultFalse;
				}
				setParamNormalized(static_cast<ParamID>(Params::IDs::VOLUME), volume);
				return kResultTrue;
			}
		}
	}
}
