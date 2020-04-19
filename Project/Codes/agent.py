import numpy as np


LOWER_QUANT_LIMIT = 2
HIGHER_QUANT_LIMIT = 9

def maxAction(Q, state):
    values = np.array([Q[state,a] for a in range(LOWER_QUANT_LIMIT, HIGHER_QUANT_LIMIT)])
    action = np.argmax(values)
    return action + LOWER_QUANT_LIMIT