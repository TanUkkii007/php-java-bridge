# PHP Java Bridge

php-java-bridge enables to call PHP from Java.


## Build setup

```
git clone git@github.com:TanUkkii007/php-java-bridge.git
cd php-java-bridge
git clone git@github.com:php/php-src.git
cd php-src
git checkout php-5.6.30
cd ..
./build.sh
./build-bridge.sh
```


## Run tests

```
sbt test
```

## Known issues

### Invoke JNI function hangs

Running tests involving PHP call via JNI result in hanging up.