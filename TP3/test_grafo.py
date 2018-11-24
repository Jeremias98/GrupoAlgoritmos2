from grafo import Grafo

def test_agregar_vertice():

    grafo = Grafo()

    grafo.agregar_vertice("A")
    grafo.agregar_vertice("B")
    grafo.agregar_vertice("C")
    grafo.agregar_vertice("D")

    grafo.agregar_arista({"A", "B"})
    grafo.agregar_arista({"A", "C"})
    grafo.agregar_arista({"B", "D"})

    print("Imprimo los todos vertices")
    for v in grafo.obtener_vertices():
        print(v)

    print("Adyacentes a A: ")
    for v in grafo.obtener_adyacentes("A"):
        print(v)

    print("Adyacentes a C: ")
    for v in grafo.obtener_adyacentes("B"):
        print(v)

test_agregar_vertice()
