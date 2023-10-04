import time



class SubstrRank:
    def __init__(self, left_rank=0, right_rank=0, index=0):
        self.left_rank = left_rank
        self.right_rank = right_rank
        self.index = index

def make_ranks(substr_rank, n):
    r = 1
    rank = [-1] * n
    rank[substr_rank[0].index] = r
    for i in range(n):
        if (substr_rank[i].left_rank != substr_rank[i-1].left_rank or
			substr_rank[i].right_rank != substr_rank[i-1].right_rank):
            r += 1
        rank[substr_rank[i].index] = r
    return rank


def suffix_array(T):
    
    n = len(T)
    substr_rank = []
    for i in range(n):
        substr_rank.append(SubstrRank(ord(T[i]), ord(T[i + 1]) if i < n-1 else 0, i))
    substr_rank.sort(key=lambda sr: (sr.left_rank, sr.right_rank))
    l = 2
    while l < n:
        rank = make_ranks(substr_rank, n)
        for i in range(n):
            substr_rank[i].left_rank = rank[i]
            substr_rank[i].right_rank = rank[i+l] if i+l < n else 0
            substr_rank[i].index = i
        l *= 2

        substr_rank.sort(key=lambda sr: (sr.left_rank, sr.right_rank))

    SA = [substr_rank[i].index for i in range(n)]
    return SA


def search_substr(patron, texto, suffrray, N):
    L = 0  # low
    R = N - 1  # high
    
    while L <= R:
        M = (R + L) // 2  # calcular el valor medio correctamente
        subTexto = texto[suffrray[M]: suffrray[M] + len(patron)]
        if patron == subTexto:
            return suffrray[M]
        elif subTexto < patron:
            L = M + 1
        else:
            R = M - 1 
    
    return -1


filename = "Shakespeare.txt"
with open(filename, encoding="utf8") as f:
    libro = f.read()



# Guarda el tiempo de inicio
inicio = time.time()


#ejemplo = "banana"
SA = suffix_array(libro)
#print(SA)

# Guarda el tiempo de finalización
fin = time.time()

# Calcula el tiempo de ejecución en segundos
tiempo_ejecucion = fin - inicio

print("Tiempo de ejecucion: ",round(tiempo_ejecucion,2), " segundos")
#search("nana",ejemplo, SA, len(SA))
'''
pat = "Bram"
index = search_substr(pat,libro, SA, len(SA))
print("El index es: ",index)
print(libro[index:index+len(pat)])
'''
