from collections import deque
from clasesflycombi import Pila, Cola
import operator

#----------------------------------------------------------------------------------------
tipos_camino = ["barato", "rapido"]
def camino_mas(grafo, tipo, desde, hasta, ciudades):

        if desde not in ciudades or hasta not in ciudades: return False

        aeps_desde = ciudades[desde]
        aeps_hasta = ciudades[hasta]

        if tipo not in tipos_camino: return False

        camino_minimo = []

        for aep_desde in aeps_desde:
            for aep_hasta in aeps_hasta:
                camino = dijkstra(grafo, tipo, aep_desde, aep_hasta)
                if len(camino) < len(camino_minimo) or len(camino_minimo) == 0:
                    camino_minimo = camino

        print(' -> '.join(camino_minimo))

        return True
#----------------------------------------------------------------------------------------
def ordenar_vertices(dist):
    pepe = dist.copy()
    dist_ordenado = sorted(pepe.items(),key = operator.itemgetter(1),reverse = True)
    return dist_ordenado

def centralidad(grafo, k):

    importantes = _centralidad(grafo, k)
    print(', '.join(importantes))

    return True

def _centralidad(grafo, k):

    tope = int(k)
    cent = {} # Inicializo la centralidad
    for v in grafo: cent[v] = 0

    for v in grafo:
        padre, dist = dijkstra(grafo,"rapido", v, None)
        cent_aux = {}
        for w in grafo: cent_aux[w] = 0
        vertices_ordenados = ordenar_vertices(dist)
        for (u,v) in vertices_ordenados:
            if not padre[u]: continue
            cent_aux[padre[u]] += 1 + cent_aux[u]
        for w in grafo:
            if w == v: continue
            cent[w] += cent_aux[w]

    k_centrales = ordenar_vertices(cent)
    lista_centrales = []
    cont = 0
    for v,w in k_centrales:
        cont += 1
        lista_centrales.append(v)
        if cont == tope: break

    return lista_centrales

#----------------------------------------------------------------------------------------
def dijkstra(grafo, tipo, origen, destino):

    if origen not in grafo.vertices():
        return []

    # Mi infinito
    inf = float('inf')

    distancias = {vertice: inf for vertice in grafo.vertices() } # Guardo las distancias
    camino = { vertice: None for vertice in grafo.vertices() } # Guardo el camino que voy creando
    distancias[origen] = 0
    vertices = grafo.vertices().copy()
    padre = {}
    padre[origen] = None

    while vertices:
        # Obtiene el minimo, es como si fuera un heap de minimos
        vertice_actual = min(
            vertices, key=lambda vertice: distancias[vertice])

        vertices.remove(vertice_actual)

        if distancias[vertice_actual] == inf:
            break
        for adyacente in grafo.adyacentes(vertice_actual):
            peso_arista = grafo.get_peso(vertice_actual, adyacente)
            peso = int(peso_arista.precio) if tipo == tipos_camino[0] else int(peso_arista.tiempo_promedio)
            ruta_alternativa = distancias[vertice_actual] + peso
            if ruta_alternativa < distancias[adyacente]:
                distancias[adyacente] = ruta_alternativa
                camino[adyacente] = vertice_actual
                padre[adyacente] = vertice_actual

    cola = deque()

    if destino:
        actual = destino
        while camino[actual] is not None:
            cola.appendleft(actual)
            actual = camino[actual]
        if cola:
            cola.appendleft(actual)
    else:
        return padre, distancias

    return list(cola)

#-------------------------------------------------------------------------------------------
def reconstruir_camino(origen, destino, padre):
	'''Dado un origen, destino y el diccionario de padres del grafo, devuelve una lista
	con los vertices que componen el camino en orden.'''
	camino = []
	actual = destino

	while actual:
		camino.insert(0, actual)
		actual = padre[actual]  # Cuando itere el origen el padre es None

	return camino

#--------------------------------------------------------------------------------------------
def _camino_minimo_escalas(grafo, origen, destino):
	''' Devuelve una lista con los aeropuertos de camino minimo entre
	origen y destino. Si no existe el destino o es None, devuelve la tupla (padre, dist).'''
	dist = {}
	padre = {}
	visitados = set()

	padre[origen] = None
	dist[origen] = 0
	visitados.add(origen)
	cola = Cola()
	cola.encolar(origen)

	while not cola.esta_vacia():
		v = cola.desencolar()
		if v == destino: return reconstruir_camino(origen, destino, padre)
		for w in grafo.adyacentes(v):

			if w not in visitados:
				visitados.add(w)
				dist[w] = dist[v] + 1
				padre[w] = v
				cola.encolar(w)

	return padre, dist

#-----------------------------------------------------------------------------------------
def camino_minimo_escalas(grafo, ciudad_origen, ciudad_destino, ciudades):
	'''Dado una ciudad de origen y otra de destino, devuelve una lista de aeropuertos de camino minimo.'''
	aips_origen = ciudades[ciudad_origen]
	aips_destino = ciudades[ciudad_destino]

	camino_resul = []

	for origen in aips_origen:
		for destino in aips_destino:

			camino = _camino_minimo_escalas(grafo, origen, destino)
			if len(camino) < len(camino_resul) or len(camino_resul) == 0:
				camino_resul = camino

	return camino_resul
