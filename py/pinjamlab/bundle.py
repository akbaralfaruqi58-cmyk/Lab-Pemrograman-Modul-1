"""Composite equipment bundle demonstrating composition over inheritance."""
from __future__ import annotations
from typing import List
from .asset_tag import AssetTag
from .equipment import Equipment


class EquipmentBundle(Equipment):
    """A collection of equipment loaned together as a single kit (Composite pattern).

    Demonstrates composition over inheritance: fine rate is dynamically
    computed as the sum of its bundled items plus a small package discount.
    """

    def __init__(self, tag: AssetTag, bundle_name: str, items: List[Equipment] | None = None) -> None:
        super().__init__(tag, bundle_name)
        self._items: List[Equipment] = list(items) if items else []

    def add_item(self, item: Equipment) -> None:
        self._items.append(item)

    @property
    def items(self) -> List[Equipment]:
        return list(self._items)

    def daily_fine(self) -> int:
        # Sum of items with 10% bundle discount
        total = sum(item.daily_fine() for item in self._items)
        return int(total * 0.9)

    def category_name(self) -> str:
        return "EquipmentBundle"
