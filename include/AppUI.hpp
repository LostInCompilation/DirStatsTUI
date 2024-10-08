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
/*  AppUI.hpp                                                       */
/*  Created: 30.07.2024                                             */
/*------------------------------------------------------------------*/

#ifndef AppUI_hpp
#define AppUI_hpp

class AppUI : public ftxui::ComponentBase
{
public:
    enum class Sorting : uint8_t
    {
        SIZE_DESCENDING = 0,
        NAME_ASCENDING = 1,
    };
    
private:
    ftxui::ScreenInteractive*    m_Screen;
    
    // File system instance
    FileSystem          m_FileSystem;
    
    // State
    FileSystem::Path    m_StartingPath = "";
    bool                m_ShowAllFiles = false;
    //Sorting             m_FileSorting = Sorting::SIZE_DESCENDING;
    
    std::function<void()> m_QuitFunction;
    
    // UI
    std::string     m_SpaceInfoText = "";
    float           m_GaugeValueUsedSpace = 0.0f;
    
    // Spinner
    std::atomic<uint32_t>   m_SpinnerValue = 0;
    std::atomic<bool>       m_StopSpinnerThread = false;
    std::thread             m_SpinnerUpdateThread;
    
    //int32_t         m_CurrentFolderSelected = 0;
    std::shared_ptr<MenuComponent>   m_Menu;
    
    // Methods
    void            UpdateSpinnerTask() noexcept;
    
public:
    AppUI(ftxui::ScreenInteractive* screen, std::function<void()> quit);
    virtual ~AppUI();
    
    bool            UpdateSpaceInfo();
    
    bool            UpdateMainView();
    
    ftxui::Element  Render() override;
    bool            OnEvent(ftxui::Event event) override;
    
    void SetStartingPath(const FileSystem::Path& path) noexcept { m_StartingPath = path; }
    void SetShowAllFiles(bool showAll) noexcept { m_ShowAllFiles = showAll; }
};


#endif /* AppUI_hpp */
