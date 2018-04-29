// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#include "cistk/InstrumentNode.h"

using namespace ci;

namespace cistk {

InstrumentNode::InstrumentNode( stk::Instrmnt *instrument, const ci::audio::Node::Format &format )
	: InputNode( format ), mInstrument( instrument )
{
	if( ! format.isAutoEnableSet() )
		setAutoEnabled( true );
}

void InstrumentNode::initialize()
{
	mStkFrames.resize( getFramesPerBlock(), getNumChannels() );
}

void InstrumentNode::process( audio::Buffer *buffer )
{
	mInstrument->tick( mStkFrames );
	for( size_t ch = 0; ch < buffer->getNumChannels(); ch++ ) {
		float *channel = buffer->getChannel( ch );
		for( size_t i = 0; i < buffer->getNumFrames(); i++ ) {
			channel[i] = mStkFrames( i, ch );
		}
	}
}

} // namespace cistk