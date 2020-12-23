#include "Processor.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			const FUID Processor::ID(0x5723451D, 0x439B4252, 0x91216D73, 0x853F5169);

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
				SpeakerArrangement *inputs, int32 numInputs,
				SpeakerArrangement *outputs, int32 numOutputs
			) {
				// Support one stereo output
				if (numInputs == 0 && numOutputs == 1 && outputs[0] == SpeakerArr::kStereo) {
					return AudioEffect::setBusArrangements(inputs, numInputs, outputs, numOutputs);
				}
				return kResultFalse;
			}

			tresult PLUGIN_API Processor::setState(IBStream *state) {
				IBStreamer streamer(state, kLittleEndian);
				if (!streamer.readDouble(Params::volume)) {
					return kResultFalse;
				}
				return kResultTrue;
			}
			tresult PLUGIN_API Processor::getState(IBStream *state) {
				IBStreamer streamer(state, kLittleEndian);
				streamer.writeDouble(Params::volume);
				return kResultTrue;
			}

			tresult PLUGIN_API Processor::canProcessSampleSize(int32 symbolicSampleSize) {
				// Support 32-bit and 64-bit precision
				if (symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64) {
					return kResultTrue;
				}
				return kResultFalse;
			}
			tresult PLUGIN_API Processor::setActive(TBool state) {
				if (state) {
					//Reset all voices when setting active
					for (uint8 i = 0; i < VoiceProcessor::MAX_VOICES; ++i) {
						voiceProcessor.getVoice(i)->reset();
					}
				}
				return AudioEffect::setActive(state);
			}
			tresult PLUGIN_API Processor::process(ProcessData& data) {
				if (data.inputParameterChanges) {
					int32 count = data.inputParameterChanges->getParameterCount();
					for (int32 i = 0; i < count; ++i) {
						IParamValueQueue *queue = data.inputParameterChanges->getParameterData(i);
						if (queue) {
							int32 sampleOffset;
							ParamValue value;
							ParamID paramID = queue->getParameterId();
							if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) == kResultTrue) {
								switch (paramID) {
								case static_cast<int>(Params::IDs::VOLUME):
									Params::volume = value;
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
						IParamValueQueue* queue = data.outputParameterChanges->addParameterData(
							static_cast<ParamID>(Params::IDs::ACTIVE_VOICES), index
						);
						if (queue) {
							queue->addPoint(
								0,
								static_cast<double>(voiceProcessor.getNumActiveVoices()) / VoiceProcessor::MAX_VOICES,
								index
							);
						}
					}
					if (voiceProcessor.getNumActiveVoices() == 0 && data.numOutputs > 0) {
						data.outputs[0].silenceFlags = 0x11; // Left and right channel are silent
					}
				}
				return result;
			}
		}
	}
}
