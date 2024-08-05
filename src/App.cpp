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
/*  App.cpp                                                         */
/*  Created: 03.08.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

App::App(int argc, char** argv)
    : m_ArgC(argc)
    , m_ArgV(argv)
{
}

int App::ParseCommandLine()
{
    // Parse command line
    CLI::App cliApp(GetDescription(), GetAppName());
    m_ArgV = cliApp.ensure_utf8(m_ArgV); // For Unicode support on all platforms

    // Settings
    cliApp.footer("Footer");
    cliApp.get_formatter()->label("TEXT", "STRING");
#ifdef PLATFORM_WINDOWS
    cliApp.allow_windows_style_options();
#endif
    
    // Set correct default starting path depending on platform
#ifdef PLATFORM_WINDOWS
    std::string startPathStr = "C:\\";
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
    std::string startPathStr = "/";
#endif
    
    // Command line options
    cliApp.add_option("-p,--path", startPathStr, "Path to start scanning in");
    cliApp.add_flag("-a,--all", m_CLIShowAllFiles, "Show hidden files");//->group("SETTINGS");
    
    cliApp.set_version_flag("-v,--version", GetVersionString)->group("INFO");
    cliApp.set_help_flag("-h,--help", "Display help and exit")->group("INFO");
    
    try {
        cliApp.parse(m_ArgC, m_ArgV);
    }
    catch (const CLI::ParseError& e) {
        return cliApp.exit(e);
    }
    
    m_CLIStartingPath = CLI::to_path(startPathStr);
    
    return 0;
}

int App::Run()
{
    // Parse command line
    const int result = ParseCommandLine();
    if(result != 0)
        return result;
    
    // Debug print
    std::cout << "Show all: " << std::boolalpha << m_CLIShowAllFiles << std::endl;
    std::cout << "Path: " << m_CLIStartingPath << std::endl;
    
    return 0;
}

int32_t App::GetVersionMajor() noexcept
{
    return DirStatsTUI::CM_VERSION_MAJOR;
}

int32_t App::GetVersionMinor() noexcept
{
    return DirStatsTUI::CM_VERSION_MINOR;
}

int32_t App::GetVersionPatch() noexcept
{
    return DirStatsTUI::CM_VERSION_PATCH;
}

std::string App::GetVersionString() noexcept
{
    return DirStatsTUI::CM_VERSION_STRING;
}

std::string App::GetAppName() noexcept
{
    return DirStatsTUI::CM_APP_CMAKE_NAME;
}

std::string App::GetDescription() noexcept
{
    return DirStatsTUI::CM_APP_CMAKE_DESCRIPTION;
}
