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
    
    // Show MessageBox
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
static void DummyCallback(GtkApplication* gtkApp, gpointer user_data) {}

int32_t MessageBox::Show(const Type& type, const Buttons& buttons, const std::string& header, const std::string& message, uint32_t timeout)
{
    // Create temporary GTK app as a hack
    GtkApplication* const gtkApp = gtk_application_new("org.DirStatsTUI.message_box", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(gtkApp, "activate", G_CALLBACK(DummyCallback), nullptr);
    
    char* argv = "";
    g_application_run(G_APPLICATION(gtkApp), 0, &argv);
    
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
    GtkButtonsType buttonType = GTK_BUTTONS_OK;
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
    GtkWidget* const dialog = gtk_message_dialog_new(nullptr, GTK_DIALOG_DESTROY_WITH_PARENT, mbType, buttonType, "%s\n\n%s", header.c_str(), message.c_str());
    
    // Show dialog
    const gint mbResult = gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);

    // Destroy GTK app
    g_object_unref(gtkApp);
    
    // Return MessageBox result
    if(mbResult == GTK_RESPONSE_OK || mbResult == GTK_RESPONSE_YES)
        return 0;
    else
        return 1;
}

#elif defined(PLATFORM_WINDOWS)
int32_t MessageBox::Show(const Type& type, const Buttons& buttons, const std::string& header, const std::string& message, uint32_t timeout)
{
    // Convert (UTF-8) std::string to Windows UTF-16
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    const std::wstring wHeader = converter.from_bytes(header);
    const std::wstring wMessage = converter.from_bytes(message);
    
    // MessageBox type
    uint32_t mbType = 0;
    switch(type)
    {
        case Type::INFO:
            mbType = MB_ICONINFORMATION;
            break;
        case Type::WARNING:
            mbType = MB_ICONWARNING;
            break;
        case Type::ERROR:
            mbType = MB_ICONERROR;
            break;
        default:
            mbType = MB_ICONINFORMATION;
            break;
    }
    
    // Buttons
    switch(buttons)
    {
        case Buttons::OK:
            mbType |= MB_OK;
            break;
        case Buttons::OK_CANCEL:
            mbType |= MB_OKCANCEL;
            break;
        case Buttons::YES_NO:
            mbType |= MB_YESNO;
            break;
        default:
            mbType |= MB_OK;
            break;
    }
    
    // Set as foreground window
    mbType |= MB_SYSTEMMODAL;
    mbType |= MB_SETFOREGROUND;
    
    int mbResult = 0;
    const HMODULE hUser32 = LoadLibraryA("user32.dll");
    if (hUser32)
    {
        typedef int(__stdcall *MSGBOXWAPI)(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
        auto MessageBoxTimeoutW = (MSGBOXWAPI)GetProcAddress(hUser32, "MessageBoxTimeoutW");
        
        // Show MessageBox
        mbResult = MessageBoxTimeoutW(nullptr, wMessage.c_str(), wHeader.c_str(), mbType, 0, timeout * 1000);
        
        FreeLibrary(hUser32);
    }
    else
    {
        // Fallback to MessageBox without timeout
        mbResult = ::MessageBoxW(nullptr, wMessage.c_str(), wHeader.c_str(), mbType);
    }
    
    // Return MessageBox result
    if(mbResult == IDOK || mbResult == IDYES)
        return 0;
    else
        return 1;
}
#endif
