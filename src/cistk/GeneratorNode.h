// Copyright (c) 2018, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#pragma once

#include "cinder/Cinder.h"
#include "cinder/audio/InputNode.h"

#include "stk/Stk.h"
#include "stk/Generator.h"

namespace cistk {

typedef std::shared_ptr<class GeneratorNode>			GeneratorNodeRef;

//! Base class for InputNode that wrap an stk::GeneratorNode. By defautl InstrumentNodes are auto-enabled so you
//! don't need to call enable(), instead you trigger them with stk::Instrmnt's noteOn() and noteOff methods.
class GeneratorNode : public ci::audio::InputNode {
  public:

  protected:
	GeneratorNode( stk::Generator *generator, const ci::audio::Node::Format &format = Format() );

	void initialize() override;
	void process( ci::audio::Buffer *buffer ) override;

	//! Called by subclasses to copy frames to buffer
	virtual void performTick( stk::StkFrames *frames ) = 0;

  private:
	stk::Generator*	mGenerator;
	stk::StkFrames	mStkFrames;
};

} // namespace cistk
