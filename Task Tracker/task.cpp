#include "task.h"

Task::Task(uint16_t id, std::string description, 
	std::string status, 
	std::chrono::system_clock::time_point createdAt, 
	std::chrono::system_clock::time_point updatedAt) {

	this->id = id;
	this->description = description;
	this->status = status;
	this->createdAt = createdAt;
	this->updatedAt = updatedAt;

}

Task::~Task() {

}

void Task::trackUpdate() {
	this->updatedAt = std::chrono::system_clock::now();
}

uint16_t Task::getId() const {
	return this->id;
}

std::string Task::getDescription() const {
	return this->description;
}

void Task::setDescription(std::string description) {
	this->description = description;
	this->trackUpdate();
}

std::string Task::getStatus() const {
	return this->status;
}

void Task::setStatus(std::string status) {
	this->status = status;
	this->trackUpdate();
}

std::chrono::system_clock::time_point Task::getCreatedAt() const {
	return this->createdAt;
}

std::chrono::system_clock::time_point Task::getUpdatedAt() const {
	return this->updatedAt;
}