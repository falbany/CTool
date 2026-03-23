#include "ct_ini.hpp"
#include <fstream>

namespace ct {
    namespace ini {

        std::string Profile::_trim(const std::string& s) const {
            size_t first = s.find_first_not_of(" \t\r\n");
            if (first == std::string::npos) return "";
            size_t last = s.find_last_not_of(" \t\r\n");
            return s.substr(first, (last - first + 1));
        }

        bool Profile::load(const std::string& filepath) {
            std::ifstream file(filepath);
            if (!file.is_open()) return false;

            _data.clear();
            std::string line, current_section = "default";

            while (std::getline(file, line)) {
                line = _trim(line);
                if (line.empty() || line[0] == ';' || line[0] == '#') continue;

                // Strip inline comments
                size_t comment_pos = line.find_first_of(";#");
                if (comment_pos != std::string::npos) {
                    line = _trim(line.substr(0, comment_pos));
                }

                if (line[0] == '[' && line.back() == ']') {
                    current_section = _trim(line.substr(1, line.size() - 2));
                } else {
                    size_t sep = line.find('=');
                    if (sep != std::string::npos) {
                        std::string key             = _trim(line.substr(0, sep));
                        std::string val             = _trim(line.substr(sep + 1));
                        _data[current_section][key] = val;
                    }
                }
            }
            return true;
        }

        bool Profile::save(const std::string& filepath) const {
            std::ofstream file(filepath);
            if (!file.is_open()) return false;

            for (auto const& section : _data) {
                file << "[" << section.first << "]\n";
                for (auto const& kv : section.second) {
                    file << kv.first << " = " << kv.second << "\n";
                }
                file << "\n";
            }
            return true;
        }

        std::string Profile::get(const std::string& sec, const std::string& key, const std::string& def) const {
            auto s_it = _data.find(sec);
            if (s_it == _data.end()) return def;
            auto k_it = s_it->second.find(key);
            if (k_it == s_it->second.end()) return def;
            return k_it->second;
        }

        void Profile::set(const std::string& sec, const std::string& key, const std::string& val) { _data[sec][key] = val; }

        bool Profile::hasSection(const std::string& sec) const { return _data.find(sec) != _data.end(); }

        std::vector<std::string> Profile::getKeys(const std::string& sec) const {
            std::vector<std::string> keys;
            auto                     it = _data.find(sec);
            if (it != _data.end()) {
                for (auto const& kv : it->second) keys.push_back(kv.first);
            }
            return keys;
        }

    }    // namespace ini
}    // namespace ct