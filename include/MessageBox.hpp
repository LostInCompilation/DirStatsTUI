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
/*  MessageBox.hpp                                                  */
/*  Created: 04.09.2024                                             */
/*------------------------------------------------------------------*/

#ifndef MessageBox_hpp
#define MessageBox_hpp

namespace MessageBox
{
    enum class Type : uint8_t
    {
        INFO = 0,
        WARNING,
        ERROR
    };
    
    enum class Buttons : uint8_t
    {
        OK = 0,
        OK_CANCEL,
        YES_NO
    };

    /*
     Display()
     
     type:      The icon to display
     buttons:   The type of buttons to display
     header:    The header string
     message:   The message string
     timeout:   The timeout in seconds until the MessageBox closes by itself. Set to zero for no timeout. Not supported on linux.
     
     Return value:
        -1: Error while displaying message box
        0:  Default button got pressed (OK or YES)
        1:  Second button got pressed (CANCEL or NO)
     */
    int32_t Display(const Type& type, const Buttons& buttons, const std::string& header, const std::string& message, uint32_t timeout = 0);
};

#endif /* MessageBox_hpp */
