#pragma once

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

#include "Controller.h"

namespace Steinberg {
	namespace mts {
		class Processor : public Vst::AudioEffect {
		protected:
			Vst::ParamValue testParam;

		public:
			static const FUID ID;

			static FUnknown *createInstance(void*);

			Processor();

			tresult PLUGIN_API initialize(FUnknown *context) SMTG_OVERRIDE;
			tresult PLUGIN_API setBusArrangements(
				Vst::SpeakerArrangement *inputs, int32 numInputs,
				Vst::SpeakerArrangement *outputs, int32 numOutputs
			) SMTG_OVERRIDE;
			
			tresult PLUGIN_API setupProcessing(Vst::ProcessSetup &setup) SMTG_OVERRIDE;
			tresult PLUGIN_API setActive(TBool state) SMTG_OVERRIDE;
			tresult PLUGIN_API process(Vst::ProcessData &data) SMTG_OVERRIDE;

			tresult PLUGIN_API setState(IBStream *state) SMTG_OVERRIDE;
			tresult PLUGIN_API getState(IBStream *state) SMTG_OVERRIDE;
		};
	}
}
