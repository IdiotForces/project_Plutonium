import pygraphviz as PG

A = PG.AGraph(strict=False, directed=True)

A.add_edge('209', '83')
A.add_edge('209', '126')
A.add_edge('83', '41 L')
A.add_edge('83', '42')
A.add_edge('42', '19 H')
A.add_edge('42', '23 I')

A.add_edge('126', '55')
A.add_edge('126', '71')

A.add_edge('55', '24')

A.add_edge('24', '11 E')
A.add_edge('24', '13 F')

A.add_edge('55', '31 J')

A.add_edge('71', '34')
A.add_edge('71', '37 K')

A.add_edge('34', '17')

A.add_edge('34', '17 G')

A.add_edge('17', '7 D')
A.add_edge('17', '10')
A.add_edge('10', '5')
A.add_edge('10', '5 C')

A.add_edge('5', '2 A')
A.add_edge('5', '3 B')

# etc., etc.

# save the graph in dot format
A.write('ademo.dot')

# pygraphviz renders graphs in neato by default, 
# so you need to specify dot as the layout engine
A.layout(prog='dot')
A.draw("huffman.svg")