#pragma once

namespace Steinberg {
	namespace Vst {
		namespace mts {
			inline Voice *VoiceProcessor::getBestVoice(int32 noteID) {
				Voice *firstFreeVoice = nullptr;
				if (noteID != -1) {
					for (int32 i = 0; i < MAX_VOICES; ++i) {
						if (voices[i].getNoteID() == noteID) {
							return &voices[i];
						}
						else if (firstFreeVoice == 0 && voices[i].getNoteID() == -1) {
							firstFreeVoice = &voices[i];
						}
					}
				}
				return firstFreeVoice;
			}

			inline Voice *VoiceProcessor::findMatchingVoice(int32 noteID) {
				if (noteID != -1) {
					for (int32 i = 0; i < MAX_VOICES; ++i) {
						if (voices[i].getNoteID() == noteID) {
							return &voices[i];
						}
					}
				}
				return nullptr;
			}

			template <typename SampleType>
			inline tresult VoiceProcessor::process(ProcessData &data, SampleRate sampleRate) {
				int32 numSamples = data.numSamples;
				int32 samplesProcessed = 0;

				IEventList *inputEvents = data.inputEvents;
				int32 numEvents = inputEvents ? inputEvents->getEventCount() : 0;
				int32 eventIndex = 0;

				Event e = {};
				bool canProcessEvents = false;

				// Get first event
				if (numEvents) {
					inputEvents->getEvent(0, e);
					canProcessEvents = true;
				}

				// Initialize audio output buffers
				SampleType **buffers;
				if (typeid(SampleType) == typeid(Sample32)) {
					buffers = (SampleType**)data.outputs[0].channelBuffers32;
				}
				else if (typeid(SampleType) == typeid(Sample64)) {
					buffers = (SampleType**)data.outputs[0].channelBuffers64;
				}
				else {
					return kInvalidArgument;
				}
				for (uint8 i = 0; i < Voice::NUM_CHANNELS; ++i) {
					std::memset(buffers[i], 0, data.numSamples * sizeof(SampleType)); // Set buffer to all zeros
				}

				while (numSamples > 0) {
					int32 samplesToProcess = std::min<int32>(CHUNK_SIZE, numSamples);
					while (canProcessEvents) {
						// Event not in current chunk
						if (e.sampleOffset > samplesToProcess) {
							e.sampleOffset -= samplesToProcess;
							break;
						}

						switch (e.type) {
						case Event::kNoteOnEvent: {
							if (e.noteOn.noteId == -1) {
								e.noteOn.noteId = e.noteOn.pitch;
							}
							Voice *voice = getBestVoice(e.noteOn.noteId);
							if (voice) {
								voice->noteOn(e.noteOn.noteId, e.noteOn.pitch, e.noteOn.tuning, e.noteOn.velocity, e.sampleOffset);
								++numActiveVoices;
							}
							break;
						}
						case Event::kNoteOffEvent: {
							if (e.noteOff.noteId == -1) {
								e.noteOff.noteId = e.noteOff.pitch;
							}
							Voice *voice = findMatchingVoice(e.noteOff.noteId);
							if (voice) {
								voice->noteOff(e.noteOff.velocity, e.sampleOffset);
							}
							break;
						}
						}

						// Get next event
						++eventIndex;
						if (eventIndex >= numEvents || inputEvents->getEvent(eventIndex, e) == kResultFalse) {
							canProcessEvents = false; // No more events left, end while loop
						}
						else {
							e.sampleOffset -= samplesProcessed;
						}
					}

					// Process chunk
					for (uint8 i = 0; i < MAX_VOICES; ++i) {
						if (voices[i].getNoteID() != -1) {
							if (!voices[i].process(buffers, samplesToProcess, sampleRate)) {
								// Voice is now inactive
								voices[i].reset();
								--numActiveVoices;
							}
						}
					}

					for (uint8 i = 0; i < Voice::NUM_CHANNELS; ++i) {
						buffers[i] += samplesToProcess; // Move buffer pointer forwards
					}
					numSamples -= samplesToProcess;
					samplesProcessed += samplesToProcess;
				}

				return kResultTrue;
			}

			inline int32 VoiceProcessor::getNumActiveVoices() const {
				return numActiveVoices;
			}
		}
	}
}
