from collections import deque

tipos_camino = ["barato", "rapido"]

def listar_operaciones(comandos):
    for comando in comandos:
        print(comando)


def camino_mas(grafo, tipo, desde, hasta, ciudades):

        if desde not in ciudades or hasta not in ciudades: return False

        aeps_desde = ciudades[desde]
        aeps_hasta = ciudades[hasta]

        if tipo not in tipos_camino: return False

        camino_minimo = []

        for aep_desde in aeps_desde:
            for aep_hasta in aeps_hasta:
                camino = dijkstra(grafo, tipo, aep_desde.codigo, aep_hasta.codigo)
                if len(camino) < len(camino_minimo) or len(camino_minimo) == 0:
                    camino_minimo = camino

        print(' -> '.join(camino_minimo))

        return True

def dijkstra(grafo, tipo, origen, destino):

    if origen not in grafo.vertices() or destino not in grafo.vertices():
        return []

    # Mi infinito
    inf = float('inf')

    distancias = {vertice: inf for vertice in grafo.vertices() } # Guardo las distancias
    camino = { vertice: None for vertice in grafo.vertices() } # Guardo el camino que voy creando
    distancias[origen] = 0
    vertices = grafo.vertices().copy()

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

    cola = deque()
    vertice_actual = destino
    while camino[vertice_actual] is not None:
        cola.appendleft(vertice_actual)
        vertice_actual = camino[vertice_actual]
    if cola:
        cola.appendleft(vertice_actual)

    return list(cola)
