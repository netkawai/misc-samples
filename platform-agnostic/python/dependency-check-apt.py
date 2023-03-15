import subprocess
import re 

nested_level = 0
packageList = set()

def printDependency(package):
    global packageList
    global nested_level
    #if nested_level > 3:
    #    return
    nested_level += 1
    for i in range(nested_level):
        print('-',end='')    
    print(package)
    sp = subprocess.Popen(['apt-cache', 'depends', package], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    out, err = sp.communicate()
    errstr = err.decode().strip()
    if errstr:
        print(errstr)
        return
    packages = re.findall(r"Depends: (\S+)", out.decode())
#    print(packages)
    for pack in packages:
        if pack in packageList:
            continue
        packageList.add(pack)

        printDependency(pack)
        nested_level -= 1


rootPackage = 'npm'
printDependency(rootPackage)