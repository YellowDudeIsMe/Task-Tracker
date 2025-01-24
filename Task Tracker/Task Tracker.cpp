#include "Task Tracker.h"
#include <iostream> // For i/o
#include <string> // For strings
#include <chrono> // For timing createdAt and updatedAt
#include <ctime> // For printing time
#include <tuple>

#include "task.h"
#include "client.h"
#include "json_parser.h"

int main() {
	Client client;


	client = json_parser::client_ns::loadDataAsClient("client_data.json");
	json_parser::client_ns::saveClientData(client, "client_data.json");

	std::cout << R"(
			 _____         _           _____  _     _____ 
			|_   _|       | |         /  __ \| |   |_   _|
			  | | __ _ ___| | ________| /  \/| |     | |  
			  | |/ _` / __| |/ /______| |    | |     | |  
			  | | (_| \__ \   <       | \__/\| |_____| |_ 
			  \_/\__,_|___/_|\_\       \____/\_____/\___/

                                                                  v1.0.0-alpha
                                 
			)" << std::endl;


	while (true) {
		std::string input;
		std::getline(std::cin, input);

		client.resolveCommand(input);
		json_parser::client_ns::saveClientData(client, "client_data.json");
	}

	return 0;
}
