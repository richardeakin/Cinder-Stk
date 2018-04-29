#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"
#include "cinder/params/Params.h"

#include "cinder/audio/audio.h"
#include "cistk/CinderStk.h"

const float MAX_PITCH_MIDI	= 80;
const float	MIN_PITCH_MIDI	= 40;

using namespace ci;
using namespace ci::app;
using namespace std;

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
	void handleGeneratorSelected();
	void handleEffectSelected();
	bool handleInstrumentSpecificNote( const vec2 &pos );
	void printAudioGraph();

	ci::audio::GainNodeRef	mMasterGain;

	// note: either last Instrument or last Generator will be used as input.
	cistk::InstrumentNodeRef	mInstrument;
	cistk::GeneratorNodeRef		mGenerator;
	cistk::EffectNodeRef		mEffect;

	ci::audio::GainNodeRef	mGeneratorGain;

	params::InterfaceGlRef	mParams;
	vector<string>			mInstrumentEnumNames, mGeneratorEnumNames, mEffectEnumNames;
	int						mInstrumentEnumSelection = 13;
	int						mGeneratorEnumSelection = 0;
	int						mEffectEnumSelection = 7;

	float mLastFreq = 0;
};

void StkTestApp::setup()
{
	auto ctx = audio::master();

	stk::Stk::setSampleRate( ctx->getSampleRate() );
	cistk::initRawwavePath();

	mMasterGain = ctx->makeNode<audio::GainNode>( 0.85f );
	mMasterGain >> ctx->getOutput();

	// Generators don't have a gain like instruments, so map mouse y to this node's value
	mGeneratorGain = ctx->makeNode<audio::GainNode>( 0 );

	setupParams();

	handleInstrumentSelected();
	handleEffectSelected();
	ctx->enable();

	printAudioGraph();
}

void StkTestApp::setupParams()
{
	mParams = params::InterfaceGl::create( getWindow(), "Stk params", toPixels( ivec2( 300, 200 ) ) );

	mParams->addParam<float>( "master gain",
					  [this]( float value ) { mMasterGain->setValue( value ); },
					  [this] { return mMasterGain->getValue(); }
	).min( 0.0f ).max( 1.0f ).step( 0.05f );

	mInstrumentEnumNames = {
		"none", "BandedWG", "BlowBotl", "BlowHole", "Bowed", "Brass", "Clarinet",
		"Drummer", "Flute", "Mandolin", "Mesh2D", "ModalBar", "Moog",
		"Plucked", "Resonate", "Saxofony", "Shakers", "Simple",	"Sitar",
		"StifKarp", "VoicForm", "Whistle",	"BeeThree", "FMVoices", "HevyMetl",
		"PercFlut", "Rhodey", "TubeBell", "Wurley"
	};
	mParams->addParam( "instrument", mInstrumentEnumNames, &mInstrumentEnumSelection )
		.keyDecr( "[" ).keyIncr( "]" )
		.updateFn( [this] { handleInstrumentSelected(); printAudioGraph(); } );

	mGeneratorEnumNames = {
		"none", "Blit", "Granulate"
	};
	mParams->addParam( "generator", mGeneratorEnumNames, &mGeneratorEnumSelection )
		.keyDecr( "u" ).keyIncr( "i" )
		.updateFn( [this] { handleGeneratorSelected(); printAudioGraph(); } );

	mEffectEnumNames = {
		"none", "Echo", "Chorus", "PitShift", "LentPitShift",
		"PRCRev", "JCRev", "NRev", "FreeVerb"
	};
	mParams->addParam( "effect", mEffectEnumNames, &mEffectEnumSelection )
		.keyDecr( "o" ).keyIncr( "p" )
		.updateFn( [this] { handleEffectSelected(); printAudioGraph(); } );

}

void StkTestApp::printAudioGraph()
{
	CI_LOG_I( "\n" << audio::master()->printGraphToString() );
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
	if( mInstrument ) {
		mInstrument->noteOff( 0.5 );
	}
	else if( mGenerator ) {
		mGeneratorGain->getParam()->applyRamp( 0, 0.2f );
	}
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
	if( mGenerator )
		mGenerator->disconnectAll();

	mGeneratorEnumSelection = 0; // set to "none"
	mGenerator = nullptr;
	mGeneratorGain->setValue( 0 );

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
	else if( name == "BeeThree" ) {
		mInstrument = ctx->makeNode<cistk::BeeThreeNode>();
	}
	else if( name == "FMVoices" ) {
		mInstrument = ctx->makeNode<cistk::FMVoicesNode>();
	}
	else if( name == "HevyMetl" ) {
		mInstrument = ctx->makeNode<cistk::HevyMetlNode>();
	}
	else if( name == "PercFlut" ) {
		mInstrument = ctx->makeNode<cistk::PercFlutNode>();
	}
	else if( name == "Rhodey" ) {
		mInstrument = ctx->makeNode<cistk::RhodeyNode>();
	}
	else if( name == "TubeBell" ) {
		mInstrument = ctx->makeNode<cistk::TubeBellNode>();
	}
	else if( name == "Wurley" ) {
		mInstrument = ctx->makeNode<cistk::WurleyNode>();
	}
	else if( name == "Whistle" ) {
		mInstrument = ctx->makeNode<cistk::WhistleNode>();
	}
	else {
		CI_LOG_E( "unknowned instrument name" );
		CI_ASSERT_NOT_REACHABLE();
	}

	if( mEffect ) {
		mInstrument >> mEffect >> mMasterGain;
	}
	else {
		mInstrument >> mMasterGain;
	}

	mMasterGain >> ctx->getOutput();
}

