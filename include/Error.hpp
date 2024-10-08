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
/*  Error.hpp                                                       */
/*  Created: 31.07.2024                                             */
/*------------------------------------------------------------------*/

#ifndef Error_hpp
#define Error_hpp

class Error
{
private:
    std::error_code m_ErrorCode;
    
public:
    Error() = default;
    
    operator std::error_code& () { return m_ErrorCode; }
    operator bool () const       { return IsError(); }
    
    bool IsError() const;
    void PrintErrorInformation() const;
    
    void Clear();
    
    int32_t GetCode() const;
    int32_t GetPlatformCode() const;
    std::string GetMessage() const;
    std::string GetPlatformMessage() const;
    std::string GetCategoryName() const;
    
    // Stream out operator overload
    friend std::ostream& operator<< (std::ostream& stream, const Error& error)
    {
        error.PrintErrorInformation();
        
        return stream;
    }
};

#endif /* Error_hpp */
