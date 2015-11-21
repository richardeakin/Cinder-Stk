Wrapper to use the [Synthesis Toolkit](https://ccrma.stanford.edu/software/stk/index.html) with [Cinder](https://libcinder.org/).

For convenience, the Stk `Instrmnt` and `Effect` classes have been wrapped into `audio::Node` types so they can be easily connected with other cinder audio nodes. However this is adds a bit of indirection and copying so if best performance is crucial, it's recommended to make a custom `audio::Node` and run whatever STK algorithms and `tick()`s you like from a single `Node::process()`.
