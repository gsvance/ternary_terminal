import enum
from typing import Self


class Trit(enum.Enum):
    NEG1 = -1
    ZERO = 0
    POS1 = +1

    def __repr__(self) -> str:
        value_str = '+1' if self.value == +1 else str(self.value)
        return f'{self.__class__.__name__}({value_str})'

    def __str__(self) -> str:
        return 'T' if self.value == -1 else str(self.value)

    def __invert__(self) -> Self:
        return self.__class__(-self.value)

    def __pos__(self) -> Self:
        new_value = -1 if self.value == +1 else self.value + 1
        return self.__class__(new_value)

    def __neg__(self) -> Self:
        new_value = +1 if self.value == -1 else self.value - 1
        return self.__class__(new_value)


for i in [-1, 0, 1]:
    trit = Trit(i)
    print('str:', str(trit))
    print('repr:', repr(trit))
    print('~trit:', repr(~trit))
    print('+trit:', repr(+trit))
    print('-trit:', repr(-trit))
    print()
