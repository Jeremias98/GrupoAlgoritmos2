class Grafo():
    # Constructor
    def __init__(self, dicc = None):
        # Puedo pasarle por parámetro un diccionario de diccionarios
        # para inicializar el grafo
        if dicc == None:
            dic = {}
        self._vertices = dic # Variable protected

    # Función para obtener la tupla de la arista de forma {"a","b"}
    def _obtener_tupla_arista(self, arista):
        arista = set(arista)
        return tuple(arista)

    # Retorna una lista de los vértices del grafo
    def obtener_vertices(self):
        return list(self._vertices.keys())

    # Retorna una lista de los vértices adyacentes al vértice dado
    def obtener_adyacentes(self, vertice):
        return list(self._vertices[vertice])

    # Agrega un vértice al grafo
    def agregar_vertice(self, vertice):
        if vertice not in self._vertices:
            self._vertices[vertice] = []

    # Agrega una arista al grafo
    # suponiendo que arista es de la forma {"a","b"}
    def agregar_arista(self, arista):
        (v1, v2) = self._obtener_tupla_arista(arista)
        if v1 in self._vertices:
            self._vertices[v1].append(v2)
        else:
            self._vertices[v1] = [v2]

    # Elimina un vértice del grafo y todas las aristas que apuntan a él
    def quitar_vertice(self, vertice):
        if vertice not in self._vertices:
            return False
        else:
            # Elimino el vertice
            del self._vertices[vertice]
            # Elimino todas las aristas que le llegan
            for v in self._vertices:
                del v[vertice]

        return True


    def quitar_arista(self, arista):
        (v1, v2) = self._obtener_tupla_arista(arista)
        del self._vertices[v1][v2]
