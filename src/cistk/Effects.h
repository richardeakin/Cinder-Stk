// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#pragma once

#include "cinder/Cinder.h"
#include "cinder/audio/Node.h"

#include "stk/Stk.h"
#include "stk/Effect.h"
#include "stk/Echo.h"
#include "stk/Chorus.h"
#include "stk/PitShift.h"
#include "stk/LentPitShift.h"
#include "stk/PRCRev.h"
#include "stk/JCRev.h"
#include "stk/NRev.h"
#include "stk/FreeVerb.h"

namespace cistk {

//! Base class for GenNodes that wrap an stk::Instrmnt. By defautl InstrumentNodes are auto-enabled so you
//! don't need to call enable(), instead you trigger them with stk::Instrmnt's noteOn() and noteOff methods.
class EffectNode : public ci::audio::Node {
  public:

  protected:
	EffectNode( stk::Effect *instrmnt, const ci::audio::Node::Format &format = Format() );

	void initialize() override;
	void process( ci::audio::Buffer *buffer ) override;

	//! Called by subclasses to copy frames to buffer
	virtual void performTick( stk::StkFrames *frames ) = 0;

  private:
	stk::Effect*	mEffect;
	stk::StkFrames	mStkFrames;
};

// ----------------
// Effect sub-types

class ChorusNode : public EffectNode, public stk::Chorus {
  public:
	ChorusNode( float baseDelay = 6000, const ci::audio::Node::Format &format = Format() )
		: Chorus( baseDelay ), EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

#if 0

class EchoNode : public EffectNode, public stk::Echo {
  public:
	EchoNode( unsigned long maximumDelay = stk::Stk::sampleRate(), const ci::audio::Node::Format &format = Format() )
		: Echo( maximumDelay ), EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

#endif

class FreeVerbNode : public EffectNode, public stk::FreeVerb {
  public:
	FreeVerbNode( const ci::audio::Node::Format &format = Format() )
		: EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

#if 0
class JCRevNode : public EffectNode, public stk::JCRev {
  public:
	JCRevNode( float T60 = 1, const ci::audio::Node::Format &format = Format() )
		: JCRev( T60 ), EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

class NRevNode : public EffectNode, public stk::NRev {
  public:
	NRevNode( float T60 = 1, const ci::audio::Node::Format &format = Format() )
		: NRev( T60 ), EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

class PRCRevNode : public EffectNode, public stk::PRCRev {
  public:
	PRCRevNode( float T60 = 1, const ci::audio::Node::Format &format = Format() )
		: PRCRev( T60 ), EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

class PitShiftNode : public EffectNode, public stk::PitShift {
  public:
	PitShiftNode( const ci::audio::Node::Format &format = Format() )
		: EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

class LentPitShiftNode : public EffectNode, public stk::LentPitShift {
  public:
	LentPitShiftNode( float periodRatio = 1, int tMax = stk::RT_BUFFER_SIZE, const ci::audio::Node::Format &format = Format() )
		: LentPitShift( periodRatio, tMax ), EffectNode( this, format )
	{}

  protected:
	void performTick( stk::StkFrames *frames ) override;
};

#endif
// -------------------
// shared_ptr typedefs

typedef std::shared_ptr<EffectNode>			EffectNodeRef;
typedef std::shared_ptr<ChorusNode>			ChorusNodeRef;
typedef std::shared_ptr<FreeVerbNode>		FreeVerbNodeRef;
//typedef std::shared_ptr<JCRevNode>			JCRevNodeRef;
//typedef std::shared_ptr<NRevNode>			NRevNodeRef;
//typedef std::shared_ptr<PRCRevNode>			PRCRevNodeRef;
//typedef std::shared_ptr<PitShiftNode>		PitShiftNodeRef;
//typedef std::shared_ptr<LentPitShiftNode>	LentPitShiftNodeRef;

} // namespace cistk