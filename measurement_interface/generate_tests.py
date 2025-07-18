from random import random as rng, shuffle
import math

##

def outer_product(x, y):
	return [[xi * yj for yj in y] for xi in x]

def matrix_multiply(A, B):
	n = len(A)
	m = len(B[0])
	p = len(B)
	return [[sum(A[i][k] * B[k][j] for k in range(p)) for j in range(m)] for i in range(n)]

def transpose(A):
	return [list(row) for row in zip(*A)]

def vector_dot(x, y):
	return sum(xi * yi for xi, yi in zip(x, y))

def matrix_vector_multiply(A, x):
	return [sum(aij * xj for aij, xj in zip(row, x)) for row in A]

def is_cop_prob(M, epsi=1e-8):
	n = len(M)
	for _ in range(10000):
		x = [random.random() for _ in range(n)]
		xTx = vector_dot(x, x)
		if xTx < epsi:
			continue
		xMx = vector_dot(x, matrix_vector_multiply(M, x))
		if xMx < -epsi:
			return False
	return True

##

def normal_random(mean, std_dev):
  	u1 = rng()
  	u2 = rng()
  	z0 = math.sqrt(-2.0 * math.log(u1)) * math.cos(2.0 * math.pi * u2)
  	return mean + z0 * std_dev

def gen_zero_matr(n):
	return [[0 for i in range(n)] for j in range(n)]

def matr_to_str(m):
	n = len(m)
	ans = str(n) + '\n'
	for i in range(n):
		for j in range(n):
			ans += str(m[i][j]) + " "
		ans += '\n'
	return ans

def shuffle_adjacency_matrix(adj_m):
    n = len(adj_m)
    
    permutation = list(range(n))
    shuffle(permutation)
    
    shuffled_matrix = [
        [
            adj_m[permutation[i]][permutation[j]]
            for j in range(n)
        ]
        for i in range(n)
    ]
    
    return shuffled_matrix

##

def gaussian_random_matrix(n, m, d):
	matr = gen_zero_matr(n)
	for i in range(n):
		for j in range(i, n):
			r = normal_random(m, d)
			matr[i][j] = r
			matr[j][i] = r

	return matr_to_str(matr)

# Extremely bugged
def border_cop_matrix(n):
	x = [rng() for _ in range(n)]
	norm_x = math.sqrt(vector_dot(x, x))
	x = [xi / norm_x for xi in x]

	A = [[normal_random(0, 1) for _ in range(n)] for _ in range(n)]
	A_T = transpose(A)
	P = matrix_multiply(A_T, A)

	xPx = vector_dot(x, matrix_vector_multiply(P, x))

	xxT = outer_product(x, x)
	xTx = vector_dot(x, x)
	scale = xPx / (xTx ** 2)
	N = [[scale * xxT[i][j] for j in range(n)] for i in range(n)]

	M = [[P[i][j] - N[i][j] for j in range(n)] for i in range(n)]

	xMx = vector_dot(x, matrix_vector_multiply(M, x))
	assert abs(xMx) < 1e-8, "not on the border"

	return matr_to_str(M)

def complete_graph(n):
	return matr_to_str([[1 for i in range(n)] for j in range(n)])

def random_edge(n, p):
	p = min(1, max(0, p))

	matr = gen_zero_matr(n)

	for i in range(n):
		for j in range(i + 1, n):
			if rng() > 1 - p:
				matr[i][j], matr[j][i] = 1, 1

	return matr_to_str(shuffle_adjacency_matrix(matr))

def k_regular_graph(n, k):
	if (n * k) % 2 != 0:
		raise ValueError("n*k must be even")
	
	matr = gen_zero_matr(n)

	if k == 0:
		pass
	elif k == 1:
		for i in range(0, n, 2):
			matr[i, i+1] = 1
			matr[i+1, i] = 1
	elif k == 2:
		for i in range(n):
			matr[i][(i+1) % n] = 1
			matr[i][(i-1) % n] = 1
	elif k % 2 == 0:
		for i in range(n):
			for j in range(1, k//2 + 1):
				matr[i][(i+j) % n] = 1
				matr[i][(i-j) % n] = 1
	else:
		for i in range(n):
			for j in range(1, (k-1)//2 + 1):
				matr[i][(i+j) % n] = 1
				matr[i][(i-j) % n] = 1
		for i in range(n//2):
			matr[i][i + n//2] = 1
			matr[i + n//2][i] = 1

	return matr_to_str(shuffle_adjacency_matrix(matr))

##
##
##

def decide_mode(m):
	if m == "gauss":
		n = int(input("size: "))
		m = float(input("mean: "))
		d = float(input("distr: "))
		return [gaussian_random_matrix, 0, n, m, d]
	elif m == "border":
		n = int(input("size: "))
		return [border_cop_matrix, 0, n]
	elif m == "compl":
		n = int(input("vert: "))
		return [complete_graph, 1, n]
	elif m == "p_edge":
		n = int(input("vert: "))
		p = float(input("prob: "))
		return [random_edge, 1, n, p]
	elif m == "kreg":
		n = int(input("vert: "))
		k = int(input("deg: "))
		return [k_regular_graph, 1, n, k]


t = max(1, int(input("t >= 1: ")))
csut = max(1, min(3, int(input("csut <= 3: "))))
rv = max(0, int(input("rv >= 0: ")))
cs, cisp, cisppn, csisp = map(int, input("cs, cisp, cisppn, csisp:\n").split())

t_mode = decide_mode(input("mode: "))
cmt = input("comment: ")

filename = ""
config = [t_mode[0].__name__, t_mode[1], t, csut, rv, cs, cisp, cisppn, csisp]
for i in range(len(config) - 1):
	filename += str(config[i]) + "_"
filename += str(config[-1])
filename = "./input/" + filename

t_file = open(filename, "w")
print(filename)

t_file.writelines("comment: " + cmt + '\n')
t_file.writelines(list(map(lambda x: str(x) + '\n', config[1:])))
for i in range(t):
	t_file.write(t_mode[0](*t_mode[2:]))
