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
/*  MenuComponent.cpp                                               */
/*  Created: 06.08.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

ftxui::Element FileEntryTransform(const ftxui::EntryState& state)
{
    const std::string label = (state.focused ? "> " : "  ") + state.label;
    ftxui::Element e = ftxui::text(std::move(label));
    
    if (state.focused)
    {
        e = e | ftxui::inverted;
    }
    
    if (state.active)
    {
        e = e | ftxui::bold;// | ftxui::color(ftxui::Color::RGB(30, 255, 30));
        
//        if(state.focused)
//            e = e | ftxui::bold | ftxui::bgcolor(ftxui::Color::RGB(30, 255, 30));
//        else
//            e = e | ftxui::bold | ftxui::color(ftxui::Color::RGB(30, 255, 30)) | ftxui::bgcolor(ftxui::Color::RGB(100, 100, 100));
    }
    
    return e;
};

ftxui::Element DirectoryEntryTransform(const ftxui::EntryState& state)
{
    const std::string label = (state.focused ? "> " : "  ") + state.label;
    ftxui::Element e = ftxui::text(std::move(label));
    
    if (state.focused)
    {
        e = e | ftxui::inverted;
    }
    
    if (state.active)
    {
        e = e | ftxui::bold;// | ftxui::color(ftxui::Color::RGB(30, 255, 30));
        
//        if(state.focused)
//            e = e | ftxui::bold | ftxui::bgcolor(ftxui::Color::RGB(30, 255, 30));
//        else
//            e = e | ftxui::bold | ftxui::color(ftxui::Color::RGB(30, 255, 30)) | ftxui::bgcolor(ftxui::Color::RGB(100, 100, 100));
    }
    
    return e;// | ftxui::color(ftxui::Color::RGB(50, 100, 150));
};

MenuComponent::MenuComponent()
{
    // Container for menu
    this->Add(ftxui::Container::Vertical({}, &m_CurrentSelection));
    
    // *******************************************************
    // Entry option for files
    m_EntryOptionFile.animated_colors.foreground.enabled = true;
    m_EntryOptionFile.animated_colors.foreground.active = ftxui::Color::White;
    m_EntryOptionFile.animated_colors.foreground.inactive = ftxui::Color(150, 150, 150);
    m_EntryOptionFile.animated_colors.foreground.duration = std::chrono::milliseconds(200);
    
    m_EntryOptionFile.animated_colors.background.enabled = false;
    m_EntryOptionFile.animated_colors.background.active = ftxui::Color::Default;
    m_EntryOptionFile.animated_colors.background.inactive = ftxui::Color::Default;
    m_EntryOptionFile.animated_colors.background.duration = std::chrono::milliseconds(200);

    m_EntryOptionFile.transform = [&](const ftxui::EntryState& state)
    {
        return FileEntryTransform(state);
    };
    
    // *******************************************************
    // Entry option for directories
    m_EntryOptionDirectory.animated_colors.foreground.enabled = true;
    m_EntryOptionDirectory.animated_colors.foreground.active = ftxui::Color(100, 200, 255);
    m_EntryOptionDirectory.animated_colors.foreground.inactive = ftxui::Color(50, 100, 150);
    m_EntryOptionDirectory.animated_colors.foreground.duration = std::chrono::milliseconds(200);
    
    m_EntryOptionDirectory.animated_colors.background.enabled = false;
    m_EntryOptionDirectory.animated_colors.background.active = ftxui::Color::Default;
    m_EntryOptionDirectory.animated_colors.background.inactive = ftxui::Color::Default;
    m_EntryOptionDirectory.animated_colors.background.duration = std::chrono::milliseconds(200);
    
    m_EntryOptionDirectory.transform = [&](const ftxui::EntryState& state)
    {
        return DirectoryEntryTransform(state);
    };
    
    // *******************************************************
    // Add elements
    for(uint32_t i = 0; i < 8; i++)
    {
        MenuEntry entry;
        entry.isDirectory = false;
        entry.name = "Entry File " + std::to_string(i);
        
        AddEntry(entry);
    }
    
    for(uint32_t i = 0; i < 5; i++)
    {
        MenuEntry entry;
        entry.isDirectory = true;
        entry.name = "Entry Directory " + std::to_string(i);
        
        AddEntry(entry);
    }
    
    // Select first entry
    this->ChildAt(0)->SetActiveChild(this->ChildAt(0)->ChildAt(0));
    m_CurrentSelection = 0;
}

void MenuComponent::AddEntry(const MenuEntry& entry)
{
    m_Entries.push_back(entry);
    
    if(entry.isDirectory)
        this->ChildAt(0)->Add(ftxui::MenuEntry(entry.name, m_EntryOptionDirectory));
    else
        this->ChildAt(0)->Add(ftxui::MenuEntry(entry.name, m_EntryOptionFile));
}

void MenuComponent::ClearEntries()
{
    // Remove all entries
    this->ChildAt(0)->DetachAllChildren();
    m_Entries.clear();
    m_Entries.resize(0);
}

ftxui::Element MenuComponent::Render()
{
    return ftxui::ComponentBase::Render();
}

bool MenuComponent::OnEvent(ftxui::Event event)
{
    //if (Focused())
    const int32_t oldSelection = m_CurrentSelection;
    
    // Process events
    const bool result = this->ChildAt(0)->OnEvent(event);
    
    // Selection change?
    if (m_CurrentSelection != oldSelection)
    {
        m_CurrentFocus = m_CurrentSelection;
        m_OnChangeFunction();
    }
    
    // Get hovered entry
    //if(this->ChildAt(0)->Focused())
    {
        for(std::size_t i = 0; i < this->ChildAt(0)->ChildCount(); i++)
        {
            if(this->ChildAt(0)->ChildAt(i)->Active())
            {
                m_CurrentFocus = i;
                break;
            }
        }
    }
//    else
//    {
//        m_CurrentFocus = -1;
//    }
    
    return result;
    
    
    
    if (!CaptureMouse(event))
        return false;

    if (event.is_mouse())
        return OnMouseEvent(event);
    
    if (Focused())
    {
        const int32_t oldSelection2 = m_CurrentSelection;
        
        if (event == ftxui::Event::ArrowUp || event == ftxui::Event::Character('k'))
            m_CurrentSelection--;
        
        if (event == ftxui::Event::ArrowDown || event == ftxui::Event::Character('j'))
            m_CurrentSelection++;
        
//        if (event == Event::ArrowLeft || event == Event::Character('h'))
//            OnLeft();
//        
//        if (event == Event::ArrowRight || event == Event::Character('l'))
//            OnRight();
        
//        if (event == Event::PageUp)
//            m_CurrentSelection -= box_.y_max - box_.y_min;
//        
//        if (event == Event::PageDown)
//            m_CurrentSelection += box_.y_max - box_.y_min;
        
        if (event == ftxui::Event::Home)
            m_CurrentSelection = 0;
        
//        if (event == Event::End)
//            m_CurrentSelection = size() - 1;
        
//        if (event == Event::Tab && size())
//            selected() = (selected() + 1) % size();
//        
//        if (event == Event::TabReverse && size())
//            selected() = (selected() + size() - 1) % size();
        
        m_CurrentSelection = std::clamp<int32_t>(m_CurrentSelection, 0, m_Entries.size() - 1);
        
        if (m_CurrentSelection != oldSelection2)
        {
            m_CurrentFocus = m_CurrentSelection;
            
            //SelectedTakeFocus();
            m_OnChangeFunction();
            
            return true;
        }
    }
    
    if (event == ftxui::Event::Return)
    {
        m_OnEnterFunction();
        
        return true;
    }
    
    return false;
    
    //const bool result = ftxui::ComponentBase::OnEvent(event);
//
//    // On change event
//    if(m_OnChangeFunction && (m_CurrentSelection != m_OldSelection))
//    {
//        m_OldSelection = m_CurrentSelection;
//        m_OnChangeFunction();
//    }
//    
//    // On enter event
//    if(m_OnEnterFunction && event == ftxui::Event::Return)
//    {
//        m_OnEnterFunction();
//    }
//    
//    return result;
}

bool MenuComponent::OnMouseEvent(ftxui::Event event)
{
    //this->ChildAt(0)->OnEvent(event);
    
    return false;
}

bool MenuComponent::OnMouseWheel(ftxui::Event event)
{
    return false;
}
