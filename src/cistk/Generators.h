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

protected:
	void performTick( stk::StkFrames *frames ) override	{ tick( *frames ); }
};

// -------------------
// shared_ptr typedefs

typedef std::shared_ptr<BlitNode>			BlitNodeRef;
typedef std::shared_ptr<GranulateNode>		GranulateNodeRef;

} // namespace cistk
