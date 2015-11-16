#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "cinder/audio/audio.h"
#include "stk/include/Stk.h"
#include "stk/include/BeeThree.h"

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::shared_ptr<class StkTestNode> StkTestNodeRef;

class StkTestNode : public ci::audio::Node {
public:
	StkTestNode()
		: Node( audio::Node::Format() )
	{}

protected:
	void initialize() override
	{
		stk::Stk::setSampleRate( getSampleRate() );

		CI_LOG_I( "num channels: " << getNumChannels() );
	}

	void process( ci::audio::Buffer *buffer ) override
	{
		stk::StkFrames synthFrames( buffer->getNumFrames(), buffer->getNumChannels() );
		mSynth.tick( synthFrames );

		float *data = buffer->getData();
		for( size_t i = 0; i < buffer->getNumFrames(); i++ ) {
			data[i] = synthFrames( i, 0 );
		}
	}

public:
	stk::BeeThree mSynth;
};

class StkTestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseUp( MouseEvent event ) override;
	void update() override;
	void draw() override;

	ci::audio::GainNodeRef	mGain;
	StkTestNodeRef			mStkNode;
};

void StkTestApp::setup()
{
	stk::Stk::setRawwavePath( "/Users/r/code/cinder/rte/Cinder-Stk/lib/stk/rawwaves" );

	auto ctx = audio::master();
	mStkNode = ctx->makeNode<StkTestNode>();
	mGain = ctx->makeNode<audio::GainNode>( 0.5f );

	mStkNode >> mGain >> ctx->getOutput();

	ctx->enable();
}

void StkTestApp::mouseDown( MouseEvent event )
{
	mStkNode->mSynth.noteOn( event.getY(), 0.5f );
}

void StkTestApp::mouseUp( MouseEvent event )
{
	mStkNode->mSynth.noteOff( 0.5 );
}

void StkTestApp::update()
{
}

void StkTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( StkTestApp, RendererGl )
