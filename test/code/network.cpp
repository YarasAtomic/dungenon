#include "network.h"
#include <iostream>

std::string uploadedFileId = "";

std::string getUploadedFileId() { return uploadedFileId;}
void cleanUploadedFileId() { uploadedFileId = "";}

void uploadSucceeded(emscripten_fetch_t* fetch)
{
    std::string response(reinterpret_cast<const char*>(fetch->data), fetch->numBytes);

    emscripten_fetch_close(fetch); // Free data associated with the fetch.

    auto i = response.find("id=") + 3;

    std::string id = response.substr(i,response.size()-i);

    std::cout << "id= " << id << std::endl;
    uploadedFileId = "modules/"+id;

    std::cout << uploadedFileId << std::endl;
}

void uploadFailed(emscripten_fetch_t* fetch)
{
    printf("Failed upload to %s - HTTP failure status code: %d.\n", fetch->url, fetch->status);
    emscripten_fetch_close(fetch); // Also free data on failure.
}

bool UploadFileTest(const std::string& url, const std::string& name, std::string_view buffer)
{
    std::string dataStr(buffer.data(), buffer.size());

    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "POST");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    attr.onsuccess = uploadSucceeded;
    attr.onerror = uploadFailed;

	std::string request = "";
	request+="--bass\r\n";
	request+="Content-Disposition: multipart/form-data\r\n" ;
	request+="Content-Type: text/plain\r\n" ;
	request+="\r\n" ;
	request+="--bass\r\n" ;
	request+="Content-Disposition: multipart/form-data; name=\"file\"; filename=\"";
    request+=name;
    request+="\"\r\n";
	request+="Content-Type: application/octet-stream\r\n";
	request+="\r\n";
	request+=dataStr;
	request+="--bass--";

    // Set headers:
    const char* headers[3];

    const char contentType[] = "Content-Type";
    const char contentTypeValue[] = "multipart/form-data; boundary=--bass";

    std::string request_size = std::to_string(request.size());

    headers[0] = (const char*)malloc(strlen(contentType) + 1);
    strcpy(const_cast<char*>(headers[0]), contentType);

    headers[1] = (const char*)malloc(strlen(contentTypeValue) + 1);
    strcpy(const_cast<char*>(headers[1]), contentTypeValue);

    headers[2] = nullptr;

    attr.requestHeaders = headers;

    char * request_data = (char *)malloc(request.size()*sizeof(char));

    strcpy(request_data,request.c_str());

    attr.requestData = request_data;
    attr.requestDataSize = request.size();

    // Send HTTP request:
    
    emscripten_fetch(&attr, url.c_str());

    std::cout << attr.requestData << std::endl;

    return true;
}


void handle_upload_file(std::string const &filename, std::string const &mime_type, std::string_view buffer, void*)
{
    if(UploadFileTest("/upload",filename,buffer))
    {
        std::cout << "Se ha actualizado el archivo subido" << std::endl;
    }
    else
    {
        uploadedFileId = "";
        std::cout << "Error subiendo archivo" << std::endl;
    }
    std::cout << "coño" <<std::endl;
}