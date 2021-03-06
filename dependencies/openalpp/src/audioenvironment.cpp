/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume? University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "openalpp/audioenvironment.h"

using namespace openalpp;

AudioEnvironment::AudioEnvironment() throw (InitError): AudioBase() {
}

AudioEnvironment::AudioEnvironment(int frequency,int refresh,bool synchronous)
  throw (InitError)
  : AudioBase(frequency,refresh,synchronous) {
}

AudioEnvironment::AudioEnvironment(int frequency,int refresh) throw (InitError)
  : AudioBase(frequency,refresh) {
}

void AudioEnvironment::setSoundVelocity(float speed) throw(ValueError,FatalError){
  alDopplerVelocity(speed);
  ALenum error;
  if((error=alGetError())!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError((const char *)alGetString(error));
	break;
      default:
	throw FatalError("Unknown error in AudioEnvironment::SetSoundSpeed()");
	break;
    }
}

float AudioEnvironment::getSoundVelocity() throw (FatalError) {
  ALfloat speed;
  alGetFloatv(AL_DOPPLER_VELOCITY,&speed);
  if(alGetError()!=AL_FALSE)  // This isn't strictly necessary...
    throw FatalError("Unknown error in AudioEnviroment::GetSoundVelocity()");
  return speed;
}

void AudioEnvironment::setDopplerFactor(float factor) 
  throw (ValueError,FatalError) {
  alDopplerFactor(factor);
  ALenum error;
  if((error=alGetError())!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError((const char *)alGetString(error));
	break;
      default:
	throw FatalError("Unknown error in AudioEnvironment::SetDopplerFactor()");
	break;
    }
}

float AudioEnvironment::getDopplerFactor() throw (FatalError) {
  ALfloat(factor);
  alGetFloatv(AL_DOPPLER_FACTOR,&factor);
  if(alGetError()!=AL_FALSE)  // This isn't strictly necessary...
    throw FatalError("Unknown error in AudioEnvironment::GetDopplerFactor()");
  return factor;
}

void AudioEnvironment::setGain(float gain) {
  alListenerf(AL_GAIN,gain);
  ALenum error=alGetError();
  if(error!=AL_FALSE)
    switch(error) {
      case(AL_INVALID_VALUE):
	throw ValueError("Invalid value for gain");
	break;
      default:
	throw FatalError("Error trying to set gain!");
    }
}

float AudioEnvironment::getGain() throw (FatalError) {  
  ALfloat gain;
  alGetListenerf(AL_GAIN,&gain);
  if(alGetError()!=AL_FALSE)          // This isn't strictly necessary...
    throw FatalError("Unknown error in AudioEnvironment::GetGain()");
  return gain;
}

void AudioEnvironment::setDistanceModel(DistanceModel model)
  throw (FatalError){
  switch(model) {
    case(None):
      alDistanceModel(AL_NONE);
      break;
    case(InverseDistance):
      alDistanceModel(AL_INVERSE_DISTANCE);
      break;
    case(InverseDistanceClamped):
      alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
      break;
    default:
      throw FatalError("Unknown model in AudioEnvironment::SetDistanceModel()");
  }
  if(alGetError()!=AL_FALSE)
    throw FatalError("alDistanceModel failed in AudioEnvironment::SetDistanceModel()");
}

DistanceModel AudioEnvironment::getDistanceModel() throw (FatalError) {
  int almodel;
  alGetIntegerv(AL_DISTANCE_MODEL,&almodel);
  switch(almodel) {
    case(AL_NONE):
      return None;
    case(AL_INVERSE_DISTANCE):
      return InverseDistance;
    case(AL_INVERSE_DISTANCE_CLAMPED):
      return InverseDistanceClamped;
    default:
      throw FatalError("Unknown distance model in AudioEnvironment::GetDistanceModel");
  }
}

// Static fields in AudioBase
bool AudioBase::reverbinitiated_;
void (*AudioBase::alReverbScale)(ALuint sid, ALfloat param);
void (*AudioBase::alReverbDelay)(ALuint sid, ALfloat param);

void AudioEnvironment::initiateReverb() throw (InitError) {
  if (reverbinitiated_)
    return;
 
  alReverbScale=(void (*)(ALuint sid, ALfloat param))
    alGetProcAddress((ALchar *)"alReverbScale_LOKI");
  alReverbDelay=(void (*)(ALuint sid, ALfloat param))
    alGetProcAddress((ALchar *)"alReverbDelay_LOKI");
  if(!(alReverbScale && alReverbDelay)) {
    throw InitError("Couldn't initiate reverb");
  } else
  reverbinitiated_=true;
}

