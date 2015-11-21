
#include "cistk/CinderStk.h"

#include "cinder/Log.h"
#include "cinder/app/Platform.h"

using namespace ci;

namespace cistk {

std::once_flag sInitOnceFlag;

void initRawwavePath()
{
	std::call_once( sInitOnceFlag, [] {
		// search for rawwaves directory in an assets folder
		fs::path rawwavePath = app::Platform::get()->getAssetPath( "rawwaves" );
		if( rawwavePath.empty() ) {
			// search for it relative to this source file
			fs::path relativePath = fs::path( __FILE__ ).parent_path().parent_path() / "lib/stk/rawwaves";
			if( fs::exists( relativePath ) && fs::is_directory( relativePath ) ) {
				rawwavePath = relativePath;
			}
		}

		if( ! rawwavePath.empty() ) {
			stk::Stk::setRawwavePath( rawwavePath.c_str() );
		}
		else {
			CI_LOG_E( "failed to find path to rawwaves directory" );
		}
	} );
}

} // namespace cistk