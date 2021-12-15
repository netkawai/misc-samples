#! /usr/bin/env python2
import os
import sys

print __name__
print __file__

''' plugin folder must have one autoload.php file '''
def is_package(path):
    for name in os.listdir(path):
        if name  in ('autoload.php'):
            return False
    return True

def directory_walk(basepath,ext,packages=None):
    for dirpath, dirnames, filenames in os.walk(basepath):
        for dirname in list(dirnames):
            print dirname

            if dirname[0] == '.':
                dirnames.remove(dirname)
                continue

            if is_package(dirpath) and filenames:
                pkg_name = dirpath[len(basepath) + len(os.sep):].replace(',','.')
                if packages and pkg_name not in packages:
                    continue
                ''' the wordpress sources have wp- prefix in assumption'''
                filenames = filter(lambda x:x.startswith('wp-') and x.endswith(ext),filenames)

                for name in filenames:
                        yield name

if __name__ == '__main__':
  this_dir_path = os.path.abspath(os.path.dirname(__file__))
  for fname in directory_walk(this_dir_path,'.php'):
    print fname
