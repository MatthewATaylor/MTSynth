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
					Parameter *param = new RangeParameter(USTRING("Volume"), 10, USTRING("%"), 0, 100, 80);
					param->setPrecision(1);
					parameters.addParameter(param);
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
				setParamNormalized(10, volume);
				return kResultTrue;
			}

			// INoteExpressionController
			int32 PLUGIN_API Controller::getNoteExpressionCount(int32 busIndex, int16 channel) {

			}
			tresult PLUGIN_API Controller::getNoteExpressionInfo(
				int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo &info
			) {

			}
			tresult PLUGIN_API Controller::getNoteExpressionStringByValue(
				int32 busIndex, int16 channel, NoteExpressionTypeID id,
				NoteExpressionValue valueNormalized, String128 string
			) {

			}
			tresult PLUGIN_API Controller::getNoteExpressionValueByString(
				int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar *string,
				NoteExpressionValue &valueNormalized
			) {

			}

			// IMidiMapping
			tresult PLUGIN_API Controller::getMidiControllerAssignment(
				int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID &id
			) {
				if (busIndex == 0 && channel == 0 && midiControllerNumber < kCountCtrlNumber) {

				}
			}

			// INoteExpressionPhysicalUIMapping
			tresult PLUGIN_API Controller::getPhysicalUIMapping(
				int32 busIndex, int16 channel, PhysicalUIMapList &list
			) {

			}
		}
	}
}
