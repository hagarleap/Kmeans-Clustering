import getopt
import sys

def kmeans(k, input_data, iter=200):

    epsilon = 0.001
    
    f = open(input_data, 'r')
    vectors = []
    vector = f.readline().strip().split(",")
    while vector[0] != "":
        vectors.append([float(x) for x in vector])
        vector = f.readline().strip().split(",")
    f.close()
    
    N= len(vectors)
    
    if k>=N:
        print("Invalid number of clusters!")
        return
    
    clusters = {}
    init(k, clusters, vectors)
    
    iter_count = 0
    deltas = [1 for i in range(k)]
    
    while iter_count<iter and max(deltas)>epsilon:
        clusters_keys = list(clusters.keys())
        for x in vectors:
            argmin = euclidian_distance(x, clusters_keys[0])
            closest_cluster = clusters_keys[0]
            for i in range(1,k):
                dist = euclidian_distance(x, clusters_keys[i])
                if dist<argmin:
                    argmin = dist
                    closest_cluster = clusters_keys[i]
            clusters[closest_cluster][1]+=1
            clusters[closest_cluster][0] = [clusters[closest_cluster][0][i]+x[i] for i in range(len(x))]
        
        update_centroid(deltas, clusters, clusters_keys)  
        iter_count+=1
    
    for centroid in list(clusters.keys()):
        for i in range(len(centroid)-1):
            print(f"{round(centroid[i], 4)}," , end="")
        print(f"{round(centroid[-1],4)}")
     ####fix output to not be tuple but rather text with no parenthesis, and round up to 4 places
     ####check other files       
        
def init(k, clusters, vectors):
    vector_length = len(vectors[0])
    for i in range(k):
        clusters[tuple(vectors[i])] = [[0 for i in range(vector_length)],0]
        
def euclidian_distance(vec1, vec2):
    sum = 0
    for i in range(len(vec1)):
        sum += (vec1[i]-vec2[i])**2
    return sum**(1/2)

def update_centroid(deltas, clusters, clusters_keys):
    i=0
    for old_centroid in clusters_keys:
        new_centroid = []
        avg_divisor = clusters[old_centroid][1]
        for bit in clusters[old_centroid][0]:
            new_centroid.append(bit/avg_divisor)

        deltas[i] = euclidian_distance(old_centroid, new_centroid)
        clusters.pop(old_centroid)
        clusters[tuple(new_centroid)] = [[0 for i in range(len(new_centroid))],0]

argv = sys.argv[1:]
if len(argv)==2:
    try:
        arg0 = int(argv[0])
    except:
        print("Invalid number of clusters!")
        exit()

    if arg0>1:
        kmeans(arg0, argv[1])
    else:
        print("Invalid number of clusters!")
        exit()
elif len(argv)==3:
    try:
        arg0 = int(argv[0])
    except:
        print("Invalid number of clusters!")
        exit()
    
    try:
        arg1 = int(argv[1])
    except:
        print("Invalid maximum iteration!")
        exit()

    if arg0<=1:
        print("Invalid number of clusters!")
        exit()
    if  1<arg1<1000:
        kmeans(arg0, argv[2], arg1)
    else:
        print("Invalid maximum iteration!")
        exit()
else:
    print("An Error Has Occurred")
    exit()
