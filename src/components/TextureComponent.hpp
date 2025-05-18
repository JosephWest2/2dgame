#pragma once

#import <string>

using string = std::string;

class TextureComponent {
public:
    TextureComponent(string entity_name)
        : _entity_name{entity_name}{}

    string GetEntityName() {
        return _entity_name;
    }

private:
   string _entity_name; 
};
