/**
@file	 AsyncAudioElasticFifo.h
@brief   A FIFO for handling audio samples
@author  Tobias Blomberg / SM0SVX
@date	 2007-10-06

Implements a jitter-tolerant FIFO for storing samples.

\verbatim
Async - A library for programming event driven applications
Copyright (C) 2003-2009  Tobias Blomberg / SM0SVX

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
\endverbatim
*/


#ifndef ASYNC_AUDIO_ELASTIC_FIFO_INCLUDED
#define ASYNC_AUDIO_ELASTIC_FIFO_INCLUDED


/****************************************************************************
 *
 * System Includes
 *
 ****************************************************************************/

#include <stdint.h>


/****************************************************************************
 *
 * Project Includes
 *
 ****************************************************************************/

#include <AsyncAudioSink.h>
#include <AsyncAudioSource.h>


/****************************************************************************
 *
 * Local Includes
 *
 ****************************************************************************/

#include <sys/time.h>


/****************************************************************************
 *
 * Forward declarations
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Namespace
 *
 ****************************************************************************/

namespace Async
{


/****************************************************************************
 *
 * Forward declarations of classes inside of the declared namespace
 *
 ****************************************************************************/

  

/****************************************************************************
 *
 * Defines & typedefs
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Exported Global Variables
 *
 ****************************************************************************/



/****************************************************************************
 *
 * Class definitions
 *
 ****************************************************************************/

/**
@brief	A FIFO class for handling audio samples
@author Tobias Blomberg / SM0SVX
@date   2007-10-06

This class implements a jitter-tolerant FIFO for handling audio samples.
The FIFO is intended to buffer samples that arrive with a certain amount
of sample rate or packet jitter. Under normal operation, the FIFO is kept
half full. Varying sample rates or packet rates slowly move the amount of
samples out of center. When the FIFO reaches a full or empty state, it is
automatically reset to the half-full state.
*/
class AudioElasticFifo : public AudioSink, public AudioSource
{
  public:
    /**
     * @brief 	Constuctor
     * @param   fifo_size This is the size of the fifo expressed in number
     *                    of samples.
     */
    explicit AudioElasticFifo(unsigned fifo_size);
  
    /**
     * @brief 	Destructor
     */
    virtual ~AudioElasticFifo(void);
  
    /**
     * @brief	Set the size of the FIFO
     * @param	new_size  This is the size of the fifo expressed in number
     *                    of samples.
     *
     * Use this function to set the size of the FIFO. In doing this, the
     * FIFO will also be cleared.
     */
    void setSize(unsigned new_size);
    
    /**
     * @brief 	Check if the FIFO is empty
     * @return	Returns \em true if the FIFO is empty or else \em false
     */
    bool empty(void) const { return (tail == head); }
    
    /**
     * @brief 	Find out how many samples there are in the FIFO
     * @return	Returns the number of samples in the FIFO
     */
    unsigned samplesInFifo(bool ignore_prebuf = false) const;
    
    /**
     * @brief 	Clear all samples from the FIFO
     *
     * This will immediately reset the FIFO and discard all samples.
     * The source will be told that all samples have been flushed.
     */
    void clear(void);

    /**
     * @brief 	Write samples into the FIFO
     * @param 	samples The buffer containing the samples
     * @param 	count The number of samples in the buffer
     * @return	Returns the number of samples that has been taken care of
     *
     * This function is used to write audio into the FIFO. If it
     * returns 0, no more samples could be written.
     * If the returned number of written samples is lower than the count
     * parameter value, the sink is not ready to accept more samples.
     * In this case, the audio source requires sample buffering to temporarily
     * store samples that are not immediately accepted by the sink.
     * The writeSamples function should be called on source buffer updates
     * and after a source output request has been received through the
     * requestSamples function.
     * This function is normally only called from a connected source object.
     */
    virtual int writeSamples(const float *samples, int count);

    /**
     * @brief 	Tell the sink that there are samples available on request
     *
     * This function is used to tell the sink that there are samples available
     * that can be requested by calling the sourceRequestSamples function.
     * This function is normally only called from a connected source object.
     */
    virtual void availSamples(void);
    
    /**
     * @brief 	Tell the FIFO to flush the previously written samples
     *
     * This function is used to tell the FIFO to flush previously written
     * samples.
     * This function is normally only called from a connected source object.
     */
    virtual void flushSamples(void);

    /**
     * @brief Request audio samples from this source
     * 
     * This function will be called when the registered audio sink is ready
     * to accept more samples.
     * This function is normally only called from a connected sink object.
     */
    virtual void requestSamples(int count);
    
    /**
     * @brief Discard all audio samples from this source
     *
     * Use this method to discard all available samples from the
     * remaining audio stream.
     */
    virtual void discardSamples(void);


  protected:
      
    /**
     * @brief The registered sink has flushed all samples
     *
     * This function will be called when all samples have been flushed in the
     * registered sink.
     * This function is normally only called from a connected sink object.
     */
    virtual void allSamplesFlushed(void);
    
    
  private:
    typedef enum
    {
      STREAM_IDLE, STREAM_ACTIVE, STREAM_FLUSHING
    } StreamState;

    unsigned       sample_rate;
    float          *fifo;
    unsigned       fifo_size;
    unsigned       head, tail;
    bool      	   prebuf;
    StreamState    stream_state;
    
    void writeSamplesFromFifo(int count);
    void writeZeroBlock(int count);
    void flushSamplesFromFifo(void);

};  /* class AudioElasticFifo */


} /* namespace */

#endif /* ASYNC_AUDIO_ELASTIC_FIFO_INCLUDED */


/*
 * This file has not been truncated
 */
