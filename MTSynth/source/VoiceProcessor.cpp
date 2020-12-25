#include "VoiceProcessor.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			Voice* VoiceProcessor::getVoice(int32 noteId) {
				Voice* firstFreeVoice = 0;
				if (noteId != -1)
				{
					for (int32 i = 0; i < MAX_VOICES; i++)
					{
						if (voices[i].getNoteID() == noteId)
						{
							return &voices[i];
						}
						else if (firstFreeVoice == 0 && voices[i].getNoteID() == -1)
						{
							firstFreeVoice = &voices[i];
						}
					}
				}
				return firstFreeVoice;
			}
			
			Voice* VoiceProcessor::findVoice(int32 noteId) {
				if (noteId != -1)
				{
					for (int32 i = 0; i < MAX_VOICES; i++)
					{
						if (voices[i].getNoteID() == noteId)
						{
							return &voices[i];
						}
					}
				}
				return 0;
			}

			tresult VoiceProcessor::process(ProcessData &data) {
				const int32 kBlockSize = VOICEPROCESSOR_BLOCKSIZE;

				int32 numSamples = data.numSamples;
				int32 samplesProcessed = 0;
				int32 i;

				IEventList* inputEvents = data.inputEvents;
				Event e = {};
				Event* eventPtr = nullptr;
				int32 eventIndex = 0;
				int32 numEvents = inputEvents ? inputEvents->getEventCount() : 0;

				// get the first event
				if (numEvents)
				{
					inputEvents->getEvent(0, e);
					eventPtr = &e;
				}

				// initialize audio output buffers
				float* buffers[2];
				for (i = 0; i < 2; i++)
				{
					buffers[i] = (float*)data.outputs[0].channelBuffers32[i];
					if (mClearOutputNeeded)
						memset(buffers[i], 0, data.numSamples * sizeof(float));
				}

				while (numSamples > 0)
				{
					int32 samplesToProcess = std::min<int32>(kBlockSize, numSamples);
					while (eventPtr != nullptr)
					{
						// if the event is not in the current processing block then adapt offset for next block
						if (e.sampleOffset > samplesToProcess)
						{
							e.sampleOffset -= samplesToProcess;
							break;
						}

						switch (e.type)
						{
							//-----------------------
						case Event::kNoteOnEvent:
						{
							if (e.noteOn.noteId == -1)
								e.noteOn.noteId = e.noteOn.pitch;
							Voice* voice = getVoice(e.noteOn.noteId);
							if (voice)
							{
								voice->noteOn(e.noteOn.pitch, e.noteOn.velocity, e.noteOn.tuning, e.sampleOffset, e.noteOn.noteId);
								this->activeVoices++;
								//data.outputEvents->addEvent (e);
							}
							break;
						}
						//-----------------------
						case Event::kNoteOffEvent:
						{
							if (e.noteOff.noteId == -1)
								e.noteOff.noteId = e.noteOff.pitch;
							Voice* voice = findVoice(e.noteOff.noteId);
							if (voice)
							{
								voice->noteOff(e.noteOff.velocity, e.sampleOffset);
								//data.outputEvents->addEvent (e);
							}
							#if DEBUG_LOG
							else
							{
								FDebugPrint("Voice for kNoteOffEvent not found : %d\n", e.noteOff.noteId);
							}
							#endif
							break;
						}
						//-----------------------
						}

						// get next event
						eventIndex++;
						if (eventIndex < numEvents)
						{
							if (inputEvents->getEvent(eventIndex, e) == kResultTrue)
							{
								e.sampleOffset -= samplesProcessed;
							}
							else
							{
								eventPtr = nullptr;
							}
						}
						else
						{
							eventPtr = nullptr;
						}
					}	// end while (event != 0)

					// now process the block
					for (i = 0; i < MAX_VOICES; i++)
					{
						if (voices[i].getNoteID() != -1)
						{
							if (!voices[i].process(buffers, samplesToProcess))
							{
								voices[i].reset();
								this->activeVoices--;
							}
						}
					}

					// update the counters
					for (i = 0; i < 2; i++)
						buffers[i] += samplesToProcess;
					numSamples -= samplesToProcess;
					samplesProcessed += samplesToProcess;

				} // end while (numSamples > 0)

				return kResultTrue;
			}
		}
	}
}
