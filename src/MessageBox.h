#pragma once

#include <string>

class MessageBox {
    MessageBox() { }

public:
    static MessageBox& get();

    void error(const std::string& title, const std::string& msg);
    void warning(const std::string& title, const std::string& msg);

    void info(const std::string& title, const std::string& msg);
};

