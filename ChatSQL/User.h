#pragma once
#include <string>

class User {
	std::string name_;
	std::string login_;
	unsigned int* password_;

public:
	User() = default;
	User(const std::string& name, const std::string& login, unsigned int* password);
	~User() = default;
	const std::string& getName() const;
	const std::string& getLogin() const;
	const unsigned int* getPassword() const;
};

