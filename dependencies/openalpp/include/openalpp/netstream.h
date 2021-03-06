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

#ifndef NETSTREAM_H_INCLUDED_C419F72E
#define NETSTREAM_H_INCLUDED_C419F72E

#include "openalpp/stream.h"
#include "openalpp/export.h"

namespace ost {
class UDPSocket;
class TCPStream;
}

namespace openalpp {

/**
 * Class for handling streams through sockets.
 * Preliminary tests indicate that packets smaller than ca 1 kb should not
 * be used (tests were done with Mono8, 11025 Hz). 
 */
class OPENALPP_API NetStream : public Stream {
 public:
  /**
   * Constructor.
   * @param socket is the socket to stream data through.
   * @param controlsocket is an (optional) TCPStream that can be used to send
   * information about the stream. The constructor will begin with trying to
   * read SampleFormat, frequency, and buffer size. The sender can also use the
   * control socket to send "EXIT" when it's run out of data to send. If
   * this parameter is not given, defaults will be used (format=Mono8,
   * frequency=11025, buffersize=4096).
   */
  NetStream(ost::UDPSocket *socket,ost::TCPStream *controlsocket=NULL);

  /**
   * Constructor.
   * @param socket is the socket to stream data through.
   * @param format is the format the data will be in.
   * @param frequency is the frequency of the sound.
   * @param buffersize is the size of the sound buffer. Note that the actual
   * packets sent over the network can be smaller (or bigger!); this is just
   * the size of the buffer OpenAL++ will use to receive the data.
   */
  NetStream(ost::UDPSocket *socket,SampleFormat format,unsigned int frequency,
	    unsigned int buffersize);

  /**
   * Copy constructor.
   */
  NetStream(const NetStream &stream);

  /**
   * Destructor.
   */
  ~NetStream();

  /**
   * Assignment operator.
   */
  NetStream &operator=(const NetStream &stream);
};

}

#endif /* NETSTREAM_H_INCLUDED_C419F72E */

