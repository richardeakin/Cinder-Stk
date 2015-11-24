// Copyright (c) 2015, Richard Eakin
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE.txt file.
// The STK source code comes with its own BSD-style license, see lib/stk/LICENSE

#pragma once

namespace cistk {

//! Uitility method that searchs for the stk's rawwaves folder, first within an app assets directory,
//! then relative to the repo (useful during development)
void initRawwavePath();

} // namespace cistk
