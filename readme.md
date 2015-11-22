Wrapper to use the [Synthesis Toolkit](https://ccrma.stanford.edu/software/stk/index.html) with [Cinder](https://libcinder.org/).

For convenience, the Stk `Instrmnt` and `Effect` classes have been wrapped into `audio::Node` types so they can be easily connected with other cinder audio nodes. However this is adds a bit of indirection and copying so if best performance is crucial, it's recommended to make a custom `audio::Node` and run whatever STK algorithms and `tick()`s you like from a single `Node::process()`.

##### Installation

If you're installing with TinderBox and decide to copy the source to your project, currently you'll have to additionally copy the STK's [rawwaves](lib/stk/rawwaves) folder into your assets directory to use some of the Instruments.  If you install as 'relative', it should just work when you call `cistk::initRawwavePath()`.


##### Usage

You need to do two things before using (enabling cinder's audio processing graph) to use the STK classes:

```cpp
auto ctx = ;

stk::Stk::setSampleRate( audio::master()->getSampleRate() ); // 1) set the samplerate
cistk::initRawwavePath(); // 2) configure the path to the rawwaves folder
```

Other than that seem the [StkTestApp](samples/StkTest/src/StkTestApp.cpp) for how to use the `InstrumentNode` or `EffectNode` wrappers, or their source for how to use STK directly.

See [here](https://libcinder.org/docs/guides/audio/index.html) for an overview on cinder's audio capabilities.
