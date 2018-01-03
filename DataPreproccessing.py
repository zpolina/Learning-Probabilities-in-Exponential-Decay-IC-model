
# coding: utf-8

# In[21]:

file = open("Retweet_Content.txt", "r") 
lines = file.readlines()


# In[32]:

print lines[5]


# In[ ]:

data_tweets = []
data_retweets = []
i = 0
while i < len(lines):
    if ("2011-" in lines[i]  or "2012-" in lines[i]) and (":" in lines[i]) and (lines[i].count('\t') == 3):
        original_row = lines[i].split()
        orig_id = int(original_row[0])
        data_tweets.append(original_row)
        i = i+1
        j = int(lines[i].split()[0])
        i = i+1
        while j > 0:
            if ("2011-" in lines[i]  or "2012-" in lines[i]) and (":" in lines[i]) and (lines[i].count('\t')) == 2:
                row = lines[i].split()
                row.append(orig_id)
                data_retweets.append(row)
                j = j-1
            i = i+1   
    else:
         i=i+1       


# In[3]:

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


# In[4]:

retweets.user_id = retweets.user_id.astype(int)
retweets.tweet_id = retweets.tweet_id.astype(int)
tweets.user_id = tweets.user_id.astype(int)
tweets.tweet_id = tweets.tweet_id.astype(int)


# In[6]:

def just_h(x):
    return x.split(":", 1)[0]


# In[7]:

retweets.time = retweets.time.apply(just_h)


# In[8]:

tweets.time = tweets.time.apply(just_h)


# In[9]:

retweets.time = pd.to_datetime(retweets.time, format='%Y-%m-%d-%H')


# In[10]:

tweets.time = pd.to_datetime(tweets.time, format='%Y-%m-%d-%H')


# In[11]:

file_2 = open("weibo_network.txt", "r") 
lines_2 = file_2.readlines()


# In[12]:

graph = {}
for i in range(1787443):
    graph[i] = []
i = 1
while i < 1787443:
    adj = lines_2[i].split()
    l = int(adj[1])
    j = 2
    while (j < (2*l + 2)):
        graph[int(adj[j])].append(int(adj[0]))
        j = j + 2
    i = i + 1


# In[13]:

mapping = open("uidlist.txt", "r") 
map_id = mapping.readlines()


# In[14]:

map_list = []
for line in map_id:
    map_list.append(int(line.split()[0]))


# In[15]:

map_list_reverse = {}
j = 0
for i in map_list:
    map_list_reverse[i] = j
    j = j + 1


# In[16]:

tweets["num"] = data_tweets.num


# In[17]:

tweets.num = tweets.num.astype(int)


# In[18]:

tweets = tweets.sort_values(by = ["num"], ascending=False)


# In[ ]:

small_tweets = tweets[:10]


# small_tweets = small_tweets[small_tweets['user_id'].isin(new_unique_users_small)]

# In[ ]:

small_tweets_ids = small_tweets.tweet_id.unique()


# In[ ]:

small_retweets = retweets[retweets["tweet_id"].isin(small_tweets_ids)]


# In[ ]:

unique_users_small = list(small_tweets.user_id.unique()) + list(small_retweets.user_id.unique())


# In[ ]:

len(unique_users_small)


# In[ ]:

u_u_small_maped = []
new_unique_users_small= []
for i in unique_users_small:
    try:
        u_u_small_maped.append(map_list_reverse[i])
        new_unique_users_small.append(i)
    except:
        pass


# In[ ]:

len(new_unique_users_small)


# In[ ]:

small_tweets = small_tweets[small_tweets['user_id'].isin(new_unique_users_small)]
small_retweets = small_retweets[small_retweets['user_id'].isin(new_unique_users_small)]


# In[ ]:

small_retweets.to_csv("small_retweets.csv", sep='\t')
small_tweets.to_csv("small_tweets.csv", sep='\t')


# In[ ]:

small_graph = {}
for user in u_u_small_maped:
    small_graph[user] = graph[user]


# In[ ]:

small_graph_upd = {}
set_unique_users = set(u_u_small_maped)
for key in small_graph:
    small_graph_upd[key] = list(set(small_graph[key]) & set_unique_users)


# In[ ]:

maped_graph = {}
for key in small_graph_upd:
    maped_graph[map_list[key]] = []
    for i in small_graph_upd[key]:
         maped_graph[map_list[key]].append(map_list[i])


# In[ ]:

import csv
w = csv.writer(open("graph.csv", "w"))
for key, val in maped_graph.items():
    w.writerow([key, val])


# In[ ]:

tweets_ids = small_tweets.tweet_id


# In[ ]:

small_tweets


# In[ ]:

tweets_collection = {} 
for tweet in tweets_ids:
    tweets_collection[tweet] = small_retweets[small_retweets.tweet_id == tweet]


# In[ ]:

for key in tweets_collection:
        filename = "twitter_data/"+str(key)+".csv"
        tweets_collection[key].to_csv(filename, sep='\t')


# In[ ]:



