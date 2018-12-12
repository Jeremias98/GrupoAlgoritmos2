import random

def contar_claves(dicc):
    ''' Devuelve la cantidad de claves que tiene el diccionario'''
    cont = 0
    for clave in dicc:
        cont += 1
    return cont

class Grafo():
    # Constructor
    def __init__(self, dicc = None):
        # Puedo pasarle por parámetro un diccionario de diccionarios
        # para inicializar el grafo
        if dicc == None:
            dicc = {}
            cant = 0
        else:
            cant = contar_claves(dicc)
        self._vertices = dicc # Variable protected
        self.len = cant 
    
    def __iter__(self):
        return _IteradorGrafo(self)
    
    def vertices(self):
        ''' Retorna un diccionario con los vértices del grafo con diccionarios vacíos
        como clave.'''
        return {vertice: {} for vertice in self._vertices}

    def adyacentes(self, vertice):
        ''' Retorna una lista de los vértices adyacentes al vértice dado.'''
        return list(self._vertices[vertice])

    def agregar_vertice(self, vertice):
        ''' Agrega un vértice al grafo (dicc) si el vertice no pertenece al grafo.
        Su valor es un dicc que contendrá sus adyacentes como clave y peso como valor 
        Devuelve True si fue agregado, False en caso contrario'''
        if vertice not in self._vertices:
            self._vertices[vertice] = {}
            self.len += 1
            return True
        else: return False


    def agregar_arista(self, vertice1, vertice2, peso):
        ''' Agrega una arista al grafo y su peso si los vertices pertenecen al grafo y devuelve True. 
        Devuelve False si los vertices no están en el grafo.'''
        if vertice1 not in self._vertices : return False
        if vertice2 not in self._vertices : return False
        self._vertices[vertice1][vertice2] = peso
        self._vertices[vertice2][vertice1] = peso
        return True

    def quitar_vertice(self, vertice):
        '''Elimina un vértice del grafo y todas las aristas que apuntan a él y devuelve True.
        Si el vertice no pertenece al grafo devuelve False.'''
        if vertice not in self._vertices:
            return False
        else:
            # Elimino el vertice
            del self._vertices[vertice]
            self.len -= 1
            # Elimino todas las aristas que le llegan
            for ady in self._vertices:
                del self._vertices[ady][vertice]

        return True

    def son_adyacentes(self, vertice1, vertice2):
        ''' Devuelve True si los vertices son adyacentes, False caso contrario.'''
        if vertice2 not in self._vertices[vertice1]:
            return False
        else: return True
    
    def quitar_arista(self, vertice1, vertice2):
        '''Elimina una arista del grafo y devuelve True.
        Si la arista no pertenece al grafo devuelve False.'''
        if not self.son_adyacentes(vertice1, vertice2):
            return FalseS
        else:    
            del self._vertices[vertice1][vertice2]
            del self._vertices[vertice2][vertice1]
            return True

    def get_peso(self, vertice1, vertice2):
        ''' Devuelve el peso entre dos vertices.'''
        return self._vertices[vertice1][vertice2]

    def get_vertice(self):
        ''' Devuelve un vertice aleatorio.'''
        return random.choice(list(self.vertices()))

    def get_aristas(self):
        aristas = []

        for v in self._vertices:
            for w in self._vertices[v]:
                if (v, w) in aristas or (w, v) in aristas: continue
                aristas.append((v, w))
        return aristas    

    def len_(self):
        '''Devuelve la cantidad de vertices del grafo.'''
        return self.len


class _IteradorGrafo:

    #Constructor iterador
    def __init__(self, grafo):
        self.visitar = list(grafo.vertices())
        self.actual = random.choice(self.visitar)

    def __next__(self):
        if len(self.visitar) == 0:
            raise StopIteration()

        vertice = self.actual
        self.visitar.remove(vertice)  
        if len(self.visitar) > 0: self.actual = random.choice(self.visitar)
        return vertice


#----------------------------------------------------------------------------

class Pila:
    '''Representa una pila con operaciones de apilar,
    desapilar y verificar si está vacía.'''
    
    def __init__(self):
        '''Crea una pila vacía.'''
        self.items = []
    
    def apilar(self, x):
        '''Apila el elemento x.'''
        self.items.append(x)
        
    def desapilar(self):
        '''Desapila el elemento x y lo devuelve.
        Si la pila está vacía levanta una excepción.'''
        if self.esta_vacia():
            raise ValueError('La pila está vacía')
        return self.items.pop()

    def esta_vacia(self):
        '''Devuelve True si la pila está vacía,
        devuelve False en caso contrario.'''
        return len(self.items) == 0
    
    def __str__(self):
        return str(self.items)        
        
#-----------------------------------------------------------------------------        

class Cola:
    '''Representa a una cola, con operaciones de encolar
    y desencolar. El primero es ser encolado también es el
    primero en ser desencolado.'''
    
    def __init__(self):
        '''Crea una cola vacía.'''
        self.items = []
        
    def encolar(self, x):
        '''Agrega el elemento x como último de la cola.'''
        self.items.append(x)
        
    def desencolar(self):
        '''Desencola el primer elemento y devuelve su valor.
        Si la cola esta vacía, levanta ValueError.'''
        if self.esta_vacia():
            raise ValueError('La cola está vacía')
        return self.items.pop(0)

    def esta_vacia(self):
        '''Devuelve True si la cola está vacía,
        devuelve False en caso contrario.'''
        return len(self.items) == 0   
    
    def __str__(self):
        return str(self.items)

