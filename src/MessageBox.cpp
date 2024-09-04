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
/*  MessageBox.cpp                                                  */
/*  Created: 04.09.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

#ifdef PLATFORM_APPLE
int32_t MessageBox::Show(const Type& type, const Buttons& buttons, const std::string& header, const std::string& message, uint32_t timeout)
{
    // Convert to CFStringRef
    const CFStringRef headerStr = CFStringCreateWithCString(nullptr, header.c_str(), static_cast<CFStringEncoding>(header.length()));
    const CFStringRef messageStr = CFStringCreateWithCString(nullptr, message.c_str(), static_cast<CFStringEncoding>(message.length()));
    
    // MessageBox type
    CFOptionFlags mbType = 0;
    switch(type)
    {
        case Type::INFO:
            mbType = kCFUserNotificationNoteAlertLevel;
            break;
        case Type::WARNING:
            mbType = kCFUserNotificationCautionAlertLevel;
            break;
        case Type::ERROR:
            mbType = kCFUserNotificationStopAlertLevel;
            break;
        default:
            mbType = kCFUserNotificationNoteAlertLevel;
            break;
    }
    
    // Buttons
    CFStringRef defaultBtn = nullptr;
    CFStringRef secondBtn = nullptr;
    switch(buttons)
    {
        case Buttons::OK:
            defaultBtn = CFSTR("OK");
            break;
        case Buttons::OK_CANCEL:
            defaultBtn = CFSTR("OK");
            secondBtn = CFSTR("Cancel");
            break;
        case Buttons::YES_NO:
            defaultBtn = CFSTR("Yes");
            secondBtn = CFSTR("No");
            break;
        default:
            defaultBtn = CFSTR("OK");
            break;
    }
    
    CFOptionFlags mbResult = 0;
    CFUserNotificationDisplayAlert(timeout,
                                   mbType,
                                   nullptr, // Icon
                                   nullptr,
                                   nullptr,
                                   headerStr,
                                   messageStr,
                                   defaultBtn,
                                   secondBtn,
                                   nullptr, // Third button
                                   &mbResult);
    
    CFRelease(headerStr);
    CFRelease(messageStr);
    
    // Return MessageBox result
    if(mbResult == kCFUserNotificationDefaultResponse)
        return 0;
    else
        return 1;
}
#elif defined(PLATFORM_LINUX)
int32_t MessageBox::Show(const Type& type, const Buttons& buttons, const std::string& header, const std::string& message, uint32_t timeout)
{
    // MessageBox type
    GtkMessageType mbType;
    switch(type)
    {
        case Type::INFO:
            mbType = GTK_MESSAGE_INFO;
            break;
        case Type::WARNING:
            mbType = GTK_MESSAGE_WARNING;
            break;
        case Type::ERROR:
            mbType = GTK_MESSAGE_ERROR;
            break;
        default:
            mbType = GTK_MESSAGE_INFO;
            break;
    }
    
    // Buttons
    GtkButtonsType buttonType = 0;
    switch(buttons)
    {
        case Buttons::OK:
            buttonType = GTK_BUTTONS_OK;
            break;
        case Buttons::OK_CANCEL:
            buttonType = GTK_BUTTONS_OK_CANCEL;
            break;
        case Buttons::YES_NO:
            buttonType = GTK_BUTTONS_YES_NO;
            break;
        default:
            buttonType = GTK_BUTTONS_OK;
            break;
    }
    
    // Create dialog
    GtkWidget* const dialog = gtk_message_dialog_new(nullptr, GTK_DIALOG_DESTROY_WITH_PARENT, mbType, buttonType, "%s\n\n%s", header, message);
    
    // Show dialog
    const gint mbResult = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Return MessageBox result
    if(mbResult == GTK_RESPONSE_OK || mbResult == GTK_RESPONSE_YES)
        return 0;
    else
        return 1;
}
#elif defined(PLATFORM_WINDOWS)
int32_t MessageBox::Show(const Type& type, const Buttons& buttons, const std::string& header, const std::string& message, uint32_t timeout)
{
#error Implement
}
#endif
