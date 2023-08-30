#include <emscripten.h>
#include <emscripten/fetch.h>
#include <cstring>
#include <iostream>
#include "emscripten_browser_file.h"

std::string getUploadedFileId();

void cleanUploadedFileName();

void handle_upload_file(std::string const &filename, std::string const &mime_type, std::string_view buffer, void*);