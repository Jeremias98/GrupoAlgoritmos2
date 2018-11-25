from clasesflycombi import Grafo
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



class FlyComby():

    def __init__(self, aeropuertos_csv, vuelos_csv):
        self._file_aeropuertos = open(aeropuertos_csv, newline='')
        self._file_vuelos = open(vuelos_csv, "r")
        self._grafo = Grafo()
        self._ciudades = {} # Guardo en un 'hash' las ciudades por nombre asi accedo en O(1)
        self._vuelos = {} # Guardo en un 'hash' los vuelos por código asi accedo en O(1)
        self._comandos = ["camino_mas", "camino_escalas", "centralidad", "nueva_aerolinea", "vacaciones"]

        lector = csv.reader(self._file_aeropuertos, delimiter=',')
        for fila in lector:
            ciudad = fila[0]
            if ciudad not in self._ciudades:
                self._ciudades[ciudad] = []

            aeropuerto = Aeropuerto(fila[1], fila[2], fila[3])
            self._ciudades[ciudad].append(aeropuerto)

            self._grafo.agregar_vertice(aeropuerto.codigo)


        lector = csv.reader(self._file_vuelos, delimiter=',')
        for fila in lector:
            vuelo = Vuelo(fila[0], fila[1], fila[2], fila[3], fila[4])

            self._grafo.agregar_arista(vuelo.cod_origen, vuelo.cod_destino, vuelo)

    def ejecutar_comando(self, comando, argumento):

        if comando == "camino_mas" and len(argumento) == 3:
            return self.camino_mas(argumento[0], argumento[1], argumento[2])

        return False


    def procesar_entrada(self):
        #for aeropuerto in self._ciudades["Albany"]:
            #aeropuerto.imprimir_datos()
        print("FlyComby >> ", end="", flush=True)
        while True:

            linea = sys.stdin.readline()

            if linea:
                linea = linea[:-1]

                cmd_spliteado = str.split(linea)
                comando = cmd_spliteado[0]
                argumento = None

                if len(cmd_spliteado) > 1:
                    argumento = cmd_spliteado[1:len(cmd_spliteado)]

                if comando == "listar_operaciones":
                    self.listar_operaciones()
                elif comando == "quit":
                    break
                elif comando in self._comandos:
                    if not self.ejecutar_comando(comando, argumento):
                        print("Error en comando {}".format(comando))
                else:
                    print("Error: comando desconocido")

                self.procesar_entrada()

    def listar_operaciones(self):
        for comando in self._comandos:
            print(comando)

        self.procesar_entrada()

    def camino_mas(self, tipo, desde, hasta):
        return True


if len(sys.argv) < 3:
    print("Cantidad de argumentos inválida")
else:
    flycomby = FlyComby(sys.argv[1], sys.argv[2])
    flycomby.procesar_entrada()
