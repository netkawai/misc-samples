import urllib.request, urllib.parse, urllib.error
import json
import ssl

# Ignore SSL certificate errors
ctx = ssl.create_default_context()
ctx.check_hostname = False
ctx.verify_mode = ssl.CERT_NONE


url = 'http://py4e-data.dr-chuck.net/comments_1531525.json'
print('Retrieving', url)
uh = urllib.request.urlopen(url, context=ctx)

data = uh.read()
date = data.decode()
info = json.loads(data)
print('count:', len(info))
total = 0
for item in info['comments']:
    total += int(item['count'])
print(total)