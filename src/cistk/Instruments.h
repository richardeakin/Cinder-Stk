// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

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

//! Base class for GenNodes that wrap an stk::Instrmnt. By defautl InstrumentNodes are auto-enabled so you
//! don't need to call enable(), instead you trigger them with stk::Instrmnt's noteOn() and noteOff methods.
class InstrumentNode : public ci::audio::GenNode {
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

// ------------------
// Instrmnt sub-types

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

// -------------------
// shared_ptr typedefs

typedef std::shared_ptr<InstrumentNode>		InstrumentNodeRef;
typedef std::shared_ptr<BandedWGNode>		BandedWGNodeRef;
typedef std::shared_ptr<BlowBotlNode>		BlowBotlNodeRef;
typedef std::shared_ptr<BlowHoleNode>		BlowHoleNodeRef;
typedef std::shared_ptr<BowedNode>			BowedNodeRef;
typedef std::shared_ptr<BrassNode>			BrassNodeRef;
typedef std::shared_ptr<ClarinetNode>		ClarinetNodeRef;
typedef std::shared_ptr<DrummerNode>		DrummerNodeRef;
typedef std::shared_ptr<FluteNode>			FluteNodeRef;
typedef std::shared_ptr<MandolinNode>		MandolinNodeRef;
typedef std::shared_ptr<Mesh2DNode>			Mesh2DNodeRef;
typedef std::shared_ptr<ModalBarNode>		ModalBarNodeRef;
typedef std::shared_ptr<MoogNode>			MoogNodeRef;
typedef std::shared_ptr<PluckedNode>		PluckedNodeRef;
typedef std::shared_ptr<ResonateNode>		ResonateNodeRef;
typedef std::shared_ptr<SaxofonyNode>		SaxofonyNodeRef;
typedef std::shared_ptr<ShakersNode>		ShakersNodeRef;
typedef std::shared_ptr<SimpleNode>			SimpleNodeRef;
typedef std::shared_ptr<SitarNode>			SitarNodeRef;
typedef std::shared_ptr<StifKarpNode>		StifKarpNodeRef;
typedef std::shared_ptr<VoicFormNode>		VoicFormNodeRef;
typedef std::shared_ptr<WhistleNode>		WhistleNodeRef;
typedef std::shared_ptr<BeeThreeNode>		BeeThreeNodeRef;
typedef std::shared_ptr<FMVoicesNode>		FMVoicesNodeRef;
typedef std::shared_ptr<HevyMetlNode>		HevyMetlNodeRef;
typedef std::shared_ptr<PercFlutNode>		PercFlutNodeRef;
typedef std::shared_ptr<RhodeyNode>			RhodeyNodeRef;
typedef std::shared_ptr<TubeBellNode>		TubeBellNodeRef;
typedef std::shared_ptr<WurleyNode>			WurleyNodeRef;

} // namespace cistk