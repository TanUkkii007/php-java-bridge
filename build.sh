PHP_SOURCE=php-src

cd $PHP_SOURCE
./buildconf --force
./configure --enable-embed=static --disable-cli --disable-cgi
make