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

// TODO: Need to update this so that frequency and format can be changed after
// the class has been instantiated (the ogg vorbis format allows this)

#include "openalpp/streamupdater.h"

#define ENTER_CRITICAL lock()
#define LEAVE_CRITICAL unlock()

#ifdef _DEBUG
// for memory leak reporting
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK ,__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif



using namespace openalpp;

StreamUpdater::StreamUpdater(ALuint buffer1,ALuint buffer2,
                             ALenum format,unsigned int frequency) 
                             : format_(format), frequency_(frequency), stoprunning_(false) 
{
    buffers_[0]=buffer1;
    buffers_[1]=buffer2;
}

StreamUpdater::~StreamUpdater() {
    runmutex_.lock();
    stoprunning_=true;
    runmutex_.unlock();

    /* remove all sources, there may be a better way
    ** but otherwise sources are not removed for streams that reach EOF
    */
    for (unsigned int i=0;i<sources_.size();i++)
    {
        alSourceStop(sources_[i]);
        ALuint temp[2];
        ALint nqueued;
        alGetSourceiv(sources_[i], AL_BUFFERS_QUEUED, &nqueued);
        if (nqueued)
        {
            alSourceUnqueueBuffers(sources_[i], nqueued, temp);
        }
    }
    sources_.clear();


    cancel();
}

void StreamUpdater::addSource(ALuint sourcename) {
    alSourceStop(sourcename);
    ENTER_CRITICAL;
    newsources_.push_back(sourcename);
    if(!sources_.size())
        start();
    LEAVE_CRITICAL;
}

void StreamUpdater::removeSource(ALuint sourcename) {
    ENTER_CRITICAL;
    removesources_.push_back(sourcename);
    LEAVE_CRITICAL;
}

/**
* If playing, wait for a buffer to be processed,
* unqueue, load it with the new data, and re-queue.
* If stopped, queue two new buffers and play.
* Needs refactoring, this is difficult to work with.
*/
bool StreamUpdater::update(void *buffer, unsigned int length) 
{
    if(!(length && buffer))     // Zero length or NULL pointer => return
    {
        return false;
    }

    ALint processed,state;
    ALuint albuffer;

    ENTER_CRITICAL;

    // remove sources that are queued for removal
    while (sources_.size() && removesources_.size()) 
    {
        for (unsigned int i=0;i<sources_.size();i++)
            if (removesources_.back()==sources_[i]) 
            {
                alSourceStop(removesources_.back());
                ALuint dump[2];
                ALint nqueued;
                alGetSourceiv(removesources_.back(),AL_BUFFERS_QUEUED,&nqueued);
                if(nqueued)
                    alSourceUnqueueBuffers(removesources_.back(),nqueued,dump);
                alGetError();
                while((i+1)<sources_.size())
                    sources_[i]=sources_[i+1];
                break;
            }
            sources_.pop_back();
            removesources_.pop_back();
    }


    /* Add sources queued to be added, waiting until at least
    ** one source is added.
    ** Do not add duplicates */
    while (newsources_.size() || !sources_.size()) 
    {
        if (newsources_.size()) 
        {
            while(newsources_.size())
            {
                bool duplicateSource = false;
                for (size_t n=0; n<sources_.size(); ++n)
                {
                    if (sources_[n] == newsources_.back())
                    {
                        duplicateSource = true;
                    }
                }
                if (!duplicateSource)
                {
                    sources_.push_back(newsources_.back());
                    alSourceStop(newsources_.back());
                    newsources_.pop_back();
                }
            }
        } 
        else 
        {
            LEAVE_CRITICAL;
            usleep(50*1000);
            ENTER_CRITICAL;
        }
    }


    processed = 0;
    while (!processed) 
    {
        state=AL_PLAYING;
        for(unsigned int i=0;i<sources_.size();i++) 
        {
            alGetSourceiv(sources_[i],AL_SOURCE_STATE,&state);
            if(state!=AL_PLAYING)
                break;
        }

        /* a hack to get the paused mode to work, wait for non paused state,
        ** using only the first source */
        if (state == AL_PAUSED)
        {
            while (state == AL_PAUSED)
            {
                LEAVE_CRITICAL;
                YieldCurrentThread();
				usleep(50*1000);
                ENTER_CRITICAL;

                alGetSourceiv(sources_[0], AL_SOURCE_STATE, &state);
            }
        }

        /* if there are buffers queued, play the source (i.e. restart it), 
        ** otherwise set state to AL_INITIAL to start up two new buffers
        */
        if (state == AL_STOPPED)
        {
            for(unsigned int i=0;i<sources_.size();i++) 
            {
                alSourceStop(sources_[i]);
                ALint nqueued;
                alGetSourceiv(sources_[i], AL_BUFFERS_QUEUED, &nqueued);

                if (nqueued)
                {
                    alSourcePlay(sources_[i]);
                }
                else
                {
                    state = AL_INITIAL;
                }
            }
        }

        // start up two new buffers
        if (state == AL_INITIAL)
        {
            ALuint dump[2];
            for(unsigned int i=0;i<sources_.size();i++) {
                // Yeah, this sucks, but how else could the sources be kept
                // synchronized (they have to be unless we start using different
                // buffers for each source)...
                alSourceStop(sources_[i]);
                ALint nqueued;
                alGetSourceiv(sources_[i],AL_BUFFERS_QUEUED,&nqueued);
                if(nqueued)
                    alSourceUnqueueBuffers(sources_[i],nqueued,dump);
            }
            alBufferData(buffers_[0],format_,buffer,length/2,frequency_);
            alBufferData(buffers_[1],format_,
                (char *)buffer+length/2,length/2,frequency_);
            for(unsigned int i=0;i<sources_.size();i++) 
            {
                alSourceQueueBuffers(sources_[i],2,buffers_);
                alSourcePlay(sources_[i]);  // TODO: This would be better handled by
                // alSourcePlayv((ALuint *)&sources_[0]..)...;
            }
            processed=1;

        } 
        else // AL_PLAYING
        {
            processed=1;
            for(unsigned int i=0;i<sources_.size();i++) {
                alGetSourceiv(sources_[i],AL_BUFFERS_PROCESSED,&processed);
                if(!processed)
                    break;
            }


            if(processed) {
                for(unsigned int i=0;i<sources_.size();i++)
                    alSourceUnqueueBuffers(sources_[i],1,&albuffer);
                alBufferData(albuffer,format_,buffer,length,frequency_);
                for(unsigned int i=0;i<sources_.size();i++)
                    alSourceQueueBuffers(sources_[i],1,&albuffer);
            } else {
                LEAVE_CRITICAL;
                YieldCurrentThread();
				usleep(40*1000);
                ENTER_CRITICAL;
                // Not sure if this is necessary, but just in case...
                if(removesources_.size()) {
                    LEAVE_CRITICAL;
                    return update(buffer,length);
                }
            } // if(processed) else
        } // if(state!=AL_PLAYING) else
    } // while(!processed)

    LEAVE_CRITICAL;
    bool ret;
    runmutex_.lock();
    ret=stoprunning_;
    runmutex_.unlock();
    return ret;
}

void StreamUpdater::cancelCleanup() {
    std::cerr << "StreamUpdater::cancelCleanup: Should probably not delete this" << std::endl;
    delete this;
} 
