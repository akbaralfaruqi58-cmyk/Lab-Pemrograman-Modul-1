"""Concrete Laptop equipment subclass."""
from __future__ import annotations
from .asset_tag import AssetTag
from .equipment import Equipment


class Laptop(Equipment):
    """Laboratory laptop available for student/faculty loan."""

    def __init__(
        self,
        tag: AssetTag,
        model: str,
        ram_gb: int = 16,
        processor: str = "Core i7",
    ) -> None:
        super().__init__(tag, model)
        if ram_gb <= 0:
            raise ValueError("RAM must be positive.")
        self._ram_gb = ram_gb
        self._processor = processor

    @property
    def ram_gb(self) -> int:
        return self._ram_gb

    @property
    def processor(self) -> str:
        return self._processor

    def daily_fine(self) -> int:
        # Standard daily overdue fine for high-demand computing gear
        return 50000

    def category_name(self) -> str:
        return "Laptop"

    def __repr__(self) -> str:
        return (
            f"Laptop(tag={self._tag!r}, model='{self._model}', "
            f"ram={self._ram_gb}GB, cpu='{self._processor}')"
        )
