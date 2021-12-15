import os
import re

top_images = dict()

for name in os.listdir("."):
    if re.search(r"_\d+\.png$",name):
        group_name = re.sub(r"_\d+\.png$", "", name)
        if not group_name in top_images:
            top_images[group_name] = []
        top_images[group_name].append(name) 

#print(top_images)
for group_name in top_images:
    if not os.path.exists(group_name):
       os.makedirs(group_name)
       for filename in top_images[group_name]:
           os.rename("./" + filename, "./" + group_name + "/" + filename)

        