#include "ct_str.hpp"
#include "ct_log.hpp"

#include <iomanip>
#include <algorithm>
#include <cctype>

namespace ct {
    namespace str {
        void assign(std::string& dest, const char* src) {
            if (src)
                dest.assign(src);
            else
                dest.clear();
        }

        std::string updateField(const std::string& currentStr, const std::string& prefix, const std::string& newValue, char delimiter) {
            std::vector<std::string> tokens = split(currentStr, delimiter);
            bool                     found  = false;
            std::string              result;

            for (size_t i = 0; i < tokens.size(); ++i) {
                if (tokens[i].compare(0, prefix.length(), prefix) == 0) {
                    tokens[i] = prefix + newValue;
                    found     = true;
                }
                result += tokens[i] + (i == tokens.size() - 1 ? "" : std::string(1, delimiter));
            }
            if (!found) {
                result += (result.empty() ? "" : std::string(1, delimiter)) + prefix + newValue;
            }
            return result;
        }

        std::string findAndReplace(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr) {
            std::string sOut = sIn;
            if (sOut.empty() || searchStr.empty()) return sOut;
            std::size_t pos = 0;
            while ((pos = sOut.find(searchStr, pos)) != std::string::npos) {
                sOut.replace(pos, searchStr.length(), replaceStr);
                pos += replaceStr.length();
            }
            return sOut;
        }

        std::string findAndReplaceRecursive(const std::string& sIn, const std::string& searchStr, const std::string& replaceStr) {
            std::string sOut = sIn;
            if (sOut.empty() || searchStr.empty()) return sOut;
            if (replaceStr.find(searchStr) != std::string::npos) {
                ct::log::toCerr(ct::log::Level::Error, "Recursive replace error: target contains search keyword.");
                return sOut;
            }
            std::size_t pos = 0;
            while ((pos = sOut.find(searchStr, pos)) != std::string::npos) {
                sOut.replace(pos, searchStr.length(), replaceStr);
            }
            return sOut;
        }

        std::string catchInRange(const std::string& sIn, const std::string& searchStr1, const std::string& searchStr2) {
            std::size_t startPos = sIn.find(searchStr1);
            if (startPos == std::string::npos) return "";
            startPos += searchStr1.length();
            std::size_t endPos = sIn.find(searchStr2, startPos);
            if (endPos == std::string::npos) return "";
            return sIn.substr(startPos, endPos - startPos);
        }

        std::vector<std::string> split(const std::string& str, char delimiter) {
            std::vector<std::string> tokens;
            std::stringstream        ss(str);
            std::string              item;
            while (std::getline(ss, item, delimiter)) {
                if (!item.empty()) tokens.push_back(item);
            }
            return tokens;
        }

        std::string trim(const std::string& str) {
            auto start = std::find_if_not(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
            auto end   = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) { return std::isspace(c); }).base();
            return (start < end ? std::string(start, end) : std::string());
        }
    }    // namespace str
}    // namespace ct