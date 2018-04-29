// Copyright (c) 2018, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#include "cistk/GeneratorNode.h"

using namespace ci;

namespace cistk {

GeneratorNode::GeneratorNode( stk::Generator *generator, const ci::audio::Node::Format &format )
	: InputNode( format ), mGenerator( generator )
{
	if( ! format.isAutoEnableSet() )
		setAutoEnabled( true );
}

void GeneratorNode::initialize()
{
	mStkFrames.resize( getFramesPerBlock(), getNumChannels() );
}

void GeneratorNode::process( audio::Buffer *buffer )
{
	mGenerator->tick( mStkFrames );
	for( size_t ch = 0; ch < buffer->getNumChannels(); ch++ ) {
		float *channel = buffer->getChannel( ch );
		for( size_t i = 0; i < buffer->getNumFrames(); i++ ) {
			channel[i] = mStkFrames( i, ch );
		}
	}
}

} // namespace cistk