#include "Processor.h"
#include "VoiceProcessor.h"
#include "Controller.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const FUID Processor::ID(0x6EE65CD1, 0xB83A4AF4, 0x80AA7929, 0xAEA6B8A0);

			FUnknown *Processor::createInstance(void*) {
				return static_cast<IAudioProcessor*>(new Processor());
			}

			Processor::Processor() {
				setControllerClass(Controller::ID);
			}

			tresult PLUGIN_API Processor::initialize(FUnknown *context) {
				tresult result = AudioEffect::initialize(context);
				if (result == kResultTrue) {
					addAudioOutput(STR16("Audio Output"), SpeakerArr::kStereo);
					addEventInput(STR16("Event Input"), 1);
				}
				return result;
			}

			tresult PLUGIN_API Processor::setBusArrangements(
				SpeakerArrangement *inputs, int32 numInputs, SpeakerArrangement *outputs, int32 numOutputs
			) {
				// Only support one stereo output bus
				if (numInputs == 0 && numOutputs == 1 && outputs[0] == SpeakerArr::kStereo) {
					return AudioEffect::setBusArrangements(inputs, numInputs, outputs, numOutputs);
				}
				return kResultFalse;
			}

			tresult PLUGIN_API Processor::getState(IBStream *state) {
				return ParamState::global.getState(state);
			}

			tresult PLUGIN_API Processor::setState(IBStream *state) {
				return ParamState::global.setState(state);
			}

			tresult PLUGIN_API Processor::canProcessSampleSize(int32 symbolicSampleSize) {
				// Only support 32-bit or 64-bit sample size
				if (symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64) {
					return kResultTrue;
				}
				return kResultFalse;
			}

			tresult PLUGIN_API Processor::setActive(TBool state) {
				if (state) {
					// Reset all voices when set active
					for (uint8 i = 0; i < VoiceProcessor::MAX_VOICES; ++i) {
						voiceProcessor.voices[i].reset();
					}
				}
				else {

				}
				return AudioEffect::setActive(state);
			}

			tresult PLUGIN_API Processor::process(ProcessData &data) {
				// Handle parameter updates
				if (data.inputParameterChanges) {
					int32 numParamChanges = data.inputParameterChanges->getParameterCount();
					for (int32 i = 0; i < numParamChanges; ++i) {
						IParamValueQueue *queue = data.inputParameterChanges->getParameterData(i);
						if (queue) {
							int32 sampleOffset;
							ParamValue value;

							// Grab the last point in the queue (could grab all points for more accurate sampling)
							if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) == kResultTrue) {
								switch (queue->getParameterId()) {
								case ParamState::VOLUME_ID:
									ParamState::global.volume = value;
									break;
								}
							}
						}
					}
				}

				tresult result;

				if (data.numOutputs < 1) {
					result = kResultTrue;
				}
				else {
					result = voiceProcessor.process(data);
				}

				if (result == kResultTrue) {
					if (data.outputParameterChanges) {
						int32 index;
						IParamValueQueue *queue = data.outputParameterChanges->addParameterData(ParamState::ACTIVE_VOICES_ID, index);
						if (queue) {
							queue->addPoint(
								0,
								static_cast<ParamValue>(voiceProcessor.getNumActiveVoices()) / VoiceProcessor::MAX_VOICES,
								index
							);
						}
					}
					if (voiceProcessor.getNumActiveVoices() == 0 && data.numOutputs > 0)
					{
						data.outputs[0].silenceFlags = 0x11; // Make left and right channels silent
					}
				}

				return result;
			}
		}
	}
}
