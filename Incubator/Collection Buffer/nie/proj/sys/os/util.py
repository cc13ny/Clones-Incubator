'''
  This file is intended for handling various problems in general OSs.
'''

class Deadlock(object):
    # @param {integer} nnodes
    # @param {integer[][]} edges (e.g. [0, 1] => 0 -> 1
    # @return {boolean}
    
    def hasdeadlock(self, nnodes, edges):
        indegrees = [0] * nnodes
        graphs = [set() for _ in range(nnodes)]
        for e in edges:
            if e[1] not in graphs[e[0]]: # handling replicate edges
                graphs[e[0]].add(e[1])
                indegrees[e[1]] += 1
        queue = [ i for i in range(len(indegrees)) if indegrees[i] == 0 ]
        
        while len(queue) != 0:
            root = queue.pop(0)
            for i in graphs[root]:
                indegrees[i] -= 1
                if indegrees[i] == 0:
                    queue.append(i)
        
        if sum(indegrees) != 0:
            return False
            
        return True
