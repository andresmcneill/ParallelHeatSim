#!/usr/bin/env python
import sys
import csv
import json
import statistics

current_genre = None
current_rating_count = 0

for line in sys.stdin:
    line = line.strip()
    genre, ratingString = line.split("\t", 1)
    ratingInfo = json.loads(ratingString)

    if current_genre == genre:
        try:
            current_rating_count += ratingInfo["total_count"]
        except ValueError:
            continue

    else:
        if current_genre:
            stdev = statistics.stdev(ratingInfo["ratings"])
            print ("%s\tStandard Deviation: %s" % (current_genre, stdev))
        current_genre = genre
        try:
            current_rating_count = ratingInfo["total_count"]
        except ValueError:
            continue

if current_genre == genre:
    stdev = statistics.stdev(ratingInfo["ratings"])
    print ("%s\tStandard Deviation: %s" % (current_genre, stdev))