"""Unit tests for AssetTag contract and hashing deduplication."""
import pytest
from pinjamlab.asset_tag import AssetTag


def test_asset_tag_creation_and_normalization():
    tag1 = AssetTag("lp-001")
    assert tag1.code == "LP-001"

    with pytest.raises(ValueError):
        AssetTag("   ")


def test_equality_contract():
    # 1. Reflexive: a == a
    a = AssetTag("LP-001")
    assert a == a

    # 2. Symmetric: a == b <=> b == a
    b = AssetTag("LP-001")
    assert (a == b) and (b == a)

    # 3. Transitive: a == b and b == c => a == c
    c = AssetTag("LP-001")
    assert (a == b) and (b == c) and (a == c)

    # 4. Consistency: repeated calls yield same result
    assert (a == b) and (a == b)

    # 5. Non-nullity / Inequality
    d = AssetTag("LP-002")
    assert a != d
    assert a != "LP-001"


def test_hash_consistency_and_deduplication():
    # Equal objects MUST have equal hashes
    t1 = AssetTag("LP-001")
    t2 = AssetTag("LP-001")
    t3 = AssetTag("PJ-001")
    assert hash(t1) == hash(t2)

    # Deduplication in a Python set
    tags_list = [
        AssetTag("LP-001"),
        AssetTag("LP-002"),
        AssetTag("LP-001"),  # duplicate
        AssetTag("PJ-001"),
        AssetTag("LP-002"),  # duplicate
    ]
    unique_tags = set(tags_list)
    assert len(unique_tags) == 3
    assert AssetTag("LP-001") in unique_tags
    assert AssetTag("LP-002") in unique_tags
    assert AssetTag("PJ-001") in unique_tags
