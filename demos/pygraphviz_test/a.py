import pygraphviz as PG

A = PG.AGraph(strict=False, directed=True)

A.add_edge("12", "9")
A.add_edge("12", "10")

A.add_edge("9", "7")
A.add_edge("9", "4")

A.add_edge("10", "1")
A.add_edge("10", "8")

A.add_edge("7", "3")
A.add_edge("7", "5")

A.add_edge("4", "2")

# etc., etc.

# save the graph in dot format
A.write('ademo.dot')

# pygraphviz renders graphs in neato by default, 
# so you need to specify dot as the layout engine
A.layout(prog='dot')
A.draw("heap.svg")