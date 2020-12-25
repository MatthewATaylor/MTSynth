#pragma once

#include "VoiceBase.h"
#include "Controller.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"
#include <cmath>
#include <algorithm>
#include <string>

#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif

namespace Steinberg {
	namespace Vst {
		namespace mts {

			//-----------------------------------------------------------------------------
			struct GlobalParameterState
			{

				ParamValue masterVolume;	// [0, +1]

				tresult setState(IBStream* stream);
				tresult getState(IBStream* stream);
			};

			//-----------------------------------------------------------------------------
			enum VoiceParameters
			{
				kVolumeMod,
				kNumParameters
			};

			//-----------------------------------------------------------------------------
			class VoiceStatics
			{
			public:
				//------------------------------------------------------------------------
				static double normalizedLevel2Gain(float normalized)
				{
					double level;
					if (normalized >= 0.5)
						level = scaleHeadRoom * ::pow(10, (normalized - 0.5f) * 24 / 20.0f);
					else
						level = scaleNorm2GainC1 * ::pow(normalized, scaleNorm2GainC2);

					return level;
				}

				enum {
					kNumFrequencies = 128
				};

				static float freqTab[kNumFrequencies];
				static const float scaleHeadRoom;
				static const float scaleNorm2GainC1;
				static const float scaleNorm2GainC2;
				static const double kNormTuningOneOctave;
				static const double kNormTuningOneTune;

			};

			//-----------------------------------------------------------------------------
			/** Example Note Expression Synth Voice Class

			\sa Steinberg::Vst::VoiceBase
			*/
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

				SamplePrecision sinusPhase;
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

		}
	}
} // namespaces
