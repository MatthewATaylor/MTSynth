#pragma once

#include "pluginterfaces/vst/vsttypes.h"
#include "base/source/fdebug.h"

#ifdef DEBUG_LOG
#undef DEBUG_LOG
#endif

#define DEBUG_LOG	DEVELOPMENT

namespace Steinberg {
	namespace Vst {
		namespace mts {

			//-----------------------------------------------------------------------------
			/** Example Voice class for the Steinberg::Vst::VoiceProcessorImplementation

			Implementation classes need to implement the following additional method:
			\code{.cpp}
			bool process (SamplePrecision* outputBuffers[numChannels], int32 numSamples);
			\endcode
			*/
			//-----------------------------------------------------------------------------
			template<uint32 numValues, class SamplePrecision, uint32 numChannels, class GlobalParameterStorage>
			class VoiceBase
			{
			public:
				/** Returns the current note id of this voice. */
				int32 getNoteId() const { return noteId; }

				/** Sets a new GlobalParameterStorage. */
				virtual void setGlobalParameterStorage(GlobalParameterStorage* globalParameters) { this->globalParameters = globalParameters; }
				/** Sets the sampleRate. */
				virtual void setSampleRate(ParamValue sampleRate) { this->sampleRate = sampleRate; }
				/** Returns the sampleRate. */
				float getSampleRate() const { return (float)sampleRate; }

				virtual void setNoteExpressionValue(int32 index, ParamValue value) { if (index < numValues) values[index] = value; }

				virtual void noteOn(int32 pitch, ParamValue velocity, float tuning, int32 sampleOffset, int32 noteId);
				virtual void noteOff(ParamValue velocity, int32 sampleOffset);

				virtual void reset()
				{
					noteOnSampleOffset = -1;
					noteOffSampleOffset = -1;
					noteId = -1;
					tuning = 0;
				}

				//-----------------------------------------------------------------------------
			protected:
				VoiceBase();
				VoiceBase(const VoiceBase& vb);
				virtual ~VoiceBase();

				GlobalParameterStorage* globalParameters;

				int32 noteId;
				int32 pitch;
				int32 noteOnSampleOffset;
				int32 noteOffSampleOffset;
				float tuning;

				ParamValue sampleRate;
				ParamValue noteOnVelocity;
				ParamValue noteOffVelocity;
				ParamValue values[numValues];
			};

			//-----------------------------------------------------------------------------
			template<uint32 numValues, class SamplePrecision, uint32 numChannels, class GlobalParameterStorage>
			VoiceBase<numValues, SamplePrecision, numChannels, GlobalParameterStorage>::VoiceBase()
				: globalParameters(0)
				, noteId(-1)
				, pitch(-1)
				, noteOnSampleOffset(0)
				, noteOffSampleOffset(0)
				, sampleRate(44100.)
				, noteOnVelocity(0.)
				, noteOffVelocity(0.)
			{
			}

			//-----------------------------------------------------------------------------
			template<uint32 numValues, class SamplePrecision, uint32 numChannels, class GlobalParameterStorage>
			VoiceBase<numValues, SamplePrecision, numChannels, GlobalParameterStorage>::VoiceBase(const VoiceBase<numValues, SamplePrecision, numChannels, GlobalParameterStorage>& vb)
				: globalParameters(vb.globalParameters)
				, noteId(vb.noteId)
				, pitch(vb.pitch)
				, noteOnSampleOffset(vb.noteOnSampleOffset)
				, noteOffSampleOffset(vb.noteOffSampleOffset)
				, noteOnVelocity(vb.noteOnVelocity)
				, noteOffVelocity(vb.noteOffVelocity)
				, sampleRate(vb.sampleRate)
				, values(0)
			{
				for (uint32 i = 0; i < numValues; i++)
					values[i] = vb.values[i];
			}

			//-----------------------------------------------------------------------------
			template<uint32 numValues, class SamplePrecision, uint32 numChannels, class GlobalParameterStorage>
			VoiceBase<numValues, SamplePrecision, numChannels, GlobalParameterStorage>::~VoiceBase()
			{
			}

			//-----------------------------------------------------------------------------
			template<uint32 numValues, class SamplePrecision, uint32 numChannels, class GlobalParameterStorage>
			void VoiceBase<numValues, SamplePrecision, numChannels, GlobalParameterStorage>::noteOn(int32 pitch, ParamValue velocity, float tuning, int32 sampleOffset, int32 nId)
			{
				this->pitch = pitch;
				noteOnVelocity = velocity;
				noteOnSampleOffset = sampleOffset;
				noteId = nId;
				this->tuning = tuning;

				#if DEBUG_LOG
				FDebugPrint("NoteOn :%d\n", nId);
				#endif
			}

			//-----------------------------------------------------------------------------
			template<uint32 numValues, class SamplePrecision, uint32 numChannels, class GlobalParameterStorage>
			void VoiceBase<numValues, SamplePrecision, numChannels, GlobalParameterStorage>::noteOff(ParamValue velocity, int32 sampleOffset)
			{
				noteOffVelocity = velocity;
				noteOffSampleOffset = sampleOffset;

				#if DEBUG_LOG
				FDebugPrint("NoteOff:%d\n", this->noteId);
				#endif
			}

		}
	} // namespace Vst
} // namespace Steinberg

