# emcc --no-entry subtest1.cpp -o subtest1.js -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']"
# emcc --no-entry subtest2.cpp -o subtest2.js -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']"
# emcc --no-entry subtest1.cpp -o subtest1.wasm -s
# emcc --no-entry subtest2.cpp -o subtest2.js -s NO_EXIT_RUNTIME=1 -s EXPORTED_FUNCTIONS="['_runSubprogram']" 
#emcc main.cpp -o main.html -s EXPORTED_FUNCTIONS="['_main','_runSubprogram']"
# emcc main.cpp -o main.html -s MAIN_MODULE=1 -s


# em++ module1.cpp --no-entry -o module1.wasm -s EXPORTED_FUNCTIONS="['_module1Function','_genDungeon']"
# em++ module2.cpp --no-entry -o module2.wasm -s EXPORTED_FUNCTIONS="['_module2Function','_genDungeon']"
# em++ main.cpp -o main.js -s EXPORTED_FUNCTIONS="['_main','_moduleCompleteCallback']" -s ASYNCIFY -s EXPORTED_RUNTIME_METHODS="[ccall]"


em++ module1.cpp --no-entry -o module1.wasm -s SIDE_MODULE=1
em++ main.cpp -o main.js -s ASYNCIFY -s EXPORTED_RUNTIME_METHODS="[ccall]" -s MAIN_MODULE=1