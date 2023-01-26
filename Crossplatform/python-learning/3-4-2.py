from urllib.request import urlopen
from bs4 import BeautifulSoup
import ssl

position = 18
targetRecursive = 7
currentRecursive = 0

def retriveHref(url):
    global currentRecursive
    # Ignore SSL certificate errors
    ctx = ssl.create_default_context()
    ctx.check_hostname = False
    ctx.verify_mode = ssl.CERT_NONE


    html = urlopen(url, context=ctx).read()
    soup = BeautifulSoup(html, "html.parser")

    # Retrieve all of the anchor tags
    tags = soup('a')
    next_url = tags[position-1].get('href', None)
    print(next_url)
    currentRecursive = currentRecursive + 1
    if currentRecursive < targetRecursive:
        retriveHref(next_url)


retriveHref('http://py4e-data.dr-chuck.net/known_by_Maddox.html')
