#include "Message.h"
#include <string>

Message::Message(const std::string& fromMessage, const std::string& toMessage, const std::string& text, std::time_t& timestamp) : fromMessage_(fromMessage), toMessage_(toMessage), text_(text), timestamp_(timestamp){}

const std::string& Message::getFromMessage() const { return fromMessage_; }
const std::string& Message::getToMessage() const { return toMessage_; }
const std::string& Message::getText() const { return text_; }
const std::time_t& Message::getTime() const { return timestamp_; }

