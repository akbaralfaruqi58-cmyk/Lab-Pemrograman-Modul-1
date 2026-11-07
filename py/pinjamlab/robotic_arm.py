"""Robotic arm equipment subclass."""
from __future__ import annotations

from .asset_tag import AssetTag
from .equipment import Equipment


class RoboticArm(Equipment):
    """Robotic arm with a 10 kg payload and 1300 mm maximum reach."""

    def __init__(
        self,
        tag: AssetTag,
        model: str,
        payload_kg: float = 10.0,
        max_reach_mm: float = 1300.0,
    ) -> None:
        super().__init__(tag, model)
        if payload_kg <= 0:
            raise ValueError("Robotic arm payload must be positive.")
        if max_reach_mm <= 0:
            raise ValueError("Robotic arm maximum reach must be positive.")
        self._payload_kg = float(payload_kg)
        self._max_reach_mm = float(max_reach_mm)

    @property
    def payload_kg(self) -> float:
        return self._payload_kg

    @property
    def max_reach_mm(self) -> float:
        return self._max_reach_mm

    def daily_fine(self) -> int:
        """Return the overdue fine per day in IDR."""
        return 180000

    def category_name(self) -> str:
        return "RoboticArm"
