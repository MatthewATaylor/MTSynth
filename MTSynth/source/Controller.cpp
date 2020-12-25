#include "Controller.h"
#include "Voice.h" // only needed for setComponentState
#include "base/source/fstring.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/base/ustring.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {

			FUID Controller::cid(0x2AC0A888, 0x9406497F, 0xBBA6EABF, 0xC78D1372);

			//-----------------------------------------------------------------------------
			//-----------------------------------------------------------------------------
			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::initialize(FUnknown* context)
			{
				tresult result = EditController::initialize(context);
				if (result == kResultTrue)
				{
					// Init parameters
					Parameter* param;

					param = new RangeParameter(USTRING("Master Volume"), ParamState::VOLUME_ID, USTRING("%"), 0, 100, 80);
					param->setPrecision(1);
					parameters.addParameter(param);

					parameters.addParameter(new RangeParameter(USTRING("Active Voices"), ParamState::ACTIVE_VOICES_ID, nullptr, 0, VoiceProcessor::MAX_VOICES, 0, VoiceProcessor::MAX_VOICES, ParameterInfo::kIsReadOnly));

					// Init Default MIDI-CC Map
					std::for_each(midiCCMapping.begin(), midiCCMapping.end(), [](ParamID& pid) { pid = INVALID_PARAM_ID; });
					midiCCMapping[ControllerNumbers::kCtrlVolume] = ParamState::VOLUME_ID;
				}
				return kResultTrue;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::terminate()
			{
				return EditController::terminate();
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::setComponentState(IBStream* state)
			{
				ParamState paramState;
				tresult result = paramState.setState(state);
				if (result == kResultTrue) {
					setParamNormalized(ParamState::VOLUME_ID, paramState.volume);
				}
				return result;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Controller::getMidiControllerAssignment(int32 busIndex, int16 channel,
				CtrlNumber midiControllerNumber,
				ParamID& id /*out*/)
			{
				if (busIndex == 0 && channel == 0 && midiControllerNumber < kCountCtrlNumber)
				{
					if (midiCCMapping[midiControllerNumber] != INVALID_PARAM_ID)
					{
						id = midiCCMapping[midiControllerNumber];
						return kResultTrue;
					}
				}
				return kResultFalse;
			}

			//------------------------------------------------------------------------
		} // NoteExpressionSynth
	} // Vst
} // Steinberg
