#include "Processor.h"

namespace Steinberg {
	namespace mts {
		const FUID Processor::ID(0x5723451D, 0x439B4252, 0x91216D73, 0x853F5169);
		
		FUnknown *Processor::createInstance(void*) {
			return static_cast<Vst::IAudioProcessor*>(new Processor());
		}

		Processor::Processor() {
			setControllerClass(Controller::ID);
		}

		tresult PLUGIN_API Processor::initialize(FUnknown *context) {
			tresult result = AudioEffect::initialize(context);
			if (result != kResultTrue) {
				return kResultFalse;
			}

			addAudioInput(STR16("Audio Input"), Vst::SpeakerArr::kStereo);
			addAudioOutput(STR16("Audio Output"), Vst::SpeakerArr::kStereo);

			return kResultTrue;
		}

		tresult PLUGIN_API Processor::setBusArrangements(
			Vst::SpeakerArrangement *inputs, int32 numInputs,
			Vst::SpeakerArrangement* outputs, int32 numOutputs
		) {
			// One input bus, one output bus, and matching number of channels
			if (numInputs == 1 && numOutputs == 1 && inputs[0] == outputs[0]) {
				return AudioEffect::setBusArrangements(inputs, numInputs, outputs, numOutputs);
			}

			return kResultFalse;
		}

		tresult PLUGIN_API Processor::setupProcessing(Vst::ProcessSetup &setup) {
			// Data acquired from setup: sampleRate, processMode, max samples per audio block
			return AudioEffect::setupProcessing(setup);
		}

		tresult PLUGIN_API Processor::setActive(TBool state) {
			if (state) {
				// Allocate memory
			}
			else {
				// Free memory
			}
			return AudioEffect::setActive(state);
		}

		tresult PLUGIN_API Processor::process(Vst::ProcessData &data) {
			if (data.inputParameterChanges) {
				int32 numParamsChanged = data.inputParameterChanges->getParameterCount();
				for (int32 i = 0; i < numParamsChanged; ++i) {
					Vst::IParamValueQueue *paramQueue =
						data.inputParameterChanges->getParameterData(i);
					
					if (paramQueue) {
						int32 numPoints = paramQueue->getPointCount();
						int32 sampleOffset;
						Vst::ParamValue value;
						
						switch (paramQueue->getParameterId()) {
						case Controller::testParamID:
							if (paramQueue->getPoint(numPoints - 1, sampleOffset, value) == kResultTrue) {
								testParam = value;
							}
							break;
						}
					}
				}
			}

			if (data.numInputs == 0 || data.numOutputs == 0) {
				return kResultOk;
			}

			if (data.numSamples > 0) {
				// Process audio
			}

			return kResultOk;
		}

		tresult PLUGIN_API Processor::setState(IBStream *state) {
			// Reload parameters when preset/project is loaded

			if (!state) {
				return kResultFalse;
			}

			IBStreamer streamer(state, kLittleEndian);
			float testValue = 0.0f;
			if (!streamer.readFloat(testValue)) {
				return kResultFalse;
			}
			testParam = testValue;

			return kResultOk;
		}

		tresult PLUGIN_API Processor::getState(IBStream *state) {
			// Save parameters
			IBStreamer streamer(state, kLittleEndian);
			streamer.writeFloat(static_cast<float>(testParam));
			return kResultOk;
		}
	}
}
