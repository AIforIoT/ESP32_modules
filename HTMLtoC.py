#PYTHON: HTML to C string converter
#Sergi Mercade Laborda

with open('AP_Login.html') as fp:
    lines = fp.read().splitlines()
    for line in lines:
       print("\""+line.replace("\"", "\\\"")+"\\n\"")
