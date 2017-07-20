import sys
import itertools


SIDES = ('U','D','L','R','F','B')

OPPOSITE = {
    'U' : 'D',
    'D' : 'U',
    'L' : 'R',
    'R' : 'L',
    'F' : 'B',
    'B' : 'F',
}


EDGES_LIST = []

EDGES = {}

CORNERS_LIST = []

CORNERS = {}

BASE_OPERATORS = {
    'B' : {
        'U' : 'R',
        'R' : 'D',
        'D' : 'L',
        'L' : 'U',
    },
    'F' : {
        'R' : 'U',
        'D' : 'R',
        'L' : 'D',
        'U' : 'L',
    },
    'R' : {
        'U' : 'F',
        'F' : 'D',
        'D' : 'B',
        'B' : 'U',
    },
    'L' : {
        'F' : 'U',
        'D' : 'F',
        'B' : 'D',
        'U' : 'B',
    },
    'D' : {
        'R' : 'F',
        'F' : 'L',
        'L' : 'B',
        'B' : 'R',
    },
    'U' : {
        'F' : 'R',
        'L' : 'F',
        'B' : 'L',
        'R' : 'B',
    },
}

OPERATORS = {}

OPERATOR_STATES = {}

def init():
    for a, b in itertools.combinations(SIDES, 2):
        if OPPOSITE[a] == b:
            continue
        l = [a,b]

        l.sort()
        EDGES_LIST.append(tuple(l))

    for i, c in enumerate(EDGES_LIST):
        for p in itertools.permutations(c, 2):
            EDGES[p] = i

    for a, b, c in itertools.combinations(SIDES, 3):
        if OPPOSITE[a] == b or OPPOSITE[a] == c or OPPOSITE[b] == c:
            continue
        l = [a,b,c]

        l.sort()
        CORNERS_LIST.append(tuple(l))

    for i, c in enumerate(CORNERS_LIST):
        for p in itertools.permutations(c, 3):
            CORNERS[p] = i

    for op, replace_dict in BASE_OPERATORS.iteritems():
        OPERATORS[op] = replace_dict
        replace_dict[op] = op
        replace_dict[OPPOSITE[op]] = OPPOSITE[op]
        OPERATORS[op + '\''] = {}
        OPERATORS[op + '2'] = {}
        for source, target in replace_dict.iteritems():
            OPERATORS[op + '\''][target] = source
            OPERATORS[op + '2'][source] = replace_dict[target]

    for op, replace_dict in OPERATORS.iteritems():
        output_corners = []
        output_edges = []
        for corner in CORNERS_LIST:
            if op[0] not in corner:
                output_corners.append(corner)
                continue

            output_corners.append(tuple(replace_dict[c] for c in corner))

        for edge in EDGES_LIST:
            if op[0] not in edge:
                output_edges.append(edge)
                continue

            output_edges.append(tuple(replace_dict[c] for c in edge))

        OPERATOR_STATES[op] = (tuple(output_corners), tuple(output_edges))


init()

START_STATE = (tuple(CORNERS_LIST), tuple(EDGES_LIST))


def multiply_states(state1, state2):
    corners1, edges1 = state1
    corners2, edges2 = state2

    corners_output = []
    edges_output = []

    corner_idx2 = [CORNERS[p] for p in corners2]
    edges_idx2 = [EDGES[p] for p in edges2]

    for target, source in enumerate(corner_idx2):
        corner2_before =  CORNERS_LIST[source]
        corner2_after = corners2[target]
        if corner2_before == corner2_after:
            corners_output.append(corners1[source])
            continue

        corner1_before = corners1[source]
        corner1_after = []
        for c in corner2_after:
            corner1_after.append(corner1_before[corner2_before.index(c)])

        corners_output.append(tuple(corner1_after))

    for target, source in enumerate(edges_idx2):
        edge2_before =  EDGES_LIST[source]
        edge2_after = edges2[target]
        if edge2_before == edge2_after:
            edges_output.append(edges1[source])
        else:
            edges_output.append((edges1[source][1], edges1[source][0]))

    return (tuple(corners_output), tuple(edges_output))

def get_order(state):
    corner_cycles = [None] * 8
    edge_cycles = [None] * 12

    i = 1
    result = state
    for j in range(8):
        if result[0][j] == START_STATE[0][j] and corner_cycles[j] is None:
            corner_cycles[j] = i

    for j in range(12):
        if result[1][j] == START_STATE[1][j] and edge_cycles[j] is None:
            edge_cycles[j] = i

    while result != START_STATE:
        result = multiply_states(state, result)
        i += 1
        for j in range(8):
            if result[0][j] == START_STATE[0][j] and corner_cycles[j] is None:
                corner_cycles[j] = i

        for j in range(12):
            if result[1][j] == START_STATE[1][j] and edge_cycles[j] is None:
                edge_cycles[j] = i

    print corner_cycles
    print edge_cycles
    return i

def create_state(s):
    result = START_STATE
    current_s = ''
    current_op = ''
    for c in s:
        if c.strip() == '':
            continue
        if c in SIDES and len(current_op) > 0:
            result = multiply_states(result, OPERATOR_STATES[current_op])
            current_s += current_op
            current_op = ''
            o = get_order(result)
            print current_s, o

        current_op += c

    result = multiply_states(result, OPERATOR_STATES[current_op])
    o = get_order(result)
    print s, o
    return result

def create_face_state(side):
    corners, edges = START_STATE
    face_corners = []
    face_edges = []
    for c in corners:
        if side in c:
            face_corners.append(c)
        else:
            face_corners.append(('*','*','*'))

    for e in edges:
        if side in e:
            face_edges.append(e)
        else:
            face_edges.append(('*','*'))

    return (tuple(face_corners), tuple(face_edges))

def create_edge_state(side1, side2):
    corners, edges = START_STATE
    face_corners = []
    face_edges = []
    for c in corners:
        if side1 in c and side2 in c:
            face_corners.append(c)
        else:
            face_corners.append(('*','*','*'))

    for e in edges:
        if side1 in e and side2 in e:
            face_edges.append(e)
        else:
            face_edges.append(('*','*'))

    return (tuple(face_corners), tuple(face_edges))

def create_corner_state(side1, side2, side3):
    corners, edges = START_STATE
    face_corners = []
    face_edges = []
    for c in corners:
        if side1 in c and side2 in c and side3 in c:
            face_corners.append(c)
        else:
            face_corners.append(('*','*','*'))

    for e in edges:
        if (side1 in e and side2 in e) or (side1 in e and side3 in e) or (side2 in e and side3 in e):
            face_edges.append(e)
        else:
            face_edges.append(('*','*'))

    return (tuple(face_corners), tuple(face_edges))

def create_PDB():
    g = 1
    counts = []

    start_state = create_face_state('F')
    #start_state = create_edge_state('F', 'R')
    #start_state = create_corner_state('F', 'R', 'U')
    next_states = [start_state]
    h = {start_state : 0}
    while len(next_states) > 0:
        counts.append(len(next_states))
        print g, counts[-1], sum(counts)
        current_states = next_states
        next_states = []
        for state in current_states:
            for opstate in OPERATOR_STATES.values():
                res = multiply_states(state, opstate)
                if res in h:
                    continue

                next_states.append(res)
                h[res] = g

        g += 1
    counts.append(len(next_states))
    print counts
    print sum(counts)
    return h





def main():

    start = create_state("BLB'L'F2D'RB'UD2BLB2RDR'B2R2B2R'")
    # start = create_state("DB'DU2R'")

    #create_PDB()
    print get_order(start)






if __name__ == '__main__':
    main()