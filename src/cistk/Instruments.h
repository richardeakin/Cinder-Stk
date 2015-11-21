#pragma once

#include "cinder/audio/GenNode.h"

#include "stk/include/Stk.h"
#include "stk/include/Instrmnt.h"
#include "stk/include/BandedWG.h"
#include "stk/include/BlowBotl.h"
#include "stk/include/BlowHole.h"
#include "stk/include/Bowed.h"
#include "stk/include/Brass.h"
#include "stk/include/Clarinet.h"
#include "stk/include/Drummer.h"
#include "stk/include/Flute.h"
#include "stk/include/Mandolin.h"
#include "stk/include/Mesh2D.h"
#include "stk/include/ModalBar.h"
#include "stk/include/Moog.h"
#include "stk/include/Plucked.h"
#include "stk/include/Resonate.h"
#include "stk/include/Saxofony.h"
#include "stk/include/Shakers.h"
#include "stk/include/Simple.h"
#include "stk/include/Sitar.h"
#include "stk/include/StifKarp.h"
#include "stk/include/VoicForm.h"
#include "stk/include/Whistle.h"

#include "stk/include/BeeThree.h"
#include "stk/include/FMVoices.h"
#include "stk/include/HevyMetl.h"
#include "stk/include/PercFlut.h"
#include "stk/include/Rhodey.h"
#include "stk/include/TubeBell.h"
#include "stk/include/Wurley.h"

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
	BlowHoleNode( float lowestFrequency = 10, const ci::audio::Node::Format &format = Format() )
		: BlowHole( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class BowedNode : public InstrumentNode, public stk::Bowed {
  public:
	BowedNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class BrassNode : public InstrumentNode, public stk::Brass {
  public:
	BrassNode( float lowestFrequency = 8, const ci::audio::Node::Format &format = Format() )
		: Brass( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class ClarinetNode : public InstrumentNode, public stk::Clarinet {
  public:
	ClarinetNode( float lowestFrequency = 8, const ci::audio::Node::Format &format = Format() )
		: Clarinet( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class DrummerNode : public InstrumentNode, public stk::Drummer {
  public:
	DrummerNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class FluteNode : public InstrumentNode, public stk::Flute {
  public:
	FluteNode( float lowestFrequency = 10, const ci::audio::Node::Format &format = Format() )
		: Flute( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class MandolinNode : public InstrumentNode, public stk::Mandolin {
  public:
	MandolinNode( float lowestFrequency = 5, const ci::audio::Node::Format &format = Format() )
		: Mandolin( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class Mesh2DNode : public InstrumentNode, public stk::Mesh2D {
  public:
	Mesh2DNode( unsigned short nX = 10, unsigned short nY = 10, const ci::audio::Node::Format &format = Format() )
		: Mesh2D( nX, nY ), InstrumentNode( this, format )
	{}
};

class ModalBarNode : public InstrumentNode, public stk::ModalBar {
  public:
	ModalBarNode(const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class MoogNode : public InstrumentNode, public stk::Moog {
  public:
	MoogNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class PluckedNode : public InstrumentNode, public stk::Plucked {
  public:
	PluckedNode( float lowestFrequency = 10, const ci::audio::Node::Format &format = Format() )
		: Plucked( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class ResonateNode : public InstrumentNode, public stk::Resonate {
  public:
	ResonateNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class SaxofonyNode : public InstrumentNode, public stk::Saxofony {
  public:
	SaxofonyNode( float lowestFrequency = 10, const ci::audio::Node::Format &format = Format() )
		: Saxofony( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class ShakersNode : public InstrumentNode, public stk::Shakers {
  public:
	ShakersNode( int type = 0, const ci::audio::Node::Format &format = Format() )
		: Shakers( type ), InstrumentNode( this, format )
	{}
};

class SimpleNode : public InstrumentNode, public stk::Simple {
  public:
	SimpleNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class SitarNode : public InstrumentNode, public stk::Sitar {
  public:
	SitarNode( float lowestFrequency = 8, const ci::audio::Node::Format &format = Format() )
		: Sitar( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class StifKarpNode : public InstrumentNode, public stk::StifKarp {
  public:
	StifKarpNode( float lowestFrequency = 8, const ci::audio::Node::Format &format = Format() )
		: StifKarp( lowestFrequency ), InstrumentNode( this, format )
	{}
};

class VoicFormNode : public InstrumentNode, public stk::VoicForm {
  public:
	VoicFormNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class WhistleNode : public InstrumentNode, public stk::Whistle {
  public:
	WhistleNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

// ------------
// FM sub-types

class BeeThreeNode : public InstrumentNode, public stk::BeeThree {
  public:
	BeeThreeNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class FMVoicesNode : public InstrumentNode, public stk::FMVoices {
  public:
	FMVoicesNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class HevyMetlNode : public InstrumentNode, public stk::HevyMetl {
  public:
	HevyMetlNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class PercFlutNode : public InstrumentNode, public stk::PercFlut {
  public:
	PercFlutNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class RhodeyNode : public InstrumentNode, public stk::Rhodey {
  public:
	RhodeyNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class TubeBellNode : public InstrumentNode, public stk::TubeBell {
  public:
	TubeBellNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

class WurleyNode : public InstrumentNode, public stk::Wurley {
  public:
	WurleyNode( const ci::audio::Node::Format &format = Format() )
		: InstrumentNode( this, format )
	{}
};

} // namespace cistk