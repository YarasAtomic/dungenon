#include"network.h"
// File uploads

char * uploadPostRequest;
std::string uploadedFileId = "";
std::string uploadedFileName = "";

std::string getUploadedFileId() { return uploadedFileId;}
std::string getUploadedFileName() { return uploadedFileName;}
void cleanUploadedFileId() { uploadedFileId = "";}

void uploadSucceeded(emscripten_fetch_t* fetch) 
{
    free(uploadPostRequest); // Free upload buffer

    std::string response(reinterpret_cast<const char*>(fetch->data), fetch->numBytes);

    emscripten_fetch_close(fetch); // Free data associated with the fetch.

    auto i = response.find("id=") + 3; // Get the index of the identifier substr

    std::string id = response.substr(i,response.size()-i); // Get the identifier

    uploadedFileId = id;

    std::cout << "Succesful upload - id :" << id << std::endl;
}

void uploadFailed(emscripten_fetch_t* fetch) 
{
    free(uploadPostRequest);
    printf("Failed upload to %s - HTTP failure status code: %d.\n", fetch->url, fetch->status);
    emscripten_fetch_close(fetch); // Also free data on failure.
};

void handleUploadFile(std::string const &filename, std::string const &mime_type, std::string_view buffer, void*)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = uploadSucceeded;
    attr.onerror = uploadFailed;


    std::string buffer_data(buffer.data(),buffer.size());

    std::string request_data = "";

    uploadedFileName = filename;

    request_data+="--bass\r\n";
    request_data+="Content-Disposition: multipart/form-data\r\n" ;
    request_data+="Content-Type: text/plain\r\n" ;
    request_data+="\r\n" ;
    request_data+="--bass\r\n" ;
    request_data+="Content-Disposition: multipart/form-data; name=\"file\"; filename=\"";
    request_data+=filename;
    request_data+="\"\r\n";
    request_data+="Content-Type: application/octet-stream\r\n";
    request_data+="\r\n";
    request_data+=buffer_data;
    request_data+="--bass--";

    

    const char * headers[3];

    const char contentType[] = "Content-Type";
    const char contentTypeValue[] = "multipart/form-data; boundary=--bass";

    headers[0] = (const char*)malloc(strlen(contentType) + 1);
    strcpy(const_cast<char*>(headers[0]), contentType);

    headers[1] = (const char*)malloc(strlen(contentTypeValue) + 1);
    strcpy(const_cast<char*>(headers[1]), contentTypeValue);

    headers[2] = nullptr;

    attr.requestHeaders = headers;

    uploadPostRequest = (char *)malloc(request_data.size()*sizeof(char));

    std::memcpy(uploadPostRequest,request_data.c_str(),request_data.size());

    attr.requestData = uploadPostRequest;

    attr.requestDataSize = request_data.size();

    emscripten_fetch(&attr, "/upload");
}

// Get module names

std::vector<std::pair<std::string,std::string>> downloadedNames;

bool downloadComplete = false;

unsigned int getDownloadedNamesCount() {return downloadedNames.size();};
void cleanDownloadedNames(){downloadedNames.clear();};

bool checkDownload()
{
    if(downloadComplete)
    {
        downloadComplete = false;
        return true;
    }
    else 
        return false;
}

std::pair<std::string,std::string> getDownloadedName(int i){return downloadedNames[i];};

void getNamesSucceeded(emscripten_fetch_t* fetch) 
{
    downloadedNames.clear();
    
    std::stringstream ss(fetch->data);
    std::string to;
    std::string name;
    bool isName = true;

    if (fetch->data != NULL)
    {
        while(std::getline(ss,to,'\n'))
        {
            if(isName) 
                name = to;
            else 
            {
                std::pair<std::string,std::string> pair(name,to);
                downloadedNames.push_back(pair);
            }
            isName = !isName;
        }

        downloadComplete = true;
    }
}

void getNamesFailed(emscripten_fetch_t* fetch) 
{
    printf("Failed to get modules names - HTTP failure status code: %d.\n", fetch->status);
    emscripten_fetch_close(fetch); // Also free data on failure.
};

void getModuleNames()
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = getNamesSucceeded;
    attr.onerror = getNamesFailed;

    const char * headers[3];

    const char contentType[] = "Content-Type";
    const char contentTypeValue[] = "text/plain";

    headers[0] = (const char*)malloc(strlen(contentType) + 1);
    strcpy(const_cast<char*>(headers[0]), contentType);

    headers[1] = (const char*)malloc(strlen(contentTypeValue) + 1);
    strcpy(const_cast<char*>(headers[1]), contentTypeValue);

    headers[2] = nullptr;

    attr.requestHeaders = headers;

    attr.requestData = "getModules";

    attr.requestDataSize = 10;

    emscripten_fetch(&attr, "/get");
}

// Clean modules
void clearModules()
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = getNamesSucceeded;
    attr.onerror = getNamesFailed;

    const char * headers[3];

    const char contentType[] = "Content-Type";
    const char contentTypeValue[] = "text/plain";

    headers[0] = (const char*)malloc(strlen(contentType) + 1);
    strcpy(const_cast<char*>(headers[0]), contentType);

    headers[1] = (const char*)malloc(strlen(contentTypeValue) + 1);
    strcpy(const_cast<char*>(headers[1]), contentTypeValue);

    headers[2] = nullptr;

    attr.requestHeaders = headers;

    attr.requestData = "clearModules";

    attr.requestDataSize = strlen("clearModules");

    emscripten_fetch(&attr, "/get");
}