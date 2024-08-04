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
/*  Error.cpp                                                       */
/*  Created: 30.07.2024                                             */
/*------------------------------------------------------------------*/

#include "Main.hpp"

void Error::PrintErrorInformation() const
{
    std::cout << std::endl;
    std::cout << "********************************************************************" << std::endl;
    std::cout << "* ERROR                                                            *" << std::endl;
    std::cout << "*                                                                  *" << std::endl;
    std::cout << "* CODE:             " << std::setw(46) << std::left <<this->GetCode() << std::setw(0) << " *" << std::endl;
    std::cout << "* CATEGORY:         " << std::setw(46) << std::left <<this->GetCategoryName() << std::setw(0) << " *" << std::endl;
    std::cout << "*                                                                  *" << std::endl;
    std::cout << "* MESSAGE:          " << std::setw(46) << std::left << this->GetMessage() << std::setw(0) << " *" << std::endl;
    std::cout << "*                                                                  *" << std::endl;
    
#ifdef DST_PRINT_PLATFORM_SPECIFIC_ERROR_DESCRIPTION
    std::cout << "* ---------------------------------------------------------------- *" << std::endl;
    std::cout << "* PLATFORM CODE:    " << std::setw(46) << std::left <<this->GetPlatformCode() << std::setw(0) << " *" << std::endl;
    std::cout << "* PLATFORM MESSAGE: " << std::setw(46) << std::left << this->GetPlatformMessage() << std::setw(0) << " *" << std::endl;
    std::cout << "*                                                                  *" << std::endl;
#endif
    std::cout << "********************************************************************" << std::endl;
    std::cout << std::endl;
}

void Error::Clear()
{
    m_ErrorCode.clear();
}

bool Error::IsError() const
{
    return static_cast<bool>(m_ErrorCode);
}

int32_t Error::GetCode() const
{
    return m_ErrorCode.default_error_condition().value();
}

int32_t Error::GetPlatformCode() const
{
    return m_ErrorCode.value();
}

std::string Error::GetMessage() const
{
    return m_ErrorCode.default_error_condition().message();
}

std::string Error::GetPlatformMessage() const
{
    return m_ErrorCode.message();
}

std::string Error::GetCategoryName() const
{
    return m_ErrorCode.default_error_condition().category().name();
}
