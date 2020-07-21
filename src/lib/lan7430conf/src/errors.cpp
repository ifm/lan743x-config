/** @file errors.hpp
 *
 *  @brief contains all possible errors
 *
 *  @author Matthias Abt(deabtma)
 *
 *  Copyright (C) 2020 ifm electronic GmbH
 *  See accompanied file licence.txt for license information.
 */

#include "lan7430conf/errors.hpp"

#include <string>
#include <map>

namespace ifm {

static const std::map<int, const std::string> errToStr = {
    { IFM_NO_ERROR, "Success" },
    { MAC_ADDRESS_EMPTY, "Empty MAC address" },
    { MAC_ADDRESS_INVALID, "Invalid MAC address" },
    { FOLDER_PATH_DOESNT_EXIST, "Folder doesn't exist" },
    { FILE_PATH_DOESNT_EXIST, "File doesn't exist" },
    { FILE_CANT_READ, "File can't be read" },
    { FILE_CANT_WRITE, "File can't be written" },
    { EEPROM_WRONG_SIZE, "EEPROM has wrong size" },
    { EEPROM_INVALID_MAGIC, "EEPROM has invalid magic number" },
};

int error_type::code() const noexcept { return m_errnum; }

const char* error_type::what() const noexcept
{
    if (errToStr.count(m_errnum))
    {
        return errToStr.at(m_errnum).c_str();
    }
    return "No description, please add error code to ``errToStr``";
}
}  // namespace ifm
