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
    , m_Screen(ftxui::ScreenInteractive::Fullscreen())
{
    m_CLIApp = std::make_unique<CLI::App>(GetDescription(), GetAppName());
    m_AppUI = std::make_shared<AppUI>(&m_Screen, m_Screen.ExitLoopClosure());
}

void App::ParseCommandLine()
{
    // Parse command line
    m_ArgV = m_CLIApp->ensure_utf8(m_ArgV); // For Unicode support on all platforms

    // Settings
    m_CLIApp->footer("Footer");
    m_CLIApp->get_formatter()->label("TEXT", "STRING");
#ifdef PLATFORM_WINDOWS
    m_CLIApp->allow_windows_style_options();
#endif
    
    // Set correct default starting path depending on platform
#ifdef PLATFORM_WINDOWS
    std::string startPathStr = "C:\\";
#elif defined(PLATFORM_LINUX) || defined(PLATFORM_APPLE)
    std::string startPathStr = "/";
#endif
    
    // Command line options
    m_CLIApp->add_option("-p,--path", startPathStr, "Path to start scanning in");
    m_CLIApp->add_flag("-a,--all", m_CLIShowAllFiles, "Show hidden files");//->group("SETTINGS");
    
    m_CLIApp->set_version_flag("-v,--version", GetVersionString)->group("INFO");
    m_CLIApp->set_help_flag("-h,--help", "Display help and exit")->group("INFO");
    
    // Parse
    m_CLIApp->parse(m_ArgC, m_ArgV); // May throw CLI::ParseError
    
    m_CLIStartingPath = CLI::to_path(startPathStr);
}

int App::Run()
{
    // Parse command line
    try {
        ParseCommandLine();
    }
    catch (const CLI::ParseError& e) {
        return m_CLIApp->exit(e);
    }
    
    // Set arguments from CLI
    m_AppUI->SetStartingPath(m_CLIStartingPath);
    m_AppUI->SetShowAllFiles(m_CLIShowAllFiles);
    
    if(!m_AppUI->UpdateSpaceInfo())
        return -5;
    
    // Run UI
    m_Screen.Loop(m_AppUI);
    
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
