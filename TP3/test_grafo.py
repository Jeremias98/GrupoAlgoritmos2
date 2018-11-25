from clasesflycombi import Grafo

def test_agregar_vertice():

    grafo = Grafo()

    grafo.agregar_vertice("A")
    grafo.agregar_vertice("B")
    grafo.agregar_vertice("C")
    grafo.agregar_vertice("D")

    grafo.agregar_arista("A", "B", 5)
    grafo.agregar_arista("A", "C", 2)
    grafo.agregar_arista("A", "D", 8)
    grafo.agregar_arista("B", "C", 1)
    grafo.agregar_arista("B", "D", 6)
    grafo.agregar_arista("C", "D", 19)
    

    print("Imprimo los todos vertices")
    for v in grafo:
        print(v)

    print("Llamo get_vertice: {}".format(grafo.get_vertice()))
    print("Llamo get_vertice: {}".format(grafo.get_vertice()))
    print("Llamo get_vertice: {}".format(grafo.get_vertice()))

    print("Adyacentes a A: ")
    for v in grafo.adyacentes("A"):
        print("{} : {}".format(v, grafo.get_peso("A", v)))

    print("Adyacentes a B: ")
    for v in grafo.adyacentes("B"):
        print("{} : {}".format(v, grafo.get_peso("B", v)))

    print("Adyacentes a C: ")
    for v in grafo.adyacentes("C"):
        print("{} : {}".format(v, grafo.get_peso("C", v)))

    print("Len Grafo: {}".format(grafo.len_()))    

    grafo.quitar_vertice("D")
    print("\nQuito vertice D")

    print("Adyacentes a A: ")
    for v in grafo.adyacentes("A"):
        print("{} : {}".format(v, grafo.get_peso("A", v)))

    print("Adyacentes a B: ")
    for v in grafo.adyacentes("B"):
        print("{} : {}".format(v, grafo.get_peso("B", v)))

    print("Adyacentes a C: ")
    for v in grafo.adyacentes("C"):
        print("{} : {}".format(v, grafo.get_peso("C", v)))

    print("Len Grafo: {}".format(grafo.len_()))   

    grafo.quitar_arista("A", "B")
    print("\nQuito arista A-B")

    print("Adyacentes a A: ")
    for v in grafo.adyacentes("A"):
        print("{} : {}".format(v, grafo.get_peso("A", v)))

    print("Adyacentes a B: ")
    for v in grafo.adyacentes("B"):
        print("{} : {}".format(v, grafo.get_peso("B", v)))


test_agregar_vertice()
