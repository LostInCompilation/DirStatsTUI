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
/*  MenuComponent.hpp                                               */
/*  Created: 06.08.2024                                             */
/*------------------------------------------------------------------*/

#ifndef MenuComponent_hpp
#define MenuComponent_hpp

class MenuComponent : public ftxui::ComponentBase
{
public:
    struct MenuEntry
    {
        std::string name = "";
        
        bool isDirectory = false;
        uintmax_t count = 0;
    };
    
private:
    std::function<void()>   m_OnChangeFunction;
    std::function<void()>   m_OnEnterFunction;
    
    ftxui::MenuEntryOption  m_EntryOptionFile = {};
    ftxui::MenuEntryOption  m_EntryOptionDirectory = {};
    
    std::vector<MenuEntry>  m_Entries;
        
    int32_t                 m_CurrentSelection = -1;
    int32_t                 m_CurrentFocus = -1;
    
    bool            OnMouseEvent(ftxui::Event event);
    bool            OnMouseWheel(ftxui::Event event);
    
public:
    MenuComponent();
    
    void            AddEntry(const MenuEntry& entry);
    void            ClearEntries();
    
    ftxui::Element  Render() override;
    bool            OnEvent(ftxui::Event event) override;
    
    // Setter
    void            SetOnChangeFunction(std::function<void()> func) noexcept { m_OnChangeFunction = func; }
    void            SetOnEnterFunction(std::function<void()> func) noexcept { m_OnEnterFunction = func; }
    
    // Getter
    int32_t         GetCurrentSelection() const noexcept { return m_CurrentSelection; }
    int32_t         GetCurrentFocus() const noexcept { return m_CurrentFocus; }
};

#endif /* MenuComponent_hpp */
