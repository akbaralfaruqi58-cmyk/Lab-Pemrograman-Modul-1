"""AssetTag value object encapsulating campus inventory identifier."""
from __future__ import annotations


class AssetTag:
    """An immutable value object representing a unique asset tag (e.g. 'LP-001').

    Honours the equality and hash contract:
    - Reflexive: x == x
    - Symmetric: x == y implies y == x
    - Transitive: x == y and y == z implies x == z
    - Hash consistency: x == y implies hash(x) == hash(y)
    """

    def __init__(self, code: str) -> None:
        cleaned = code.strip().upper()
        if not cleaned:
            raise ValueError("AssetTag cannot be empty.")
        self._code: str = cleaned

    @property
    def code(self) -> str:
        return self._code

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, AssetTag):
            return NotImplemented
        return self._code == other._code

    def __hash__(self) -> int:
        return hash(self._code)

    def __repr__(self) -> str:
        return f"AssetTag('{self._code}')"

    def __str__(self) -> str:
        return self._code
