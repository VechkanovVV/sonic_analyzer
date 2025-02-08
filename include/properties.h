#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <string>

class Properties {
public:
    explicit Properties();
    std::string getPath() const;

private:
    std::string path;
};

#endif  // PROPERTIES_H