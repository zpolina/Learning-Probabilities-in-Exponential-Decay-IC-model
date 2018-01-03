
# coding: utf-8

# In[217]:

import pandas as pd


# #### Reading graph from csv file. Storing it as a dictionary, i.e. adjacent list

# In[218]:

import sys
import csv

csv.field_size_limit(sys.maxsize)
graph = {}
for key, val in csv.reader(open("graph.csv")):
    if val== "[]":
        graph[int(key)] = [] 
    else:
        graph[int(key)] = [int(n) for n in val[1:-1].split(',')]


# In[219]:

user_id_1 = []
user_id_2 = []


# In[220]:

for key in graph:
    for i in graph[key]:
        user_id_1.append(int(key))
        user_id_2.append(int(i))


# In[221]:

graph_frame = pd.DataFrame(
    {'user_id_1': user_id_1,
     'user_id_2': user_id_2
    })


# In[222]:

users_children = graph_frame.user_id_2.unique()


# #### Making graph inverted to be able to see all the parents of the node

# In[431]:

graph_inverted = {}
for i in users_children:
    graph_inverted[i] =     graph_frame.user_id_1[graph_frame.user_id_2 == i].tolist()


# In[224]:

retwitts = pd.read_csv("small_retweets.csv", sep='\t',  index_col=0)
twitts = pd.read_csv("small_tweets.csv", sep='\t',  index_col=0)


# In[225]:

retwitts.time = pd.to_datetime(retwitts.time)


# In[226]:

twitts.time = pd.to_datetime(twitts.time)


# In[227]:

len(retwitts.user_id.unique())


# #### Reading the list of frames where each frame is a log for particular twitt

# In[228]:

import glob
path ='twitter_data' # use your path
allFiles = glob.glob(path + "/*.csv")
list_of_frames = {}
SHIFT = 1000000000.*60.*60.
def time_conv(x):
    return x.value/SHIFT
for file_ in allFiles:
    df = pd.read_csv(file_,index_col=0, sep= '\t')
    df.time = pd.to_datetime(df.time)
    df.time = df.time.apply(time_conv)
    list_of_frames[int((file_.split('/')[1]).split('.')[0])] = df


# In[229]:

twitts.time = twitts.time.apply(time_conv)


# In[230]:

retwitts.time = retwitts.time.apply(time_conv)


# #### For each frame calculating the maximum time 

# In[231]:

max_times = {}


# In[232]:

from datetime import timedelta, datetime, date
for key in list_of_frames:
    max_times[key] = list_of_frames[key].time.max()


# In[234]:

for key in list_of_frames:
    list_of_frames[key] = list_of_frames[key].reset_index(drop=True)


# In[235]:

retwitts = retwitts.reset_index(drop=True)
twitts = twitts.reset_index(drop=True)


# In[236]:

twitts.head()


# In[237]:

del twitts["num"]


# In[238]:

twitts


# #### Mearging together the seed set entities and logs. Just because we need to have seed set log in the whole set of logs

# In[239]:

#for index, row in twitts.iterrows():
#   print row
#  list_of_frames[row["tweet_id"]].loc[len(list_of_frames[row["tweet_id"]])] = row


# #### For each node in each set calculating the time of activation. Making a dictionary

# In[288]:

activ_times = {}
for key in list_of_frames:
    activ_times[key] = {}
    u_users = list_of_frames[key].user_id.unique() 
    for u in u_users:
        activ_times[key][u] =             list_of_frames[key].time[list_of_frames[key].user_id == u].min()
    seed_user =         twitts.user_id[twitts.tweet_id == key].tolist()[0]
    activ_times[key][seed_user] =        twitts.time[twitts.tweet_id == key].tolist()[0]


# In[289]:

def activ_time(S, id):
    try:
        return activ_times[S][id]
    except:
        return 0


# In[400]:

## constants declaration
DELTA = 24
ETA = 5


# $$ p_{vw} \textrm{  inistialization} $$
# $$ \textrm{Function for  }  P_{w}^{(s)} \textrm{ computation } $$

# In[391]:

len(graph_frame)
import numpy as np
probs = np.random.random(len(graph_frame))
grad = np.zeros(len(graph_frame))


# In[352]:

