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
	void handleInstrumentSelected();
	void handleEffectSelected();
	bool handleInstrumentSpecificNote( const vec2 &pos );


	ci::audio::GainNodeRef	mGain;
	StkTestNodeRef			mStkNode;

	cistk::InstrumentNodeRef	mInstrument;

	params::InterfaceGlRef	mParams;
	vector<string>			mInstrumentEnumNames;
	int						mInstrumentEnumSelection = 0;

	float mLastFreq = 0;
};

void StkTestApp::setup()
{
	auto ctx = audio::master();

	stk::Stk::setSampleRate( ctx->getSampleRate() );
	cistk::initRawwavePath();

	mStkNode = ctx->makeNode<StkTestNode>();
	mGain = ctx->makeNode<audio::GainNode>( 0.85f );
	mGain >> ctx->getOutput();

	setupParams();

	handleInstrumentSelected();
	ctx->enable();
}

void StkTestApp::setupParams()
{
	mParams = params::InterfaceGl::create( getWindow(), "Stk params", toPixels( ivec2( 200, 300 ) ) );

	mParams->addParam<float>( "master gain",
					  [this]( float value ) { mGain->setValue( value ); },
					  [this] { return mGain->getValue(); }
	).min( 0.0f ).max( 1.0f ).step( 0.05f );

	mInstrumentEnumNames = {
		"BandedWG", "BlowBotl", "BlowHole", "Bowed", "Brass", "Clarinet",
		"Drummer", "Flute", "Mandolin", "Mesh2D", "ModalBar", "Moog",
		"Plucked", "Resonate", "Saxofony", "Shakers", "Simple",	"Sitar",
		"StifKarp", "VoicForm", "Whistle"
	};
	mParams->addParam( "instrument", mInstrumentEnumNames, &mInstrumentEnumSelection )
		.keyDecr( "[" ).keyIncr( "]" )
		.updateFn( [this] { handleInstrumentSelected(); } );
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

void StkTestApp::handleInstrumentSelected()
{
	if( mInstrument )
		mInstrument->disconnectAll();

	string name = mInstrumentEnumNames.at( mInstrumentEnumSelection );
	CI_LOG_I( "selecting instrument '" << name << "'" );

	auto ctx = audio::master();
	if( name == "BandedWG" ) {
		auto instr = ctx->makeNode<cistk::BandedWGNode>();
		instr->setPreset( 3 ); // preset: 'Tibetan Bowl'
		mInstrument = instr;
	}
	else if( name == "BlowBotl" ) {
		mInstrument = ctx->makeNode<cistk::BlowBotlNode>();
	}
	else if( name == "BlowHole" ) {
		mInstrument = ctx->makeNode<cistk::BlowHoleNode>();
	}
	else if( name == "Bowed" ) {
		mInstrument = ctx->makeNode<cistk::BowedNode>();
	}
	else if( name == "Brass" ) {
		mInstrument = ctx->makeNode<cistk::BrassNode>();
	}
	else if( name == "Clarinet" ) {
		mInstrument = ctx->makeNode<cistk::ClarinetNode>();
	}
	else if( name == "Drummer" ) {
		mInstrument = ctx->makeNode<cistk::DrummerNode>();
	}
	else if( name == "Flute" ) {
		mInstrument = ctx->makeNode<cistk::FluteNode>();
	}
	else if( name == "Mandolin" ) {
		mInstrument = ctx->makeNode<cistk::MandolinNode>();
	}
	else if( name == "Mesh2D" ) {
		auto instr = ctx->makeNode<cistk::Mesh2DNode>();
		instr->setDecay( 0.9f );
		mInstrument = instr;
	}
	else if( name == "ModalBar" ) {
		auto instr = ctx->makeNode<cistk::ModalBarNode>();
		instr->setPreset( 1 ); // preset: 'Vibraphone'
		mInstrument = instr;
	}
	else if( name == "Moog" ) {
		mInstrument = ctx->makeNode<cistk::MoogNode>();
	}
	else if( name == "Plucked" ) {
		mInstrument = ctx->makeNode<cistk::PluckedNode>();
	}
	else if( name == "Resonate" ) {
		mInstrument = ctx->makeNode<cistk::ResonateNode>();
	}
	else if( name == "Saxofony" ) {
		mInstrument = ctx->makeNode<cistk::SaxofonyNode>();
	}
	else if( name == "Shakers" ) {
		mInstrument = ctx->makeNode<cistk::ShakersNode>();
	}
	else if( name == "Simple" ) {
		mInstrument = ctx->makeNode<cistk::SimpleNode>();
	}
	else if( name == "Sitar" ) {
		mInstrument = ctx->makeNode<cistk::SitarNode>();
	}
	else if( name == "StifKarp" ) {
		mInstrument = ctx->makeNode<cistk::StifKarpNode>();
	}
	else if( name == "VoicForm" ) {
		auto instr = ctx->makeNode<cistk::VoicFormNode>();
		if( ! instr->setPhoneme( "ooo" ) ) {
			CI_LOG_W( "phoneme not found" );
		}
		instr->quiet();
		mInstrument = instr;
	}
	else if( name == "Whistle" ) {
		mInstrument = ctx->makeNode<cistk::WhistleNode>();
	}
	else {
		CI_LOG_E( "unknowned instrument name" );
		CI_ASSERT_NOT_REACHABLE();
	}

	mInstrument >> mGain >> ctx->getOutput();

	CI_LOG_I( "graph:\n" << ctx->printGraphToString() );
}

void StkTestApp::handleEffectSelected()
{
}

void StkTestApp::makeNote( const vec2 &pos )
{
	if( handleInstrumentSpecificNote( pos ) )
		return;

	float freq = quantizePitch( pos );
	if( fabs( mLastFreq - freq ) < 0.01f ) {
		return;
	}

	mLastFreq = freq;

	float gain = 1.0f - pos.y / (float)getWindowHeight();
	mInstrument->noteOn( freq, gain );
}

// Returns true if the note was handled completely and makeNote() shouldn't do anything else
bool StkTestApp::handleInstrumentSpecificNote( const vec2 &pos )
{
	vec2 posNormalized = glm::clamp( pos / vec2( getWindowSize() ), vec2( 0 ), vec2( 1 ) );
	posNormalized.y = 1.0f - posNormalized.y;

	auto mesh2D = dynamic_pointer_cast<cistk::Mesh2DNode>( mInstrument );
	if( mesh2D ) {
		mesh2D->setInputPosition( posNormalized.x, posNormalized.y );
		mInstrument->noteOn( 0, 1.0f );
		return true;
	}

	auto modalBar = dynamic_pointer_cast<cistk::ModalBarNode>( mInstrument );
	if( modalBar ) {
		modalBar->setStickHardness( posNormalized.y );
		return false;
	}

	return false;
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
