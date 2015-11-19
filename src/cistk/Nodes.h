#pragma once

#include "cinder/audio/GenNode.h"

#include "stk/include/Stk.h"

namespace cistk {

template <typename InstrumentT>
class InstrumentNode : public ci::audio::GenNode {
public:
	InstrumentNode( ci::audio::Node::Format format = Format() )
		: GenNode( format )
	{}

	InstrumentT&	getInstrument()	{ return mInstrument; }

protected:
	void initialize() override
	{
		mStkFrames.resize( getFramesPerBlock(), getNumChannels() );
	}

	void process( ci::audio::Buffer *buffer ) override
	{
		mInstrument.tick( mStkFrames );
		for( size_t ch = 0; ch < buffer->getNumChannels(); ch++ ) {
			float *channel = buffer->getChannel( ch );
			for( size_t i = 0; i < buffer->getNumFrames(); i++ ) {
				channel[i] = mStkFrames( i, ch );
			}
		}
	}

private:
	InstrumentT		mInstrument;
	stk::StkFrames	mStkFrames;
};

} // namespace cistk