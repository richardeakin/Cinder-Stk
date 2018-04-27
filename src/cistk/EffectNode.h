// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#pragma once

#include "cinder/Cinder.h"
#include "cinder/audio/Node.h"

#include "stk/Stk.h"
#include "stk/Effect.h"

namespace cistk {

typedef std::shared_ptr<class EffectNode>			EffectNodeRef;

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

} // namespace cistk