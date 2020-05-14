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
            median = statistics.median(ratingInfo["ratings"])
            print ("%s\tMedian: %s" % (current_genre, median))
        current_genre = genre
        try:
            current_rating_count = ratingInfo["total_count"]
        except ValueError:
            continue

if current_genre == genre:
    median = statistics.median(ratingInfo["ratings"])
    print ("%s\tMedian: %s" % (current_genre, median))