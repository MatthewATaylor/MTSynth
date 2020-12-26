#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "ParamState.h"
#include "Voice.h"
#include "VoiceProcessor.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Processor : public AudioEffect {
			private:
				VoiceProcessor voiceProcessor;

			public:
				static const FUID ID;

				static FUnknown *createInstance(void*);

				Processor();

				tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
				tresult PLUGIN_API setBusArrangements(
					SpeakerArrangement *inputs, int32 numInputs, SpeakerArrangement *outputs, int32 numOutputs
				) SMTG_OVERRIDE;

				tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;
				tresult PLUGIN_API setState(IBStream *state) SMTG_OVERRIDE;

				tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
				tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
				tresult PLUGIN_API process(ProcessData &data) SMTG_OVERRIDE;
			};
		}
	}
}
