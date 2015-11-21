#include "cistk/Instruments.h"

using namespace ci;

namespace cistk {

InstrumentNode::InstrumentNode( stk::Instrmnt *instrument, const ci::audio::Node::Format &format )
	: GenNode( format ), mInstrument( instrument )
{
	if( boost::indeterminate( format.getAutoEnable() ) )
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