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

class StkTestNode : public ci::audio::Node {
public:
	StkTestNode()
		: Node( audio::Node::Format().channels( 2 ) )
	{}

protected:
	void initialize() override
	{
		CI_LOG_I( "num channels: " << getNumChannels() );

		mStkFrames = stk::StkFrames( getFramesPerBlock(), getNumChannels() );

		mReverb.setEffectMix( 0.7f );

		mReverb.setDamping( 0.6f );
		mReverb.setRoomSize( 0.5f );
	}


	void process( ci::audio::Buffer *buffer ) override
	{
		mSynth.tick( mStkFrames );

		// For Freeverb, copy channel 0 to channel 1
		for( size_t i = 0; i < buffer->getNumFrames(); i++ ) {
			mStkFrames( i, 1 ) = mStkFrames( i, 0 );
		}

		mReverb.tick( mStkFrames );

		for( size_t ch = 0; ch < buffer->getNumChannels(); ch++ ) {
			float *channel = buffer->getChannel( ch );
			for( size_t i = 0; i < buffer->getNumFrames(); i++ ) {
				channel[i] = mStkFrames( i, ch );
			}
		}
	}

public:
	stk::Rhodey mSynth;
//	stk::NRev	mReverb;
//	stk::JCRev	mReverb;
	stk::FreeVerb	mReverb;

	stk::StkFrames mStkFrames;
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

	cistk::InstrumentNodeRef	mInstrument;

	params::InterfaceGlRef	mParams;

	float mLastFreq = 0;
};

void StkTestApp::setup()
{
	auto ctx = audio::master();

	stk::Stk::setSampleRate( ctx->getSampleRate() );
	cistk::initRawwavePath();

	mStkNode = ctx->makeNode<StkTestNode>();
	mGain = ctx->makeNode<audio::GainNode>( 0.75f );

//	mBlowBottle = ctx->makeNode<cistk::InstrumentNode<stk::BlowBotl>>();
	mInstrument = ctx->makeNode<cistk::BlowBotlNode>();

//	mStkNode >> mGain >> ctx->getOutput();
	mInstrument >> mGain >> ctx->getOutput();
//	mBlowBottle->enable();

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
	mInstrument->noteOff( 0.5 );
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
	if( fabs( mLastFreq - freq ) < 0.01f ) {
		return;
	}

	mLastFreq = freq;

	float gain = 1.0f - pos.y / (float)getWindowHeight();
	mInstrument->noteOn( freq, gain );
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
