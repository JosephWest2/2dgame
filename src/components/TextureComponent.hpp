#pragma once

#import <string>

using string = std::string;


class TextureComponent {
public:
    TextureComponent(string texture_filepath)
        : _texture_filepath{texture_filepath}{}

    string GetFilePath() {
        return _texture_filepath;
    }

private:
   string _texture_filepath; 
};
