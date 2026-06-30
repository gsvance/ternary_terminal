from typing import Any, Self


class Trit:

    __slots__ = ('_value',)

    @property
    def value(self) -> int:
        return self._value

    @value.setter
    def value(self, new_value: int) -> None:
        if new_value not in (-1, 0, +1):
            raise ValueError(f'invalid trit value: {new_value}')
        self._value: int = new_value

    def __init__(self, value: int) -> None:
        self.value = value

    def __str__(self) -> str:
        return 'T' if self.value == -1 else str(self.value)

    def __repr__(self) -> str:
        value_str = '+1' if self.value == +1 else str(self.value)
        return f'{self.__class__.__name__}({value_str})'

    def __invert__(self) -> Self:
        self.value = -self.value
        return self

    def __pos__(self) -> Self:
        self.value = -1 if self.value == +1 else self.value + 1
        return self

    def __neg__(self) -> Self:
        self.value = +1 if self.value == -1 else self.value - 1
        return self

    def __eq__(self, other: Any) -> bool:
        if isinstance(other, self.__class__):
            return self.value == other.value
        return False

    def __ne__(self, other: Any) -> bool:
        if isinstance(other, self.__class__):
            return self.value != other.value
        return False


for i in (-1, 0, +1):
    trit = Trit(i)
    print('str:', trit)
    print('repr:', repr(trit))
    ~trit
    print('~trit:', trit)
    ~trit
    +trit
    print('+trit:', trit)
    ++trit
    -trit
    print('-trit:', trit)
    --trit
    print()
