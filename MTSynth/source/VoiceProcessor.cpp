#include "VoiceProcessor.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			Voice *VoiceProcessor::getBestVoice(int32 noteID) {
				Voice *firstFreeVoice = nullptr;
				if (noteID != -1) {
					for (uint8 i = 0; i < MAX_VOICES; ++i) {
						if (voices[i].getNoteID() == noteID) {
							return &voices[i];
						}
						else if (!firstFreeVoice && voices[i].getNoteID() == -1) {
							firstFreeVoice = &voices[i];
						}
					}
				}
				return firstFreeVoice;
			}

			Voice *VoiceProcessor::findMatchingVoice(int32 noteID) {
				if (noteID != -1) {
					for (uint8 i = 0; i < MAX_VOICES; ++i) {
						if (voices[i].getNoteID() == noteID) {
							return &voices[i];
						}
					}
				}
				return nullptr;
			}

			Voice *VoiceProcessor::getVoice(uint8 index) {
				return &voices[index];
			}

			int32 VoiceProcessor::getNumActiveVoices() {
				return numActiveVoices;
			}

			tresult VoiceProcessor::process(ProcessData &data) {
				static uint32 x = 0;
				int32 numSamples = data.numSamples;
				IEventList *inputEvents = data.inputEvents;
				int32 numEvents = inputEvents ? inputEvents->getEventCount() : 0;
				int32 eventIndex = 0;
				int32 samplesProcessed = 0;

				Event e = {};
				if (numEvents) {
					inputEvents->getEvent(0, e);
				}

				const uint8 NUM_CHANNELS = 2;
				double *buffers[NUM_CHANNELS];
				for (uint8 i = 0; i < NUM_CHANNELS; ++i) {
					buffers[i] = (double*)data.outputs[0].channelBuffers64[i];
					std::memset(buffers[i], 0, numSamples * sizeof(double)); // Set buffer to all zeros
				}

				for (int32 i = 0; i < numSamples; ++i) {
					double sample = std::sin(x * 2 * 3.14 * 440 / 44100);
					buffers[0][i] = sample;
					buffers[1][i] = sample;
					FDebugPrint((std::to_string(x) + ", " + std::to_string(buffers[0][i]) + "\n").c_str());
					++x;
				}

				//while (numSamples > 0) {
				//	// Process CHUNK_SIZE samples (or all remaining)
				//	int32 samplesToProcess = std::min(static_cast<int32>(CHUNK_SIZE), numSamples);
				//	bool canProcessEvent = true;
				//	while (canProcessEvent) {
				//		if (e.sampleOffset > samplesToProcess) {
				//			e.sampleOffset -= samplesToProcess;
				//			break;
				//		}

				//		switch (e.type) {
				//		case Event::kNoteOnEvent: {
				//			if (e.noteOn.noteId == -1) {
				//				e.noteOn.noteId = e.noteOn.pitch;
				//			}
				//			Voice *voice = getBestVoice(e.noteOn.noteId);
				//			if (voice) {
				//				voice->noteOn(e.noteOn.noteId, e.noteOn.pitch, e.noteOn.velocity, e.sampleOffset);
				//				++numActiveVoices;
				//			}
				//			break;
				//		}
				//		case Event::kNoteOffEvent: {
				//			if (e.noteOff.noteId == -1) {
				//				e.noteOff.noteId = e.noteOff.pitch;
				//			}
				//			Voice *voice = findMatchingVoice(e.noteOff.noteId);
				//			if (voice) {
				//				voice->noteOff(e.noteOff.velocity, e.sampleOffset);
				//			}
				//			break;
				//		}
				//		}

				//		// Get next event
				//		++eventIndex;
				//		if (eventIndex >= numEvents || inputEvents->getEvent(eventIndex, e) != kResultTrue) {
				//			canProcessEvent = false; // Ends while loop
				//		}
				//		else {
				//			e.sampleOffset -= samplesProcessed;
				//		}
				//	}

				//	// Process chunk
				//	for (uint8 i = 0; i < MAX_VOICES; ++i) {
				//		if (voices[i].getNoteID() != -1) {
				//			if (!voices[i].process(buffers, samplesToProcess)) {
				//				voices[i].reset();
				//				--numActiveVoices;
				//			}
				//		}
				//	}

				//	// Print buffer
				//	for (int32 i = 0; i < samplesToProcess; ++i) {
				//		double sample = std::sin(x * 0.2);
				//		buffers[0][i] = sample;
				//		buffers[1][i] = sample;
				//		FDebugPrint((std::to_string(x) + ", " + std::to_string(buffers[0][i]) + "\n").c_str());
				//		++x;
				//	}

				//	for (uint8 i = 0; i < NUM_CHANNELS; ++i) {
				//		buffers[i] += samplesToProcess; // Move buffer pointer forwards
				//	}
				//	numSamples -= samplesToProcess;
				//	samplesProcessed += samplesToProcess;
				//}

				return kResultTrue;
			}
		}
	}
}
