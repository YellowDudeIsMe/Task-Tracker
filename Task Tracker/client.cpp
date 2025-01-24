#include "client.h"

Client::Client(std::vector<Task> taskList, std::vector<uint16_t> vacantIds, uint16_t newId) {
	this->taskList = taskList;
	this->vacantIds = vacantIds;
	this->newId = newId;
}

std::vector<Task> Client::getTaskList() {
	return this->taskList;
}

std::vector<uint16_t> Client::getVacantIds() {
	return this->vacantIds;
}

uint16_t Client::getNewId() {
	return this->newId;
}

uint16_t Client::retrieveNewId() {

	if (this->vacantIds.size() > 0) {
		uint16_t resolvedNewId = this->vacantIds[this->vacantIds.size() - 1];
		this->vacantIds.pop_back();
		return resolvedNewId;
	}
	else {
		return this->newId++;
	}
}

void Client::deleteTaskById(uint16_t deletedId) {

	// Resolve the deleted id, check if is the latest id
	if (deletedId + 1 == this->newId) {
		newId--;

		// Delete all deletable vacant ids to free up space
		while (this->vacantIds.size() > 0 && this->vacantIds[0] + 1 == this->newId) {
			this->vacantIds.erase(this->vacantIds.begin());
			newId--;
		}

	}
	else {
		this->vacantIds.push_back(deletedId);
		std::sort(this->vacantIds.rbegin(), this->vacantIds.rend());
	}

	// Delete the corresponding task
	{
		auto i = this->taskList.begin();
		while (i != this->taskList.end()) {
			if ((*i).getId() == deletedId) {
				i = this->taskList.erase(i);
			}
			else {
				i++;
			}
		}
	}

}

Task* Client::getTaskById(uint16_t id) {
	for (Task& task : this->taskList) {
		if (task.getId() == id) {
			return &task;
		}
	}

	return nullptr;
}

std::string Client::formatTimeToPrint(std::chrono::system_clock::time_point time) {

	std::time_t timeT = std::chrono::system_clock::to_time_t(time);
	std::string formatTime = std::ctime(&timeT);

	// At this point, time format is "AAA MMM DD hh:mm:ss YYYY"
	// Aiming to print time in format "hh:mm DD-MMM-YYYY" instead

	//std::cout << formatTime << std::endl;

	std::string dayOfWeek = formatTime.substr(0, 3);
	std::string date      = formatTime.substr(8, 2);
	std::string month     = formatTime.substr(4, 3);
	std::string year      = formatTime.substr(20, 4);
	std::string hour      = formatTime.substr(11, 2);
	std::string minute    = formatTime.substr(14, 2);
	std::string second    = formatTime.substr(17, 2);

	return hour + ":" + minute + " " + date + "-" + month + "-" + year;
}

std::string Client::centerStringToPrint(std::string str, uint16_t length) {
	size_t rightMargin = (length - str.size()) / 2;
	size_t leftMargin = (length - str.size()) - rightMargin;

	return std::string(leftMargin, ' ') + str + std::string(rightMargin, ' ');
}

// RESOLVING DIFFERENT COMMANDS

void Client::resolveAddCommand(std::vector<std::string>& argumentList) {

	// ADD COMMAND

	if (argumentList.size() != 3) {
		throw std::string{ "Invalid number of arguments" };
	}

	if (argumentList[2][0] != '"' || argumentList[2][argumentList[2].size() - 1] != '"') {
		throw std::string{ "Invalid string format, expect quotation marks (\") on both sides of string" };
	}

	size_t extractSize = argumentList[2].size() - 2;
	std::string description = argumentList[2].substr(1, extractSize);

	// Create a new task
	Task newTask(this->retrieveNewId(), description);
	taskList.push_back(newTask);

	// Message
	std::cout << std::endl;
	std::cout << "---> A new task with ID " + std::to_string(newTask.getId()) + " is added!" << std::endl;
	std::cout << std::endl;
}

void Client::resolveUpdateCommand(std::vector<std::string>& argumentList) {

	// UPDATE COMMAND

	if (argumentList.size() != 4) {
		throw std::string{ "Invalid number of arguments" };
	}

	if (argumentList[3][0] != '"' || argumentList[3][argumentList[3].size() - 1] != '"') {
		throw std::string{ "Invalid string format, expect quotation marks (\") on both sides of string" };
	}

	uint16_t id = std::stoi(argumentList[2]);

	size_t extractSize = argumentList[3].size() - 2;
	std::string description = argumentList[3].substr(1, extractSize);

	// Assume task is valid, otherwise throw error
	Task* taskPtr = this->getTaskById(id);
	if (!taskPtr) {
		throw std::string{ "Invalid task ID, cannot find task with ID " + std::to_string(id) };
	}

	taskPtr->setDescription(description);

	// Message
	std::cout << std::endl;
	std::cout << "---> Task with ID " + std::to_string(id) + " is updated to \"" + taskPtr->getDescription() + "\"!" << std::endl;
	std::cout << std::endl;
}

