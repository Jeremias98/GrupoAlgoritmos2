from clasesflycombi import Grafo
import lib_aeropuertos as lib
import sys
import csv

#class Ciudad:
#    def __init__(self, nombre, aeropuertos):
#        self.nombre = nombre
#        self.aeropuertos = aeropuertos

class Aeropuerto:

    def __init__(self, codigo, latitud, longitud):
        self.codigo = codigo
        self.latitud = latitud
        self.longitud = longitud

    def imprimir_datos(self):
        print("{} {} {}".format(self.codigo, self.latitud, self.longitud))

class Vuelo:

    def __init__(self, cod_origen, cod_destino, tiempo_promedio, precio, cant_vuelos_entre_aeropuertos):
        self.cod_origen = cod_origen # Los códigos pueden eliminarse para no tener redundancia en el peso del grafo
        self.cod_destino = cod_destino
        self.tiempo_promedio = tiempo_promedio
        self.precio = precio
        self.cant_vuelos_entre_aeropuertos = cant_vuelos_entre_aeropuertos

    def imprimir_datos(self):
        print("{} {} {} {} {}".format(self.cod_origen, self.cod_destino, self.tiempo_promedio, self.precio, self.cant_vuelos_entre_aeropuertos))


def procesar_datos(grafo, ciudades, vuelos, aeropuertos, aeropuertos_csv, vuelos_csv):

    file_aeropuertos = open(aeropuertos_csv, newline='')
    file_vuelos = open(vuelos_csv, "r")

    lector = csv.reader(file_aeropuertos, delimiter=',')
    for fila in lector:
        ciudad = fila[0]
        if ciudad not in ciudades:
            ciudades[ciudad] = []

        aeropuerto = Aeropuerto(fila[1], fila[2], fila[3])
        ciudades[ciudad].append(aeropuerto.codigo)
        aeropuertos[aeropuerto.codigo] = aeropuerto

        grafo.agregar_vertice(aeropuerto.codigo)


    lector = csv.reader(file_vuelos, delimiter=',')
    for fila in lector:
        vuelo = Vuelo(fila[0], fila[1], fila[2], fila[3], fila[4])
        vuelos.add(vuelo)

        grafo.agregar_arista(vuelo.cod_origen, vuelo.cod_destino, vuelo)

    file_aeropuertos.close()
    file_vuelos.close()


def procesar_entrada(grafo, comandos, ciudades, ult_rec, aeropuertos):

    print("FlyComby >> ", end="", flush=True)  #Se debe imprimir el encabezado?

    linea = sys.stdin.readline()
    while linea:

        hay_error = False
        linea = linea[:-1]

        cmd_spliteado = linea.split(" ", 1)
        comando = cmd_spliteado[0]
        parametros = None

        if len(cmd_spliteado) > 1:
            #parametros = cmd_spliteado[1:len(cmd_spliteado)]
            parametros = cmd_spliteado[1].split(",")
        if comando == "listar_operaciones": listar_operaciones(comandos)

        elif comando in comandos:
            if not ejecutar_comando(grafo, comando, parametros, ciudades, ult_rec, aeropuertos): hay_error = True

        if hay_error: print("Error en comando {}".format(comando))
        #NO ESPECIFICA SI HAY QUE IMPRIMIR UN ERROR GENERAL.

        linea = sys.stdin.readline()


def listar_operaciones(comandos):
    for comando in comandos:
        print(comando)

def ejecutar_comando(grafo, comando, parametros, ciudades, ult_rec, aeropuertos):

    if comando == "exportar_kml" and len(parametros) == 1:
        return lib.exportar_kml(parametros[0], ult_rec, aeropuertos)

    if comando == "camino_escalas" and len(parametros) == 2:
        return lib.camino_minimo_escalas(grafo, parametros[0], parametros[1], ciudades, ult_rec)

    if comando == "camino_mas" and len(parametros) == 3:
        return lib.camino_mas(grafo, parametros[0], parametros[1], parametros[2], ciudades, ult_rec)

    if comando == "centralidad" and len(parametros) == 1:
        return lib.centralidad(grafo, parametros[0])

    if comando == "recorrer_mundo" and len(parametros) == 1:
        return lib.recorrer_mundo(grafo, parametros[0], ciudades)

    return False

def main():

    grafo = Grafo()
    ciudades = {} # Guardo en un 'hash' las ciudades por nombre asi accedo en O(1) - Claves = Cod. Aeropuertos
    aeropuertos = {} # # Guardo en un 'hash' los cod de aeropuerto asi accedo en O(1) - Claves = Class Aeropuerto
    vuelos = set() # Guardo en un 'conjunto' los vuelos por código asi accedo en O(1)
    comandos = ["camino_mas", "camino_escalas", "centralidad", "nueva_aerolinea", "vacaciones", "exportar_kml", "recorrer_mundo"]
    ultimo_recorrido = []

    if len(sys.argv) < 3:
        print("Cantidad de argumentos inválida") # Hay que imprimirlo?

    else:
        procesar_datos(grafo, ciudades, vuelos, aeropuertos, sys.argv[1], sys.argv[2])
        procesar_entrada(grafo, comandos, ciudades, ultimo_recorrido, aeropuertos)



    '''
    v = grafo.get_vertice()         # VERTICE RANDOM
    print(v)

    for w in grafo.adyacentes(v):   # VUELOS DEL VERTICE (AIP) RANDOM
        vuelo = grafo.get_peso(v,w)
        vuelo.imprimir_datos()

    for v in grafo:                 # IMPRIME TODOS AIP
       print(v)

    for ciudad in ciudades:         # IMPRIME TODAS LAS CIUDADES y SUS AIP
        print(ciudad)
        print(ciudades[ciudad])

    for vuelo in vuelos:            # IMPRIME TODOS LOS VUELOS
        vuelo.imprimir_datos()


    #PRUEBAS QUE SE HAYAN CARGADO LOS DATOS CORRECTAMENTE
    '''

main()
