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

#ifndef AUDIOENVIROMENT_H_INCLUDED_C4211030
#define AUDIOENVIROMENT_H_INCLUDED_C4211030

#include "openalpp/audiobase.h"

namespace openalpp {

/**
 * Enum for setting/getting the current distance model.
 * None means no model is in use, i.e. no attenuation.\n
 * InverseDistance:\n
 * - \f$G=GAIN-20*log_{10}*(1+ROLLOFF*\frac{dist-REFERENCE\_DISTANCE}{REFERENCE\_DISTANCE})\f$
 * - \f$G=min (G,MAX\_GAIN)\f$
 * - \f$G=max (G,MIN\_GAIN)\f$
 *
 * InverseDistanceClamped:\n
 * - \f$dist=max (dist,REFERENCE\_DISTANCE)\f$
 * - \f$dist=min (dist,MAX\_DISTANCE)\f$
 * - And then the calculations in InverseDistance... This is equivalent to the
 * <a href="http://www.iasig.org>IASIG</a> I3DL2 distance model.\n
 *
 * In the above calculations, the variables have the following meanings:\n
 * - dist is the distance from the listener to the source.\n
 * - REFERENCE_DISTANCE are the distance at which the listener will experience
 * GAIN. Both are set per source.\n
 * - ROLLOFF is a source specific factor of attenuation. If it's set to one,
 *   the InverseDistance model will describe a "physically correct" inverse
 *   square behaviour.\n
 * - MIN_GAIN, MAX_GAIN and MAX_DISTANCE are values used for clamping gain
 *   and distance, respectively.
 */

#include "openalpp/export.h"

#ifdef None
#undef None // Defined in X-headers
#endif
typedef enum DistanceModel {None,InverseDistance,InverseDistanceClamped};

/**
 * Class for setting global parameters.
 * This doesn't have to be instantiated if one does not need to set global
 * parameters.
 */
class OPENALPP_API AudioEnvironment : public AudioBase {
public:
  /**
   * Constructor.
   */
  AudioEnvironment() throw (InitError);
    
  /**
   * Constructor.
   * The parameters are ignored if this isn't the first object to be
   * instatiated of the AudioBase descendants.
   * @param frequency is the playing frequency of the enviroment (in Hz)
   * @param refresh is the refresh rate of the enviroment (in Hz)
   * @param synchronous is true if the enviroment is synchronous
   */
  AudioEnvironment(int frequency,int refresh,bool synchronous)
    throw (InitError);


  /**
   * Constructor.
   * The parameters are ignored if this isn't the first object to be
   * instatiated of the AudioBase descendants.
   * @param frequency is the playing frequency of the enviroment (in Hz)
   * @param refresh is the refresh rate of the enviroment (in Hz)
   */
  AudioEnvironment(int frequency,int refresh=-1)
    throw (InitError);

  /**
   * Sets the speed of sound in the enviroment.
   * This is used in doppler calculations.
   * @param speed is the speed of sound in length units per second.
   */
  void setSoundVelocity(float speed) throw (ValueError,FatalError);

  /**
   * Get the speed of sound in the enviroment.
   * @return speed of sound in length units per second.
   */
  float getSoundVelocity() throw (FatalError);

  /**
   * Sets the doppler factor.
   * This can be used to exaggerate, deemphasize or completely turn off the
   * doppler effect.
   * @param factor has a default value of one.
   */
  void setDopplerFactor(float factor) throw (ValueError,FatalError);

  /**
   * Gets the doppler factor.
   * @return doppler factor.
   */
  float getDopplerFactor() throw (FatalError);

  /**
   * Sets global gain (volume).
   * The volume a source will be played at will be multiplied by this _after_
   * the attenuation calculations.
   * Note: In todays's implementation on Linux, gain is clamped to [0.0,1.0].
   * This will be changed in future releases of OpenAL.
   * @param gain is the gain [0.0,...
   */
  void setGain(float gain);
    
  /**
   * Gets the global gain
   * @return global gain
   */
  float getGain() throw (FatalError);

  /**
   * Sets the distance model used in attenuation calculations.
   * @param model is one of: None, InverseDistance, InverseDistanceClamped.
   */
  void setDistanceModel(DistanceModel model) throw (FatalError);

  /**
   * Gets the distance model used in attenuation calculations.
   * @return the model.
   */
  DistanceModel getDistanceModel() throw (FatalError);
    
  /**
   * Initiates Loki's reverb implementation.
   */
  void initiateReverb() throw (InitError);
};

}

#endif /* AUDIOENVIROMENT_H_INCLUDED_C4211030 */