void StkTestApp::handleGeneratorSelected()
{
	if( mInstrument )
		mInstrument->disconnectAll();
	if( mGenerator )
		mGenerator->disconnectAll();

	mInstrument = nullptr;
	mInstrumentEnumSelection = 0; // set to "none"

	string name = mGeneratorEnumNames.at( mGeneratorEnumSelection );
	CI_LOG_I( "selecting generator '" << name << "'" );

	auto ctx = audio::master();
	if( name == "Blit" ) {
		mGenerator = ctx->makeNode<cistk::BlitNode>();
	}
	else if( name == "Granulate" ) {
		auto gen = ctx->makeNode<cistk::GranulateNode>( 1, stk::Stk::rawwavePath() + "ahh.raw", true );
		mGenerator = gen;
	}
	else {
		CI_LOG_E( "unknowned generator name" );
		CI_ASSERT_NOT_REACHABLE();
	}

	if( mEffect ) {
		mGenerator >> mGeneratorGain >> mEffect >> mMasterGain;
	}
	else {
		mGenerator >> mGeneratorGain >> mMasterGain;
	}

	mMasterGain >> ctx->getOutput();
}

void StkTestApp::handleEffectSelected()
{
	CI_ASSERT( mInstrument || mGenerator );

	mMasterGain->disconnectAll();
	if( mEffect )
		mEffect->disconnectAll();

	string name = mEffectEnumNames.at( mEffectEnumSelection );
	CI_LOG_I( "selecting effect '" << name << "'" );

	auto ctx = audio::master();

	if( name == "none" ) {
		// reset and bypass effect
		mEffect.reset();
		mInstrument >> mMasterGain >> ctx->getOutput();
		return;
	}
	else if( name == "Echo" ) {
		mEffect = ctx->makeNode<cistk::EchoNode>();
	}
	else if( name == "Chorus" ) {
		mEffect = ctx->makeNode<cistk::ChorusNode>();
	}
	else if( name == "PitShift" ) {
		auto effect = ctx->makeNode<cistk::PitShiftNode>();
		effect->setShift( 0.5f );
		mEffect = effect;
	}
	else if( name == "LentPitShift" ) {
		auto effect = ctx->makeNode<cistk::LentPitShiftNode>();
		effect->setShift( 0.5f );
		mEffect = effect;
	}
	else if( name == "PRCRev" ) {
		mEffect = ctx->makeNode<cistk::PRCRevNode>();
	}
	else if( name == "JCRev" ) {
		mEffect = ctx->makeNode<cistk::JCRevNode>();
	}
	else if( name == "NRev" ) {
		mEffect = ctx->makeNode<cistk::NRevNode>();
	}
	else if( name == "FreeVerb" ) {
		mEffect = ctx->makeNode<cistk::FreeVerbNode>();
	}
	else {
		CI_LOG_E( "unknowned effect name" );
		CI_ASSERT_NOT_REACHABLE();
	}

	if( mInstrument )
		mInstrument >> mEffect;
	else
		mGenerator >> mEffect;

	mEffect >> mMasterGain >> ctx->getOutput();
}

void StkTestApp::makeNote( const vec2 &pos )
{
	if( mInstrument && handleInstrumentSpecificNote( pos ) )
		return;

	float freq = quantizePitch( pos );
	if( fabs( mLastFreq - freq ) < 0.01f ) {
		return;
	}

	mLastFreq = freq;

	float gain = 1.0f - pos.y / (float)getWindowHeight();
	if( mInstrument ) {
		mInstrument->noteOn( freq, gain );
	}
	else if( mGenerator ) {
		mGeneratorGain->getParam()->applyRamp( gain, 0.05f );

		auto blitNode = dynamic_pointer_cast<cistk::BlitNode>( mGenerator );
		if( blitNode ) {
			blitNode->setFrequency( freq );
			return;
		}
		auto granulatorNode = dynamic_pointer_cast<cistk::GranulateNode>( mGenerator );
		if( granulatorNode ) {

			return;
		}
	}
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
