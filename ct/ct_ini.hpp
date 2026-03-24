/**
 * @file ct_ini.hpp
 * @author Florent ALBANY (f.albany@serma.com)
 * @brief Robust INI configuration parser and serializer.
 * @version 1.1
 * @date 2026-03-20
 */

#ifndef CT_INI_HPP
#define CT_INI_HPP

#include <string>
#include <map>
#include <vector>
#include "../libctool.h"

namespace ct {
    /**
     * @namespace ct::ini
     * @brief Utilities for reading, modifying, and writing .ini files.
     */
    namespace ini {

        /**
         * @class Profile
         * @brief Represents an entire INI file structure.
         */
        class LIBCTOOL_API Profile {
          public:
            Profile() = default;

            /** @brief Loads and parses an INI file. */
            bool load(const std::string& filepath);

            /** @brief Saves the current structure back to a file. */
            bool save(const std::string& filepath) const;

            /** @brief Gets a value. Returns def if key or section missing. */
            std::string get(const std::string& section, const std::string& key, const std::string& def = "") const;

            /** @brief Sets or updates a value in a specific section. */
            void set(const std::string& section, const std::string& key, const std::string& value);

            /** @brief Checks if a section exists. */
            bool hasSection(const std::string& section) const;

            /** @brief Returns all keys within a section. */
            std::vector<std::string> getKeys(const std::string& section) const;

          private:
            std::map<std::string, std::map<std::string, std::string>> _data;
            std::string                                               _trim(const std::string& s) const;
        };

    }    // namespace ini
}    // namespace ct

#endif    // CT_INI_HPP