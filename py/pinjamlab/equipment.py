"""Abstract Equipment base class and EquipmentState enumeration."""
from __future__ import annotations
from abc import ABC, abstractmethod
from enum import Enum, auto
from .asset_tag import AssetTag


class EquipmentState(Enum):
    AVAILABLE = auto()
    LOANED = auto()
    MAINTENANCE = auto()
    DECOMMISSIONED = auto()


class Equipment(ABC):
    """Abstract base class representing any laboratory equipment."""

    def __init__(self, tag: AssetTag, model: str) -> None:
        if not model.strip():
            raise ValueError("Equipment model name cannot be empty.")
        self._tag: AssetTag = tag
        self._model: str = model.strip()
        self._state: EquipmentState = EquipmentState.AVAILABLE

    @property
    def tag(self) -> AssetTag:
        return self._tag

    @property
    def model(self) -> str:
        return self._model

    @property
    def state(self) -> EquipmentState:
        return self._state

    def mark_loaned(self) -> None:
        if self._state != EquipmentState.AVAILABLE:
            raise RuntimeError(f"Cannot loan item {self._tag}: state is {self._state.name}")
        self._state = EquipmentState.LOANED

    def mark_returned(self) -> None:
        if self._state != EquipmentState.LOANED:
            raise RuntimeError(f"Cannot return item {self._tag}: state is {self._state.name}")
        self._state = EquipmentState.AVAILABLE

    def mark_maintenance(self) -> None:
        self._state = EquipmentState.MAINTENANCE

    @abstractmethod
    def daily_fine(self) -> int:
        """Returns the overdue fine rate per day in IDR."""
        pass

    @abstractmethod
    def category_name(self) -> str:
        """Returns the human-readable category name."""
        pass

    def __repr__(self) -> str:
        return (
            f"{self.__class__.__name__}(tag={self._tag!r}, "
            f"model='{self._model}', state={self._state.name})"
        )
