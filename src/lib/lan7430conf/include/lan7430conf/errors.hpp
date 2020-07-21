/** @file errors.hpp
 *
 *  @brief contains all possible errors
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */
#ifndef LAN7430_ERRORS_HPP
#define LAN7430_ERRORS_HPP

#include <exception>

#include "lan7430conf/lan7430-config-lib_export.h"

namespace ifm {
constexpr int IFM_NO_ERROR = 0;

constexpr int MAC_ADDRESS_EMPTY = 1000;
constexpr int MAC_ADDRESS_INVALID = 1001;

constexpr int FOLDER_PATH_DOESNT_EXIST = 2000;
constexpr int FILE_PATH_DOESNT_EXIST = 2001;
constexpr int FILE_CANT_READ = 2002;
constexpr int FILE_CANT_WRITE = 2003;

constexpr int EEPROM_WRONG_SIZE = 3000;
constexpr int EEPROM_INVALID_MAGIC = 3001;

class LAN7430_CONFIG_LIB_EXPORT error_type : public std::exception
{
public:
    error_type(int errnum)
    : m_errnum(errnum)
    {
    }
    virtual ~error_type() = default;
    int code() const noexcept;
    virtual const char* what() const noexcept override;

private:
    error_type() = delete;
    int m_errnum;
};
}  // namespace ifm

#endif // LAN7430_ERRORS_HPP
