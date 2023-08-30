#include <cstring>
#include <iostream>
#include <emscripten/fetch.h>
#include <vector>
#include <map>
#include <sstream>
#include <utility>

std::string getUploadedFileId();
std::string getUploadedFileName();
void cleanUploadedFileId();

void handleUploadFile(std::string const &, std::string const &, std::string_view , void*);

unsigned int getDownloadedNamesCount();

void cleanDownloadedNames();

std::pair<std::string,std::string> getDownloadedName(int i);

bool checkDownload();

void getModuleNames();

void clearModules();

void getShader(std::string url);

void loadShaderCallback(emscripten_fetch_t* fetch);

void getShader(std::string url,std::string *);