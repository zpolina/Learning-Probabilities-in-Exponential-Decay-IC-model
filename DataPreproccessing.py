
# coding: utf-8

# In[1]:

file = open("Retweet_Content.txt", "r") 
lines = file.readlines()


# In[2]:

data_tweets = []
data_retweets = []
i = 0
while i < len(lines):
    if ("2011-" or "2012-") and (":" in lines[i]) and (lines[i].count('\t') == 3):
        original_row = lines[i].split()
        orig_id = int(original_row[0])
        data_tweets.append(original_row)
        i = i+1
        j = int(lines[i].split()[0])
        i = i+1
        while j > 0:
            if ("2011-" or "2012-") and (":" in lines[i]) and (lines[i].count('\t')) == 2:
                row = lines[i].split()
                row.append(orig_id)
                data_retweets.append(row)
                j = j-1
            i = i+1   
    else:
         i=i+1       


# In[4]:

import pandas as pd
data_retweets = pd.DataFrame(data_retweets)
data_retweets = data_retweets.rename(columns={0:"user_id", 1:"time", 2:"retweet_id" ,3:"orig_id"})
data_tweets = pd.DataFrame(data_tweets)
data_tweets = data_tweets.rename(columns={0:"orig_id", 1:"user_id", 2:"time" ,3:"num"})
retweets = pd.DataFrame()
retweets["user_id"] = data_retweets.user_id
retweets["tweet_id"] = data_retweets.orig_id
retweets["time"] = data_retweets.time

tweets = pd.DataFrame()
tweets["user_id"] = data_tweets.user_id
tweets["tweet_id"] = data_tweets.orig_id
tweets["time"] = data_tweets.time


# In[5]:

retweets.user_id = retweets.user_id.astype(int)
retweets.tweet_id = retweets.user_id.astype(int)
tweets.user_id = tweets.user_id.astype(int)
tweets.tweet_id = tweets.tweet_id.astype(int)


# In[7]:

retweets["tweet_id"] = data_retweets.orig_id


# In[ ]:

retweets.time = pd.to_datetime(retweets.time)


# In[ ]:

tweets.time = pd.to_datetime(tweets.time)


# In[ ]:

all_tweets = tweets.tweet_id


# In[ ]:

tweets_collection = {} 
for tweet in all_tweets:
    tweets_collection[tweet] = retweets[retweets.tweet_id == tweet]


# In[ ]:



