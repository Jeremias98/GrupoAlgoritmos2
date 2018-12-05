from collections import deque
from clasesflycombi import Pila, Cola
import operator
from heapq import heappush, heappop
from clasesflycombi import Grafo
import csv

def impresion_estandar(l_aeropuertos):
    result = ""

    for aeropuerto in l_aeropuertos:
        result += aeropuerto + " -> "

    print(result[:-4])
#
def recorrer_mundo(grafo, ciudad, ciudades):

    if ciudad not in ciudades: return False

    aeps = ciudades[ciudad]

    # Tomo el primer aeropuerto arbitrariamente
    mst = prim(grafo, aeps[0])
    peso_total = 0

    return True

#----------------------------------------------------------------------------------------
tipos_camino = ["barato", "rapido"]
def camino_mas(grafo, tipo, desde, hasta, ciudades, ult_rec):

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

        impresion_estandar(camino_minimo)

        while ult_rec: ult_rec.pop()
        for aep in camino_minimo:
            ult_rec.append(aep)
        return True

#----------------------------------------------------------------------------------------
def ordenar_vertices(dicc):
    items_ord = sorted(dicc.items(),key = operator.itemgetter(1),reverse = True)
    vertices_ord = []
    for v, valor in items_ord: vertices_ord.append(v)

    return vertices_ord

def centralidad(grafo, k):

    centrales = betweness_centrality(grafo, k)
    print(', '.join(centrales))

    return True

def betweness_centrality(grafo, k):

    tope = int(k)
    cent = {} # Inicializo la centralidad
    for v in grafo: cent[v] = 0

    for v in grafo:
        padre, dist = dijkstra(grafo, "rapido", v, None)
        cent_aux = {}
        for w in grafo: cent_aux[w] = 0

        vertices_ordenados = ordenar_vertices(dist)
        for w in vertices_ordenados:
            if w == v: continue
            #cent_aux[padre[w]] +=  1 + cent_aux[w]
            cent_aux[padre[w]] +=  int(grafo.get_peso(padre[w], w).cant_vuelos_entre_aeropuertos) + cent_aux[w]
        for w in grafo:
            if w == v: continue
            cent[w] += cent_aux[w]

    k_centrales = ordenar_vertices(cent)
    lista_centrales = []
    cont = 0
    for v in k_centrales:
        cont += 1
        lista_centrales.append(v)
        if cont == tope: break

    return lista_centrales

#----------------------------------------------------------------------------------------
def dijkstra(grafo, tipo, origen, destino):   #Propuesta DJKSTRA
    ''' Devuelve una lista con los aeropuertos de camino minimo según el tipo entre
    origen y destino. Si no existe el destino o es None, devuelve la tupla (padre, dist).'''
    inf = float('inf')

    dist = {vertice: inf for vertice in grafo} # Guardo las distancias
    padre = {}
    dist[origen] = 0
    padre[origen] = None
    heap = []
    heappush(heap, (dist[origen], origen))

    while heap:
        # Obtiene el minimo, es como si fuera un heap de minimos
        priority, v =  heappop(heap)
        if destino == v: return reconstruir_camino(origen, destino, padre)

        for w in grafo.adyacentes(v):
            peso_arista = grafo.get_peso(v, w)
            if tipo == tipos_camino[0]: peso = int(peso_arista.precio)
            elif tipo == tipos_camino[1]: peso = int(peso_arista.tiempo_promedio)
            else: peso = int(peso_arista.cant_vuelos_entre_aeropuertos)

            if dist[v] + peso < dist[w]:
                dist[w] = dist[v] + peso
                padre[w] = v
                heappush(heap, (dist[w], w))

    return padre, dist

#-------------------------------------------------------------------------------------------
def prim(grafo, vertice_inicial):

    visitados = set()
    visitados.add(vertice_inicial)

    heap = []

    for w in grafo.adyacentes(vertice_inicial):
        heappush(heap, (vertice_inicial, w, grafo.get_peso(vertice_inicial, w)))
    arbol = Grafo()

    for v in grafo.vertices():
        arbol.agregar_vertice(v)

    while heap:
        (v,w,p) = heappop(heap)
        if w in visitados: continue
        arbol.agregar_arista(v,w,p)
        visitados.add(w)
        for x in grafo.adyacentes(w):
            if x not in visitados:
                heappush(heap, (w,x, grafo.get_peso(w, x)))

    return arbol

#-------------------------------------------------------------------------------------------
def prim2(grafo):

    vertice = grafo.get_vertice()
    visitados = set()
    visitados.add(vertice)

    heap = []
    for w in grafo.adyacentes(vertice):
        heappush(heap, (int(grafo.get_peso(vertice, w).precio), (vertice, w)))
    
    arbol = Grafo(grafo.vertices())  #Genera un grafo solo con los vertices
    
    while heap:
        priority, (v,w) = heappop(heap)
        if w in visitados: continue
        arbol.agregar_arista(v, w, grafo.get_peso(v,w))
        visitados.add(w)
        for x in grafo.adyacentes(w):
            if x not in visitados:
                heappush(heap, (int(grafo.get_peso(w,x).precio), (w,x)))

    return arbol

#-------------------------------------------------------------------------------------------
def nueva_aerolinea(grafo, ruta):

    arbol = prim2(grafo)
    vuelos_agregados = set()

    with open(ruta, 'w') as file:
        writer = csv.writer(file, delimiter= ',')
        for aip_i in arbol:
            for aip_j in arbol.adyacentes(aip_i):
                
                vuelo = arbol.get_peso(aip_i, aip_j)
                if vuelo not in vuelos_agregados:
                    writer.writerow([aip_i, aip_j, vuelo.tiempo_promedio, vuelo.precio, vuelo.cant_vuelos_entre_aeropuertos])
                    vuelos_agregados.add(vuelo)
    print("OK")
    return True

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
def bfs(grafo, origen, destino):
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
def camino_minimo_escalas(grafo, ciudad_origen, ciudad_destino, ciudades, ult_rec):
    '''Dado una ciudad de origen y otra de destino, devuelve una lista de aeropuertos de camino minimo.'''
    if ciudad_origen not in ciudades: return False
    if ciudad_destino not in ciudades: return False

    aips_origen = ciudades[ciudad_origen]
    aips_destino = ciudades[ciudad_destino]

    camino_resul = []

    for origen in aips_origen:
        for destino in aips_destino:

            camino = bfs(grafo, origen, destino)
            if len(camino) < len(camino_resul) or len(camino_resul) == 0:
                camino_resul = camino

    impresion_estandar(camino_resul)

    while ult_rec: ult_rec.pop()
    for aep in camino_resul:
        ult_rec.append(aep)
    return True

#----------------------------------------------------------------------------------------------
def exportar_kml(ruta, l_cod_aeps, aeropuertos):

    l_aeps = []
    for aeropuerto in l_cod_aeps:
        l_aeps.append(aeropuertos[aeropuerto])

    f = open(ruta, 'w')
    f.write('<?xml version="1.0" encoding="UTF-8"?>\n')
    f.write('<kml xmlns="http://www.opengis.net/kml/2.2">\n')
    f.write("    <Document>\n")

    for aep in l_aeps:
        f.write("        <Placemark>\n")
        f.write("            <name>{}</name>\n".format(aep.codigo))
        f.write("            <Point>\n")
        f.write("                <coordinates>{}, {}</coordinates>\n".format(aep.longitud, aep.latitud))
        f.write("            </Point>\n")
        f.write("        </Placemark>\n")

    for i in range(0, len(l_aeps) - 1):
        f.write("        <Placemark>\n")
        f.write("            <LineString>\n")
        f.write("                <coordinates>{}, {} {}, {}</coordinates>\n".format(
        l_aeps[i].longitud, l_aeps[i].latitud, l_aeps[i+1].longitud, l_aeps[i+1].latitud))
        f.write("            </LineString>\n")
        f.write("        </Placemark>\n")

    f.write("    </Document>\n")
    f.write("</kml>\n")
    f.close()

    print("OK")
    return True
