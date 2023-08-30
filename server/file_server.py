from http.server import HTTPServer, BaseHTTPRequestHandler
import os
import json
import random
import string
import re

class FileServerHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            with open('./dungenon.html', 'rb') as f:
                self.wfile.write(f.read())
        else:
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            with open('./' + self.path, 'rb') as f:
                self.wfile.write(f.read())
        # else:
        #     self.send_response(404)
        #     self.end_headers()

    def do_POST(self):
        
        if self.path == '/upload':

            content_type = self.headers.get('Content-Type')
            length = int(self.headers.get('content-length'))
            
            boundary = self.find_attr(content_type,"boundary")

            content_type = content_type.split("; ")[0]

            if content_type == 'multipart/form-data' and boundary != "":
                request_data = self.rfile.read(length)

                parts = request_data.split(boundary.encode('utf-8'))

                attrPart = parts[1]
                dataPart = parts[2].split(b"\r\n\r\n")

                dataHeaders = dataPart[0].decode('utf-8')
                data = dataPart[1]

                dispHeader = self.find_header(dataHeaders,"Content-Disposition").replace(" ", "")
                typeHeader = self.find_header(dataHeaders,"Content-Type").replace(" ", "")

                modName = self.find_attr(dispHeader,"filename").replace("\"", "")

                print(modName)

                filename = "dun-"+self.string_generator(16)

                jsonData = {
                    'file_id': filename ,
                    'modname': modName
                }
                self.store_data(jsonData)

                if typeHeader == "application/octet-stream":
                    uploaded_file_path = os.path.join('modules/', filename)
                    with open(uploaded_file_path, 'wb') as f:
                        f.write(data)
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    response_text = f"Uploaded file id={filename}"
                    self.wfile.write(response_text.encode('utf-8'))
                else:
                    self.send_response(400)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    self.wfile.write(b'Non readable content in data section')
            else:
                self.send_response(400)
                self.send_header('Content-Type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Non readable content')

        elif self.path == '/get':

            content_type = self.headers.get('Content-Type')
            length = int(self.headers.get('content-length'))
            if content_type == 'text/plain':
                request_data = self.rfile.read(length).decode('utf-8')
                if request_data == 'getModules':
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    response_text = self.read_data()
                    self.wfile.write(response_text.encode('utf-8'))
                elif request_data == 'clearModules':
                    self.send_response(200)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    self.clear_modules()
                    response_text = self.read_data()
                    self.wfile.write(response_text.encode('utf-8'))
                else:
                    self.send_response(400)
                    self.send_header('Content-Type', 'text/plain')
                    self.end_headers()
                    self.wfile.write(b'Invalid text request')
            else:
                self.send_response(400)
                self.send_header('Content-Type', 'text/plain')
                self.end_headers()
                self.wfile.write(b'Non readable content')
        else:
            self.send_response(404)
            self.end_headers()

    def find_attr(self,data,attr):
        reg = (attr+'=(.*)')
        attr_match = re.search(reg, data)
        if attr_match:
            return attr_match.group(1)
        else:
            return ""
    def find_header(self,data,attr):
        reg = (attr+':(.*)')
        attr_match = re.search(reg, data)
        if attr_match:
            return attr_match.group(1)
        else:
            return ""

    def read_data(self):
        file_path = 'files.json'
        data = ''
        if os.path.exists(file_path):
            with open(file_path, 'r') as json_file:
                json_data = json.load(json_file)
                for entry in json_data:
                    data += entry['modname'] + '\n'
                    data += entry['file_id'] + '\n'
        return data

    def store_data(self, data):
        file_path = 'files.json'
        if os.path.exists(file_path):
            with open(file_path, 'r') as json_file:
                existing_data = json.load(json_file)
        else:
            existing_data = []

        existing_data.append(data)

        with open(file_path, 'w') as json_file:
            json.dump(existing_data, json_file, indent=2)

    def clear_modules(self):
        file_path = 'files.json'
        folder_path = 'modules/'
        if os.path.exists(file_path):
            data = []
            with open(file_path, 'w') as json_file:
                json.dump(data, json_file, indent=2)
        for filename in os.listdir(folder_path):
            file_path = os.path.join(folder_path, filename)
            if os.path.isfile(file_path):
                os.remove(file_path)

    def string_generator(self,size=32):
        characters = string.ascii_letters + string.digits  # Puedes ajustar los conjuntos de caracteres según tus necesidades
        random_string = ''.join(random.choice(characters) for _ in range(size))
        return random_string

                
def run(server_class=HTTPServer, handler_class=FileServerHandler, port=8000):
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    print(f"Servidor iniciado en el puerto {port}")
    httpd.serve_forever()
    

if __name__ == '__main__':
    run()
