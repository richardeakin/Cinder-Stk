#pragma once

#include "cinder/audio/GenNode.h"

#include "stk/include/Stk.h"
#include "stk/include/Instrmnt.h"
#include "stk/include/BandedWG.h"
#include "stk/include/BlowBotl.h"
#include "stk/include/BlowHole.h"
#include "stk/include/Bowed.h"

namespace cistk {

typedef std::shared_ptr<class InstrumentNode>	InstrumentNodeRef;
typedef std::shared_ptr<class BlowBotlNode>		BlowBotlNodeRef;

//! Base class for GenNodes that wrap an stk::Instrmnt. By defautl InstrumentNodes are auto-enabled so you
//! don't need to call enable(), instead you trigger them with stk::Instrmnt's noteOn() and noteOff methods.
class InstrumentNode : public ci::audio::GenNode {
  public:
    virtual void clear( void ) {};

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


class BandedWGNode : public InstrumentNode, public stk::BandedWG {
  public:
	BandedWGNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class BlowBotlNode : public InstrumentNode, public stk::BlowBotl {
  public:
	BlowBotlNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class BlowHoleNode : public InstrumentNode, public stk::BlowHole {
  public:
	BlowHoleNode( float lowestFrequency = 10.0f, const ci::audio::Node::Format &format = Format() )
		: BlowHole( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class BowedNode : public InstrumentNode, public stk::Bowed {
  public:
	BowedNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

} // namespace cistk