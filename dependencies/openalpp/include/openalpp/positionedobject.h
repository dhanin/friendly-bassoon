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

#ifndef POSITIONEDOBJECT_H_INCLUDED_C4278579
#define POSITIONEDOBJECT_H_INCLUDED_C4278579

#include "openalpp/export.h"
#include "openalpp/audiobase.h"

namespace openalpp {

/**
 * Virtual base class for positioned objects.
 * (I.e. listeners and sources).
 */
class OPENALPP_API PositionedObject : public AudioBase {

 public:

   PositionedObject() {}
 
 /**
   * Set position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  virtual void setPosition(float x, float y, float z)=0;

  /**
   * Get position.
   * @param x x coordinate.
   * @param y y coordinate.
   * @param z z coordinate.
   */
  virtual void getPosition(float &x, float &y, float &z) const =0;

  /**
   * Set velocity.
   * @param vx x member of velocity vector.
   * @param vy y member of velocity vector.
   * @param vz z member of velocity vector.
   */
  virtual void setVelocity(float vx, float vy, float vz)=0;

  /**
   * Get velocity.
   * @param vx x member of velocity vector.
   * @param vy y member of velocity vector.
   * @param vz z member of velocity vector.
   */
  virtual void getVelocity(float &vx, float &vy, float &vz) const =0;

 protected:
    virtual ~PositionedObject() {}

};

}

#endif /* POSITIONEDOBJECT_H_INCLUDED_C4278579 */
