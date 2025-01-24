#ifndef JSON_PARSER_H
#define JSON_PARSER_H

// Follow the tutorial in this link https://stackoverflow.com/questions/34090638/c-convert-json-to-object

#include <string>
#include <vector>
#include <chrono>
#include <iomanip> // For formatting time from JSON file
#include <sstream> // For parsing the date string
#include <map> // For processing objects
#include <fstream> // For loading and saving data into JSON file

#include "task.h"
#include "client.h"

namespace json_parser {

	class StringJSON : public std::string {
	public:
		explicit operator std::chrono::system_clock::time_point();
		explicit operator uint16_t();
		explicit operator Task();
		explicit operator std::vector<uint16_t>();
		explicit operator std::vector<Task>();
		explicit operator Client();

	};

	namespace process_string_ns {

		template<typename T>
		static auto processStringValue(std::string s) -> T {
			return static_cast<T>(static_cast<StringJSON>(s));
		}

		template<typename T>
		static auto getVector(std::string str) -> std::vector<T> {
			uint16_t parenCount = 1;
			const uint16_t indentLevel = 1;
			std::string item;

			bool isWithinQuote{ false };

			std::vector<T> ret;

			str = str.substr(1, str.size() - 1);

			for (int i = 0; i < str.size(); i++) {
				char c = str[i];

				// Special characters
				switch (c) {
				case '\\':
					i++;
					item.push_back(str[i]);
					continue;
					break;

				case '"':
					isWithinQuote = !isWithinQuote;
					break;

				case '(':
				case '[':
				case '{':
					parenCount++;
					break;

				case ')':
				case ']':
				case '}':
					parenCount--;
					break;
				}


				// Check if end of an item or not
				if (isWithinQuote || parenCount > indentLevel) {
					item.push_back(c);
				}
				else {
					if (c == ',' || c == ']') {
						if (!item.empty()) {
							ret.push_back(json_parser::process_string_ns::processStringValue<T>(item));
						}

						item.clear();
					}
					else if (!std::isspace(c)) {
						item.push_back(c);
					}
				}

			}

			return ret;
		}

		static auto getObjectMap(std::string) -> std::map<std::string, std::string>;

	} // namespace

	namespace to_string_ns {

		// OVERLOAD a bunch of to_string here for different types that we will use

		std::string to_string(std::string);
		std::string to_string(std::chrono::system_clock::time_point);
		std::string to_string(uint16_t);
		std::string to_string(Task);
		std::string to_string(std::vector<uint16_t>);
		std::string to_string(std::vector<Task>);
		std::string to_string(Client);
	} //namespace

	namespace client_ns {

		Client loadDataAsClient(std::string);

		void saveClientData(Client, std::string);
	}

} // namespace

#endif