/*
    The zlib License

    Copyright (C) 2024 Marc Schöndorf
 
This software is provided 'as-is', without any express or implied warranty. In
no event will the authors be held liable for any damages arising from the use of
this software.

Permission is granted to anyone to use this software for any purpose, including
commercial applications, and to alter it and redistribute it freely, subject to
the following restrictions:

1.  The origin of this software must not be misrepresented; you must not claim
    that you wrote the original software. If you use this software in a product,
    an acknowledgment in the product documentation would be appreciated but is
    not required.

2.  Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

3.  This notice may not be removed or altered from any source distribution.
*/

/*------------------------------------------------------------------*/
/*                                                                  */
/*                      (C) 2024 Marc Schöndorf                     */
/*                            See license                           */
/*                                                                  */
/*  Main.hpp                                                        */
/*  Created: 29.07.2024                                             */
/*------------------------------------------------------------------*/

#ifndef Main_hpp
#define Main_hpp

// *******************************************************************
// Pre-processor settings

// Enable printing of platform specific error code and message,
// additionally to the platform independent ones
#define DST_PRINT_PLATFORM_SPECIFIC_ERROR_DESCRIPTION

// *******************************************************************
// System includes
#include <iostream>
#include <cstdint>
#include <vector>
#include <functional>
#include <system_error>
#include <filesystem>
//#include <future>
#include <thread>
#include <atomic>

// *******************************************************************
// CLI11 include
#include "CLI11.hpp"

// *******************************************************************
// FTXUI includes
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>

// *******************************************************************
// Project includes
#include "PlatformMacros.hpp"
#include "DirStatsTUIVersion.hpp"
#include "Error.hpp"
#include "FileSystem.hpp"
#include "MenuComponent.hpp"
#include "AppUI.hpp"
#include "App.hpp"

#endif /* Main_hpp */
