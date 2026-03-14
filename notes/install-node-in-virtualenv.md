Install node/npm into virtualenv in python specifically
```bash
{
$ curl https://nodejs.org/dist/node-latest.tar.gz | tar xvz
$ cd node-v*
$ ./configure --prefix=$VIRTUAL_ENV
$ make install
}
```