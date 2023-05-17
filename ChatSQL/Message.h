#pragma once
#include <string>
//#include <ctime>
#include <iostream>

class Message {
	std::string fromMessage_;
	std::string toMessage_;
	std::string text_;
	//std::time_t _timestamp;

public:
	Message() = default;

	Message(const std::string& fromMessage, const std::string& toMessage, const std::string& text);
	~Message() = default;

	const std::string& getFromMessage() const;
	const std::string& getToMessage() const;
	const std::string& getText() const;
	//const std::time_t& getTime() const;

};