edge_index = {}
subset = graph_frame[["user_id_1", "user_id_2"]]
edges = [tuple(x) for x in subset.values]


# In[353]:

j = 0
for i in edges:
    edge_index[i] = j
    j = j + 1


# In[354]:

def P(w, S):
    res = 1
    w_act = activ_time(S, w)
    if w in graph_inverted:
        for tau in range(0, DELTA):
            for v in graph_inverted[w]:
                if (activ_time(S, v) == w_act - 1 - tau):
                    res= res * (1 - probs[edge_index[(v,w)]]*np.exp(-tau/ETA))
    return 1-res


# In[409]:

P( 120671, 5615734448357004557 )


# In[410]:

for key in list_of_frames:
    print list_of_frames[key].head()


# ### Computation of

# $$
# 	\frac{\partial P_{w}^{(s)}}{\partial p_{vw}}
# 	= 
# 	\left({\displaystyle \prod_{\tau = 0}^{\Delta-1}\prod_{\substack{v' \in B(w) \cap D(t_w-\tau-1)\\v' \neq v}} (1-p_{v'w}e^{-\tau/\eta})}\right) e^{-(t_w-t_v-1)/\eta}
# $$

# In[411]:

def partial(S,v,w):
    w_act = activ_time(S, w)
    res = 1
    if w in graph_inverted:
        for tau in range(0,DELTA):
            for v_prime in graph_inverted[w]:
                if (activ_time(S, v_prime) == w_act - 1 - tau) and (v_prime != v):
                    res = res * (1 - probs[edge_index[(v_prime,w)]]*np.exp(-tau/ETA))           
    res = res*np.exp(-(w_act - activ_time(S, v) -1)/ETA)
    return res


# In[412]:

partial(3445888074596137,1616700401,1066926080)


# ### Computation of

# $$
# 	Z_{vw}
# 	=
# 	-\sum_{\tau = 0}^{t_w-t_v-2}\frac{1}{1 - p_{vw}e^{-\tau/\eta}} e^{-\tau/\eta}
# $$

# In[413]:

def Z(S, v, w):
    res = 0
    w_act = activ_time(S, w)
    v_act = activ_time(S, v)
    for tau in range(0, int(w_act - v_act - 1)):
        e = np.exp(-tau/ETA)
        res = res + e/(1 -  probs[edge_index[(v,w)]]*e)
    return res


# In[428]:

Z(3445888074596137,1616700401,1066926080)


# ### Computation of gradient component

# $$
# 	\frac{\partial L}{\partial p_{vw}} = \sum_{s=1}^{S}
# 	\left\{
# 	\begin{matrix}
# 		{\displaystyle \left[\frac{1}{P_w^{(s)}} \frac{\partial P_{w}^{(s)}}{\partial p_{vw}} + Z_{vw}\right]}
# 			&
# 			\text{if } t_{v}^{(s)} \in [t_{w}^{(s)}-\Delta-1, t_{w}^{(s)}-1] \text{ \& } v \in B(w)
# 			\\[2em]
# 			0 & \text{otherwise}
# 	\end{matrix}
# 	\right.
# $$

# In[398]:

S_set = twitts.tweet_id.tolist()


# In[401]:

users_per_set = {}
for key in list_of_frames:
    users_per_set[key] = list_of_frames[key].user_id.unique().tolist()


# In[417]:

def grad_comp(v,w):
    res = 0
    for S in S_set:
        w_act = activ_time(S, w)
        v_act = activ_time(S, v)
        if (w in graph_inverted) and (w in users_per_set[S]):
            if (v_act > w_act-DELTA-1) and (v_act < w_act-1):
                if v in graph_inverted[w]:
                    p = P(w,S)
                    ### this to be deleted,but for now we have values of P equals to zero
                    if p == 0:
                        p = 1
                    ###
                    res = res + 1/p * partial(S,v,w) + Z(S,v,w)
    return res
            


# In[418]:

grad_comp(1616700401,1066926080)


# ### Gradient Computation

# In[424]:

alpha = 0.001


# In[425]:

for i in edge_index:
    grad[edge_index[i]] = grad[edge_index[i]] + alpha*grad_comp(i[0], i[1])


# In[ ]:



