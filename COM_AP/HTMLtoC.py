#PYTHON: HTML document to C string converter
#Sergi Mercade Laborda
#In Linux: python HTMLtoC.py htmlcode.html > cSring.txt
import sys

html_document=sys.argv[1]
with open(html_document) as doc:
    lines = doc.read().splitlines()
    for line in lines:
       print("\""+line.replace("\"", "\\\"")+"\\n\"")
