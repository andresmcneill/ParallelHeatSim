#!/usr/bin/env python
import sys
import csv
import json
import statistics as stat

current_userId = None
rating_list = []
user_id_list = []
genre_list = {}

for line in sys.stdin:
    line = line.strip()
    current_user = json.loads(line)
    rating_list.append(current_user)
    user_id_list.append(list(current_user.keys())[0])

top_user = stat.mode(user_id_list)
rating_count = user_id_list.count(top_user)

for rating in rating_list:
    if list(rating.keys())[0] != top_user:
        continue
    
    else:
        for genre in rating[str(top_user)]:
            if genre in genre_list:
                genre_list[genre] += 1
            else:
                genre_list[genre] = 1
                
#print(list(genre_list.values()))

max_genre_count = max(list(genre_list.values()))

#most_watched = ''

for key, value in genre_list.items():
    if value == max_genre_count:
        most_watched = key

print('User identification')
print(top_user,'-- Total rating counts:',rating_count,'-- Most rated genre:',most_watched,'-',max_genre_count)


#print('User',top_user,'had the most ratings, ', end='')
#print('and their top genre(s) were: ',end='')
#for genre in most_watched:
#    print(genre,end=' ')
#print('')
#for rating in ratingList

#for rating in ratingList:
#    print(int(rating.keys()))
    


    #for userId in current_user.keys():
    #    if (current_userId != userId):
    #        current_userId = userId
    #        rating_counts = {}
    #    else:
    #        for rating in rating_counts:
    #            highest_count = 0
    #            if int(rating_counts.get(rating)) > highest_count:
    #                highest_count = rating
    #        
    #        for genre in rating_counts.keys():
    #            if (rating_counts.get(genre) == highest_count):
    #                most_watched = genre
    #                print("%s" % (most_watched))

    #current_genre_list = current_user.get(current_userId)
    #current_genre_list.sort()

    #for genre in current_genre_list:
    #    if rating_counts.get(genre) != None:
    #        rating_counts[genre] += 1
    #    else:
    #        rating_counts[genre] = 1