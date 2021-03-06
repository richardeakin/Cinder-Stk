// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#pragma once

#include "cinder/Cinder.h"
#include "cinder/audio/InputNode.h"

#include "stk/Stk.h"
#include "stk/Instrmnt.h"

namespace cistk {

typedef std::shared_ptr<class InstrumentNode>		InstrumentNodeRef;

//! Base class for GenNodes that wrap an stk::Instrmnt. By defautl InstrumentNodes are auto-enabled so you
//! don't need to call enable(), instead you trigger them with stk::Instrmnt's noteOn() and noteOff methods.
class InstrumentNode : public ci::audio::InputNode {
  public:
    //! Reset and clear all internal state (for subclasses).
    void clear()													{ mInstrument->clear(); }
	//! Start a note with the given frequency and amplitude.
	void noteOn( stk::StkFloat frequency, stk::StkFloat amplitude )	{ mInstrument->noteOn( frequency, amplitude ); }
	//! Stop a note with the given amplitude (speed of decay).
	void noteOff( stk::StkFloat amplitude )							{ mInstrument->noteOff( amplitude ); }
	//! Set instrument parameters for a particular frequency.
	void setFrequency( stk::StkFloat frequency )					{ mInstrument->setFrequency( frequency ); }
	//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
	void controlChange( int number, stk::StkFloat value )			{ mInstrument->controlChange( number, value ); }

  protected:
	InstrumentNode( stk::Instrmnt *instrmnt, const ci::audio::Node::Format &format = Format() );

	void initialize() override;
	void process( ci::audio::Buffer *buffer ) override;

  private:
	stk::Instrmnt*	mInstrument;
	stk::StkFrames	mStkFrames;
};

} // namespace cistk