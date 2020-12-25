#include "Processor.h"
#include "VoiceProcessor.h"
#include "Controller.h"
#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <algorithm>

namespace Steinberg {
	namespace Vst {
		namespace mts {

			//-----------------------------------------------------------------------------
			FUID Processor::cid(0x6EE65CD1, 0xB83A4AF4, 0x80AA7929, 0xAEA6B8A0);

			//-----------------------------------------------------------------------------
			Processor::Processor()
			{
				setControllerClass(Controller::cid);
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::initialize(FUnknown* context)
			{
				tresult result = AudioEffect::initialize(context);
				if (result == kResultTrue)
				{
					addAudioOutput(STR16("Audio Output"), SpeakerArr::kStereo);
					addEventInput(STR16("Event Input"), 1);
				}
				return result;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::setState(IBStream* state)
			{
				return ParamState::global.setState(state);
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::getState(IBStream* state)
			{
				return ParamState::global.getState(state);
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::setBusArrangements(SpeakerArrangement* inputs, int32 numIns,
				SpeakerArrangement* outputs, int32 numOuts)
			{
				// we only support one stereo output bus
				if (numIns == 0 && numOuts == 1 && outputs[0] == SpeakerArr::kStereo)
				{
					return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
				}
				return kResultFalse;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::canProcessSampleSize(int32 symbolicSampleSize)
			{
				if (symbolicSampleSize == kSample32 || symbolicSampleSize == kSample64)
				{
					return kResultTrue;
				}
				return kResultFalse;
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::setActive(TBool state)
			{
				if (state)
				{
					for (int i = 0; i < VoiceProcessor::MAX_VOICES; ++i) {
						voiceProcessor.voices[i].reset();
					}
				}
				else
				{

				}
				return AudioEffect::setActive(state);
			}

			//-----------------------------------------------------------------------------
			tresult PLUGIN_API Processor::process(ProcessData& data)
			{
				// TODO: maybe try to make this nearly sample accurate
				if (data.inputParameterChanges)
				{
					int32 count = data.inputParameterChanges->getParameterCount();
					for (int32 i = 0; i < count; i++)
					{
						IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
						if (queue)
						{
							int32 sampleOffset;
							ParamValue value;
							ParamID pid = queue->getParameterId();
							if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) ==
								kResultTrue)
							{
								switch (pid)
								{
								case ParamState::VOLUME_ID:
								{
									ParamState::global.volume = value;
									break;
								}
								}
							}
						}
					}
				}
				tresult result;

				// flush mode
				if (data.numOutputs < 1)
					result = kResultTrue;
				else
					result = voiceProcessor.process(data);
				if (result == kResultTrue)
				{
					if (data.outputParameterChanges)
					{
						int32 index;
						IParamValueQueue* queue =
							data.outputParameterChanges->addParameterData(ParamState::ACTIVE_VOICES_ID, index);
						if (queue)
						{
							queue->addPoint(
								0, (ParamValue)voiceProcessor.getActiveVoices() / (ParamValue)VoiceProcessor::MAX_VOICES,
								index);
						}
					}
					if (voiceProcessor.getActiveVoices() == 0 && data.numOutputs > 0)
					{
						data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
					}
				}
				return result;
			}
		} // NoteExpressionSynth
	} // Vst
} // Steinberg
