#ifndef TASK_H
#define TASK_H

#include <string>
#include <chrono>

class Task {
protected:

	uint16_t id;
	std::string description;
	std::string status;
	std::chrono::system_clock::time_point createdAt;
	std::chrono::system_clock::time_point updatedAt;

public:

	Task(uint16_t, std::string,
		std::string = "todo", 
		std::chrono::system_clock::time_point = std::chrono::system_clock::now(), 
		std::chrono::system_clock::time_point = std::chrono::system_clock::now());
	~Task();

	auto trackUpdate() -> void;
	auto getId() const -> uint16_t;
	auto getDescription() const -> std::string;
	auto setDescription(std::string) -> void;
	auto getStatus() const -> std::string;
	auto setStatus(std::string) -> void;
	auto getCreatedAt() const -> std::chrono::system_clock::time_point;
	auto getUpdatedAt() const -> std::chrono::system_clock::time_point;
};

#endif