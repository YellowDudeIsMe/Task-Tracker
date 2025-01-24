#ifndef CLIENT_H
#define CLIENT_H

#include <iostream> // For i/o
#include <string> // For strings
#include <chrono> // For timing createdAt and updatedAt
#include <vector> // For list of tasks
#include <algorithm> // For sorting the ids
#include <iomanip> // For formatting text output
#include <ctime> // For printing time

#include "task.h"

class Client {
private:

	std::vector<Task> taskList;
	std::vector<uint16_t> vacantIds; // Deleted ids to reuse, sort descending
	uint16_t newId = 1;

	auto retrieveNewId() -> uint16_t;
	auto deleteTaskById(uint16_t) -> void;
	auto getTaskById(uint16_t) -> Task*;

	auto formatTimeToPrint(std::chrono::system_clock::time_point) -> std::string;
	auto centerStringToPrint(std::string, uint16_t) -> std::string;

	auto resolveAddCommand(std::vector<std::string>&) -> void;
	auto resolveUpdateCommand(std::vector<std::string>&) -> void;
	auto resolveDeleteCommand(std::vector<std::string>&) -> void;
	auto resolveMarkCommand(std::vector<std::string>&, std::string) -> void;
	auto resolveListCommand(std::vector<std::string>&) -> void;

public:

	Client(std::vector<Task> = std::vector<Task>(), std::vector<uint16_t> = std::vector<uint16_t>(), uint16_t = 1);

	auto getTaskList() -> std::vector<Task>;
	auto getVacantIds() -> std::vector<uint16_t>;
	auto getNewId() -> uint16_t;

	auto resolveCommand(std::string) -> void;
};

#endif