g++ -c -std=c++11 -Iphp-src -Iphp-src/main -Iphp-src/TSRM -Iphp-src/Zend -Iphp-src/sapi/embed -I${JAVA_HOME}/include -I${JAVA_HOME}/include/darwin native/bridge.cpp
g++ -I/usr/local/include -Lphp-src/libs -lphp5 -lresolv -liconv -liconv -lm -lxml2 -lxml2 -lxml2 -lxml2 -lxml2 -lxml2 bridge.o -dynamiclib -framework JavaVM -o libbridge.dylib

# ToDo copy library to out/native/platform/libbridge.dylib