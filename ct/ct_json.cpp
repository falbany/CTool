#include "ct_json.hpp"
#include <sstream>
#include <algorithm>

namespace ct {
    namespace json {

        std::string stringify(const std::map<std::string, std::string>& kv_pairs) {
            std::stringstream ss;
            ss << "{";
            for (auto it = kv_pairs.begin(); it != kv_pairs.end(); ++it) {
                ss << "\"" << it->first << "\":\"" << it->second << "\"";
                if (std::next(it) != kv_pairs.end()) ss << ",";
            }
            ss << "}";
            return ss.str();
        }

        std::string stringifyArray(const std::vector<std::string>& list) {
            std::stringstream ss;
            ss << "[";
            for (size_t i = 0; i < list.size(); ++i) {
                ss << "\"" << list[i] << "\"";
                if (i < list.size() - 1) ss << ",";
            }
            ss << "]";
            return ss.str();
        }

        std::string getValue(const std::string& json, const std::string& key) {
            std::string searchKey = "\"" + key + "\"";
            size_t      keyPos    = json.find(searchKey);
            if (keyPos == std::string::npos) return "";

            // Find the colon after the key
            size_t colonPos = json.find(':', keyPos + searchKey.length());
            if (colonPos == std::string::npos) return "";

            // Find the start of the value (the next quote)
            size_t valStart = json.find('"', colonPos);
            if (valStart == std::string::npos) return "";

            // Find the end of the value
            size_t valEnd = json.find('"', valStart + 1);
            if (valEnd == std::string::npos) return "";

            return json.substr(valStart + 1, valEnd - valStart - 1);
        }

        bool isValid(const std::string& json) {
            if (json.empty()) return false;
            std::string trimmed = json;
            trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r"));
            trimmed.erase(trimmed.find_last_not_of(" \t\n\r") + 1);

            return (trimmed.front() == '{' && trimmed.back() == '}') || (trimmed.front() == '[' && trimmed.back() == ']');
        }

        std::string minify(const std::string& json) {
            std::string result;
            bool        in_quotes = false;
            for (char c : json) {
                if (c == '\"') in_quotes = !in_quotes;
                if (!in_quotes && (c == ' ' || c == '\n' || c == '\r' || c == '\t')) continue;
                result += c;
            }
            return result;
        }

    }    // namespace json
}    // namespace ct