// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#include "cistk/EffectNode.h"

#include "cinder/Log.h"
#include "cinder/audio/Context.h"

using namespace ci;

namespace cistk {

EffectNode::EffectNode( stk::Effect *effect, const ci::audio::Node::Format &format )
	: Node( format ), mEffect( effect )
{
	setChannelMode( ChannelMode::SPECIFIED );
	setNumChannels( 2 );
}

void EffectNode::initialize()
{
	mStkFrames.resize( getFramesPerBlock(), getNumChannels() );
}

// TODO: instead of copying in and copying out, might as well just do the single float tick
// - should also try to optimize for those Effects that take a mono input and output stereo, no need to copy both channels for them
void EffectNode::process( audio::Buffer *buffer )
{
	const size_t numChannels = buffer->getNumChannels();
	const size_t numFrames = buffer->getNumFrames();

	for( size_t ch = 0; ch < numChannels; ch++ ) {
		float *channel = buffer->getChannel( ch );
		for( size_t i = 0; i < numFrames; i++ ) {
			mStkFrames( i, ch ) = channel[i];
		}
	}

	performTick( &mStkFrames );

	for( size_t ch = 0; ch < numChannels; ch++ ) {
		float *channel = buffer->getChannel( ch );
		for( size_t i = 0; i < numFrames; i++ ) {
			channel[i] = mStkFrames( i, ch );
		}
	}
}

} // namespace cistk