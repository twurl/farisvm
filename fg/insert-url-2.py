# apt-get install python-pymongo
#!/usr/bin/env python
import time

#from pymongo import Connection
from pymongo import MongoClient
import sys

connect = MongoClient("127.0.0.1", 27017)
db = connect["farisvm"]
callers = db["apkurl"]

argvs = sys.argv  
argc = len(argvs) 

f = open(argvs[1])

line = f.readline() 

while line:

    try:
        tmp = line.decode("utf8").split("apk:")
        #print tmp[1].rstrip()
        tmpstr = tmp[1].decode("utf8")
        print tmpstr
        
        callers.insert( { 'url' : tmpstr, 'referer' : 'http://referer.com/' } )    
        
    except:
        pass
        

    line = f.readline() 


