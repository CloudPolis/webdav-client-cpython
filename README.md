PyWDC
===

```ShellSession
$ export PYTHON_INCLUDE_DIR=$(python -c "
from distutils.sysconfig import get_python_inc; 
print(get_python_inc())
")
$ export PYTHON_LIB_DIR=$(python -c "
import distutils.sysconfig as sysconfig; 
print(sysconfig.get_config_var('LIBDIR'))
")
# need to change on your version
$ export PYTHON_LIBRARY=${PYTHON_LIB_DIR}/libpython2.7.dylib
$ cmake -H. -B_builds -DCMAKE_INSTALL_PREFIX=_install \
-DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR} -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
$ cmake --build _builds --target install
$ cd _install/bin
$ python 
>>> options = dict()
>>> options['webdav_hostname'] = "your_webdav_hostname"
>>> options['webdav_username'] = "your_webdav_username"
>>> options['webdav_password'] = "your_webdav_password"
>>> client = pywdc.Client(options)
>>> client.check()
True
>>> client.list("/")
["file.dat"]
>>> client.info("file.dat")
{'size': '19', 'type': '', 'modified': 'Tue, 08 Aug 2017 18:33:38 GMT', 'name': 'file.dat', 'created': '2017-08-08T18:33:38Z'}
>>> client.mkdir("backup/")
True
>>> client.copy("file.dat", "backup/file.dat")
>>> client.clean("file.dat")
True
>>> client.check("file.dat")
False
>>> client.move("file.dat", "backup/file.dat")
True
>>> client.download("file.dat", "file.local.dat")
True
>>> client.upload("backup/file.dat", "file.local.dat")
True
```
