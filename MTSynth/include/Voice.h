#pragma once

#include <cmath>

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"

#include "ParamState.h"
#include "FrequencyTable.h"

namespace Steinberg {
	namespace Vst {
		namespace mts {
			class Voice {
			private:
				static const double PI;
				static const double SAMPLE_RATE;

				uint32 sampleIndex = 0;

				int32 noteID = -1;
				int32 pitch = -1;
				float tuning = 0.0f;

				float noteOnVelocity = 0.0f;
				float noteOffVelocity = 0.0f;
				
				int32 noteOnSampleOffset = 0;
				int32 noteOffSampleOffset = 0;

				ParamValue volume;

			public:
				void noteOn(int32 noteID, int32 pitch, float tuning, float velocity, int32 sampleOffset);
				void noteOff(float velocity, int32 sampleOffset);
				bool process(float *outputBuffers[2], int32 numSamples);
				void reset();
				int32 getNoteID();
			};

			//-----------------------------------------------------------------------------
			/** Example Note Expression Synth Voice Class

			\sa Steinberg::Vst::VoiceBase
			*/
			/*
			template<class SamplePrecision>
			class Voice : public VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>
			{
			public:
				void noteOn(int32 pitch, ParamValue velocity, float tuning, int32 sampleOffset, int32 nId) SMTG_OVERRIDE;
				void noteOff(ParamValue velocity, int32 sampleOffset) SMTG_OVERRIDE;
				bool process(SamplePrecision* outputBuffers[2], int32 numSamples);
				void reset() SMTG_OVERRIDE;

			protected:
				uint32 n;

				ParamValue currentSinusF;
				ParamValue currentVolume;
			};

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			bool Voice<SamplePrecision>::process(SamplePrecision* outputBuffers[2], int32 numSamples)
			{
				currentVolume = globalParameters->masterVolume;

				//---compute tuning-------------------------
				ParamValue sinusFreq = (VoiceStatics::freqTab[this->pitch]) * M_PI_MUL_2 / this->getSampleRate();
				if (currentSinusF == -1)
					currentSinusF = sinusFreq;

				for (int32 i = 0; i < numSamples; i++)
				{
					this->noteOnSampleOffset--;
					this->noteOffSampleOffset--;

					if (this->noteOnSampleOffset <= 0)
					{
						// we are in Release
						if (this->noteOffSampleOffset == 0)
						{
							currentVolume = 0;
							this->noteOffSampleOffset = this->noteOnSampleOffset = -1;
							return false;
						}
						// sinus osc
						SamplePrecision sample = (SamplePrecision)(sin(n * sinusFreq + sinusPhase));

						n++;

						// store in output
						outputBuffers[0][i] += (SamplePrecision)(sample * currentVolume);
						outputBuffers[1][i] += (SamplePrecision)(sample * currentVolume);
					}
				}
				return true;
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::noteOn(int32 _pitch, ParamValue velocity, float tuning, int32 sampleOffset, int32 nId)
			{
				VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOn(_pitch, velocity, tuning, sampleOffset, nId);
				this->noteOnSampleOffset++;
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::noteOff(ParamValue velocity, int32 sampleOffset)
			{
				VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOff(velocity, sampleOffset);
				this->noteOffSampleOffset++;
			}

			//-----------------------------------------------------------------------------
			template<class SamplePrecision>
			void Voice<SamplePrecision>::reset()
			{
				n = 0;
				sinusPhase = 0.;
				this->values[kVolumeMod] = 0.;

				VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::reset();
			}
			*/
		}
	}
}
