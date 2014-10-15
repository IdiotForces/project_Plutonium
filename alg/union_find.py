# by seCOnDatkE 2014.
#
# including:
#
#   * union_find_tree class
#       quick-union algorithm, support path-compression(by default)
#
# dependency:
#
#   * python 2.7
#   * python re
#   * pygraphviz
#
# changelog:
#
#   * 2014.10.15 EVE initial commit.
#
#   http://github.com/secondwtq/project_Plutonium
#
# comments:
#
# VC suggests to use python set + set operations
# though I just want to try writing algorithm in python
# & try implementing general union-find
# & try print tree at runtime with pygraphviz
# & ... the most important, complete the homework
# & validate the answer
#
# and the path compression is really magic...
#
# reference:
#
#   * Sedgewick
#   * Shaffer
#   * http://blog.csdn.net/sysucph/article/details/7263010
#

import pygraphviz

class union_find_tree(object):

    def __init__(self):
        self._tree = { }
        self._sizes = { }

        self.enable_path_compression = True

    def create_node(self, node_idx):
        self._tree[node_idx] = node_idx
        self._sizes[node_idx] = 1

    def has_node(self, node_idx):
        return node_idx in self._tree

    def union(self, a, b):
        root_a, root_b = self.find(a), self.find(b)
        if root_a == root_b:
            return

        if self._sizes[root_a] > self._sizes[root_b]:
            root_a, root_b = root_b, root_a
        elif self._sizes[root_a] == self._sizes[root_b]:
            if root_a < root_b:
                root_a, root_b = root_b, root_a

        # make root_a be child of root_b
        self._tree[root_a] = root_b
        self._sizes[root_b] += self._sizes[root_a]

    def find(self, node_idx):
        if self.enable_path_compression:
            # path compression
            if node_idx != self._tree[node_idx]:
                self._tree[node_idx] = self.find(self._tree[node_idx])
            return self._tree[node_idx]
        else:
            while self._tree[node_idx] != node_idx:
                node_idx = self._tree[node_idx]
            return node_idx

    def debug_print(self):
        for i, v in self._tree.items():
            print(i, v)

    def debug_image(self):
        graph = pygraphviz.AGraph(strict=True, directed=True)

        for a, b in self._tree.items():
            if a != b:
                graph.add_edge(b, a)

        graph.layout(prog='dot')
        return graph

    def create_and_link(self, a, b):
        if not self.has_node(a):
            self.create_node(a)
        if not self.has_node(b):
            self.create_node(b)

        self.union(a, b)

def parse_union(src):
    import re
    exp = re.compile(r"\(([0-9].*),[ ]?([0-9].*)\)")
    groups = map(int, exp.match(src).groups())
    return groups

def main():
    org_str = "(0,2)\n(1,2)\n(3,4)\n(3,1)\n(3,5)\n(9,11)\n(12,14)\n(3,9)\n(4,14)\n(6,7)\n(8,10)\n(8,7)\n(7, 0)\n(10, 15)\n(10, 13)"
    #org_str = "(0,2)\n(1,2)\n(3,4)\n(3,1)\n(3,5)\n(9,11)\n(12,14)"
    data = [parse_union(item) for item in org_str.splitlines()]
    print data

    uft = union_find_tree()

    #uft.enable_path_compression = False

    for item in data:
        uft.create_and_link(item[0], item[1])

    uft.debug_print()

    uft.debug_image().draw('union_find.png')

if __name__ == '__main__':
    main()
