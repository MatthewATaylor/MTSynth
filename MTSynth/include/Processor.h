#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "Voice.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class VoiceProcessor;

			//-----------------------------------------------------------------------------
			/** Example Note Expression Audio Processor

			\sa Steinberg::Vst::VoiceProcessor
			\sa Steinberg::Vst::VoiceBase
			*/
			class Processor : public AudioEffect
			{
			public:
				Processor();

				tresult PLUGIN_API initialize(FUnknown* context) SMTG_OVERRIDE;
				tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) SMTG_OVERRIDE;

				tresult PLUGIN_API setState(IBStream* state) SMTG_OVERRIDE;
				tresult PLUGIN_API getState(IBStream* state) SMTG_OVERRIDE;

				tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) SMTG_OVERRIDE;
				tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
				tresult PLUGIN_API process(ProcessData& data) SMTG_OVERRIDE;

				static FUnknown* createInstance(void*) { return (IAudioProcessor*)new Processor(); }

				static FUID cid;
			protected:
				VoiceProcessor* voiceProcessor;
				GlobalParameterState paramState;
			};

		}
	}
} // namespaces
