# by seCOnDatkE 2014.
#
# including:
#
#   * an simple bitree node impl.
#   * a bitree reconstruction (from preorder sequential representaion form) algorithm
#   * a function to draw bitree, with the impl. above
#
# dependency:
#
#   * python 2.7
#   * pygraphviz
#
# changelog:
#
#   * 2014.10.15 EVE initial commit.
#
#   http://github.com/secondwtq/project_Plutonium
#
# reference:
#
#   * Shaffer

class bitree_node(object):

    def __init__(self):

        self.value = None
        self.left = None
        self.right = None

def reconstruct_bitree(src, start, root):
    valuet = src[start]
    if valuet == '/':
        return start
    else:
        root.value = src[start]
        start += 1

        if src[start] == '/':
            start += 1
        else:
            root.left = bitree_node()
            start = reconstruct_bitree(src, start, root.left)

        if src[start] == '/':
            start += 1
        else:
            root.right = bitree_node()
            start = reconstruct_bitree(src, start, root.right)

        return start

def print_bitree(root, graph):
    if root != None:
        if root.left and root.left.value != None:
            graph.add_edge(root.value, root.left.value)
        if root.right and root.right.value != None:
            graph.add_edge(root.value, root.right.value)

        print_bitree(root.left, graph)
        print_bitree(root.right, graph)

def main():
    bitree = bitree_node()
    reconstruct_bitree('AB/D//CEG///FH//I//', 0, bitree)

    import pygraphviz
    graph = pygraphviz.AGraph()
    print_bitree(bitree, graph)

    graph.layout(prog='dot')
    graph.draw('a.png')


if __name__ == '__main__':
    main()
