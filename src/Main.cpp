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
/*  Main.cpp                                                        */
/*  Created: 29.07.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

int main(int argc, char** argv)
{
    MessageBox::Display(MessageBox::Type::INFO, MessageBox::Buttons::OK_CANCEL, "Header", "Message", 3);
    return 0;
    
    // Create App and run it
    std::unique_ptr<App> app = std::make_unique<App>(argc, argv);
    const int result = app->Run();
    
    return result;
    
    
    // Our file system
    FileSystem fs;
    //const FileSystem::Path path = "/Users/marc/Desktop/iMessage Export 04.08.2024";
    const FileSystem::Path path = "/Users/marc/Documents";
    
    // Get whole volume space info
    uintmax_t capacity;
    uintmax_t free;
    uintmax_t available;
    if(!fs.GetSpaceInfo("/", capacity, free, available))
    {
        std::cout << fs.GetLastError() << std::endl;
        return -1;
    }
    
    std::cout << "Capacity:  " << std::setprecision(4) << static_cast<float>(capacity) / 1000.0f / 1000.0f / 1000.0f << "GB" << std::endl;
    std::cout << "Free:      " << std::setprecision(4) << static_cast<float>(free) / 1000.0f / 1000.0f / 1000.0f << "GB" << std::endl;
    std::cout << "Available: " << std::setprecision(4) << static_cast<float>(available) / 1000.0f / 1000.0f / 1000.0f << "GB" << std::endl << std::endl;
    
    // Get size of every file and directory inside path
    std::unordered_map<FileSystem::Path, FileSystem::DirectoryStats> directoryStats;
    uintmax_t totalSize = 0;
    
    if(!fs.GetSizesOfDirectoryRecursively(path, directoryStats, totalSize))
    {
        std::cout << fs.GetLastError() << std::endl;
        return -1;
    }
    
    std::cout << std::endl << "Total Size: " << static_cast<float>(totalSize) / 1000.0f / 1000.0f << "MB" << std::endl;
    
    return 0;
    
    
    
    ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();
    //exitFunction = screen.ExitLoopClosure();
    
    // **************************************************************
    // Menu
    std::vector<std::string> menu_entries = {"entry 1", "entry 2", "entry 3", "entry 4", "entry 5", "entry 6", "entry 7", "entry 8", "entry 9"};
    int menu_selected = 0;
    
    // Menu style and options
    ftxui::MenuOption menuOption = ftxui::MenuOption::Vertical();
    //menuOption.on_enter = MenuEnterEvent;
    menuOption.entries_option.transform = [](ftxui::EntryState state)
    {
        state.label = (state.active ? "> " : "  ") + state.label;
        
        ftxui::Element e = ftxui::hbox({
            ftxui::text(state.label),
            ftxui::gauge(0.5f) | ftxui::size(ftxui::WIDTH, ftxui::GREATER_THAN, 10)
        });
        
        if (state.focused)
          e = e | ftxui::bgcolor(ftxui::Color::Blue);
        
        if (state.active)
          e = e | ftxui::bold;
        
        return e;
    };
    
    ftxui::Component menu = ftxui::Menu(&menu_entries, &menu_selected, menuOption);
    
    // Wrap menu inside frame
    menu = Renderer(menu, [menu] { return menu->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 5) | ftxui::border; });
    
    // **************************************************************
    // Radio box
    std::vector<std::string> radiobox_list = {"Use gcc", "Use clang", "Use emscripten", "Use tcc"};
    int radio_selected = 0;
    
    // Menu style and options
    ftxui::RadioboxOption radioOption = ftxui::RadioboxOption::Simple();
    //radioOption.on_enter = MenuEnterEvent;
    radioOption.transform = [](ftxui::EntryState s)
    {
        ftxui::Element prefix = ftxui::text(s.state ? "◉ " : "○ ");

        ftxui::Element t = ftxui::text(s.label);
        
        if (s.active)
          t |= ftxui::bold;
            
        if (s.focused)
          t |= ftxui::inverted;
        
        return ftxui::hbox({prefix, t});
    };
    
    ftxui::Component radiobox = ftxui::Radiobox(&radiobox_list, &radio_selected, radioOption);
    
    // Wrap radiobox inside frame
    radiobox = Renderer(radiobox, [radiobox] { return radiobox->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 5) | ftxui::border; });
    
    
    
    
    // Container
    //ftxui::Component container = ftxui::Container::Vertical({menu, radiobox});
    
//    ftxui::Element windowDOM = ftxui::window(
//        ftxui::text("Window"),
//        menu->Render()
//    );
    
                  //ftxui::vbox({
                  //    ftxui::hcenter(ftxui::bold(ftxui::text("startingPath - Split: " + std::to_string(42)))),
                  //    ftxui::separator(),
                    //menu | ftxui::flex,
                    //separator(),
                    //statusLine
                    //})
               // );
    
    /*ftxui::Component renderer = ftxui::Renderer(container, [&] {
        return ftxui::vbox({
            menu->Render(),// | ftxui::frame | ftxui::border | ftxui::vscroll_indicator | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 5),
            //ftxui::window(ftxui::text("Window"), menu->Render() | ftxui::vscroll_indicator | ftxui::size(ftxui::HEIGHT, ftxui::LESS_THAN, 8)),
            //ftxui::window(ftxui::text("Window"), container->Render()),
            //ftxui::text("Render") | ftxui::inverted,
            //ftxui::separator(),
            //menu->Render() | ftxui::border | ftxui::flex,
            //ftxui::filler(),
            radiobox->Render() | ftxui::vscroll_indicator | ftxui::frame | ftxui::size(ftxui::HEIGHT, ftxui::GREATER_THAN, 8) | ftxui::border
        }) | ftxui::border;
    });*/
    
    // Container
    ftxui::Component container = ftxui::Container::Vertical({menu, radiobox});
    
    ftxui::Component mainRenderer = ftxui::Renderer(container, [&] {
        return ftxui::vbox({
            menu->Render(),
            ftxui::filler(),
            radiobox->Render()
        });
    });
    
    screen.Loop(mainRenderer);
//    screen.Loop(renderer);
    
    return 0;
}
