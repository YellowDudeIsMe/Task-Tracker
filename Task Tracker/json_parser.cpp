#include "json_parser.h"

// Class StringJSON

json_parser::StringJSON::operator std::chrono::system_clock::time_point() {
	// Follow this on StackOverflow: https://stackoverflow.com/questions/21021388/how-to-parse-a-date-string-into-a-c11-stdchrono-time-point-or-similar

	std::tm tm{};
	std::string str{ this->c_str() };

	if (str[0] == '"' && str[str.size() - 1] == '"') {
		str = str.substr(1, str.size() - 2);
	}
	else {
		// throw error
	}

	std::stringstream ss{ str };
	ss >> std::get_time(&tm, "%a %b %d %H:%M:%S %Y");

	return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

json_parser::StringJSON::operator uint16_t() {
	return std::stoi(this->c_str());
}

json_parser::StringJSON::operator Task() {
	using namespace json_parser::process_string_ns;

	std::map<std::string, std::string> object = getObjectMap(this->c_str());

	Task task(
		processStringValue<uint16_t>(object["id"]),
		processStringValue<std::string>(object["description"]),
		processStringValue<std::string>(object["status"]),
		processStringValue<std::chrono::system_clock::time_point>(object["createdAt"]),
		processStringValue<std::chrono::system_clock::time_point>(object["updatedAt"])
	);
	
	return task;
}

json_parser::StringJSON::operator std::vector<uint16_t>() {
	using namespace json_parser::process_string_ns;

	std::vector<uint16_t> ret = getVector<uint16_t>(this->c_str());

	return ret;
}

json_parser::StringJSON::operator std::vector<Task>() {
	using namespace json_parser::process_string_ns;

	std::vector<Task> ret = getVector<Task>(this->c_str());

	return ret;
}

json_parser::StringJSON::operator Client() {
	using namespace json_parser::process_string_ns;

	std::map<std::string, std::string> object = getObjectMap(this->c_str());

	Client client(
		processStringValue<std::vector<Task>>(object["taskList"]),
		processStringValue<std::vector<uint16_t>>(object["vacantIds"]),
		processStringValue<uint16_t>(object["newId"])
	);

	return client;
}

// Namespace process_string_ns

std::map<std::string, std::string> json_parser::process_string_ns::getObjectMap(std::string str) {

	std::map<std::string, std::string> object;

	uint16_t parenCount = 1;
	const uint16_t indentLevel = 1;

	std::string name;
	std::string value;
	std::string item;

	bool isName{ true };
	bool isWithinQuote{ false };

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
			switch (c) {
			case ',':
			case '}':
				if (item[0] == '"' && item[item.size() - 1] == '"') {
					value = item.substr(1, item.size() - 2);
				}
				else {
					value = item;
				}

				object[name] = value;

				item.clear();
				break;

			case ':':
				if (item[0] == '"' && item[item.size() - 1] == '"') {
					name = item.substr(1, item.size() - 2);
				}

				item.clear();
				break;

			default:
				if (!std::isspace(c)) {
					item.push_back(c);
				}
				break;
			}
		}
	}

	return object;
}

// Namespace to_string_ns

std::string json_parser::to_string_ns::to_string(std::string s) {
	return s;
}

std::string json_parser::to_string_ns::to_string(std::chrono::system_clock::time_point time) {
	std::time_t timeDuration = std::chrono::system_clock::to_time_t(time);
	std::string ret = std::ctime(&timeDuration);
	ret.erase(ret.end() - 1);

	return ret;
}

std::string json_parser::to_string_ns::to_string(uint16_t n) {
	return std::to_string(n);
}

std::string json_parser::to_string_ns::to_string(Task task) {
	using namespace json_parser::to_string_ns;

	return
		"{ \"id\" : " + to_string(task.getId()) + " ," +
		" \"description\" : \"" + task.getDescription() + "\" ," +
		" \"status\" : \"" + task.getStatus() + "\" ," +
		" \"createdAt\" : \"" + to_string(task.getCreatedAt()) + "\" ," +
		" \"updatedAt\" : \"" + to_string(task.getUpdatedAt()) + "\" }";
}

std::string json_parser::to_string_ns::to_string(std::vector<uint16_t> arr) {
	std::string ret{ "[ " };
	for (uint16_t i : arr) {
		ret += " " + std::to_string(i) + " ,";
	}

	ret.erase(ret.end() - 1, ret.end());

	return ret + "]";
}

std::string json_parser::to_string_ns::to_string(std::vector<Task> arr) {
	std::string ret{ "[ " };
	for (Task i : arr) {
		ret += " " + json_parser::to_string_ns::to_string(i) + " ,";
	}

	ret.erase(ret.end() - 1, ret.end());

	return ret + "]";
}

std::string json_parser::to_string_ns::to_string(Client client) {
	using namespace json_parser::to_string_ns;

	return
		"{ \"taskList\" : " + to_string(client.getTaskList()) + " ," +
		" \"vacantIds\" : " + to_string(client.getVacantIds()) + " ," +
		" \"newId\" : " + to_string(client.getNewId()) + " }";
}

// Namespace client_ns

Client json_parser::client_ns::loadDataAsClient(std::string dir) {
	std::fstream file{ dir, std::ios::in };

	if (file) {

		// File does exist

		std::cout << "Loading \"client_data.json\"..." << std::endl;

		std::string fileContent;
		std::string line;

		while (std::getline(file, line)) {
			fileContent += line;
		}

		file.close();

		Client client;

		try {
			client = json_parser::process_string_ns::processStringValue<Client>(fileContent);
		}
		catch (std::string) {
			std::cerr << "Cannot read \"client_data.json\". Loading the default client instead..." << std::endl;
			std::cerr << "Warning: File \"client_data.json\" may be overwritted!" << std::endl;
		}

		std::cout << "Finish loading \"client_data.json\"!" << std::endl;
		std::cout << std::endl;

		return client;

	}
	else {
		// File does not exist

		std::cout << "Creating \"client_data.json\"..." << std::endl;

		file.close();
		file.open(dir, std::ios::out);

		Client defaultClient;

		std::cout << "Finish creating \"client_data.json\"!" << std::endl;
		std::cout << std::endl;

		file.close();
		return defaultClient;
	}
}

void json_parser::client_ns::saveClientData(Client client, std::string dir) {
	std::ofstream file{ dir };

	file << json_parser::to_string_ns::to_string(client);

	file.close();
	return;
}