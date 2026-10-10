#!/usr/bin/env python3
"""
Import symbol addresses from progress/candidates.json into symbol_addrs/ files.

This script extracts symbol addresses from the candidates JSON and generates
symbol_addrs/usa/symbol_addrs.txt and symbol_addrs/eu/symbol_addrs.txt files.
"""

import json
import os


def extract_symbol_addresses():
    """
    Extract symbol addresses from progress/candidates.json
    and generate symbol_addrs/ files.
    """
    # Paths
    candidates_path = "progress/candidates.json"
    symbol_addrs_usa = "symbol_addrs/usa/symbol_addrs.txt"
    symbol_addrs_eu = "symbol_addrs/eu/symbol_addrs.txt"

    # Load candidates data
    with open(candidates_path, 'r') as f:
        data = json.load(f)

    # Get the functions array
    functions = data.get('functions', [])

    print(f"Processing {len(functions)} functions from progress/candidates.json")

    # Collect symbols by region
    symbols_usa = {}
    symbols_eu = {}

    for func in functions:
        symbol = func.get('symbol', '')
        if not symbol:
            continue

        address = func.get('address', '')
        if not address:
            continue

        # Convert address to hex format
        if isinstance(address, int):
            hex_addr = f"0x{address:x}"
        else:
            hex_addr = f"0x{int(address):x}"

        # Determine region based on address range
        addr_int = int(hex_addr, 16)

        # USA and EU have different address ranges
        # Boot functions are typically in lower ranges
        # Level functions have different ranges per region

        # Default to USA for boot/low memory functions
        if addr_int < 0x80000000:
            # This is a kernel address
            symbols_usa[symbol] = hex_addr
        else:
            # User space - default to USA
            symbols_usa[symbol] = hex_addr

    # Write USA symbol_addrs.txt
    os.makedirs(os.path.dirname(symbol_addrs_usa), exist_ok=True)
    with open(symbol_addrs_usa, 'w') as f:
        f.write("# symbol_addrs.txt - USA region (SCUS_972.68)\n\n")
        f.write("# Symbol addresses imported from progress/candidates.json\n")
        f.write("# Add new symbols here when identified during decompilation.\n\n")

        for name, addr in sorted(symbols_usa.items()):
            f.write(f"{name} = {addr};\n")

    # Write EU symbol_addrs.txt
    os.makedirs(os.path.dirname(symbol_addrs_eu), exist_ok=True)
    with open(symbol_addrs_eu, 'w') as f:
        f.write("# symbol_addrs.txt - EU region (SCES-51607)\n\n")
        f.write("# Symbol addresses imported from progress/candidates.json\n")
        f.write("# Add new symbols here when identified during decompilation.\n\n")

        for name, addr in sorted(symbols_eu.items()):
            f.write(f"{name} = {addr};\n")

    print(f"Imported {len(symbols_usa)} USA symbols to {symbol_addrs_usa}")
    print(f"Imported {len(symbols_eu)} EU symbols to {symbol_addrs_eu}")


if __name__ == '__main__':
    extract_symbol_addresses()
