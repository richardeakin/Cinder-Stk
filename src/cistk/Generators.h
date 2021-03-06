// Copyright (c) 2018, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#pragma once

#include "cistk/GeneratorNode.h"

#include "stk/Blit.h"
#include "stk/Granulate.h"

namespace cistk {

class BlitNode : public GeneratorNode, public stk::Blit {
public:
	BlitNode( const ci::audio::Node::Format &format = Format() )
		: GeneratorNode( this, format )
	{}

protected:
	void performTick( stk::StkFrames *frames ) override	{ tick( *frames ); }
};

class GranulateNode : public GeneratorNode, public stk::Granulate {
public:
	GranulateNode( const ci::audio::Node::Format &format = Format() )
		: GeneratorNode( this, format )
	{}

	GranulateNode( unsigned int nVoices, std::string fileName, bool typeRaw = false, const ci::audio::Node::Format &format = Format() )
		: GeneratorNode( this, format ), Granulate( nVoices, fileName, typeRaw )
	{}

	// TODO: move these to stk::Granulate
	unsigned int voices() const				{ return grains_.size(); }
	unsigned int grainDuration() const		{ return gDuration_; }
	unsigned int grainRampPercent() const	{ return gRampPercent_; }
	unsigned int grainDelay() const			{ return gDelay_; }
	int grainOffset() const					{ return gOffset_; }
	float randomFactor() const				{ return gRandomFactor_; }

	void loadBuffer( const ci::audio::Buffer &buffer )
	{
		data_.resize( buffer.getNumFrames(), buffer.getNumChannels() );
		for( size_t ch = 0; ch < buffer.getNumChannels(); ch++ ) {
			const float *channel = buffer.getChannel( ch );
			for( size_t i = 0; i < buffer.getNumFrames(); i++ ) {
				data_( i, ch ) = channel[i];
			}
		}

		lastFrame_.resize( 1, buffer.getNumChannels(), 0.0 );

		Granulate::reset();
	}

	void loadBuffer( const ci::audio::BufferRef &buffer )
	{
		loadBuffer( *buffer );
	}

protected:
	void performTick( stk::StkFrames *frames ) override	{ tick( *frames ); }
};

// -------------------
// shared_ptr typedefs

typedef std::shared_ptr<BlitNode>			BlitNodeRef;
typedef std::shared_ptr<GranulateNode>		GranulateNodeRef;

} // namespace cistk
