/**
 * OpenAL++ - an object oriented toolkit for spatial sound
 * Copyright (C) 2002 VRlab, Ume? University
 *
 * OpenAL++ was created using the libraries:
 *                 OpenAL (http://www.openal.org), 
 *              PortAudio (http://www.portaudio.com/), and
 *              CommonC++ (http://cplusplus.sourceforge.net/)
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

#ifndef NETUPDATER_H_INCLUDED_C419EF97
#define NETUPDATER_H_INCLUDED_C419EF97

#include <cc++/socket.h>
#include "openalpp/streamupdater.h"
#include "stdlib.h"
#include "openalpp/export.h"

namespace openalpp {

/**
 * Updater for streams through sockets.
 */
class OPENALPP_API NetUpdater : public StreamUpdater {
  /**
   * Socket to receive sound data from.
   */
  ost::UDPSocket *socket_;

  /**
   * Stream to receive control messages from.
   */
  ost::TCPStream *controlsocket_;

  /**
   * Size of sound buffer (in bytes).
   */
  unsigned int buffersize_;
 public:
  /**
   * Constructor.
   * @param socket is the socket to receive sound data from.
   * @param controlsocket is the stream to receive control messages from. Set
   * to NULL if control messages are not used.
   * @param buffer1 and...
   * @param buffer2 are the sound buffers to use.
   * @param format is the (OpenAL) format of the sound data.
   * @param frequency is the frequency of the sound data.
   * @param buffersize is how much data to buffer before adding to the stream.
   */
  NetUpdater(ost::UDPSocket *socket,ost::TCPStream *controlsocket,
	     const ALuint buffer1,ALuint buffer2,
	     ALenum format,unsigned int frequency,
	     unsigned int buffersize);

  /**
   * Inherited from Thread.
   * This will be called when the updater is Start():ed..
   */
  void run();
};

}

#endif /* NETUPDATER_H_INCLUDED_C419EF97 */