void Client::resolveDeleteCommand(std::vector<std::string>& argumentList) {

	// DELETE COMMAND

	if (argumentList.size() != 3) {
		throw std::string{ "Invalid number of arguments" };
	}

	uint16_t id = std::stoi(argumentList[2]);

	// Assume task is valid, otherwise throw error

	Task* taskPtr = this->getTaskById(id);
	if (!taskPtr) {
		throw std::string{ "Invalid task ID, cannot find task with ID " + std::to_string(id) };
	}

	this->deleteTaskById(taskPtr->getId());

	// Message
	std::cout << std::endl;
	std::cout << "---> Task with ID " + std::to_string(id) + " is deleted!" << std::endl;
	std::cout << std::endl;
}

void Client::resolveMarkCommand(std::vector<std::string>& argumentList, std::string markStatus) {

	//MARK IN PROGRESS COMMAND

	if (argumentList.size() != 3) {
		throw std::string{ "Invalid number of arguments" };
	}

	uint16_t id = std::stoi(argumentList[2]);

	// Check if task is valid
	Task* taskPtr = this->getTaskById(id);
	if (!taskPtr) {
		throw std::string{ "Invalid task ID, cannot find task with ID " + std::to_string(id) };
	}

	taskPtr->setStatus(markStatus);

	//Message
	std::cout << std::endl;
	std::cout << "---> Task with ID " + std::to_string(id) + " is marked as \"" + taskPtr->getStatus() + "\"!" << std::endl;
	std::cout << std::endl;
}

void Client::resolveListCommand(std::vector<std::string>& argumentList) {

	// LIST COMMAND

	if (argumentList.size() < 2 && argumentList.size() > 3) {
		throw std::string{ "Invalid number of arguments" };
	}

	std::string printedStatus{};

	if (argumentList.size() == 3) {
		printedStatus = argumentList[2];

		if (printedStatus != "todo" && printedStatus != "in-progress" && printedStatus != "done") {
			throw std::string{ "Invalid status. Valid status include \"todo\", \"in-progress\" and \"done\"" };
		}

		std::cout << std::endl;
		std::cout << "---> The following is a list of all tasks with status \"" + printedStatus + "\":" << std::endl;
		std::cout << std::endl;
	}
	else {
		std::cout << std::endl;
		std::cout << "---> The following is a list of all tasks:" << std::endl;
		std::cout << std::endl;
	}

	// Print the whole list
	uint16_t taskCounter{};

	std::cout << "   ID   |     Created at    |     Updated at    |    Status   | Description"   << std::endl;
	std::cout << "--------+-------------------+-------------------+-------------+--------------" << std::endl;

	for (Task task : this->taskList) {

		uint16_t id = task.getId();
		std::string description = task.getDescription();
		std::string status = task.getStatus();

		// Skip tasks that does not have the required status
		if (!printedStatus.empty() && status != printedStatus) {
			continue;
		}
		
		std::cout << this->centerStringToPrint(std::to_string(id), 8) << "| "
			      << this->formatTimeToPrint(task.getCreatedAt()) << " | "
				  << this->formatTimeToPrint(task.getUpdatedAt()) << " | "
				  << this->centerStringToPrint(status, 11) + " | " << description << std::endl;

		taskCounter++;
	}

	std::cout << "--------+-------------------+-------------------+-------------+--------------" << std::endl;
	std::cout << std::endl;
	std::cout << "TASK COUNT: " << taskCounter << std::endl;
	std::cout << std::endl;
}

void Client::resolveCommand(std::string command) {

	// Process the command
	std::vector<std::string> argumentList;

	{
		std::string argumentTmp;
		bool acceptWhitespace{ false };


		for (char c : command) {

			if (c == '"') {
				acceptWhitespace = !acceptWhitespace;
			}

			if (c == ' ' && !acceptWhitespace) {
				argumentList.push_back(argumentTmp);
				argumentTmp.clear();
			}
			else {
				argumentTmp += c;
			}


		}

		argumentList.push_back(argumentTmp);
	}

	// Resolve cases
	try {

		// Track whether the command is valid, else return an error message
		bool validCommand{ false };


		if (argumentList.size() > 1 && argumentList[0] == "task-cli") {

			std::string action = argumentList[1];
			if (action == "add") {

				this->resolveAddCommand(argumentList);
				validCommand = true;
			}
			else if (action == "update") {

				this->resolveUpdateCommand(argumentList);
				validCommand = true;
			}
			else if (action == "delete") {

				this->resolveDeleteCommand(argumentList);
				validCommand = true;
			}
			else if (action == "mark-in-progress") {

				this->resolveMarkCommand(argumentList, "in-progress");
				validCommand = true;
			}
			else if (action == "mark-done") {

				this->resolveMarkCommand(argumentList, "done");
				validCommand = true;
			}
			else if (action == "list") {

				this->resolveListCommand(argumentList);
				validCommand = true;
			}
		}


		if (!validCommand) {
			throw std::string{ "Invalid command, please try again" };
		}
	}
	catch (std::string errorMessage) {
		//if the arguments are invalid, throw error
		std::cerr << std::endl;
		std::cerr << "===> Error: " << errorMessage << std::endl;
		std::cerr << std::endl;
	}
}