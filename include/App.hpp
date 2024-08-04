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
/*  App.hpp                                                         */
/*  Created: 03.08.2024                                             */
/*------------------------------------------------------------------*/

#ifndef App_hpp
#define App_hpp

class App
{
private:
    int              m_ArgC = 0;
    char**           m_ArgV = nullptr;
    
    bool             m_CLIShowAllFiles = false;
    FileSystem::Path m_CLIStartingPath = "";
    
public:
    App(int argc, char** argv);
    
    int Run();
    
    // Version info
    static int32_t GetVersionMajor() noexcept;
    static int32_t GetVersionMinor() noexcept;
    static int32_t GetVersionPatch() noexcept;
    static std::string GetVersionString() noexcept;
    static std::string GetAppName() noexcept;
    static std::string GetDescription() noexcept;
};

#endif /* App_hpp */
