#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "cinder/audio/audio.h"
#include "CinderStk.h"

const float MAX_PITCH_MIDI	= 80;
const float	MIN_PITCH_MIDI	= 40;

using namespace ci;
using namespace ci::app;
using namespace std;

typedef std::shared_ptr<class StkTestNode> StkTestNodeRef;

// TODO: make this a templated type on different Stk types (Instrment, Filter, etc)
class StkTestNode : public ci::audio::Node {
public:
	StkTestNode()
		: Node( audio::Node::Format() )
	{}

protected:
	void initialize() override
	{

		CI_LOG_I( "num channels: " << getNumChannels() );
	}

	// TODO NEXT: try out NRev, FreeVerb, and FCRev
	
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
	stk::Rhodey mSynth;
};

class StkTestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void mouseDrag( MouseEvent event ) override;
	void mouseUp( MouseEvent event ) override;
	void update() override;
	void draw() override;

	void setupParams();
	void makeNote( const vec2 &pos );
	float quantizePitch( const vec2 &pos );


	ci::audio::GainNodeRef	mGain;
	StkTestNodeRef			mStkNode;

	params::InterfaceGlRef	mParams;
};

void StkTestApp::setup()
{
	auto ctx = audio::master();

	stk::Stk::setSampleRate( ctx->getSampleRate() );

	// TODO: make method that sets this using a variety of search locations
	// - relative to source code using __FILE__
	// - rawwaves file in assets dir
	stk::Stk::setRawwavePath( "/Users/r/code/cinder/rte/Cinder-Stk/lib/stk/rawwaves" );

	mStkNode = ctx->makeNode<StkTestNode>();
	mGain = ctx->makeNode<audio::GainNode>( 0.75f );

	mStkNode >> mGain >> ctx->getOutput();

	ctx->enable();

	setupParams();
}

void StkTestApp::setupParams()
{
	mParams = params::InterfaceGl::create( getWindow(), "Stk params", toPixels( ivec2( 200, 300 ) ) );

	mParams->addParam<float>( "master gain",
					  [this]( float value ) { mGain->setValue( value ); },
					  [this] { return mGain->getValue(); }
	).min( 0.0f ).max( 1.0f ).step( 0.05f );

}

void StkTestApp::mouseDown( MouseEvent event )
{
	makeNote( event.getPos() );
}

void StkTestApp::mouseDrag( MouseEvent event )
{
	makeNote( event.getPos() );
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
	gl::clear();

	mParams->draw();
}

void StkTestApp::makeNote( const vec2 &pos )
{
	float freq = quantizePitch( pos );
	float gain = 1.0f - pos.y / (float)getWindowHeight();

	mStkNode->mSynth.noteOn( freq, gain );
}

// returns a quantized pitch (in hertz) within the minor scale
float StkTestApp::quantizePitch( const vec2 &pos )
{
	const size_t scaleLength = 7;
	float scale[scaleLength] = { 0, 2, 3, 5, 7, 8, 10 };

	int pitchMidi = lroundf( lmap( pos.x, 0.0f, (float)getWindowWidth(), MIN_PITCH_MIDI, MAX_PITCH_MIDI ) );

	bool quantized = false;
	while( ! quantized ) {
		int note = pitchMidi % 12;
		for( size_t i = 0; i < scaleLength; i++ ) {
			if( note == scale[i] ) {
				quantized = true;
				break;
			}
		}
		if( ! quantized )
			pitchMidi--;
	}

	return audio::midiToFreq( pitchMidi );
}

CINDER_APP( StkTestApp, RendererGl )
