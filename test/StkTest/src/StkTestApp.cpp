#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "cinder/audio/audio.h"
#include "stk/include/Stk.h"

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
	}

	void process( ci::audio::Buffer *buffer ) override
	{

	}
};

class StkTestApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	ci::audio::GainNodeRef	mGain;
	StkTestNodeRef			mStkNode;
};

void StkTestApp::setup()
{
	auto ctx = audio::master();
	mStkNode = ctx->makeNode<StkTestNode>();
	mGain = ctx->makeNode<audio::GainNode>( 0.5f );

	mStkNode >> mGain >> ctx->getOutput();
}

void StkTestApp::mouseDown( MouseEvent event )
{
}

void StkTestApp::update()
{
}

void StkTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( StkTestApp, RendererGl )
