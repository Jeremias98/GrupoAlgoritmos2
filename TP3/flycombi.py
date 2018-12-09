#!/usr/bin/python3
from clasesflycombi import Grafo
import lib_aeropuertos as lib
import sys
import csv


class Aeropuerto:

    def __init__(self, codigo, latitud, longitud):
        self.codigo = codigo
        self.latitud = latitud
        self.longitud = longitud

class Vuelo:

    def __init__(self, cod_origen, cod_destino, tiempo_promedio, precio, cant_vuelos_entre_aeropuertos):
        self.cod_origen = cod_origen # Los códigos pueden eliminarse para no tener redundancia en el peso del grafo
        self.cod_destino = cod_destino
        self.tiempo_promedio = tiempo_promedio
        self.precio = precio
        self.cant_vuelos_entre_aeropuertos = cant_vuelos_entre_aeropuertos


def procesar_datos(grafo, ciudades, aeropuertos, aeropuertos_csv, vuelos_csv):

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

        grafo.agregar_arista(vuelo.cod_origen, vuelo.cod_destino, vuelo)

    file_aeropuertos.close()
    file_vuelos.close()


def procesar_entrada(grafo, comandos, ciudades, ult_rec, aeropuertos):

    linea = sys.stdin.readline()
    while linea:

        hay_error = False
        linea = linea[:-1]

        cmd_spliteado = linea.split(" ", 1)
        comando = cmd_spliteado[0]
        parametros = None

        if len(cmd_spliteado) > 1: parametros = cmd_spliteado[1].split(",")
        
        if comando == "listar_operaciones": listar_operaciones(comandos)

        elif comando in comandos:
            if not ejecutar_comando(grafo, comando, parametros, ciudades, ult_rec, aeropuertos): hay_error = True

        else: hay_error = True

        if hay_error: print("Error en comando {}".format(comando))
        #NO ESPECIFICA SI HAY QUE IMPRIMIR UN ERROR GENERAL.

        linea = sys.stdin.readline()


def listar_operaciones(comandos):
    for comando in comandos:
        print(comando)

def ejecutar_comando(grafo, comando, parametros, ciudades, ult_rec, aeropuertos):

    if comando == "itinerario" and len(parametros) == 1:
        return lib.itinerario_cultural(grafo, parametros[0], ciudades, ult_rec)

    if comando == "vacaciones" and len(parametros) == 2:
        return lib.vacaciones(grafo, parametros[0], int(parametros[1]), ciudades, ult_rec)   

    if comando == "exportar_kml" and len(parametros) == 1:
        return lib.exportar_kml(parametros[0], ult_rec, aeropuertos)

    if comando == "camino_escalas" and len(parametros) == 2:
        return lib.camino_minimo_escalas(grafo, parametros[0], parametros[1], ciudades, ult_rec)

    if comando == "camino_mas" and len(parametros) == 3:
        return lib.camino_mas(grafo, parametros[0], parametros[1], parametros[2], ciudades, ult_rec)

    if comando == "centralidad" and len(parametros) == 1:
        return lib.centralidad(grafo, parametros[0])

    if comando == "nueva_aerolinea" and len(parametros) == 1:
        return lib.nueva_aerolinea(grafo, parametros[0])

    return False

def main():

    grafo = Grafo()
    ciudades = {} # Guardo en un 'hash' las ciudades por nombre asi accedo en O(1) - Claves = Cod. Aeropuertos
    aeropuertos = {} # # Guardo en un 'hash' los cod de aeropuerto asi accedo en O(1) - Claves = Class Aeropuerto
    comandos = ["camino_mas", "camino_escalas", "centralidad", "nueva_aerolinea", "exportar_kml", "itinerario", "vacaciones"]
    ultimo_recorrido = []
    
    if len(sys.argv) < 3:
        print("Cantidad de argumentos inválida") # Hay que imprimirlo?

    else:
        procesar_datos(grafo, ciudades, aeropuertos, sys.argv[1], sys.argv[2])
        procesar_entrada(grafo, comandos, ciudades, ultimo_recorrido, aeropuertos)

main()
