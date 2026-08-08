"""The idea here is to see if there's any easy combination of operations that
can cycle a tryte through all possible 3**5 states.
"""

type Tryte = tuple[int, int, int, int, int]


def invert(t: Tryte) -> Tryte:
    return tuple(-x for x in t)


def rotate_left(t: Tryte) -> Tryte:
    l = list(t)
    l.append(l.pop(0))
    return tuple(l)


def shift_right(t: Tryte) -> Tryte:
    return tuple([0] + list(t[:4]))


def cycle_lst(t: Tryte) -> Tryte:
    l = list(t)
    x = l.pop()
    l.append(x + 1 if x < +1 else -1)
    return tuple(l)


def is_zero(t: Tryte) -> bool:
    return all(x == 0 for x in t)


def display(t: Tryte) -> str:
    ter = ''.join(('T' if x == -1 else str(x)) for x in t)
    dec = t[0] * 3**4 + t[1] * 3**3 + t[2] * 3**2 + t[3] * 3**1 + t[4] * 3**0
    return f'ter {ter} (dec {dec})'


T = -1
my_t = (1, 0, T, T, 0)
print('0:', display(my_t))


def ops(t: Tryte) -> Tryte:
    t = shift_right(t)
    return t


next_t = my_t
i = 0
while True:
    next_t = ops(next_t)
    i += 1
    print(f'{i}:', display(next_t))
    if next_t == my_t:
        break
    if is_zero(next_t):
        break
