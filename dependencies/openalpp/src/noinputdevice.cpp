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

#include "openalpp/inputdevice.h"

namespace openalpp {

// Static
int InputDevice::nobjects_=0;

void InputDevice::init() {
  throw InitError("No input device support compiled in.");
}

InputDevice::InputDevice() {
  init();
}

InputDevice::InputDevice(int /*device*/, unsigned int /*samplerate*/, unsigned int /*buffersize*/,
			 SampleFormat /*format*/) {
  init();
}

InputDevice::InputDevice(const InputDevice &input)
  : Stream((const Stream &)input) {
}

InputDevice &InputDevice::operator=(const InputDevice &/*input*/) {
  return *this;
}

InputDevice::~InputDevice() {
}

}
