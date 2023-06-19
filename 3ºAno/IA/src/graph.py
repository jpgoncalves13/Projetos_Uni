import networkx as nx  # Graph handling library required for representation
import matplotlib.pyplot as plt  # Graph handling library required for representation
import math  # Library needed to be able to use the math.sqrt function


# Graph class
class Graph:

    # Graph constructor
    def __init__(self, directed=False):
        self.nodes = set()  # Set of nodes
        self.directed = directed  # True if graph is directed
        self.graph = dict()  # Dictionary -> node : [ (node, weight), ...]
        self.heuristic = dict()  # Dictionary -> node : heuristic

    # Returns the string representation of a graph
    def __str__(self):
        out = ""
        for key in self.graph.keys():
            out = out + str(key) + ": " + str(self.graph[key]) + "\n"
        return out

    # Add edge to the graph, with weight
    def add_edge(self, n1, n2, weight):
        if n1 not in self.nodes:
            self.nodes.add(n1)
            self.graph[n1] = set()

        if n2 not in self.nodes:
            self.nodes.add(n2)
            self.graph[n2] = set()

        self.graph[n1].add((n2, weight))

        # If the graph is undirected, put the inverse edge
        if not self.directed:
            self.graph[n2].add((n1, weight))

    # Show edges
    def show_edges(self):
        out = ""
        for node in self.graph.keys():
            for (adjacent, weight) in self.graph[node]:
                out = out + "(" + str(node) + " -> " + str(adjacent) + ", Cost=" + str(weight) + ")\n"
        return out.rstrip(out[-1])

    # Show nodes
    def show_nodes(self):
        out = ""
        for node in self.nodes:
            out = out + str(node) + "\n"
        return out.rstrip(out[-1])

    # Return edge cost
    def get_arc_cost(self, node1, node2):
        edges = self.graph[node1]
        for (adjacent, weight) in edges:
            if adjacent == node2:
                return weight
        return math.inf

    # Return the cost of a path
    def path_cost(self, path):
        cost = 0
        i = 1
        while i < len(path):
            cost = cost + self.get_arc_cost(path[i - 1], path[i])
            i = i + 1
        return cost

    # BFS search, returns path and the path cost
    def search_dfs(self, start, end, path=None, visited=None, depth=None, players=None):  # end -> goals set
        if path is None:
            path = []
        if visited is None:
            visited = set()
        if depth is None:
            depth = math.inf

        visited.add(start)
        path.append(start)

        if start.coord in end:
            return path, self.path_cost(path), len(visited)
        elif depth == 0:
            path.pop()
            return None

        for (adjacent, weight) in self.graph[start]:
            if (adjacent not in visited) and ((players is None) or (adjacent.coord not in players)):
                result = self.search_dfs(adjacent, end, path, visited, depth-1, players)
                if result is not None:
                    return result

        path.pop()
        return None

    def iterative_search(self, start, end, players=None):
        depth = 0
        ret = None

        while ret is None:
            ret = self.search_dfs(start,end,depth=depth, players=players)
            depth += 1

        return ret

    # BFS search, returns path and the path cost
    def search_bfs(self, start, end, players=None):  # end -> goals set
        path = []
        visited = set()
        queue = []
        parent = dict()
        count = 0

        visited.add(start)
        queue.append(start)
        parent[start] = None

        if start.coord in end:
            return path, 0

        while len(queue) != 0:
            count += 1
            node = queue.pop(0)
            for (adjacent, weight) in self.graph[node]:
                if (players is None) or (adjacent.coord not in players):
                    if adjacent.coord in end:
                        parent[adjacent] = node
                        aux = adjacent
                        while aux is not None:
                            path.append(aux)
                            aux = parent[aux]

                        path.reverse()
                        return path, self.path_cost(path), count

                    if adjacent not in visited:
                        visited.add(adjacent)
                        parent[adjacent] = node
                        queue.append(adjacent)

        return None

    # Graph search with the Uniform Cost algorithm
    def search_uniform_cost(self, start, end, players=None):
        open_list = set()
        open_list.add(start)
        closed_list = set()
        parent = dict()
        parent[start] = None
        cost = dict()
        cost[start] = 0
        count = 0

        while len(open_list) > 0:
            count += 1
            n1 = None
            for n2 in open_list:
                if (n1 is None) or (cost[n2] < cost[n1]):
                    n1 = n2

            if n1.coord in end:
                n_aux = n1
                path = []

                while n_aux is not None:
                    path.append(n_aux)
                    n_aux = parent[n_aux]

                path.reverse()
                return path, self.path_cost(path), count

            for (adjacent, weight) in self.graph[n1]:
                if (adjacent not in open_list) and (adjacent not in closed_list) and ((players is None) or (adjacent.coord not in players)):
                    open_list.add(adjacent)
                    parent[adjacent] = n1
                    cost[adjacent] = cost[n1] + weight

            open_list.remove(n1)
            closed_list.add(n1)
        return None

    # Graph search with the Greedy algorithm
    def search_greedy(self, start, end, players=None):
        count = 0
        open_list = set()
        open_list.add(start)
        closed_list = set()
        parent = dict()
        parent[start] = None

        while len(open_list) > 0:
            count += 1
            n1 = None
            for n2 in open_list:
                if (n1 is None) or (self.heuristic[n2] < self.heuristic[n1]):
                    n1 = n2

            if n1.coord in end:
                n_aux = n1
                path = []

                while n_aux is not None:
                    path.append(n_aux)
                    n_aux = parent[n_aux]

                path.reverse()
                return path, self.path_cost(path), count

            for (adjacent, weight) in self.graph[n1]:
                if (adjacent not in open_list) and (adjacent not in closed_list) and ((players is None) or (adjacent.coord not in players)):
                    open_list.add(adjacent)
                    parent[adjacent] = n1

            open_list.remove(n1)
            closed_list.add(n1)

        return None

    # Graph search with the A* algorithm
    def search_star_a(self, start, end, players=None):
        open_list = set()
        open_list.add(start)
        closed_list = set()
        parent = dict()
        parent[start] = None
        cost = dict()
        cost[start] = 0
        count = 0

        while len(open_list) > 0:
            count += 1
            n1 = None
            for n2 in open_list:
                if (n1 is None) or (self.heuristic[n2] + cost[n2]) < (self.heuristic[n1] + cost[n1]):
                    n1 = n2

            if n1.coord in end:
                n_aux = n1
                path = []

                while n_aux is not None:
                    path.append(n_aux)
                    n_aux = parent[n_aux]

                path.reverse()
                return path, self.path_cost(path), count

            for (adjacent, weight) in self.graph[n1]:
                if (adjacent not in open_list) and (adjacent not in closed_list) and ((players is None) or (adjacent.coord not in players)):
                    open_list.add(adjacent)
                    parent[adjacent] = n1
                    cost[adjacent] = cost[n1] + weight

            open_list.remove(n1)
            closed_list.add(n1)
        return None

    # Draw the graph
    def graph_draw(self):
        list_nodes = self.nodes
        g = nx.Graph()

        for node in list_nodes:
            g.add_node(node)
            for (adjacent, peso) in self.graph[node]:
                g.add_edge(node, adjacent, weight=peso)

        # Drawing
        pos = nx.spring_layout(g)
        nx.draw_networkx(g, pos, with_labels=True, font_weight='bold')
        labels = nx.get_edge_attributes(g, 'weight')
        nx.draw_networkx_edge_labels(g, pos, edge_labels=labels)

        plt.draw()
        plt.show()
