#!/usr/bin/env python3
"""Simple test for function size ranking with mock data."""

# Simple categorization function
def categorize_function(size: int) -> str:
    """Categorize a function by size."""
    if size <= 100:
        return "small"
    elif size <= 500:
        return "medium"
    else:
        return "big"


# Mock function data (simulating what we'd get from nm/readelf)
MOCK_FUNCTIONS = [
    ("FUN_001163A0", 0x001163A0, 64, "small"),  # small function
    ("FUN_001163E0", 0x001163E0, 128, "medium"),  # medium function
    ("FUN_00116460", 0x00116460, 256, "medium"),  # medium function
    ("FUN_00116560", 0x00116560, 32, "small"),  # tiny function
    ("FUN_00116580", 0x00116580, 512, "big"),  # big function
    ("FUN_00116780", 0x00116780, 640, "big"),  # large function
    ("FUN_00116A00", 0x00116A00, 96, "small"),  # small function near limit
    ("FUN_00116A60", 0x00116A60, 104, "medium"),  # just over small limit
    ("FUN_00116AC0", 0x00116AC0, 496, "medium"),  # just under big limit
    ("FUN_00116CC0", 0x00116CC0, 504, "big"),  # just over medium limit
]


def rank_functions(functions):
    """Rank functions by size (largest first)."""
    return sorted(functions, key=lambda f: f[2], reverse=True)


def filter_functions(functions, min_size=0, max_size=None, category=None):
    """Filter functions by size and/or category."""
    filtered = []
    for func in functions:
        name, addr, size, cat = func
        if size < min_size:
            continue
        if max_size is not None and size > max_size:
            continue
        if category and cat != category:
            continue
        filtered.append(func)
    return filtered


def main():
    print("=" * 70)
    print("Function Size Ranking Test - Ratchet & Clank 2")
    print("=" * 70)
    print()
    
    # Test categorization
    print("Testing function categorization:")
    test_cases = [
        (0, "small"),
        (50, "small"),
        (100, "small"),
        (101, "medium"),
        (250, "medium"),
        (500, "medium"),
        (501, "big"),
        (1000, "big"),
    ]
    
    all_passed = True
    for size, expected in test_cases:
        result = categorize_function(size)
        status = "✓" if result == expected else "✗"
        if result != expected:
            all_passed = False
        print(f"  {status} Size {size:4} -> {result:8} (expected: {expected})")
    
    print()
    
    # Test ranking
    print("Testing function ranking (largest first):")
    ranked = rank_functions(MOCK_FUNCTIONS)
    print("  Address    Size  Category  Name")
    print("  " + "-" * 56)
    for func in ranked:
        name, addr, size, cat = func
        print(f"  {hex(addr):<10} {size:>4}  {cat:<8}  {name}")
    
    print()
    
    # Test filtering
    print("Testing function filtering:")
    
    print("\n  Small functions (0-100 bytes):")
    small = filter_functions(ranked, category="small")
    for func in small:
        print(f"    {func[0]}: {func[2]} bytes")
    
    print("\n  Medium functions (101-500 bytes):")
    medium = filter_functions(ranked, category="medium")
    for func in medium:
        print(f"    {func[0]}: {func[2]} bytes")
    
    print("\n  Big functions (501+ bytes):")
    big = filter_functions(ranked, category="big")
    for func in big:
        print(f"    {func[0]}: {func[2]} bytes")
    
    print("\n  Functions with size 100-500 bytes:")
    range_filtered = filter_functions(ranked, min_size=100, max_size=500)
    for func in range_filtered:
        print(f"    {func[0]}: {func[2]} bytes")
    
    print()
    
    # Summary
    print("=" * 70)
    print("Summary:")
    print(f"  Total functions: {len(MOCK_FUNCTIONS)}")
    print(f"  Small (0-100):   {len(small)}")
    print(f"  Medium (101-500): {len(medium)}")
    print(f"  Big (501+):      {len(big)}")
    print("=" * 70)
    
    if all_passed:
        print("\n✓ All tests passed!")
    else:
        print("\n✗ Some tests failed!")
    
    return 0 if all_passed else 1


if __name__ == "__main__":
    exit(main())
