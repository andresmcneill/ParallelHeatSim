#!/usr/bin/env python
import sys
import csv
import json

# for nonHDFS run
movieFile = "./movielens/movies.csv"

# for HDFS run
#movieFile = "./movies.csv"

movieList = {}
userList = []

with open(movieFile, mode = 'r') as infile:
    reader = csv.reader(infile)
    for row in reader:
        movieList[row[0]] = {}
        movieList[row[0]]["title"] = row[1]
        movieList[row[0]]["genre"] = row[2]

for oneUser in sys.stdin:
    oneUser = oneUser.strip()
    ratingInfo = oneUser.split(",")

    if ratingInfo[0] == "userId":
        continue

    try:
        thisUser = {}
        genreList = movieList[ratingInfo[1]]["genre"].split('|')
        thisUser[ratingInfo[0]] = genreList
        userList.append(thisUser)
    except ValueError:
        continue
        
for user in userList:
    print ("%s" % json.dumps(user))