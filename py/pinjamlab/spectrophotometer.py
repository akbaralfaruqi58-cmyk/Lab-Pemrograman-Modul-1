"""Spectrophotometer equipment subclass."""
from __future__ import annotations

from .asset_tag import AssetTag
from .equipment import Equipment


class Spectrophotometer(Equipment):
    """UV-Vis spectrophotometer with 190–1100 nm range and 1 nm bandwidth."""

    def __init__(
        self,
        tag: AssetTag,
        model: str,
        min_wavelength_nm: float = 190.0,
        max_wavelength_nm: float = 1100.0,
        spectral_bandwidth_nm: float = 1.0,
    ) -> None:
        super().__init__(tag, model)
        if min_wavelength_nm <= 0:
            raise ValueError("Minimum wavelength must be positive.")
        if max_wavelength_nm <= min_wavelength_nm:
            raise ValueError("Maximum wavelength must exceed minimum wavelength.")
        if spectral_bandwidth_nm <= 0 or spectral_bandwidth_nm > max_wavelength_nm - min_wavelength_nm:
            raise ValueError("Spectral bandwidth must be positive and fit within the wavelength range.")
        self._min_wavelength_nm = float(min_wavelength_nm)
        self._max_wavelength_nm = float(max_wavelength_nm)
        self._spectral_bandwidth_nm = float(spectral_bandwidth_nm)

    @property
    def min_wavelength_nm(self) -> float:
        return self._min_wavelength_nm

    @property
    def max_wavelength_nm(self) -> float:
        return self._max_wavelength_nm

    @property
    def spectral_bandwidth_nm(self) -> float:
        return self._spectral_bandwidth_nm

    def daily_fine(self) -> int:
        """Return the overdue fine per day in IDR."""
        return 120000

    def category_name(self) -> str:
        return "Spectrophotometer"
