#include "Message.h"
#include <string>

Message::Message(const std::string& fromMessage, const std::string& toMessage, const std::string& text) : fromMessage_(fromMessage), toMessage_(toMessage), text_(text) {}

const std::string& Message::getFromMessage() const { return fromMessage_; }
const std::string& Message::getToMessage() const { return toMessage_; }
const std::string& Message::getText() const { return text_; }
//const std::time_t& Message::getTime() const { return _timestamp; }

