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
/*  AppUI.cpp                                                       */
/*  Created: 30.07.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

std::string onChangeFctStr = "OnChange: X";

void OnChange()
{
    static bool toggle = true;
    toggle = !toggle;
    
    onChangeFctStr = "OnChange: " + std::string(toggle ? "X" : "O");
}

void AppUI::UpdateSpinnerTask() noexcept
{
    while(!m_StopSpinnerThread)
    {
        m_SpinnerValue++;
        
        if(m_SpinnerValue > 199)
            m_SpinnerValue = 0;
        
        // Post a custom event to request rendering a new frame
        m_Screen->Post(ftxui::Event::Custom);
        
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::milliseconds(95));
    }
}

AppUI::AppUI(ftxui::ScreenInteractive* screen, std::function<void()> quit)
    : m_Screen(screen)
    , m_QuitFunction(quit)
{
    // Add main menu component
    m_Menu = std::make_shared<MenuComponent>();
    m_Menu->SetOnEnterFunction(m_QuitFunction);
    m_Menu->SetOnChangeFunction(OnChange);
    
    this->Add(m_Menu);
    
    //UpdateMainView();
    
    // Start spinner task
    m_SpinnerUpdateThread = std::thread(&AppUI::UpdateSpinnerTask, this);
    //std::async(std::launch::async, &AppUI::UpdateSpinnerTask, this);
}

AppUI::~AppUI()
{
    // Wait for spinner thread to join
    m_StopSpinnerThread = true;
    m_SpinnerUpdateThread.join();
}

bool AppUI::UpdateSpaceInfo()
{
    uintmax_t capacity = 0;
    uintmax_t free = 0;
    uintmax_t available = 0;
    
    if(!m_FileSystem.GetSpaceInfo(m_StartingPath, capacity, free, available))
    {
        // Error
        m_FileSystem.GetLastError().PrintErrorInformation();
        return false;
    }
    
    m_GaugeValueUsedSpace = 1.0f - (static_cast<float>(available) / static_cast<float>(capacity));
    m_SpaceInfoText = "Available space: [" + std::to_string(available / 1000.0f / 1000.0f / 1000.0f) + " GB] / ["
                        + std::to_string(capacity / 1000.0f / 1000.0f / 1000.0f) + " GB]  ";
    
    return true;
}

bool AppUI::UpdateMainView()
{
    return true;
}

ftxui::Element AppUI::Render()
{
    using namespace ftxui;

    // Main menu view
    auto mainView =
            hbox({
                m_Menu->Render() | flex | frame
        });
    
    // Bottom status line
    auto statusLine = hbox({
                //text(m_SpaceInfoText),
        
                hbox({spinner(15, m_SpinnerValue), text(" Loading...")}) | bold | size(WIDTH, GREATER_THAN, 14),
        
                text(onChangeFctStr) | bgcolor(Color::Yellow) | color(Color::Black) /*| flex*/ | size(WIDTH, EQUAL, 25),
        
                //text("["), gauge(m_GaugeValueUsedSpace) | flex | size(WIDTH, EQUAL, 10), text(std::format("] {:.2f}%", m_GaugeValueUsedSpace * 100.0f)),
                //text("space"),
        
                text("Selected: " + std::to_string(m_Menu->GetCurrentSelection()) + ", Focused: " + std::to_string(m_Menu->GetCurrentFocus())) | center | flex | bgcolor(Color::Blue),
                
                text("abc") | bgcolor(Color::Blue)
        });
    
    const std::wstring currentPathStr = L"Current path: " + m_StartingPath.wstring();
    
    return window(text("DirStatsTUI") | bold | center,
                vbox({
                    text(currentPathStr) | inverted,
                    separator(),
                    mainView | flex,
                    separator(),
                    text("Info about selected entry") | flex | size(HEIGHT, GREATER_THAN, 5),
                    separator(),
                    statusLine /*| bgcolor(Color::Blue)*/ /*| inverted*/
            })
            );
    
    return window(text(L"REPLACE ME") | bold | center, text("Content"));
}

bool AppUI::OnEvent(ftxui::Event event)
{
//    if (event == ftxui::Event::Character('h'))
//    {
//        return true;
//    }

    if (event == ftxui::Event::Escape)
    {
        m_QuitFunction();
        return true;
    }

    if (event == ftxui::Event::Character('q'))
    {
        m_QuitFunction();
        return true;
    }

    return ComponentBase::OnEvent(event);
}
